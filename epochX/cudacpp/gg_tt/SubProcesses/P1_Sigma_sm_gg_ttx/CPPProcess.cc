//==========================================================================
// This file has been automatically generated for CUDA/C++ standalone by
// MadGraph5_aMC@NLO v. 2.9.5, 2021-08-22
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <memory>

#include "mgOnGpuConfig.h"

#include "CPPProcess.h"
#include "HelAmps_sm.h"
#include "MemoryAccessAmplitudes.h"
#include "MemoryAccessMomenta.h"
#include "MemoryAccessWavefunctions.h"

// Test ncu metrics for CUDA thread divergence
#undef MGONGPU_TEST_DIVERGENCE
//#define MGONGPU_TEST_DIVERGENCE 1

//==========================================================================
// Class member functions for calculating the matrix elements for
// Process: g g > t t~ WEIGHTED<=2 @1

#ifdef __CUDACC__
namespace mg5amcGpu
#else
namespace mg5amcCpu
#endif
{
  using mgOnGpu::np4; // dimensions of 4-momenta (E,px,py,pz)
  using mgOnGpu::npar; // #particles in total (external = initial + final): e.g. 4 for e+ e- -> mu+ mu-
  using mgOnGpu::ncomb; // #helicity combinations: e.g. 16 for e+ e- -> mu+ mu- (2**4 = fermion spin up/down ** npar)

  using mgOnGpu::nwf; // #wavefunctions = #external (npar) + #internal: e.g. 5 for e+ e- -> mu+ mu- (1 internal is gamma or Z)
  using mgOnGpu::nw6; // dimensions of each wavefunction (HELAS KEK 91-11): e.g. 6 for e+ e- -> mu+ mu- (fermions and vectors)

  // Physics parameters (masses, coupling, etc...)
  // For CUDA performance, hardcoded constexpr's would be better: fewer registers and a tiny throughput increase
  // However, physics parameters are user-defined through card files: use CUDA constant memory instead (issue #39)
  // [NB if hardcoded parameters are used, it's better to define them here to avoid silent shadowing (issue #263)]
#ifdef MGONGPU_HARDCODE_CIPC
  __device__ const fptype cIPC[4] = {
    Parameters_sm::GC_10.real(), Parameters_sm::GC_10.imag(),
    Parameters_sm::GC_11.real(), Parameters_sm::GC_11.imag() };
  __device__ const fptype cIPD[2] = {
    Parameters_sm::mdl_MT,
    Parameters_sm::mdl_WT };
#else
#ifdef __CUDACC__
  __device__ __constant__ fptype cIPC[4];
  __device__ __constant__ fptype cIPD[2];
#else
  static fptype cIPC[4];
  static fptype cIPD[2];
#endif
#endif

  // Helicity combinations (and filtering of "good" helicity combinations)
#ifdef __CUDACC__
  __device__ __constant__ short cHel[ncomb][npar];
  __device__ __constant__ int cNGoodHel; // FIXME: assume process.nprocesses == 1 for the moment (eventually cNGoodHel[nprocesses]?)
  __device__ __constant__ int cGoodHel[ncomb];
#else
  static short cHel[ncomb][npar];
  static int cNGoodHel; // FIXME: assume process.nprocesses == 1 for the moment (eventually cNGoodHel[nprocesses]?)
  static int cGoodHel[ncomb];
#endif

  //--------------------------------------------------------------------------

  // Evaluate |M|^2 for each subprocess
  // NB: calculate_wavefunctions ADDS |M|^2 for a given ihel to the running sum of |M|^2 over helicities for the given event(s)
  __device__
  INLINE
  void calculate_wavefunctions( int ihel,
                                const fptype* allmomenta, // input: momenta[nevt*npar*4]
                                fptype* allMEs            // output: allMEs[nevt], |M|^2 running_sum_over_helicities
#ifndef __CUDACC__
                                , const int nevt          // input: #events (for cuda: nevt == ndim == gpublocks*gputhreads)
#endif
                                )
  //ALWAYS_INLINE // attributes are not permitted in a function definition
  {
#ifdef __CUDACC__
    using namespace mg5amcGpu;
    using M_ACCESS = DeviceAccessMomenta;
    using W_ACCESS = DeviceAccessWavefunctions;
    using A_ACCESS = DeviceAccessAmplitudes;
#else
    using namespace mg5amcCpu;
    using M_ACCESS = HostAccessMomenta;
    using W_ACCESS = HostAccessWavefunctions;
    using A_ACCESS = HostAccessAmplitudes;
#endif
    mgDebug( 0, __FUNCTION__ );
#ifndef __CUDACC__
    //printf( "calculate_wavefunctions: nevt %d\n", nevt );
#endif

    // The number of colors
    constexpr int ncolor = 2;

    // Local TEMPORARY variables for a subset of Feynman diagrams in the given CUDA event (ievt) or C++ event page (ipagV)
    // [NB these variables are reused several times (and re-initialised each time) within the same event or event page]
    //MemoryBufferWavefunctions w_buffer[nwf]{ neppV };
    cxtype_sv w_sv[nwf][nw6]; // particle wavefunctions within Feynman diagrams (nw6 is often 6, the dimension of spin 1/2 or spin 1 particles)
    cxtype_sv amp_sv[1]; // invariant amplitude for one given Feynman diagram

    // Proof of concept for using fptype* in the interface
    fptype* w_fp[nwf];
    for ( int iwf=0; iwf<nwf; iwf++ ) w_fp[iwf] = reinterpret_cast<fptype*>( w_sv[iwf] );
    fptype* amp_fp;
    amp_fp = reinterpret_cast<fptype*>( amp_sv );

    // Local variables for the given CUDA event (ievt) or C++ event page (ipagV)
    // [jamp: sum (for one event or event page) of the invariant amplitudes for all Feynman diagrams in a given color combination]
    cxtype_sv jamp_sv[ncolor] = {}; // all zeros (NB: vector cxtype_v IS initialized to 0, but scalar cxype is NOT, if "= {}" is missing!)

    // === Calculate wavefunctions and amplitudes for all diagrams in all processes - Loop over nevt events ===
#ifndef __CUDACC__
    const int npagV = nevt / neppV;
#ifdef MGONGPU_CPPSIMD
    const bool isAligned_allMEs = ( (size_t)(allMEs) % mgOnGpu::cppAlign == 0 ); // require SIMD-friendly alignment by at least neppV*sizeof(fptype)
#endif
    // ** START LOOP ON IPAGV **
#ifdef _OPENMP
    // (NB gcc9 or higher, or clang, is required)
    // - default(none): no variables are shared by default
    // - shared: as the name says
    // - private: give each thread its own copy, without initialising
    // - firstprivate: give each thread its own copy, and initialise with value from outside
#ifdef MGONGPU_CPPSIMD
#pragma omp parallel for default(none) shared(allmomenta,allMEs,cHel,cIPC,cIPD,ihel,npagV,amp_fp,w_fp,isAligned_allMEs) private (amp_sv,w_sv,jamp_sv)
#else
#pragma omp parallel for default(none) shared(allmomenta,allMEs,cHel,cIPC,cIPD,ihel,npagV,amp_fp,w_fp) private (amp_sv,w_sv,jamp_sv)
#endif
#endif
    for ( int ipagV = 0; ipagV < npagV; ++ipagV )
#endif
    {
#ifdef __CUDACC__
      // CUDA kernels take an input buffer with momenta for all events
      const fptype* momenta = allmomenta;
#else
      // C++ kernels take an input buffer with momenta for one specific event (the first in the current event page)
      const int ievt0 = ipagV*neppV;
      const fptype* momenta = MemoryAccessMomenta::ieventAccessRecordConst( allmomenta, ievt0 );
#endif

      // Reset color flows (reset jamp_sv) at the beginning of a new event or event page
      for( int i=0; i<ncolor; i++ ){ jamp_sv[i] = cxzero_sv(); }

      // *** DIAGRAM 1 OF 3 ***

      // Wavefunction(s) for diagram number 1
      vxxxxx<M_ACCESS, W_ACCESS>( momenta, 0., cHel[ihel][0], -1, w_fp[0], 0 );

      vxxxxx<M_ACCESS, W_ACCESS>( momenta, 0., cHel[ihel][1], -1, w_fp[1], 1 );

      oxxxxx<M_ACCESS, W_ACCESS>( momenta, cIPD[0], cHel[ihel][2], +1, w_fp[2], 2 );

      ixxxxx<M_ACCESS, W_ACCESS>( momenta, cIPD[0], cHel[ihel][3], -1, w_fp[3], 3 );

      VVV1P0_1<W_ACCESS>( w_fp[0], w_fp[1], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_fp[4] );

      // Amplitude(s) for diagram number 1
      FFV1_0<W_ACCESS, A_ACCESS>( w_fp[3], w_fp[2], w_fp[4], cxmake( cIPC[2], cIPC[3] ), &amp_fp[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[1] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 2 OF 3 ***

      // Wavefunction(s) for diagram number 2
      FFV1_1<W_ACCESS>( w_fp[2], w_fp[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_fp[4] );

      // Amplitude(s) for diagram number 2
      FFV1_0<W_ACCESS, A_ACCESS>( w_fp[3], w_fp[4], w_fp[1], cxmake( cIPC[2], cIPC[3] ), &amp_fp[0] );
      jamp_sv[0] -= amp_sv[0];

      // *** DIAGRAM 3 OF 3 ***

      // Wavefunction(s) for diagram number 3
      FFV1_2<W_ACCESS>( w_fp[3], w_fp[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_fp[4] );

      // Amplitude(s) for diagram number 3
      FFV1_0<W_ACCESS, A_ACCESS>( w_fp[4], w_fp[2], w_fp[1], cxmake( cIPC[2], cIPC[3] ), &amp_fp[0] );
      jamp_sv[1] -= amp_sv[0];

      // *** COLOR ALGEBRA BELOW ***
      // (This method used to be called CPPProcess::matrix_1_gg_ttx()?)

      // The color matrix (initialize all array elements, with ncolor=1)
      // [NB do keep 'static' for these constexpr arrays, see issue #283]
      static constexpr fptype denom[ncolor] = {3, 3}; // 1-D array[2]
      static constexpr fptype cf[ncolor][ncolor] = {
        {16, -2},
        {-2, 16}}; // 2-D array[2][2]

      // Sum and square the color flows to get the matrix element
      // (compute |M|^2 by squaring |M|, taking into account colours)
      fptype_sv deltaMEs = {0}; // all zeros https://en.cppreference.com/w/c/language/array_initialization#Notes
      for( int icol = 0; icol < ncolor; icol++ )
      {
        cxtype_sv ztemp_sv = cxzero_sv();
        for( int jcol = 0; jcol < ncolor; jcol++ )
          ztemp_sv += cf[icol][jcol] * jamp_sv[jcol];
        deltaMEs += cxreal( ztemp_sv * cxconj( jamp_sv[icol] ) ) / denom[icol];
      }

      // *** STORE THE RESULTS ***

      // Store the leading color flows for choice of color
      // (NB: jamp2_sv must be an array of fptype_sv)
      // for( int icol = 0; icol < ncolor; icol++ )
      // jamp2_sv[0][icol] += cxreal( jamp_sv[icol]*cxconj( jamp_sv[icol] ) );

      // NB: calculate_wavefunctions ADDS |M|^2 for a given ihel to the running sum of |M|^2 over helicities for the given event(s)
      // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
#ifdef __CUDACC__
      const int ievt = blockDim.x * blockIdx.x + threadIdx.x; // index of event (thread) in grid
      allMEs[ievt] += deltaMEs;
      //if ( cNGoodHel > 0 ) printf( "calculate_wavefunction: %6d %2d %f\n", ievt, ihel, allMEs[ievt] );
#else
#ifdef MGONGPU_CPPSIMD
      if ( isAligned_allMEs )
      {
        *reinterpret_cast<fptype_sv*>( &( allMEs[ipagV*neppV] ) ) += deltaMEs;
      }
      else
      {
        for ( int ieppV=0; ieppV<neppV; ieppV++ )
          allMEs[ipagV*neppV + ieppV] += deltaMEs[ieppV];
      }
      //if ( cNGoodHel > 0 )
      //  for ( int ieppV=0; ieppV<neppV; ieppV++ )
      //    printf( "calculate_wavefunction: %6d %2d %f\n", ipagV*neppV+ieppV, ihel, allMEs[ipagV][ieppV] );
#else
      allMEs[ipagV] += deltaMEs;
      //if ( cNGoodHel > 0 ) printf( "calculate_wavefunction: %6d %2d %f\n", ipagV, ihel, allMEs[ipagV] );
#endif
#endif
    }
    mgDebug( 1, __FUNCTION__ );
    return;
  }

  //--------------------------------------------------------------------------

  CPPProcess::CPPProcess( int numiterations,
                          int ngpublocks,
                          int ngputhreads,
                          bool verbose,
                          bool debug )
    : m_numiterations( numiterations )
    , m_ngpublocks( ngpublocks )
    , m_ngputhreads( ngputhreads )
    , m_verbose( verbose )
    , m_debug( debug )
#ifndef MGONGPU_HARDCODE_CIPC
    , m_pars( 0 )
#endif
    , m_masses()
  {
    // Helicities for the process [NB do keep 'static' for this constexpr array, see issue #283]
    static constexpr short tHel[ncomb][mgOnGpu::npar] = {
      {-1, -1, -1, -1},
      {-1, -1, -1, 1},
      {-1, -1, 1, -1},
      {-1, -1, 1, 1},
      {-1, 1, -1, -1},
      {-1, 1, -1, 1},
      {-1, 1, 1, -1},
      {-1, 1, 1, 1},
      {1, -1, -1, -1},
      {1, -1, -1, 1},
      {1, -1, 1, -1},
      {1, -1, 1, 1},
      {1, 1, -1, -1},
      {1, 1, -1, 1},
      {1, 1, 1, -1},
      {1, 1, 1, 1}};
#ifdef __CUDACC__
    checkCuda( cudaMemcpyToSymbol( cHel, tHel, ncomb * mgOnGpu::npar * sizeof(short) ) );
#else
    memcpy( cHel, tHel, ncomb * mgOnGpu::npar * sizeof(short) );
#endif
  }

  //--------------------------------------------------------------------------

  CPPProcess::~CPPProcess() {}

  //--------------------------------------------------------------------------

#ifndef MGONGPU_HARDCODE_CIPC
  // Initialize process (with parameters read from user cards)
  void CPPProcess::initProc( const std::string& param_card_name )
  {
    // Instantiate the model class and set parameters that stay fixed during run
    m_pars = Parameters_sm::getInstance();
    SLHAReader slha( param_card_name, m_verbose );
    m_pars->setIndependentParameters( slha );
    m_pars->setIndependentCouplings();
    m_pars->setDependentParameters();
    m_pars->setDependentCouplings();
    if ( m_verbose )
    {
      m_pars->printIndependentParameters();
      m_pars->printIndependentCouplings();
      m_pars->printDependentParameters();
      m_pars->printDependentCouplings();
    }
    // Set external particle masses for this matrix element
    m_masses.push_back( m_pars->ZERO );
    m_masses.push_back( m_pars->ZERO );
    m_masses.push_back( m_pars->mdl_MT );
    m_masses.push_back( m_pars->mdl_MT );
    // Read physics parameters like masses and couplings from user configuration files (static: initialize once)
    // Then copy them to CUDA constant memory (issue #39) or its C++ emulation in file-scope static memory
    const cxtype tIPC[2] = { cxmake( m_pars->GC_10 ), cxmake( m_pars->GC_11 ) };
    const fptype tIPD[2] = { (fptype)m_pars->mdl_MT, (fptype)m_pars->mdl_WT };
#ifdef __CUDACC__
    checkCuda( cudaMemcpyToSymbol( cIPC, tIPC, 2 * sizeof(cxtype) ) );
    checkCuda( cudaMemcpyToSymbol( cIPD, tIPD, 2 * sizeof(fptype) ) );
#else
    memcpy( cIPC, tIPC, 2 * sizeof(cxtype) );
    memcpy( cIPD, tIPD, 2 * sizeof(fptype) );
#endif
    //for ( i=0; i<3; i++ ) std::cout << std::setprecision(17) << "tIPC[i] = " << tIPC[i] << std::endl;
    //for ( i=0; i<2; i++ ) std::cout << std::setprecision(17) << "tIPD[i] = " << tIPD[i] << std::endl;
  }
#else
  // Initialize process (with hardcoded parameters)
  void CPPProcess::initProc( const std::string& /*param_card_name*/ )
  {
    // Use hardcoded physics parameters
    if ( m_verbose )
    {
      Parameters_sm::printIndependentParameters();
      Parameters_sm::printIndependentCouplings();
      Parameters_sm::printDependentParameters();
      Parameters_sm::printDependentCouplings();
    }
    // Set external particle masses for this matrix element
    m_masses.push_back( Parameters_sm::ZERO );
    m_masses.push_back( Parameters_sm::ZERO );
    m_masses.push_back( Parameters_sm::ZERO );
    m_masses.push_back( Parameters_sm::ZERO );
  }
#endif

  //--------------------------------------------------------------------------

  // Retrieve the compiler that was used to build this module
  const std::string CPPProcess::getCompiler()
  {
    std::stringstream out;
    // CUDA version (NVCC)
    // [Use __NVCC__ instead of __CUDACC__ here!]
    // [This tests if 'nvcc' was used even to build a .cc file, even if not necessarily 'nvcc -x cu' for a .cu file]
    // [Check 'nvcc --compiler-options -dM -E dummy.c | grep CUDA': see https://stackoverflow.com/a/53713712]
#ifdef __NVCC__
#if defined __CUDACC_VER_MAJOR__ && defined __CUDACC_VER_MINOR__ && defined __CUDACC_VER_BUILD__
    out << "nvcc " << __CUDACC_VER_MAJOR__ << "." << __CUDACC_VER_MINOR__ << "." << __CUDACC_VER_BUILD__;
#else
    out << "nvcc UNKNOWN";
#endif
    out << " (";
#endif
    // ICX version (either as CXX or as host compiler inside NVCC)
#if defined __INTEL_COMPILER
#error "icc is no longer supported: please use icx"
#elif defined __INTEL_LLVM_COMPILER // alternative: __INTEL_CLANG_COMPILER
    out << "icx " << __INTEL_LLVM_COMPILER;
#ifdef __NVCC__
    out << ", ";
#else
    out << " (";
#endif
#endif
    // CLANG version (either as CXX or as host compiler inside NVCC or inside ICX)
#if defined __clang__
#if defined __clang_major__ && defined __clang_minor__ && defined __clang_patchlevel__
    out << "clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__;
    // GCC toolchain version inside CLANG
    std::string tchainout;
    std::string tchaincmd = "readelf -p .comment $(${CXX} -print-libgcc-file-name) |& grep 'GCC: (GNU)' | grep -v Warning | sort -u | awk '{print $5}'";
    std::unique_ptr<FILE, decltype(&pclose)> tchainpipe( popen( tchaincmd.c_str(), "r" ), pclose );
    if ( !tchainpipe ) throw std::runtime_error( "`readelf ...` failed?" );
    std::array<char, 128> tchainbuf;
    while ( fgets( tchainbuf.data(), tchainbuf.size(), tchainpipe.get() ) != nullptr ) tchainout += tchainbuf.data();
    tchainout.pop_back(); // remove trailing newline
#if defined __NVCC__ or defined __INTEL_LLVM_COMPILER
    out << ", gcc " << tchainout;
#else
    out << " (gcc " << tchainout << ")";
#endif
#else
    out << "clang UNKNOWKN";
#endif
#else
    // GCC version (either as CXX or as host compiler inside NVCC)
#if defined __GNUC__ && defined __GNUC_MINOR__ && defined __GNUC_PATCHLEVEL__
    out << "gcc " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
#else
    out << "gcc UNKNOWKN";
#endif
#endif
#if defined __NVCC__ or defined __INTEL_LLVM_COMPILER
    out << ")";
#endif
    return out.str();
  }

  //--------------------------------------------------------------------------

#ifdef __CUDACC__
  __global__
  void sigmaKin_getGoodHel( const fptype* allmomenta, // input: momenta[nevt*npar*4]
                            fptype* allMEs,           // output: allMEs[nevt], |M|^2 final_avg_over_helicities
                            bool* isGoodHel )         // output: isGoodHel[ncomb] - device array
  {
    const int ievt = blockDim.x * blockIdx.x + threadIdx.x; // index of event (thread) in grid
    // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
    fptype allMEsLast = 0;
    for ( int ihel = 0; ihel < ncomb; ihel++ )
    {
      // NB: calculate_wavefunctions ADDS |M|^2 for a given ihel to the running sum of |M|^2 over helicities for the given event(s)
      calculate_wavefunctions( ihel, allmomenta, allMEs );
      if ( allMEs[ievt] != allMEsLast )
      {
        //if ( !isGoodHel[ihel] ) std::cout << "sigmaKin_getGoodHel ihel=" << ihel << " TRUE" << std::endl;
        isGoodHel[ihel] = true;
      }
      allMEsLast = allMEs[ievt]; // running sum up to helicity ihel for event ievt
    }
  }
#else
  void sigmaKin_getGoodHel( const fptype* allmomenta, // input: momenta[nevt*npar*4]
                            fptype* allMEs,           // output: allMEs[nevt], |M|^2 final_avg_over_helicities
                            bool* isGoodHel           // output: isGoodHel[ncomb] - device array
                            , const int nevt )        // input: #events (for cuda: nevt == ndim == gpublocks*gputhreads)
  {
    //assert( (size_t)(allmomenta) % mgOnGpu::cppAlign == 0 ); // SANITY CHECK: require SIMD-friendly alignment [COMMENT OUT TO TEST MISALIGNED ACCESS]
    //assert( (size_t)(allMEs) % mgOnGpu::cppAlign == 0 ); // SANITY CHECK: require SIMD-friendly alignment [COMMENT OUT TO TEST MISALIGNED ACCESS]
    const int maxtry0 = ( neppV > 16 ? neppV : 16 ); // 16, but at least neppV (otherwise the npagV loop does not even start)
    fptype allMEsLast[maxtry0] = {0}; // all zeros https://en.cppreference.com/w/c/language/array_initialization#Notes
    const int maxtry = std::min( maxtry0, nevt ); // 16, but at most nevt (avoid invalid memory access if nevt<maxtry0)
    for ( int ievt = 0; ievt < maxtry; ++ievt )
    {
      // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
      allMEs[ievt] = 0; // all zeros
    }
    for ( int ihel = 0; ihel < ncomb; ihel++ )
    {
      //std::cout << "sigmaKin_getGoodHel ihel=" << ihel << ( isGoodHel[ihel] ? " true" : " false" ) << std::endl;
      calculate_wavefunctions( ihel, allmomenta, allMEs, maxtry );
      for ( int ievt = 0; ievt < maxtry; ++ievt )
      {
        // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
        const bool differs = ( allMEs[ievt] != allMEsLast[ievt] );
        if ( differs )
        {
          //if ( !isGoodHel[ihel] ) std::cout << "sigmaKin_getGoodHel ihel=" << ihel << " TRUE" << std::endl;
          isGoodHel[ihel] = true;
        }
        allMEsLast[ievt] = allMEs[ievt]; // running sum up to helicity ihel
      }
    }
  }
#endif

  //--------------------------------------------------------------------------

  void sigmaKin_setGoodHel( const bool* isGoodHel ) // input: isGoodHel[ncomb] - host array
  {
    int nGoodHel = 0; // FIXME: assume process.nprocesses == 1 for the moment (eventually nGoodHel[nprocesses]?)
    int goodHel[ncomb] = {0}; // all zeros https://en.cppreference.com/w/c/language/array_initialization#Notes
    for ( int ihel = 0; ihel < ncomb; ihel++ )
    {
      //std::cout << "sigmaKin_setGoodHel ihel=" << ihel << ( isGoodHel[ihel] ? " true" : " false" ) << std::endl;
      if ( isGoodHel[ihel] )
      {
        //goodHel[nGoodHel[0]] = ihel; // FIXME: assume process.nprocesses == 1 for the moment
        //nGoodHel[0]++; // FIXME: assume process.nprocesses == 1 for the moment
        goodHel[nGoodHel] = ihel;
        nGoodHel++;
      }
    }
#ifdef __CUDACC__
    checkCuda( cudaMemcpyToSymbol( cNGoodHel, &nGoodHel, sizeof(int) ) ); // FIXME: assume process.nprocesses == 1 for the moment
    checkCuda( cudaMemcpyToSymbol( cGoodHel, goodHel, ncomb*sizeof(int) ) );
#else
    cNGoodHel = nGoodHel;
    for ( int ihel = 0; ihel < ncomb; ihel++ ) cGoodHel[ihel] = goodHel[ihel];
#endif
  }

  //--------------------------------------------------------------------------
  // Evaluate |M|^2, part independent of incoming flavour
  // FIXME: assume process.nprocesses == 1 (eventually: allMEs[nevt] -> allMEs[nevt*nprocesses]?)

  __global__
  void sigmaKin( const fptype* allmomenta, // input: momenta[nevt*npar*4]
                 fptype* allMEs            // output: allMEs[nevt], |M|^2 final_avg_over_helicities
#ifndef __CUDACC__
                 , const int nevt          // input: #events (for cuda: nevt == ndim == gpublocks*gputhreads)
#endif
                 )
  {
    mgDebugInitialise();

    // Denominators: spins, colors and identical particles
    const int denominators = 256; // FIXME: assume process.nprocesses == 1 for the moment (eventually denominators[nprocesses]?)

    // Set the parameters which change event by event
    // Need to discuss this with Stefan
    //m_pars->setDependentParameters();
    //m_pars->setDependentCouplings();

#ifdef __CUDACC__
    // Remember: in CUDA this is a kernel for one event, in c++ this processes n events
    const int ievt = blockDim.x * blockIdx.x + threadIdx.x; // index of event (thread) in grid
    //printf( "sigmakin: ievt %d\n", ievt );
#else
    //assert( (size_t)(allmomenta) % mgOnGpu::cppAlign == 0 ); // SANITY CHECK: require SIMD-friendly alignment [COMMENT OUT TO TEST MISALIGNED ACCESS]
    //assert( (size_t)(allMEs) % mgOnGpu::cppAlign == 0 ); // SANITY CHECK: require SIMD-friendly alignment [COMMENT OUT TO TEST MISALIGNED ACCESS]
#endif

    // Start sigmaKin_lines
    // PART 0 - INITIALISATION (before calculate_wavefunctions)
    // Reset the "matrix elements" - running sums of |M|^2 over helicities for the given event
    // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
#ifdef __CUDACC__
    allMEs[ievt] = 0;
#else
    const int npagV = nevt/neppV;
    for ( int ipagV = 0; ipagV < npagV; ++ipagV )
    {
      for ( int ieppV=0; ieppV<neppV; ieppV++ )
        allMEs[ipagV*neppV + ieppV] = 0; // all zeros
    }
#endif

    // PART 1 - HELICITY LOOP: CALCULATE WAVEFUNCTIONS
    // (in both CUDA and C++, using precomputed good helicities)
    // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
    for ( int ighel = 0; ighel < cNGoodHel; ighel++ )
    {
      const int ihel = cGoodHel[ighel];
#ifdef __CUDACC__
      calculate_wavefunctions( ihel, allmomenta, allMEs );
#else
      calculate_wavefunctions( ihel, allmomenta, allMEs, nevt );
#endif
      //if ( ighel == 0 ) break; // TEST sectors/requests (issue #16)
    }

    // PART 2 - FINALISATION (after calculate_wavefunctions)
    // Get the final |M|^2 as an average over helicities/colors of the running sum of |M|^2 over helicities for the given event
    // [NB 'sum over final spins, average over initial spins', eg see
    // https://www.uzh.ch/cmsssl/physik/dam/jcr:2e24b7b1-f4d7-4160-817e-47b13dbf1d7c/Handout_4_2016-UZH.pdf]
    // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
#ifdef __CUDACC__
    allMEs[ievt] /= denominators;
#else
    for ( int ipagV = 0; ipagV < npagV; ++ipagV )
    {
      for ( int ieppV=0; ieppV<neppV; ieppV++ )
        allMEs[ipagV*neppV + ieppV] /= denominators;
    }
#endif
    mgDebugFinalise();
  }

  //--------------------------------------------------------------------------

} // end namespace

//==========================================================================

