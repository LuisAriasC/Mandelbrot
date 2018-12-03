#include "lines.h"
#include "common.h"
#include "Mandelbrot.h"

using namespace std;
using namespace caveofprogramming;


__global__ void kernel(){
  printf("HI FROM KERNEL\n");
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
  SAFE_CALL(cudaMemcpy(d_fractal, m_fractal, fractalBytes, cudaMemcpyHostToDevice), "CUDA Memcpy Host To Device Failed");

  const dim3 block(16, 16);
  const dim3 grid((int)ceil((float)M_WIDTH / block.x), (int)ceil((float)M_HEIGHT/ block.y));

  kernel<<<grid, block >>>();

  SAFE_CALL(cudaDeviceSynchronize(), "Kernel Launch Failed");
  // SAFE_CALL kernel error
  SAFE_CALL(cudaGetLastError(), "Error with last error");

  SAFE_CALL(cudaFree(d_fractal), "CUDA Free Failed");
  SAFE_CALL(cudaFree(d_histogram), "CUDA Free Failed");

  // Reset device
  SAFE_CALL(cudaDeviceReset(), "Error reseting");

}
