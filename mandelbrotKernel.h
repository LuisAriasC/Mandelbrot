#include "lines.h"
#include "Mandelbrot.h"

__global__ void kernel(){

}

void runCuda(std::unique_ptr<int [], std::default_delete<int []>>& m_fractal,std::unique_ptr<int [], std::default_delete<int []>>& m_histogram, double scale, double xCenter, double yCenter){

  int* d_fractal, d_histogram;
  int fractalBytes = M_WIDTH * M_HEIGHT * sizeof(int);
  int histogramBytes = Mandelbrot::MAX_ITERATIONS * sizeof(int);


}
