//==========================================================================
// This file has been automatically generated for CUDA/C++ standalone by
%(info_lines)s
//==========================================================================

#ifndef MG5_%(process_file_name)s_H
#define MG5_%(process_file_name)s_H 1

#include "mgOnGpuConfig.h"
#include "mgOnGpuVectors.h"

#include "checkCuda.h"
#include "Parameters_%(model_name)s.h"

#include <vector>

//--------------------------------------------------------------------------

#ifdef __CUDACC__
namespace mg5amcGpu
#else
namespace mg5amcCpu
#endif
{

%(process_class_definitions)s
  //--------------------------------------------------------------------------

  __global__
  void sigmaKin_getGoodHel( const fptype* allmomenta, // input: momenta[nevt*npar*4]
                            fptype* allMEs,           // output: allMEs[nevt], |M|^2 final_avg_over_helicities
                            bool* isGoodHel           // output: isGoodHel[ncomb] - device array
#ifndef __CUDACC__
                            , const int nevt          // input: #events (for cuda: nevt == ndim == gpublocks*gputhreads)
#endif
                            );

  //--------------------------------------------------------------------------

  void sigmaKin_setGoodHel( const bool* isGoodHel ); // input: isGoodHel[ncomb] - host array

  //--------------------------------------------------------------------------

  __global__
  void sigmaKin( const fptype* allmomenta, // input: momenta[nevt*npar*4]
                 fptype* allMEs            // output: allMEs[nevt], |M|^2 final_avg_over_helicities
#ifndef __CUDACC__
                 , const int nevt          // input: #events (for cuda: nevt == ndim == gpublocks*gputhreads)
#endif
                 );

  //--------------------------------------------------------------------------
}

#endif // MG5_%(process_file_name)s_H
