//==========================================================================
// This file has been automatically generated for CUDA/C++ standalone by
// MadGraph5_aMC@NLO v. 2.9.5, 2021-08-22
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#include <CL/sycl.hpp>
#include "../../src/HelAmps_sm.cc"

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <memory>

#include "mgOnGpuConfig.h"
#include "mgOnGpuTypes.h"

#include "CPPProcess.h"
#include "extras.h"

#undef MGONGPU_TEST_DIVERGENCE
//#define MGONGPU_TEST_DIVERGENCE 1

//==========================================================================
// Class member functions for calculating the matrix elements for
// Process: g g > t t~ g g WEIGHTED<=4 @1

namespace Proc
{
  using mgOnGpu::np4; // dimensions of 4-momenta (E,px,py,pz)
  using mgOnGpu::npar; // #particles in total (external = initial + final): e.g. 4 for e+ e- -> mu+ mu-
  using mgOnGpu::ncomb; // #helicity combinations: e.g. 16 for e+ e- -> mu+ mu- (2**4 = fermion spin up/down ** npar)

  using mgOnGpu::nwf; // #wavefunctions = #external (npar) + #internal: e.g. 5 for e+ e- -> mu+ mu- (1 internal is gamma or Z)
  using mgOnGpu::nw6; // dimensions of each wavefunction (HELAS KEK 91-11): e.g. 6 for e+ e- -> mu+ mu- (fermions and vectors)

  // Physics parameters (masses, coupling, etc...)
#ifdef MGONGPU_HARDCODE_CIPC
  __device__ const fptype cIPC[6] = {
    Parameters_sm::GC_10.real(), Parameters_sm::GC_10.imag(),
    Parameters_sm::GC_11.real(), Parameters_sm::GC_11.imag(),
    Parameters_sm::GC_12.real(), Parameters_sm::GC_12.imag() };
  __device__ const fptype cIPD[2] = {
    Parameters_sm::mdl_MT,
    Parameters_sm::mdl_WT };
#else
  static fptype cIPC[6];
  static fptype cIPD[2];
#endif

  // Helicity combinations (and filtering of "good" helicity combinations)
  static short cHel[ncomb][npar];

  //--------------------------------------------------------------------------

