#ifndef MGONGPUCONFIG_H
#define MGONGPUCONFIG_H 1


namespace mgOnGpu
{

  // --- Physics process-specific constants that are best declared at compile time

  const int np4 = 4; // dimensions of 4-momenta (E,px,py,pz)

  const int npari = %(nincoming)d; // #particles in the initial state (incoming): e.g. 2 (e+ e-) for e+ e- -> mu+ mu-
  const int nparf = %(noutcoming)d; // #particles in the final state (outgoing): e.g. 2 (mu+ mu-) for e+ e- -> mu+ mu-
  const int npar = npari + nparf; // #particles in total (external = initial + final): e.g. 4 for e+ e- -> mu+ mu-

  const int ncomb = %(nbhel)d; // #helicity combinations: e.g. 16 for e+ e- -> mu+ mu- (2**4 = fermion spin up/down ** npar)

  const int nw6 = %(wavefuncsize)d; // dimensions of each wavefunction (HELAS KEK 91-11): e.g. 6 for e+ e- -> mu+ mu- (fermions and vectors)
  const int nwf = %(nwavefunc)d; // #wavefunctions = #external (npar) + #internal: e.g. 5 for e+ e- -> mu+ mu- (1 internal is gamma or Z)
  
  const int ncouplings = %(ncouplings)d;
  const int ncouplingstimes2 = %(ncouplingstimes2)d;
  const int nparams = %(nparams)d;

  // --- Platform-specific software implementation details

  // Maximum number of blocks per grid
  // ** NB Some arrays of pointers will be allocated statically to fit all these blocks
  // ** (the actual memory for each block will then be allocated dynamically only for existing blocks)
  //const int nbpgMAX = 2048;

  // Maximum number of threads per block
  //const int ntpbMAX = 256; // AV Apr2021: why had I set this to 256?
#ifdef MGONGPU_NTPBMAX
  const int ntpbMAX = MGONGPU_NTPBMAX;
#else
  const int ntpbMAX = 256; // NB: 512 is ok, but 1024 does fail with "too many resources requested for launch"
#endif

}

#endif // MGONGPUCONFIG_H



/* #ifndef MGKOKKOSCONFIG_H */
/* #define MGKOKKOSCONFIG_H 1 */

/* namespace mgKokkos */
/* { */
/*   // --- Physics process-specific constants that are best declared at compile time */

/*   constexpr int np4 = 4; // the dimension of 4-momenta (E,px,py,pz) */
/*   constexpr int nw6 = %(wavefuncsize)d; // dimension of each wavefunction (see KEK 91-11) */

/*   constexpr int npari = %(nincoming)d; // #particles in the initial state (incoming): e+ e- */
/*   constexpr int nparf = %(noutcoming)d; // #particles in the final state (outgoing): mu+ mu- */
/*   constexpr int npar = npari + nparf; // #particles in total (external): e+ e- -> mu+ mu- */

/*   constexpr int nwf = %(nwavefunc)d; // #wavefunctions: npar (4 external) + 1 (internal, reused for gamma and Z) */

/*   constexpr int ncomb = %(nbhel)d; // #helicity combinations: 16=2(spin up/down for fermions)**4(npar) */

/*   // --- Platform-specific software implementation details */

/*   // Maximum number of threads per block */
/*   constexpr int ntpbMAX = 256; */

/* } */

/* #endif // MGKOKKOSCONFIG_H */
