#include "lines.h"
#include "common.h"
#include "Mandelbrot.h"

using namespace std;
using namespace caveofprogramming;


__global__ void kernel(){

}

void runCuda(std::unique_ptr<int [], std::default_delete<int []>>& m_fractal,std::unique_ptr<int [], std::default_delete<int []>>& m_histogram, double scale, double xCenter, double yCenter){

  int * d_fractal, * d_histogram;
  int fractalBytes = M_WIDTH * M_HEIGHT * sizeof(int);
  int histogramBytes = Mandelbrot::MAX_ITERATIONS * sizeof(int);

  d_fractal = (int *)malloc(fractalBytes);
  d_histogram = (int *)malloc(histogramBytes);
  memset(d_fractal, 0, fractalBytes);
  memset(d_histogram, 0, histogramBytes);

  SAFE_CALL(cudaMalloc<int>(&d_fractal, fractalBytes), "CUDA Malloc Failed");
  SAFE_CALL(cudaMalloc<int>(&d_histogram, histogramBytes), "CUDA Malloc Failed");

  SAFE_CALL(cudaMemcpy(d_fractal, m_fractal, fractalBytes, cudaMemcpyHostToDevice), "CUDA Memcpy Host To Device Failed");

}