    // Evaluate |M|^2 for each subprocess
    // NB: calculate_wavefunctions ADDS |M|^2 for a given ihel to the running sum of |M|^2 over helicities for the given event(s)
    void calculate_wavefunctions( int ihel,
                                  const fptype *allmomenta,
                                  fptype *allMEs,
                                  size_t ievt,
                                  short *cHel,
                                  const fptype *cIPC,
                                  const fptype *cIPD
                                ) {
        using namespace MG5_sm;
        mgDebug( 0, __FUNCTION__ );

    // The number of colors
    constexpr int ncolor = 24;

    // Local TEMPORARY variables for a subset of Feynman diagrams in the given CUDA event (ievt) or C++ event page (ipagV)
    // [NB these variables are reused several times (and re-initialised each time) within the same event or event page]
    cxtype_sv w_sv[nwf][nw6]; // particle wavefunctions within Feynman diagrams (nw6 is often 6, the dimension of spin 1/2 or spin 1 particles)
    cxtype_sv amp_sv[1]; // invariant amplitude for one given Feynman diagram

    // Local variables for the given CUDA event (ievt) or C++ event page (ipagV)
    cxtype_sv jamp_sv[ncolor] = {}; // sum of the invariant amplitudes for all Feynman diagrams in the event or event page

    // === Calculate wavefunctions and amplitudes for all diagrams in all processes - Loop over nevt events ===

    {
      // Reset color flows (reset jamp_sv) at the beginning of a new event or event page
      for( int i=0; i<ncolor; i++ ){ jamp_sv[i] = cxzero_sv(); }

      // *** DIAGRAM 1 OF 123 ***

      // Wavefunction(s) for diagram number 1
#ifdef SYCL_LANGUAGE_VERSION
      vxxxxx(allmomenta, ievt, 0., cHel[ihel*npar + 0], -1, w_sv[0], 0);
#else
      vxxxxx(p4IparIpagV( allmomenta, 0, ipagV ), ievt, 0., cHel[ihel*npar + 0], -1, w_sv[0] );
#endif

#ifdef SYCL_LANGUAGE_VERSION
      vxxxxx(allmomenta, ievt, 0., cHel[ihel*npar + 1], -1, w_sv[1], 1);
#else
      vxxxxx(p4IparIpagV( allmomenta, 1, ipagV ), ievt, 0., cHel[ihel*npar + 1], -1, w_sv[1] );
#endif

#ifdef SYCL_LANGUAGE_VERSION
      oxxxxx(allmomenta, ievt, cIPD[0], cHel[ihel*npar + 2], +1, w_sv[2], 2);
#else
      oxxxxx(p4IparIpagV( allmomenta, 2, ipagV ), ievt, cIPD[0], cHel[ihel*npar + 2], +1, w_sv[2] );
#endif

#ifdef SYCL_LANGUAGE_VERSION
      ixxxxx(allmomenta, ievt, cIPD[0], cHel[ihel*npar + 3], -1, w_sv[3], 3);
#else
      ixxxxx(p4IparIpagV( allmomenta, 3, ipagV ), ievt, cIPD[0], cHel[ihel*npar + 3], -1, w_sv[3] );
#endif

#ifdef SYCL_LANGUAGE_VERSION
      vxxxxx(allmomenta, ievt, 0., cHel[ihel*npar + 4], +1, w_sv[4], 4);
#else
      vxxxxx(p4IparIpagV( allmomenta, 4, ipagV ), ievt, 0., cHel[ihel*npar + 4], +1, w_sv[4] );
#endif

#ifdef SYCL_LANGUAGE_VERSION
      vxxxxx(allmomenta, ievt, 0., cHel[ihel*npar + 5], +1, w_sv[5], 5);
#else
      vxxxxx(p4IparIpagV( allmomenta, 5, ipagV ), ievt, 0., cHel[ihel*npar + 5], +1, w_sv[5] );
#endif

      VVV1P0_1( w_sv[0], w_sv[1], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[6] );
      FFV1P0_3( w_sv[3], w_sv[2], cxmake( cIPC[2], cIPC[3] ), 0., 0., w_sv[7] );

      // Amplitude(s) for diagram number 1
      VVVV1_0( w_sv[6], w_sv[7], w_sv[4], w_sv[5], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[1] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[6] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[7] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[17] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];
      VVVV3_0( w_sv[6], w_sv[7], w_sv[4], w_sv[5], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[6] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];
      VVVV4_0( w_sv[6], w_sv[7], w_sv[4], w_sv[5], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 2 OF 123 ***

      // Wavefunction(s) for diagram number 2
      VVV1P0_1( w_sv[6], w_sv[4], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[8] );

      // Amplitude(s) for diagram number 2
      VVV1_0( w_sv[7], w_sv[5], w_sv[8], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[6] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 3 OF 123 ***

      // Wavefunction(s) for diagram number 3
      VVV1P0_1( w_sv[6], w_sv[5], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[9] );

      // Amplitude(s) for diagram number 3
      VVV1_0( w_sv[7], w_sv[4], w_sv[9], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 4 OF 123 ***

      // Wavefunction(s) for diagram number 4
      VVV1P0_1( w_sv[4], w_sv[5], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[10] );

      // Amplitude(s) for diagram number 4
      VVV1_0( w_sv[6], w_sv[7], w_sv[10], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[1] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[6] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[7] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[17] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 5 OF 123 ***

      // Wavefunction(s) for diagram number 5
      FFV1_1( w_sv[2], w_sv[4], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[11] );
      FFV1_2( w_sv[3], w_sv[6], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[12] );

      // Amplitude(s) for diagram number 5
      FFV1_0( w_sv[12], w_sv[11], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 6 OF 123 ***

      // Wavefunction(s) for diagram number 6
      // (none)

      // Amplitude(s) for diagram number 6
      FFV1_0( w_sv[3], w_sv[11], w_sv[9], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[12] += +amp_sv[0];
      jamp_sv[14] -= amp_sv[0];
      jamp_sv[16] -= amp_sv[0];
      jamp_sv[17] += +amp_sv[0];

      // *** DIAGRAM 7 OF 123 ***

      // Wavefunction(s) for diagram number 7
      FFV1_2( w_sv[3], w_sv[5], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[13] );

      // Amplitude(s) for diagram number 7
      FFV1_0( w_sv[13], w_sv[11], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[12] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[14] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 8 OF 123 ***

      // Wavefunction(s) for diagram number 8
      FFV1_1( w_sv[2], w_sv[5], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[14] );

      // Amplitude(s) for diagram number 8
      FFV1_0( w_sv[12], w_sv[14], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[22] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 9 OF 123 ***

      // Wavefunction(s) for diagram number 9
      // (none)

      // Amplitude(s) for diagram number 9
      FFV1_0( w_sv[3], w_sv[14], w_sv[8], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[18] += +amp_sv[0];
      jamp_sv[20] -= amp_sv[0];
      jamp_sv[22] -= amp_sv[0];
      jamp_sv[23] += +amp_sv[0];

      // *** DIAGRAM 10 OF 123 ***

      // Wavefunction(s) for diagram number 10
      FFV1_2( w_sv[3], w_sv[4], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[15] );

      // Amplitude(s) for diagram number 10
      FFV1_0( w_sv[15], w_sv[14], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[18] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[20] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 11 OF 123 ***

      // Wavefunction(s) for diagram number 11
      FFV1_1( w_sv[2], w_sv[6], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[16] );

      // Amplitude(s) for diagram number 11
      FFV1_0( w_sv[15], w_sv[16], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 12 OF 123 ***

      // Wavefunction(s) for diagram number 12
      // (none)

      // Amplitude(s) for diagram number 12
      FFV1_0( w_sv[15], w_sv[2], w_sv[9], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[1] += +amp_sv[0];
      jamp_sv[7] -= amp_sv[0];
      jamp_sv[18] -= amp_sv[0];
      jamp_sv[20] += +amp_sv[0];

      // *** DIAGRAM 13 OF 123 ***

      // Wavefunction(s) for diagram number 13
      // (none)

      // Amplitude(s) for diagram number 13
      FFV1_0( w_sv[13], w_sv[16], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[6] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 14 OF 123 ***

      // Wavefunction(s) for diagram number 14
      // (none)

      // Amplitude(s) for diagram number 14
      FFV1_0( w_sv[13], w_sv[2], w_sv[8], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] += +amp_sv[0];
      jamp_sv[6] -= amp_sv[0];
      jamp_sv[12] -= amp_sv[0];
      jamp_sv[14] += +amp_sv[0];

      // *** DIAGRAM 15 OF 123 ***

      // Wavefunction(s) for diagram number 15
      // (none)

      // Amplitude(s) for diagram number 15
      FFV1_0( w_sv[3], w_sv[16], w_sv[10], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] += +amp_sv[0];
      jamp_sv[1] -= amp_sv[0];
      jamp_sv[6] -= amp_sv[0];
      jamp_sv[7] += +amp_sv[0];

      // *** DIAGRAM 16 OF 123 ***

      // Wavefunction(s) for diagram number 16
      // (none)

      // Amplitude(s) for diagram number 16
      FFV1_0( w_sv[12], w_sv[2], w_sv[10], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[16] += +amp_sv[0];
      jamp_sv[17] -= amp_sv[0];
      jamp_sv[22] -= amp_sv[0];
      jamp_sv[23] += +amp_sv[0];

      // *** DIAGRAM 17 OF 123 ***

      // Wavefunction(s) for diagram number 17
      FFV1_1( w_sv[2], w_sv[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[12] );
      FFV1_2( w_sv[3], w_sv[1], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[16] );
      FFV1_1( w_sv[12], w_sv[4], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[8] );

      // Amplitude(s) for diagram number 17
      FFV1_0( w_sv[16], w_sv[8], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[3] -= amp_sv[0];

      // *** DIAGRAM 18 OF 123 ***

      // Wavefunction(s) for diagram number 18
      FFV1_1( w_sv[12], w_sv[5], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[9] );

      // Amplitude(s) for diagram number 18
      FFV1_0( w_sv[16], w_sv[9], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[5] -= amp_sv[0];

      // *** DIAGRAM 19 OF 123 ***

      // Wavefunction(s) for diagram number 19
      // (none)

      // Amplitude(s) for diagram number 19
      FFV1_0( w_sv[16], w_sv[12], w_sv[10], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[3] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[5] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 20 OF 123 ***

      // Wavefunction(s) for diagram number 20
      VVV1P0_1( w_sv[1], w_sv[4], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[6] );
      FFV1P0_3( w_sv[3], w_sv[12], cxmake( cIPC[2], cIPC[3] ), 0., 0., w_sv[17] );

      // Amplitude(s) for diagram number 20
      VVV1_0( w_sv[6], w_sv[5], w_sv[17], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] += +amp_sv[0];
      jamp_sv[2] -= amp_sv[0];
      jamp_sv[4] -= amp_sv[0];
      jamp_sv[5] += +amp_sv[0];

      // *** DIAGRAM 21 OF 123 ***

      // Wavefunction(s) for diagram number 21
      // (none)

      // Amplitude(s) for diagram number 21
      FFV1_0( w_sv[3], w_sv[9], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[4] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[5] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 22 OF 123 ***

      // Wavefunction(s) for diagram number 22
      // (none)

      // Amplitude(s) for diagram number 22
      FFV1_0( w_sv[13], w_sv[12], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 23 OF 123 ***

      // Wavefunction(s) for diagram number 23
      VVV1P0_1( w_sv[1], w_sv[5], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[18] );

      // Amplitude(s) for diagram number 23
      VVV1_0( w_sv[18], w_sv[4], w_sv[17], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[1] += +amp_sv[0];
      jamp_sv[2] -= amp_sv[0];
      jamp_sv[3] += +amp_sv[0];
      jamp_sv[4] -= amp_sv[0];

      // *** DIAGRAM 24 OF 123 ***

      // Wavefunction(s) for diagram number 24
      // (none)

      // Amplitude(s) for diagram number 24
      FFV1_0( w_sv[3], w_sv[8], w_sv[18], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[2] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[3] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 25 OF 123 ***

      // Wavefunction(s) for diagram number 25
      // (none)

      // Amplitude(s) for diagram number 25
      FFV1_0( w_sv[15], w_sv[12], w_sv[18], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 26 OF 123 ***

      // Wavefunction(s) for diagram number 26
      FFV1_1( w_sv[12], w_sv[1], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[19] );

      // Amplitude(s) for diagram number 26
      FFV1_0( w_sv[15], w_sv[19], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[1] -= amp_sv[0];

      // *** DIAGRAM 27 OF 123 ***

      // Wavefunction(s) for diagram number 27
      // (none)

      // Amplitude(s) for diagram number 27
      FFV1_0( w_sv[15], w_sv[9], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[4] -= amp_sv[0];

      // *** DIAGRAM 28 OF 123 ***

      // Wavefunction(s) for diagram number 28
      // (none)

      // Amplitude(s) for diagram number 28
      FFV1_0( w_sv[13], w_sv[19], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] -= amp_sv[0];

      // *** DIAGRAM 29 OF 123 ***

      // Wavefunction(s) for diagram number 29
      // (none)

      // Amplitude(s) for diagram number 29
      FFV1_0( w_sv[13], w_sv[8], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[2] -= amp_sv[0];

      // *** DIAGRAM 30 OF 123 ***

      // Wavefunction(s) for diagram number 30
      // (none)

      // Amplitude(s) for diagram number 30
      FFV1_0( w_sv[3], w_sv[19], w_sv[10], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[1] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 31 OF 123 ***

      // Wavefunction(s) for diagram number 31
      // (none)

      // Amplitude(s) for diagram number 31
      VVV1_0( w_sv[1], w_sv[10], w_sv[17], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] += +amp_sv[0];
      jamp_sv[1] -= amp_sv[0];
      jamp_sv[3] -= amp_sv[0];
      jamp_sv[5] += +amp_sv[0];

      // *** DIAGRAM 32 OF 123 ***

      // Wavefunction(s) for diagram number 32
      VVVV1P0_1( w_sv[1], w_sv[4], w_sv[5], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[17] );
      VVVV3P0_1( w_sv[1], w_sv[4], w_sv[5], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[19] );
      VVVV4P0_1( w_sv[1], w_sv[4], w_sv[5], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[8] );

      // Amplitude(s) for diagram number 32
      FFV1_0( w_sv[3], w_sv[12], w_sv[17], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] += +amp_sv[0];
      jamp_sv[1] -= amp_sv[0];
      jamp_sv[3] -= amp_sv[0];
      jamp_sv[5] += +amp_sv[0];
      FFV1_0( w_sv[3], w_sv[12], w_sv[19], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[1] -= amp_sv[0];
      jamp_sv[2] += +amp_sv[0];
      jamp_sv[3] -= amp_sv[0];
      jamp_sv[4] += +amp_sv[0];
      FFV1_0( w_sv[3], w_sv[12], w_sv[8], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] -= amp_sv[0];
      jamp_sv[2] += +amp_sv[0];
      jamp_sv[4] += +amp_sv[0];
      jamp_sv[5] -= amp_sv[0];

      // *** DIAGRAM 33 OF 123 ***

      // Wavefunction(s) for diagram number 33
      FFV1_2( w_sv[3], w_sv[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[12] );
      FFV1_1( w_sv[2], w_sv[1], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[9] );
      FFV1_2( w_sv[12], w_sv[4], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[20] );

      // Amplitude(s) for diagram number 33
      FFV1_0( w_sv[20], w_sv[9], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[11] -= amp_sv[0];

      // *** DIAGRAM 34 OF 123 ***

      // Wavefunction(s) for diagram number 34
      FFV1_2( w_sv[12], w_sv[5], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[21] );

      // Amplitude(s) for diagram number 34
      FFV1_0( w_sv[21], w_sv[9], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[9] -= amp_sv[0];

      // *** DIAGRAM 35 OF 123 ***

      // Wavefunction(s) for diagram number 35
      // (none)

      // Amplitude(s) for diagram number 35
      FFV1_0( w_sv[12], w_sv[9], w_sv[10], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[9] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[11] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 36 OF 123 ***

      // Wavefunction(s) for diagram number 36
      FFV1P0_3( w_sv[12], w_sv[2], cxmake( cIPC[2], cIPC[3] ), 0., 0., w_sv[22] );

      // Amplitude(s) for diagram number 36
      VVV1_0( w_sv[6], w_sv[5], w_sv[22], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[9] += +amp_sv[0];
      jamp_sv[15] -= amp_sv[0];
      jamp_sv[21] -= amp_sv[0];
      jamp_sv[23] += +amp_sv[0];

      // *** DIAGRAM 37 OF 123 ***

      // Wavefunction(s) for diagram number 37
      // (none)

      // Amplitude(s) for diagram number 37
      FFV1_0( w_sv[21], w_sv[2], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[9] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[15] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 38 OF 123 ***

      // Wavefunction(s) for diagram number 38
      // (none)

      // Amplitude(s) for diagram number 38
      FFV1_0( w_sv[12], w_sv[14], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 39 OF 123 ***

      // Wavefunction(s) for diagram number 39
      // (none)

      // Amplitude(s) for diagram number 39
      VVV1_0( w_sv[18], w_sv[4], w_sv[22], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[11] += +amp_sv[0];
      jamp_sv[15] -= amp_sv[0];
      jamp_sv[17] += +amp_sv[0];
      jamp_sv[21] -= amp_sv[0];

      // *** DIAGRAM 40 OF 123 ***

      // Wavefunction(s) for diagram number 40
      // (none)

      // Amplitude(s) for diagram number 40
      FFV1_0( w_sv[20], w_sv[2], w_sv[18], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[11] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 41 OF 123 ***

      // Wavefunction(s) for diagram number 41
      // (none)

      // Amplitude(s) for diagram number 41
      FFV1_0( w_sv[12], w_sv[11], w_sv[18], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 42 OF 123 ***

      // Wavefunction(s) for diagram number 42
      FFV1_2( w_sv[12], w_sv[1], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[23] );

      // Amplitude(s) for diagram number 42
      FFV1_0( w_sv[23], w_sv[11], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[17] -= amp_sv[0];

      // *** DIAGRAM 43 OF 123 ***

      // Wavefunction(s) for diagram number 43
      // (none)

      // Amplitude(s) for diagram number 43
      FFV1_0( w_sv[21], w_sv[11], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[15] -= amp_sv[0];

      // *** DIAGRAM 44 OF 123 ***

      // Wavefunction(s) for diagram number 44
      // (none)

      // Amplitude(s) for diagram number 44
      FFV1_0( w_sv[23], w_sv[14], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[23] -= amp_sv[0];

      // *** DIAGRAM 45 OF 123 ***

      // Wavefunction(s) for diagram number 45
      // (none)

      // Amplitude(s) for diagram number 45
      FFV1_0( w_sv[20], w_sv[14], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[21] -= amp_sv[0];

      // *** DIAGRAM 46 OF 123 ***

      // Wavefunction(s) for diagram number 46
      // (none)

      // Amplitude(s) for diagram number 46
      FFV1_0( w_sv[23], w_sv[2], w_sv[10], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[17] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 47 OF 123 ***

      // Wavefunction(s) for diagram number 47
      // (none)

      // Amplitude(s) for diagram number 47
      VVV1_0( w_sv[1], w_sv[10], w_sv[22], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[9] += +amp_sv[0];
      jamp_sv[11] -= amp_sv[0];
      jamp_sv[17] -= amp_sv[0];
      jamp_sv[23] += +amp_sv[0];

      // *** DIAGRAM 48 OF 123 ***

      // Wavefunction(s) for diagram number 48
      // (none)

      // Amplitude(s) for diagram number 48
      FFV1_0( w_sv[12], w_sv[2], w_sv[17], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[9] += +amp_sv[0];
      jamp_sv[11] -= amp_sv[0];
      jamp_sv[17] -= amp_sv[0];
      jamp_sv[23] += +amp_sv[0];
      FFV1_0( w_sv[12], w_sv[2], w_sv[19], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[11] -= amp_sv[0];
      jamp_sv[15] += +amp_sv[0];
      jamp_sv[17] -= amp_sv[0];
      jamp_sv[21] += +amp_sv[0];
      FFV1_0( w_sv[12], w_sv[2], w_sv[8], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[9] -= amp_sv[0];
      jamp_sv[15] += +amp_sv[0];
      jamp_sv[21] += +amp_sv[0];
      jamp_sv[23] -= amp_sv[0];

      // *** DIAGRAM 49 OF 123 ***

      // Wavefunction(s) for diagram number 49
      VVV1P0_1( w_sv[0], w_sv[4], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[12] );
      FFV1_2( w_sv[3], w_sv[12], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[22] );

      // Amplitude(s) for diagram number 49
      FFV1_0( w_sv[22], w_sv[9], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[10] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[11] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 50 OF 123 ***

      // Wavefunction(s) for diagram number 50
      VVV1P0_1( w_sv[12], w_sv[5], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[23] );

      // Amplitude(s) for diagram number 50
      FFV1_0( w_sv[3], w_sv[9], w_sv[23], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[6] += +amp_sv[0];
      jamp_sv[8] -= amp_sv[0];
      jamp_sv[10] -= amp_sv[0];
      jamp_sv[11] += +amp_sv[0];

      // *** DIAGRAM 51 OF 123 ***

      // Wavefunction(s) for diagram number 51
      // (none)

      // Amplitude(s) for diagram number 51
      FFV1_0( w_sv[13], w_sv[9], w_sv[12], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[6] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 52 OF 123 ***

      // Wavefunction(s) for diagram number 52
      FFV1_1( w_sv[2], w_sv[12], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[20] );

      // Amplitude(s) for diagram number 52
      FFV1_0( w_sv[16], w_sv[20], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[3] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[13] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 53 OF 123 ***

      // Wavefunction(s) for diagram number 53
      // (none)

      // Amplitude(s) for diagram number 53
      FFV1_0( w_sv[16], w_sv[2], w_sv[23], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[3] += +amp_sv[0];
      jamp_sv[13] -= amp_sv[0];
      jamp_sv[19] -= amp_sv[0];
      jamp_sv[22] += +amp_sv[0];

      // *** DIAGRAM 54 OF 123 ***

      // Wavefunction(s) for diagram number 54
      // (none)

      // Amplitude(s) for diagram number 54
      FFV1_0( w_sv[16], w_sv[14], w_sv[12], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 55 OF 123 ***

      // Wavefunction(s) for diagram number 55
      // (none)

      // Amplitude(s) for diagram number 55
      FFV1_0( w_sv[3], w_sv[20], w_sv[18], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[2] += +amp_sv[0];
      jamp_sv[3] -= amp_sv[0];
      jamp_sv[12] -= amp_sv[0];
      jamp_sv[13] += +amp_sv[0];

      // *** DIAGRAM 56 OF 123 ***

      // Wavefunction(s) for diagram number 56
      // (none)

      // Amplitude(s) for diagram number 56
      FFV1_0( w_sv[22], w_sv[2], w_sv[18], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[10] += +amp_sv[0];
      jamp_sv[11] -= amp_sv[0];
      jamp_sv[20] -= amp_sv[0];
      jamp_sv[21] += +amp_sv[0];

      // *** DIAGRAM 57 OF 123 ***

      // Wavefunction(s) for diagram number 57
      // (none)

      // Amplitude(s) for diagram number 57
      VVV1_0( w_sv[12], w_sv[18], w_sv[7], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[3] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[10] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[11] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[13] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 58 OF 123 ***

      // Wavefunction(s) for diagram number 58
      // (none)

      // Amplitude(s) for diagram number 58
      VVVV1_0( w_sv[12], w_sv[1], w_sv[7], w_sv[5], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[2] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[6] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[8] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[19] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[22] += +cxtype(0,1)*amp_sv[0];
      VVVV3_0( w_sv[12], w_sv[1], w_sv[7], w_sv[5], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[2] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[3] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] -= cxtype(0,1)*amp_sv[0];
      VVVV4_0( w_sv[12], w_sv[1], w_sv[7], w_sv[5], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[3] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[6] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 59 OF 123 ***

      // Wavefunction(s) for diagram number 59
      VVV1P0_1( w_sv[12], w_sv[1], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[21] );

      // Amplitude(s) for diagram number 59
      VVV1_0( w_sv[7], w_sv[5], w_sv[21], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[2] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[6] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[8] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[19] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[22] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 60 OF 123 ***

      // Wavefunction(s) for diagram number 60
      // (none)

      // Amplitude(s) for diagram number 60
      VVV1_0( w_sv[1], w_sv[7], w_sv[23], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[3] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[6] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 61 OF 123 ***

      // Wavefunction(s) for diagram number 61
      // (none)

      // Amplitude(s) for diagram number 61
      FFV1_0( w_sv[3], w_sv[14], w_sv[21], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[19] += +amp_sv[0];
      jamp_sv[20] -= amp_sv[0];
      jamp_sv[21] += +amp_sv[0];
      jamp_sv[22] -= amp_sv[0];

      // *** DIAGRAM 62 OF 123 ***

      // Wavefunction(s) for diagram number 62
      // (none)

      // Amplitude(s) for diagram number 62
      FFV1_0( w_sv[22], w_sv[14], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 63 OF 123 ***

      // Wavefunction(s) for diagram number 63
      // (none)

      // Amplitude(s) for diagram number 63
      FFV1_0( w_sv[13], w_sv[2], w_sv[21], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[2] += +amp_sv[0];
      jamp_sv[6] -= amp_sv[0];
      jamp_sv[8] += +amp_sv[0];
      jamp_sv[12] -= amp_sv[0];

      // *** DIAGRAM 64 OF 123 ***

      // Wavefunction(s) for diagram number 64
      // (none)

      // Amplitude(s) for diagram number 64
      FFV1_0( w_sv[13], w_sv[20], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[2] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 65 OF 123 ***

      // Wavefunction(s) for diagram number 65
      VVV1P0_1( w_sv[0], w_sv[5], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[20] );
      FFV1_2( w_sv[3], w_sv[20], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[21] );

      // Amplitude(s) for diagram number 65
      FFV1_0( w_sv[21], w_sv[9], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[8] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 66 OF 123 ***

      // Wavefunction(s) for diagram number 66
      VVV1P0_1( w_sv[20], w_sv[4], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[22] );

      // Amplitude(s) for diagram number 66
      FFV1_0( w_sv[3], w_sv[9], w_sv[22], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[7] += +amp_sv[0];
      jamp_sv[8] -= amp_sv[0];
      jamp_sv[9] += +amp_sv[0];
      jamp_sv[10] -= amp_sv[0];

      // *** DIAGRAM 67 OF 123 ***

      // Wavefunction(s) for diagram number 67
      // (none)

      // Amplitude(s) for diagram number 67
      FFV1_0( w_sv[15], w_sv[9], w_sv[20], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[7] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 68 OF 123 ***

      // Wavefunction(s) for diagram number 68
      FFV1_1( w_sv[2], w_sv[20], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[23] );

      // Amplitude(s) for diagram number 68
      FFV1_0( w_sv[16], w_sv[23], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[19] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 69 OF 123 ***

      // Wavefunction(s) for diagram number 69
      // (none)

      // Amplitude(s) for diagram number 69
      FFV1_0( w_sv[16], w_sv[2], w_sv[22], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[5] += +amp_sv[0];
      jamp_sv[13] -= amp_sv[0];
      jamp_sv[16] += +amp_sv[0];
      jamp_sv[19] -= amp_sv[0];

      // *** DIAGRAM 70 OF 123 ***

      // Wavefunction(s) for diagram number 70
      // (none)

      // Amplitude(s) for diagram number 70
      FFV1_0( w_sv[16], w_sv[11], w_sv[20], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 71 OF 123 ***

      // Wavefunction(s) for diagram number 71
      // (none)

      // Amplitude(s) for diagram number 71
      FFV1_0( w_sv[3], w_sv[23], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[4] += +amp_sv[0];
      jamp_sv[5] -= amp_sv[0];
      jamp_sv[18] -= amp_sv[0];
      jamp_sv[19] += +amp_sv[0];

      // *** DIAGRAM 72 OF 123 ***

      // Wavefunction(s) for diagram number 72
      // (none)

      // Amplitude(s) for diagram number 72
      FFV1_0( w_sv[21], w_sv[2], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[8] += +amp_sv[0];
      jamp_sv[9] -= amp_sv[0];
      jamp_sv[14] -= amp_sv[0];
      jamp_sv[15] += +amp_sv[0];

      // *** DIAGRAM 73 OF 123 ***

      // Wavefunction(s) for diagram number 73
      // (none)

      // Amplitude(s) for diagram number 73
      VVV1_0( w_sv[20], w_sv[6], w_sv[7], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[8] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[19] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 74 OF 123 ***

      // Wavefunction(s) for diagram number 74
      // (none)

      // Amplitude(s) for diagram number 74
      VVVV1_0( w_sv[20], w_sv[1], w_sv[7], w_sv[4], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[4] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[10] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[13] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[15] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];
      VVVV3_0( w_sv[20], w_sv[1], w_sv[7], w_sv[4], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[4] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[5] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[9] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[15] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];
      VVVV4_0( w_sv[20], w_sv[1], w_sv[7], w_sv[4], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[5] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[7] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[9] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 75 OF 123 ***

      // Wavefunction(s) for diagram number 75
      VVV1P0_1( w_sv[20], w_sv[1], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[12] );

      // Amplitude(s) for diagram number 75
      VVV1_0( w_sv[7], w_sv[4], w_sv[12], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[4] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[10] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[13] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[15] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 76 OF 123 ***

      // Wavefunction(s) for diagram number 76
      // (none)

      // Amplitude(s) for diagram number 76
      VVV1_0( w_sv[1], w_sv[7], w_sv[22], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[5] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[7] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[9] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 77 OF 123 ***

      // Wavefunction(s) for diagram number 77
      // (none)

      // Amplitude(s) for diagram number 77
      FFV1_0( w_sv[3], w_sv[11], w_sv[12], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[13] += +amp_sv[0];
      jamp_sv[14] -= amp_sv[0];
      jamp_sv[15] += +amp_sv[0];
      jamp_sv[16] -= amp_sv[0];

      // *** DIAGRAM 78 OF 123 ***

      // Wavefunction(s) for diagram number 78
      // (none)

      // Amplitude(s) for diagram number 78
      FFV1_0( w_sv[21], w_sv[11], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[15] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 79 OF 123 ***

      // Wavefunction(s) for diagram number 79
      // (none)

      // Amplitude(s) for diagram number 79
      FFV1_0( w_sv[15], w_sv[2], w_sv[12], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[4] += +amp_sv[0];
      jamp_sv[7] -= amp_sv[0];
      jamp_sv[10] += +amp_sv[0];
      jamp_sv[18] -= amp_sv[0];

      // *** DIAGRAM 80 OF 123 ***

      // Wavefunction(s) for diagram number 80
      // (none)

      // Amplitude(s) for diagram number 80
      FFV1_0( w_sv[15], w_sv[23], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[4] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 81 OF 123 ***

      // Wavefunction(s) for diagram number 81
      FFV1_1( w_sv[9], w_sv[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[23] );

      // Amplitude(s) for diagram number 81
      FFV1_0( w_sv[15], w_sv[23], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[7] -= amp_sv[0];

      // *** DIAGRAM 82 OF 123 ***

      // Wavefunction(s) for diagram number 82
      FFV1_2( w_sv[15], w_sv[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[12] );

      // Amplitude(s) for diagram number 82
      FFV1_0( w_sv[12], w_sv[9], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[10] -= amp_sv[0];

      // *** DIAGRAM 83 OF 123 ***

      // Wavefunction(s) for diagram number 83
      // (none)

      // Amplitude(s) for diagram number 83
      FFV1_0( w_sv[13], w_sv[23], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[6] -= amp_sv[0];

      // *** DIAGRAM 84 OF 123 ***

      // Wavefunction(s) for diagram number 84
      FFV1_2( w_sv[13], w_sv[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[21] );

      // Amplitude(s) for diagram number 84
      FFV1_0( w_sv[21], w_sv[9], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[8] -= amp_sv[0];

      // *** DIAGRAM 85 OF 123 ***

      // Wavefunction(s) for diagram number 85
      // (none)

      // Amplitude(s) for diagram number 85
      FFV1_0( w_sv[3], w_sv[23], w_sv[10], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[6] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 86 OF 123 ***

      // Wavefunction(s) for diagram number 86
      VVV1P0_1( w_sv[0], w_sv[10], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[23] );

      // Amplitude(s) for diagram number 86
      FFV1_0( w_sv[3], w_sv[9], w_sv[23], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[6] += +amp_sv[0];
      jamp_sv[7] -= amp_sv[0];
      jamp_sv[9] -= amp_sv[0];
      jamp_sv[11] += +amp_sv[0];

      // *** DIAGRAM 87 OF 123 ***

      // Wavefunction(s) for diagram number 87
      FFV1_2( w_sv[16], w_sv[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[22] );

      // Amplitude(s) for diagram number 87
      FFV1_0( w_sv[22], w_sv[11], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[16] -= amp_sv[0];

      // *** DIAGRAM 88 OF 123 ***

      // Wavefunction(s) for diagram number 88
      FFV1_1( w_sv[11], w_sv[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[20] );

      // Amplitude(s) for diagram number 88
      FFV1_0( w_sv[16], w_sv[20], w_sv[5], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[13] -= amp_sv[0];

      // *** DIAGRAM 89 OF 123 ***

      // Wavefunction(s) for diagram number 89
      // (none)

      // Amplitude(s) for diagram number 89
      FFV1_0( w_sv[22], w_sv[14], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[22] -= amp_sv[0];

      // *** DIAGRAM 90 OF 123 ***

      // Wavefunction(s) for diagram number 90
      FFV1_1( w_sv[14], w_sv[0], cxmake( cIPC[2], cIPC[3] ), cIPD[0], cIPD[1], w_sv[24] );

      // Amplitude(s) for diagram number 90
      FFV1_0( w_sv[16], w_sv[24], w_sv[4], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[19] -= amp_sv[0];

      // *** DIAGRAM 91 OF 123 ***

      // Wavefunction(s) for diagram number 91
      // (none)

      // Amplitude(s) for diagram number 91
      FFV1_0( w_sv[22], w_sv[2], w_sv[10], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 92 OF 123 ***

      // Wavefunction(s) for diagram number 92
      // (none)

      // Amplitude(s) for diagram number 92
      FFV1_0( w_sv[16], w_sv[2], w_sv[23], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[3] += +amp_sv[0];
      jamp_sv[5] -= amp_sv[0];
      jamp_sv[16] -= amp_sv[0];
      jamp_sv[22] += +amp_sv[0];

      // *** DIAGRAM 93 OF 123 ***

      // Wavefunction(s) for diagram number 93
      // (none)

      // Amplitude(s) for diagram number 93
      VVVV1_0( w_sv[0], w_sv[6], w_sv[7], w_sv[5], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];
      VVVV3_0( w_sv[0], w_sv[6], w_sv[7], w_sv[5], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];
      VVVV4_0( w_sv[0], w_sv[6], w_sv[7], w_sv[5], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[8] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[19] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 94 OF 123 ***

      // Wavefunction(s) for diagram number 94
      VVV1P0_1( w_sv[0], w_sv[6], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[22] );

      // Amplitude(s) for diagram number 94
      VVV1_0( w_sv[7], w_sv[5], w_sv[22], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 95 OF 123 ***

      // Wavefunction(s) for diagram number 95
      VVV1P0_1( w_sv[0], w_sv[7], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[25] );

      // Amplitude(s) for diagram number 95
      VVV1_0( w_sv[6], w_sv[5], w_sv[25], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 96 OF 123 ***

      // Wavefunction(s) for diagram number 96
      // (none)

      // Amplitude(s) for diagram number 96
      FFV1_0( w_sv[3], w_sv[14], w_sv[22], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[18] += +amp_sv[0];
      jamp_sv[19] -= amp_sv[0];
      jamp_sv[21] -= amp_sv[0];
      jamp_sv[23] += +amp_sv[0];

      // *** DIAGRAM 97 OF 123 ***

      // Wavefunction(s) for diagram number 97
      // (none)

      // Amplitude(s) for diagram number 97
      FFV1_0( w_sv[3], w_sv[24], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[18] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[19] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 98 OF 123 ***

      // Wavefunction(s) for diagram number 98
      // (none)

      // Amplitude(s) for diagram number 98
      FFV1_0( w_sv[13], w_sv[2], w_sv[22], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] += +amp_sv[0];
      jamp_sv[2] -= amp_sv[0];
      jamp_sv[8] -= amp_sv[0];
      jamp_sv[14] += +amp_sv[0];

      // *** DIAGRAM 99 OF 123 ***

      // Wavefunction(s) for diagram number 99
      // (none)

      // Amplitude(s) for diagram number 99
      FFV1_0( w_sv[21], w_sv[2], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[8] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[14] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 100 OF 123 ***

      // Wavefunction(s) for diagram number 100
      // (none)

      // Amplitude(s) for diagram number 100
      VVVV1_0( w_sv[0], w_sv[18], w_sv[7], w_sv[4], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];
      VVVV3_0( w_sv[0], w_sv[18], w_sv[7], w_sv[4], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[3] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      VVVV4_0( w_sv[0], w_sv[18], w_sv[7], w_sv[4], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[3] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[10] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[11] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[13] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 101 OF 123 ***

      // Wavefunction(s) for diagram number 101
      VVV1P0_1( w_sv[0], w_sv[18], cxmake( cIPC[0], cIPC[1] ), 0., 0., w_sv[6] );

      // Amplitude(s) for diagram number 101
      VVV1_0( w_sv[7], w_sv[4], w_sv[6], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 102 OF 123 ***

      // Wavefunction(s) for diagram number 102
      // (none)

      // Amplitude(s) for diagram number 102
      VVV1_0( w_sv[18], w_sv[4], w_sv[25], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[3] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 103 OF 123 ***

      // Wavefunction(s) for diagram number 103
      // (none)

      // Amplitude(s) for diagram number 103
      FFV1_0( w_sv[3], w_sv[11], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[12] += +amp_sv[0];
      jamp_sv[13] -= amp_sv[0];
      jamp_sv[15] -= amp_sv[0];
      jamp_sv[17] += +amp_sv[0];

      // *** DIAGRAM 104 OF 123 ***

      // Wavefunction(s) for diagram number 104
      // (none)

      // Amplitude(s) for diagram number 104
      FFV1_0( w_sv[3], w_sv[20], w_sv[18], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[12] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[13] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 105 OF 123 ***

      // Wavefunction(s) for diagram number 105
      // (none)

      // Amplitude(s) for diagram number 105
      FFV1_0( w_sv[15], w_sv[2], w_sv[6], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[1] += +amp_sv[0];
      jamp_sv[4] -= amp_sv[0];
      jamp_sv[10] -= amp_sv[0];
      jamp_sv[20] += +amp_sv[0];

      // *** DIAGRAM 106 OF 123 ***

      // Wavefunction(s) for diagram number 106
      // (none)

      // Amplitude(s) for diagram number 106
      FFV1_0( w_sv[12], w_sv[2], w_sv[18], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[10] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[20] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 107 OF 123 ***

      // Wavefunction(s) for diagram number 107
      // (none)

      // Amplitude(s) for diagram number 107
      VVVV1_0( w_sv[0], w_sv[1], w_sv[7], w_sv[10], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[1] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[6] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[7] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[17] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];
      VVVV3_0( w_sv[0], w_sv[1], w_sv[7], w_sv[10], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[1] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[3] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];
      VVVV4_0( w_sv[0], w_sv[1], w_sv[7], w_sv[10], cxmake( cIPC[4], cIPC[5] ), &amp_sv[0] );
      jamp_sv[3] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[6] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 108 OF 123 ***

      // Wavefunction(s) for diagram number 108
      // (none)

      // Amplitude(s) for diagram number 108
      VVV1_0( w_sv[1], w_sv[10], w_sv[25], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[1] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[3] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 109 OF 123 ***

      // Wavefunction(s) for diagram number 109
      // (none)

      // Amplitude(s) for diagram number 109
      VVV1_0( w_sv[1], w_sv[7], w_sv[23], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[3] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[6] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 110 OF 123 ***

      // Wavefunction(s) for diagram number 110
      // (none)

      // Amplitude(s) for diagram number 110
      FFV1_0( w_sv[13], w_sv[20], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[12] -= amp_sv[0];

      // *** DIAGRAM 111 OF 123 ***

      // Wavefunction(s) for diagram number 111
      // (none)

      // Amplitude(s) for diagram number 111
      FFV1_0( w_sv[21], w_sv[11], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[14] -= amp_sv[0];

      // *** DIAGRAM 112 OF 123 ***

      // Wavefunction(s) for diagram number 112
      // (none)

      // Amplitude(s) for diagram number 112
      FFV1_0( w_sv[15], w_sv[24], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[18] -= amp_sv[0];

      // *** DIAGRAM 113 OF 123 ***

      // Wavefunction(s) for diagram number 113
      // (none)

      // Amplitude(s) for diagram number 113
      FFV1_0( w_sv[12], w_sv[14], w_sv[1], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[20] -= amp_sv[0];

      // *** DIAGRAM 114 OF 123 ***

      // Wavefunction(s) for diagram number 114
      VVVV1P0_1( w_sv[0], w_sv[1], w_sv[4], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[12] );
      VVVV3P0_1( w_sv[0], w_sv[1], w_sv[4], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[24] );
      VVVV4P0_1( w_sv[0], w_sv[1], w_sv[4], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[21] );

      // Amplitude(s) for diagram number 114
      VVV1_0( w_sv[12], w_sv[7], w_sv[5], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[14] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];
      VVV1_0( w_sv[24], w_sv[7], w_sv[5], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[6] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[8] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[19] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[20] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] -= cxtype(0,1)*amp_sv[0];
      VVV1_0( w_sv[21], w_sv[7], w_sv[5], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[6] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[12] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[14] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[18] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[20] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[22] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[23] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 115 OF 123 ***

      // Wavefunction(s) for diagram number 115
      // (none)

      // Amplitude(s) for diagram number 115
      FFV1_0( w_sv[3], w_sv[14], w_sv[12], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[18] += +amp_sv[0];
      jamp_sv[19] -= amp_sv[0];
      jamp_sv[21] -= amp_sv[0];
      jamp_sv[23] += +amp_sv[0];
      FFV1_0( w_sv[3], w_sv[14], w_sv[24], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[19] -= amp_sv[0];
      jamp_sv[20] += +amp_sv[0];
      jamp_sv[21] -= amp_sv[0];
      jamp_sv[22] += +amp_sv[0];
      FFV1_0( w_sv[3], w_sv[14], w_sv[21], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[18] -= amp_sv[0];
      jamp_sv[20] += +amp_sv[0];
      jamp_sv[22] += +amp_sv[0];
      jamp_sv[23] -= amp_sv[0];

      // *** DIAGRAM 116 OF 123 ***

      // Wavefunction(s) for diagram number 116
      // (none)

      // Amplitude(s) for diagram number 116
      FFV1_0( w_sv[13], w_sv[2], w_sv[12], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] += +amp_sv[0];
      jamp_sv[2] -= amp_sv[0];
      jamp_sv[8] -= amp_sv[0];
      jamp_sv[14] += +amp_sv[0];
      FFV1_0( w_sv[13], w_sv[2], w_sv[24], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[2] -= amp_sv[0];
      jamp_sv[6] += +amp_sv[0];
      jamp_sv[8] -= amp_sv[0];
      jamp_sv[12] += +amp_sv[0];
      FFV1_0( w_sv[13], w_sv[2], w_sv[21], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[0] -= amp_sv[0];
      jamp_sv[6] += +amp_sv[0];
      jamp_sv[12] += +amp_sv[0];
      jamp_sv[14] -= amp_sv[0];

      // *** DIAGRAM 117 OF 123 ***

      // Wavefunction(s) for diagram number 117
      VVVV1P0_1( w_sv[0], w_sv[1], w_sv[5], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[21] );
      VVVV3P0_1( w_sv[0], w_sv[1], w_sv[5], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[13] );
      VVVV4P0_1( w_sv[0], w_sv[1], w_sv[5], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[24] );

      // Amplitude(s) for diagram number 117
      VVV1_0( w_sv[21], w_sv[7], w_sv[4], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[12] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[20] += +cxtype(0,1)*amp_sv[0];
      VVV1_0( w_sv[13], w_sv[7], w_sv[4], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[7] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[10] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[13] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[14] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[18] += +cxtype(0,1)*amp_sv[0];
      VVV1_0( w_sv[24], w_sv[7], w_sv[4], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[1] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[7] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[12] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[14] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[16] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[17] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[18] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[20] -= cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 118 OF 123 ***

      // Wavefunction(s) for diagram number 118
      // (none)

      // Amplitude(s) for diagram number 118
      FFV1_0( w_sv[3], w_sv[11], w_sv[21], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[12] += +amp_sv[0];
      jamp_sv[13] -= amp_sv[0];
      jamp_sv[15] -= amp_sv[0];
      jamp_sv[17] += +amp_sv[0];
      FFV1_0( w_sv[3], w_sv[11], w_sv[13], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[13] -= amp_sv[0];
      jamp_sv[14] += +amp_sv[0];
      jamp_sv[15] -= amp_sv[0];
      jamp_sv[16] += +amp_sv[0];
      FFV1_0( w_sv[3], w_sv[11], w_sv[24], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[12] -= amp_sv[0];
      jamp_sv[14] += +amp_sv[0];
      jamp_sv[16] += +amp_sv[0];
      jamp_sv[17] -= amp_sv[0];

      // *** DIAGRAM 119 OF 123 ***

      // Wavefunction(s) for diagram number 119
      // (none)

      // Amplitude(s) for diagram number 119
      FFV1_0( w_sv[15], w_sv[2], w_sv[21], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[1] += +amp_sv[0];
      jamp_sv[4] -= amp_sv[0];
      jamp_sv[10] -= amp_sv[0];
      jamp_sv[20] += +amp_sv[0];
      FFV1_0( w_sv[15], w_sv[2], w_sv[13], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[4] -= amp_sv[0];
      jamp_sv[7] += +amp_sv[0];
      jamp_sv[10] -= amp_sv[0];
      jamp_sv[18] += +amp_sv[0];
      FFV1_0( w_sv[15], w_sv[2], w_sv[24], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[1] -= amp_sv[0];
      jamp_sv[7] += +amp_sv[0];
      jamp_sv[18] += +amp_sv[0];
      jamp_sv[20] -= amp_sv[0];

      // *** DIAGRAM 120 OF 123 ***

      // Wavefunction(s) for diagram number 120
      VVVV1P0_1( w_sv[0], w_sv[4], w_sv[5], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[24] );
      VVVV3P0_1( w_sv[0], w_sv[4], w_sv[5], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[15] );
      VVVV4P0_1( w_sv[0], w_sv[4], w_sv[5], cxmake( cIPC[4], cIPC[5] ), 0., 0., w_sv[13] );

      // Amplitude(s) for diagram number 120
      FFV1_0( w_sv[3], w_sv[9], w_sv[24], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[6] += +amp_sv[0];
      jamp_sv[7] -= amp_sv[0];
      jamp_sv[9] -= amp_sv[0];
      jamp_sv[11] += +amp_sv[0];
      FFV1_0( w_sv[3], w_sv[9], w_sv[15], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[7] -= amp_sv[0];
      jamp_sv[8] += +amp_sv[0];
      jamp_sv[9] -= amp_sv[0];
      jamp_sv[10] += +amp_sv[0];
      FFV1_0( w_sv[3], w_sv[9], w_sv[13], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[6] -= amp_sv[0];
      jamp_sv[8] += +amp_sv[0];
      jamp_sv[10] += +amp_sv[0];
      jamp_sv[11] -= amp_sv[0];

      // *** DIAGRAM 121 OF 123 ***

      // Wavefunction(s) for diagram number 121
      // (none)

      // Amplitude(s) for diagram number 121
      FFV1_0( w_sv[16], w_sv[2], w_sv[24], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[3] += +amp_sv[0];
      jamp_sv[5] -= amp_sv[0];
      jamp_sv[16] -= amp_sv[0];
      jamp_sv[22] += +amp_sv[0];
      FFV1_0( w_sv[16], w_sv[2], w_sv[15], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[5] -= amp_sv[0];
      jamp_sv[13] += +amp_sv[0];
      jamp_sv[16] -= amp_sv[0];
      jamp_sv[19] += +amp_sv[0];
      FFV1_0( w_sv[16], w_sv[2], w_sv[13], cxmake( cIPC[2], cIPC[3] ), &amp_sv[0] );
      jamp_sv[3] -= amp_sv[0];
      jamp_sv[13] += +amp_sv[0];
      jamp_sv[19] += +amp_sv[0];
      jamp_sv[22] -= amp_sv[0];

      // *** DIAGRAM 122 OF 123 ***

      // Wavefunction(s) for diagram number 122
      // (none)

      // Amplitude(s) for diagram number 122
      VVV1_0( w_sv[24], w_sv[1], w_sv[7], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[3] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[6] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[22] -= cxtype(0,1)*amp_sv[0];
      VVV1_0( w_sv[15], w_sv[1], w_sv[7], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[7] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[8] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[10] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[13] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[16] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[19] -= cxtype(0,1)*amp_sv[0];
      VVV1_0( w_sv[13], w_sv[1], w_sv[7], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[3] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[6] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[8] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[10] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[11] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[13] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[19] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[22] += +cxtype(0,1)*amp_sv[0];

      // *** DIAGRAM 123 OF 123 ***

      // Wavefunction(s) for diagram number 123
      // (none)

      // Amplitude(s) for diagram number 123
      VVV1_0( w_sv[0], w_sv[17], w_sv[7], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[3] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[5] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[9] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[11] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[23] += +cxtype(0,1)*amp_sv[0];
      VVV1_0( w_sv[0], w_sv[19], w_sv[7], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[1] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[3] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[11] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[17] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      VVV1_0( w_sv[0], w_sv[8], w_sv[7], cxmake( cIPC[0], cIPC[1] ), &amp_sv[0] );
      jamp_sv[0] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[2] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[4] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[5] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[9] -= cxtype(0,1)*amp_sv[0];
      jamp_sv[15] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[21] += +cxtype(0,1)*amp_sv[0];
      jamp_sv[23] -= cxtype(0,1)*amp_sv[0];

      // *** COLOR ALGEBRA BELOW ***
      // (This method used to be called CPPProcess::matrix_1_gg_ttxgg()?)

      // The color matrix [NB do keep 'static' for these constexpr arrays, see issue #283]
      static constexpr fptype denom[ncolor] = {54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54};
      static constexpr fptype cf[ncolor][ncolor] = {
      {512, -64, -64, 8, 8, 80, -64, 8, 8, -1, -1, -10, 8, -1, 80, -10, 71, 62, -1, -10, -10, 62, 62, -28},
      {-64, 512, 8, 80, -64, 8, 8, -64, -1, -10, 8, -1, -1, -10, -10, 62, 62, -28, 8, -1, 80, -10, 71, 62},
      {-64, 8, 512, -64, 80, 8, 8, -1, 80, -10, 71, 62, -64, 8, 8, -1, -1, -10, -10, -1, 62, -28, -10, 62},
      {8, 80, -64, 512, 8, -64, -1, -10, -10, 62, 62, -28, 8, -64, -1, -10, 8, -1, -1, 8, 71, 62, 80, -10},
      {8, -64, 80, 8, 512, -64, -1, 8, 71, 62, 80, -10, -10, -1, 62, -28, -10, 62, -64, 8, 8, -1, -1, -10},
      {80, 8, 8, -64, -64, 512, -10, -1, 62, -28, -10, 62, -1, 8, 71, 62, 80, -10, 8, -64, -1, -10, 8, -1},
      {-64, 8, 8, -1, -1, -10, 512, -64, -64, 8, 8, 80, 80, -10, 8, -1, 62, 71, -10, 62, -1, -10, -28, 62},
      {8, -64, -1, -10, 8, -1, -64, 512, 8, 80, -64, 8, -10, 62, -1, -10, -28, 62, 80, -10, 8, -1, 62, 71},
      {8, -1, 80, -10, 71, 62, -64, 8, 512, -64, 80, 8, 8, -1, -64, 8, -10, -1, 62, -28, -10, -1, 62, -10},
      {-1, -10, -10, 62, 62, -28, 8, 80, -64, 512, 8, -64, -1, -10, 8, -64, -1, 8, 71, 62, -1, 8, -10, 80},
      {-1, 8, 71, 62, 80, -10, 8, -64, 80, 8, 512, -64, 62, -28, -10, -1, 62, -10, 8, -1, -64, 8, -10, -1},
      {-10, -1, 62, -28, -10, 62, 80, 8, 8, -64, -64, 512, 71, 62, -1, 8, -10, 80, -1, -10, 8, -64, -1, 8},
      {8, -1, -64, 8, -10, -1, 80, -10, 8, -1, 62, 71, 512, -64, -64, 8, 8, 80, 62, -10, -28, 62, -1, -10},
      {-1, -10, 8, -64, -1, 8, -10, 62, -1, -10, -28, 62, -64, 512, 8, 80, -64, 8, -10, 80, 62, 71, 8, -1},
      {80, -10, 8, -1, 62, 71, 8, -1, -64, 8, -10, -1, -64, 8, 512, -64, 80, 8, -28, 62, 62, -10, -10, -1},
      {-10, 62, -1, -10, -28, 62, -1, -10, 8, -64, -1, 8, 8, 80, -64, 512, 8, -64, 62, 71, -10, 80, -1, 8},
      {71, 62, -1, 8, -10, 80, 62, -28, -10, -1, 62, -10, 8, -64, 80, 8, 512, -64, -1, 8, -10, -1, -64, 8},
      {62, -28, -10, -1, 62, -10, 71, 62, -1, 8, -10, 80, 80, 8, 8, -64, -64, 512, -10, -1, -1, 8, 8, -64},
      {-1, 8, -10, -1, -64, 8, -10, 80, 62, 71, 8, -1, 62, -10, -28, 62, -1, -10, 512, -64, -64, 8, 8, 80},
      {-10, -1, -1, 8, 8, -64, 62, -10, -28, 62, -1, -10, -10, 80, 62, 71, 8, -1, -64, 512, 8, 80, -64, 8},
      {-10, 80, 62, 71, 8, -1, -1, 8, -10, -1, -64, 8, -28, 62, 62, -10, -10, -1, -64, 8, 512, -64, 80, 8},
      {62, -10, -28, 62, -1, -10, -10, -1, -1, 8, 8, -64, 62, 71, -10, 80, -1, 8, 8, 80, -64, 512, 8, -64},
      {62, 71, -10, 80, -1, 8, -28, 62, 62, -10, -10, -1, -1, 8, -10, -1, -64, 8, 8, -64, 80, 8, 512, -64},
      {-28, 62, 62, -10, -10, -1, 62, 71, -10, 80, -1, 8, -10, -1, -1, 8, 8, -64, 80, 8, 8, -64, -64, 512}};

      // Sum and square the color flows to get the matrix element
      // (compute |M|^2 by squaring |M|, taking into account colours)
      fptype_sv deltaMEs = { 0 }; // all zeros
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
      allMEs[ievt] += deltaMEs;
      //if ( cNGoodHel > 0 ) printf( "calculate_wavefunction: %6d %2d %f\n", ievt, ihel, allMEs[ievt] );
    }
    mgDebug( 1, __FUNCTION__ );
    return;
  }

  //--------------------------------------------------------------------------

  CPPProcess::CPPProcess(
          int numiterations,
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
      {-1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, 1},
      {-1, -1, -1, -1, 1, -1},
      {-1, -1, -1, -1, 1, 1},
      {-1, -1, -1, 1, -1, -1},
      {-1, -1, -1, 1, -1, 1},
      {-1, -1, -1, 1, 1, -1},
      {-1, -1, -1, 1, 1, 1},
      {-1, -1, 1, -1, -1, -1},
      {-1, -1, 1, -1, -1, 1},
      {-1, -1, 1, -1, 1, -1},
      {-1, -1, 1, -1, 1, 1},
      {-1, -1, 1, 1, -1, -1},
      {-1, -1, 1, 1, -1, 1},
      {-1, -1, 1, 1, 1, -1},
      {-1, -1, 1, 1, 1, 1},
      {-1, 1, -1, -1, -1, -1},
      {-1, 1, -1, -1, -1, 1},
      {-1, 1, -1, -1, 1, -1},
      {-1, 1, -1, -1, 1, 1},
      {-1, 1, -1, 1, -1, -1},
      {-1, 1, -1, 1, -1, 1},
      {-1, 1, -1, 1, 1, -1},
      {-1, 1, -1, 1, 1, 1},
      {-1, 1, 1, -1, -1, -1},
      {-1, 1, 1, -1, -1, 1},
      {-1, 1, 1, -1, 1, -1},
      {-1, 1, 1, -1, 1, 1},
      {-1, 1, 1, 1, -1, -1},
      {-1, 1, 1, 1, -1, 1},
      {-1, 1, 1, 1, 1, -1},
      {-1, 1, 1, 1, 1, 1},
      {1, -1, -1, -1, -1, -1},
      {1, -1, -1, -1, -1, 1},
      {1, -1, -1, -1, 1, -1},
      {1, -1, -1, -1, 1, 1},
      {1, -1, -1, 1, -1, -1},
      {1, -1, -1, 1, -1, 1},
      {1, -1, -1, 1, 1, -1},
      {1, -1, -1, 1, 1, 1},
      {1, -1, 1, -1, -1, -1},
      {1, -1, 1, -1, -1, 1},
      {1, -1, 1, -1, 1, -1},
      {1, -1, 1, -1, 1, 1},
      {1, -1, 1, 1, -1, -1},
      {1, -1, 1, 1, -1, 1},
      {1, -1, 1, 1, 1, -1},
      {1, -1, 1, 1, 1, 1},
      {1, 1, -1, -1, -1, -1},
      {1, 1, -1, -1, -1, 1},
      {1, 1, -1, -1, 1, -1},
      {1, 1, -1, -1, 1, 1},
      {1, 1, -1, 1, -1, -1},
      {1, 1, -1, 1, -1, 1},
      {1, 1, -1, 1, 1, -1},
      {1, 1, -1, 1, 1, 1},
      {1, 1, 1, -1, -1, -1},
      {1, 1, 1, -1, -1, 1},
      {1, 1, 1, -1, 1, -1},
      {1, 1, 1, -1, 1, 1},
      {1, 1, 1, 1, -1, -1},
      {1, 1, 1, 1, -1, 1},
      {1, 1, 1, 1, 1, -1},
      {1, 1, 1, 1, 1, 1}};
    memcpy( *cHel, *tHel, ncomb * mgOnGpu::npar * sizeof(short) );

    // SANITY CHECK: GPU memory usage may be based on casts of fptype[2] to cxtype
    static_assert( sizeof(cxtype) == 2 * sizeof(fptype) );
    // SANITY CHECK: check that neppR, neppM and neppV are powers of two (https://stackoverflow.com/a/108360)
    auto ispoweroftwo = []( int n ) { return ( n > 0 ) && !( n & ( n - 1 ) ); };
    static_assert( ispoweroftwo( mgOnGpu::neppR ) );
    static_assert( ispoweroftwo( mgOnGpu::neppM ) );
    static_assert( ispoweroftwo( neppV ) );
  }

  //--------------------------------------------------------------------------

  CPPProcess::~CPPProcess() {}

  //--------------------------------------------------------------------------

  //const std::vector<fptype>& CPPProcess::getMasses() const { return mME; }

  //--------------------------------------------------------------------------

  short * CPPProcess::get_cHel_ptr() const {return *cHel;}

  //--------------------------------------------------------------------------

  fptype* CPPProcess::get_cIPC_ptr() {return cIPC;}

  //--------------------------------------------------------------------------

  fptype* CPPProcess::get_cIPD_ptr() {return cIPD;}

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
    m_masses.push_back( m_pars->ZERO );
    m_masses.push_back( m_pars->ZERO );
    // Read physics parameters like masses and couplings from user configuration files (static: initialize once)
    // Then copy them to CUDA constant memory (issue #39) or its C++ emulation in file-scope static memory
    const cxtype tIPC[3] = { cxmake( m_pars->GC_10 ), cxmake( m_pars->GC_11 ), cxmake( m_pars->GC_12 ) };
    const fptype tIPD[2] = { (fptype)m_pars->mdl_MT, (fptype)m_pars->mdl_WT };
    memcpy( cIPC, tIPC, 3 * sizeof(cxtype) );
    memcpy( cIPD, tIPD, 2 * sizeof(fptype) );
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
  const std::string CPPProcess::getCompiler() {
    std::stringstream out;
    // CUDA version (NVCC)
#ifdef __CUDACC__
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
    out << "icx " << __INTEL_LLVM_COMPILER << " (";
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
#if defined __CUDACC__ or defined __INTEL_LLVM_COMPILER
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
#if defined __CUDACC__ or defined __INTEL_LLVM_COMPILER
    out << ")";
#endif
    return out.str();
  }

  //--------------------------------------------------------------------------

  SYCL_EXTERNAL
  void sigmaKin_getGoodHel( const fptype* allmomenta, // input: momenta as AOSOA[npagM][npar][4][neppM] with nevt=npagM*neppM
                            fptype* allMEs,           // output: allMEs[nevt], |M|^2 final_avg_over_helicities
                            bool * isGoodHel,  // output: isGoodHel[ncomb] - device array
                            size_t ievt,
                            short *cHel,
                            const fptype *cIPC,
                            const fptype *cIPD
                            ) {
      // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
      fptype allMEsLast = 0;
      for ( int ihel = 0; ihel < ncomb; ihel++ )
      {
        // NB: calculate_wavefunctions ADDS |M|^2 for a given ihel to the running sum of |M|^2 over helicities for the given event(s)
        calculate_wavefunctions( ihel, allmomenta, allMEs, ievt, cHel, cIPC, cIPD );
        if ( allMEs[ievt] != allMEsLast )
        {
          //if ( !isGoodHel[ihel] ) std::cout << "sigmaKin_getGoodHel ihel=" << ihel << " TRUE" << std::endl;
          isGoodHel[ihel] = true;
        }
        allMEsLast = allMEs[ievt]; // running sum up to helicity ihel for event ievt
      }
  }

  //--------------------------------------------------------------------------

  SYCL_EXTERNAL
  void sigmaKin_setGoodHel( const bool* isGoodHel, int* cNGoodHel, int* cGoodHel) {
    *cNGoodHel = 0;
    for ( int ihel = 0; ihel < ncomb; ihel++ )
    {
      if ( isGoodHel[ihel] ) {
        cGoodHel[*cNGoodHel] = ihel;
        (*cNGoodHel)++;
      }
    }
  }

  //--------------------------------------------------------------------------
  // Evaluate |M|^2, part independent of incoming flavour
  // FIXME: assume process.nprocesses == 1 (eventually: allMEs[nevt] -> allMEs[nevt*nprocesses]?)

  SYCL_EXTERNAL
  void sigmaKin(
          const fptype* allmomenta, // input: momenta as AOSOA[npagM][npar][4][neppM] with nevt=npagM*neppM
          fptype* allMEs,            // output: allMEs[nevt], |M|^2 final_avg_over_helicities
          size_t ievt,
          short *cHel,
          const fptype *cIPC,
          const fptype *cIPD,
          int *cNGoodHel,
          int *cGoodHel
          )  {
    mgDebugInitialise();

    // Denominators: spins, colors and identical particles
    const int denominators = 512; // FIXME: assume process.nprocesses == 1 for the moment (eventually denominators[nprocesses]?)

    // Set the parameters which change event by event
    // Need to discuss this with Stefan
    //m_pars->setDependentParameters();
    //m_pars->setDependentCouplings();

    // Start sigmaKin_lines
    // PART 0 - INITIALISATION (before calculate_wavefunctions)
    // Reset the "matrix elements" - running sums of |M|^2 over helicities for the given event
    // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
    allMEs[ievt] = 0;

    // PART 1 - HELICITY LOOP: CALCULATE WAVEFUNCTIONS
    // (in both CUDA and C++, using precomputed good helicities)
    // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
    for ( int ighel = 0; ighel < *cNGoodHel; ighel++ ) {
        const int ihel = cGoodHel[ighel];
        calculate_wavefunctions( ihel, allmomenta, allMEs, ievt, cHel, cIPC, cIPD );
    }

    // PART 2 - FINALISATION (after calculate_wavefunctions)
    // Get the final |M|^2 as an average over helicities/colors of the running sum of |M|^2 over helicities for the given event
    // [NB 'sum over final spins, average over initial spins', eg see
    // https://www.uzh.ch/cmsssl/physik/dam/jcr:2e24b7b1-f4d7-4160-817e-47b13dbf1d7c/Handout_4_2016-UZH.pdf]
    // FIXME: assume process.nprocesses == 1 for the moment (eventually: need a loop over processes here?)
    allMEs[ievt] /= denominators;
    mgDebugFinalise();
  }

  //--------------------------------------------------------------------------

} // end namespace


