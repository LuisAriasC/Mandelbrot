#include "lines.h"
#include "common.h"
#include <cmath>
#include "Mandelbrot.h"
#include <cuComplex.h>
#include <cuPrintf.cu>
//#include <cuda_fp16.h>

using namespace std;
using namespace caveofprogramming;


__global__ void kernel(int * d_fractal, int * d_histogram, int step,double scale, double xCenter, double yCenter){


  unsigned int xIndex = threadIdx.x + blockIdx.x * blockDim.x;
  unsigned int yIndex = threadIdx.y + blockIdx.y * blockDim.y;
  const int tid  = yIndex * step + xIndex;
  int iterations = 0;

  __shared__ int s_histo[Mandelbrot::MAX_ITERATIONS];

  //Initialize shared histogram to 0
  if ( tid < Mandelbrot::MAX_ITERATIONS)
    s_histo[tid] = 0;
  __syncthreads();

  if ((xIndex < M_WIDTH) && (yIndex < M_HEIGHT)){
    double div = tid / M_HEIGHT;

    double x = tid % M_WIDTH;
    double y = floor(div);

    double xFractal = (x - M_WIDTH / 2) * scale + xCenter;
    double yFractal = (y - M_HEIGHT / 2) * scale + yCenter;


    double2 a; a.x = a.y = 0.0;
    cuDoubleComplex z = a;
    cuDoubleComplex c = make_cuDoubleComplex(x, y);

    while(iterations < Mandelbrot::MAX_ITERATIONS) {
      z = cuCadd(cuCmul(z, z), c);

      if(cuCabs(z) > 2) {
        break;
      }

      iterations++;
    }

    cuPrintf("Value %d", iterations);
    d_fractal[tid] = iterations;
  }

  if (iterations != Mandelbrot::MAX_ITERATIONS)
    d_histogram[iterations]++;
  __syncthreads();

}

void runCuda(int * m_fractal,int * m_histogram, double scale, double xCenter, double yCenter){

  int * d_fractal, * d_histogram;
  int fractalBytes = M_WIDTH * M_HEIGHT * sizeof(int);
  int histogramBytes = Mandelbrot::MAX_ITERATIONS * sizeof(int);

  d_fractal = (int *)malloc(fractalBytes);
  d_histogram = (int *)malloc(histogramBytes);
  memset(d_fractal, 0, fractalBytes);
  memset(d_histogram, 0, histogramBytes);


  SAFE_CALL(cudaMalloc<int>(&d_histogram, histogramBytes), "CUDA Malloc Failed");
  SAFE_CALL(cudaMalloc<int>(&d_fractal, fractalBytes), "CUDA Malloc Failed");

  SAFE_CALL(cudaMemcpy(d_histogram, m_histogram, histogramBytes, cudaMemcpyHostToDevice), "CUDA Memcpy Host To Device Failed");
  SAFE_CALL(cudaMemcpy(d_fractal, m_fractal,fractalBytes, cudaMemcpyHostToDevice), "CUDA Memcpy Host To Device Failed");

  const dim3 block(16, 16);
  const dim3 grid((int)ceil((float)M_WIDTH / block.x), (int)ceil((float)M_HEIGHT/ block.y));

  printf("Starting\n");
  kernel<<<grid, block >>>(d_fractal, d_histogram, M_WIDTH, scale, xCenter, yCenter);
  printf("Ending\n");
  SAFE_CALL(cudaDeviceSynchronize(), "Kernel Launch Failed");
  // SAFE_CALL kernel error
  SAFE_CALL(cudaGetLastError(), "Error with last error");

  SAFE_CALL(cudaMemcpy(m_fractal, d_fractal, fractalBytes, cudaMemcpyDeviceToHost), "CUDA Memcpy Device To Device Failed");
  SAFE_CALL(cudaMemcpy(m_histogram, d_histogram, histogramBytes, cudaMemcpyDeviceToHost), "CUDA Memcpy Device To Device Failed");

  SAFE_CALL(cudaFree(d_fractal), "CUDA Free Failed");
  SAFE_CALL(cudaFree(d_histogram), "CUDA Free Failed");

  // Reset device
  SAFE_CALL(cudaDeviceReset(), "Error reseting");

}
