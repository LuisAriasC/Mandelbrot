#ifndef __BASIC_INTEROP_H__
#define __BASIC_INTEROP_H__

// includes, cuda
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <device_launch_parameters.h>

// Utilities and timing functions
#include <helper_functions.h>    // includes cuda.h and cuda_runtime_api.h

// CUDA helper functions
#include <helper_cuda.h>         // helper functions for CUDA error check
#include <helper_cuda_gl.h>      // helper functions for CUDA/GL interop
#include <complex>

__global__ void kernel();

void runCuda(int * m_fractal,int * m_histogram, double scale, double xCenter, double yCenter);

#endif
