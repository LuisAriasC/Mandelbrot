#include "lines.h"

__global__ void kernel(){

}

void runCuda(int * m_fractal,int * m_histogram, double scale, double xCenter, double yCenter){

  int* d_fractal, d_histogram;
  int fractalBytes = M_WIDTH * M_HEIGHT * sizeof(int);
  int histogramBytes = 256 * sizeof(int);

  printf("HELLO THERE\n");
}
