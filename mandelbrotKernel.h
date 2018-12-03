#include "lines.h"
#include "common.h"
#include <cmath>
#include "Mandelbrot.h"
//#include <cuda_fp16.h>

using namespace std;
using namespace caveofprogramming;


__global__ void kernel(int * d_fractal, int * d_histogram, int step,double scale, double xCenter, double yCenter){
  //printf("HI FROM KERNEL\n");
  int i = 0;

  unsigned int xIndex = threadIdx.x + blockIdx.x * blockDim.x;
  unsigned int yIndex = threadIdx.y + blockIdx.y * blockDim.y;

  const int tid  = yIndex * step + xIndex;

  double xFractal = tid % M_WIDTH;
  double yFractal = __device__ floor(tid / M_HEIGHT);

/*
  for (int y = 0; y < m_height; y++) {
    for (int x = 0; x < m_width; x++) {
      pair<double, double> coords = m_zoomList.doZoom(x, y);

      int iterations = Mandelbrot::getIterations(coords.first,
          coords.second);

      m_fractal[y * m_width + x] = iterations;

      if (iterations != Mandelbrot::MAX_ITERATIONS) {
        m_histogram[iterations]++;
      }

    }
  }
*/
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

  SAFE_CALL(cudaFree(d_fractal), "CUDA Free Failed");
  SAFE_CALL(cudaFree(d_histogram), "CUDA Free Failed");

  // Reset device
  SAFE_CALL(cudaDeviceReset(), "Error reseting");

}
