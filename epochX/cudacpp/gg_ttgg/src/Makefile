INCFLAGS = -I.
OPTFLAGS = -O3 # this ends up in CUFLAGS too (should it?), cannot add -Ofast or -ffast-math here
CXXFLAGS = $(OPTFLAGS) -std=c++17 $(INCFLAGS) $(USE_NVTX) -Wall -Wshadow -Wextra
CXXFLAGS+= -ffast-math # see issue #117
###CXXFLAGS+= -Ofast # performance is not different from --fast-math
###CXXFLAGS+= -g # FOR DEBUGGING ONLY

# install target
INSTALL_HEADERS=Parameters_sm.h mgOnGpuConfig.h mgOnGpuFptypes.h mgOnGpuCxtypes.h mgOnGpuVectors.h read_slha.h
INSTALL_INC_DIR=../include

# Note: AR and CXX are implicitly defined if not set externally
# See https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
RANLIB = ranlib

# OpenMP flags
CXXFLAGS += $(OMPFLAGS)

# Assuming uname is available, detect if architecture is PowerPC
UNAME_P := $(shell uname -p)

# PowerPC-specific CXX compiler flags (being reviewed)
ifeq ($(UNAME_P),ppc64le)
  CXXFLAGS+= -mcpu=power9 -mtune=power9 # gains ~2-3% both for none and sse4
  # Throughput references without the extra flags below: none=1.41-1.42E6, sse4=2.15-2.19E6
  ###CXXFLAGS+= -DNO_WARN_X86_INTRINSICS # no change
  ###CXXFLAGS+= -fpeel-loops # no change
  ###CXXFLAGS+= -funroll-loops # gains ~1% for none, loses ~1% for sse4
  ###CXXFLAGS+= -ftree-vectorize # no change
  ###CXXFLAGS+= -flto # BUILD ERROR IF THIS ADDED IN SRC?!
else
  ###AR=gcc-ar # needed by -flto
  ###RANLIB=gcc-ranlib # needed by -flto
  ###CXXFLAGS+= -flto # NB: build error from src/Makefile unless gcc-ar and gcc-ranlib are used
  ######CXXFLAGS+= -fno-semantic-interposition # no benefit (neither alone, nor combined with -flto)
endif

# Set the build flags appropriate to each AVX choice (example: "make AVX=none")
# [NB MGONGPU_PVW512 is needed because "-mprefer-vector-width=256" is not exposed in a macro]
# [See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=96476]
$(info AVX=$(AVX))
ifeq ($(UNAME_P),ppc64le)
  ifeq ($(AVX),sse4)
    override AVXFLAGS = -D__SSE4_2__ # Power9 VSX with 128 width (VSR registers)
  else ifneq ($(AVX),none)
    $(error Unknown AVX='$(AVX)': only 'none' and 'sse4' are supported on PowerPC for the moment)
  endif
else
  ifeq ($(AVX),sse4)
    override AVXFLAGS = -march=nehalem # SSE4.2 with 128 width (xmm registers)
  else ifeq ($(AVX),avx2)
    override AVXFLAGS = -march=haswell # AVX2 with 256 width (ymm registers) [DEFAULT for clang]
  else ifeq ($(AVX),512y)
    override AVXFLAGS = -march=skylake-avx512 -mprefer-vector-width=256 # AVX512 with 256 width (ymm registers) [DEFAULT for gcc]
  else ifeq ($(AVX),512z)
    override AVXFLAGS = -march=skylake-avx512 -DMGONGPU_PVW512 # AVX512 with 512 width (zmm registers)
  else ifneq ($(AVX),none)
    $(error Unknown AVX='$(AVX)': only 'none', 'sse4', 'avx2', '512y' and '512z' are supported)
  endif
endif
# For the moment, use AVXFLAGS everywhere: eventually, use them only in encapsulated implementations?
CXXFLAGS+= $(AVXFLAGS)

# Set the build flags appropriate to each FPTYPE choice (example: "make FPTYPE=f")
###$(info FPTYPE=$(FPTYPE))
ifeq ($(FPTYPE),d)
  CXXFLAGS += -DMGONGPU_FPTYPE_DOUBLE
else ifeq ($(FPTYPE),f)
  CXXFLAGS += -DMGONGPU_FPTYPE_FLOAT
else
  $(error Unknown FPTYPE='$(FPTYPE)': only 'f' and 'd' are supported)
endif

# Set the build flags appropriate to each HELINL choice (example: "make HELINL=1")
###$(info HELINL=$(HELINL))
ifeq ($(HELINL),1)
  CXXFLAGS += -DMGONGPU_INLINE_HELAMPS
else ifneq ($(HELINL),0)
  $(error Unknown HELINL='$(HELINL)': only '0' and '1' are supported)
endif

# Set the build flags appropriate to each HRDCOD choice (example: "make HRDCOD=1")
###$(info HRDCOD=$(HRDCOD))
ifeq ($(HRDCOD),1)
  CXXFLAGS += -DMGONGPU_HARDCODE_CIPC
else ifneq ($(HRDCOD),0)
  $(error Unknown HRDCOD='$(HRDCOD)': only '0' and '1' are supported)
endif

# Set the build flags appropriate to each RNDGEN choice (example: "make RNDGEN=hasNoCurand")
###$(info RNDGEN=$(RNDGEN))
ifeq ($(RNDGEN),hasNoCurand)
  CXXFLAGS += -DMGONGPU_HAS_NO_CURAND
else ifneq ($(RNDGEN),hasCurand)
  $(error Unknown RNDGEN='$(RNDGEN)': only 'hasCurand' and 'hasNoCurand' are supported)
endif

# Build directory "short" tag (defines target and path to the optional build directory)
# (Rationale: keep directory names shorter, e.g. do not include random number generator choice)
override DIRTAG = $(AVX)_$(FPTYPE)_inl$(HELINL)_hrd$(HRDCOD)

# Build lockfile "full" tag (defines full specification of build options that cannot be intermixed)
# (Rationale: avoid mixing of CUDA and no-CUDA environment builds with different random number generators)
override TAG = $(AVX)_$(FPTYPE)_inl$(HELINL)_hrd$(HRDCOD)_$(RNDGEN)

# Build directory: current directory by default, or build.$(DIRTAG) if USEBUILDDIR==1
ifeq ($(USEBUILDDIR),1)
  override BUILDDIR = build.$(DIRTAG)
  override LIBDIR   = ../lib/$(BUILDDIR)
  ###$(info Building in BUILDDIR=$(BUILDDIR) for tag=$(TAG) (USEBUILDDIR=1 is set))
else
  override BUILDDIR = .
  override LIBDIR   = ../lib
  ###$(info Building in BUILDDIR=$(BUILDDIR) for tag=$(TAG) (USEBUILDDIR is not set))
endif
######$(info Building in BUILDDIR=$(BUILDDIR) for tag=$(TAG))

# Enable ccache if USECCACHE=1
ifeq ($(USECCACHE)$(shell echo $(CXX) | grep ccache),1)
  override CXX:=ccache $(CXX)
endif
ifeq ($(USECCACHE)$(shell echo $(AR) | grep ccache),1)
  override AR:=ccache $(AR)
endif
#ifneq ($(NVCC),)
#  ifeq ($(USECCACHE)$(shell echo $(NVCC) | grep ccache),1)
#    override NVCC:=ccache $(NVCC)
#  endif
#endif

# NB there are no cuda objects as we avoid rdc (e.g. HelAmps_sm.cc is included by gcheck.cu)
target=$(LIBDIR)/libmodel_sm.a
cxx_objects=$(addprefix $(BUILDDIR)/, Parameters_sm.o read_slha.o)
cu_objects=

all.$(TAG): $(BUILDDIR)/.build.$(TAG) $(LIBDIR)/.build.$(TAG) $(target)

install: all.$(TAG) $(INSTALL_INC_DIR) $(addprefix $(INSTALL_INC_DIR)/, $(INSTALL_HEADERS))

$(BUILDDIR)/.build.$(TAG): $(LIBDIR)/.build.$(TAG)

override oldtagsb=`if [ -d $(BUILDDIR) ]; then find $(BUILDDIR) -maxdepth 1 -name '.build.*' ! -name '.build.$(TAG)' -exec echo $(shell pwd)/{} \; ; fi`
override oldtagsl=`if [ -d $(LIBDIR) ]; then find $(LIBDIR) -maxdepth 1 -name '.build.*' ! -name '.build.$(TAG)' -exec echo $(shell pwd)/{} \; ; fi`
$(LIBDIR)/.build.$(TAG):
	@if [ "$(oldtagsl)" != "" ]; then echo -e "Cannot build for tag=$(TAG) as old builds exist in $(LIBDIR) for other tags:\n$(oldtagsl)\nPlease run 'make clean' first\nIf 'make clean' is not enough: run 'make clean USEBUILDDIR=1 AVX=$(AVX) FPTYPE=$(FPTYPE)' or 'make cleanall'"; exit 1; fi
	@if [ "$(oldtagsb)" != "" ]; then echo -e "Cannot build for tag=$(TAG) as old builds exist in $(BUILDDIR) for other tags:\n$(oldtagsb)\nPlease run 'make clean' first\nIf 'make clean' is not enough: run 'make clean USEBUILDDIR=1 AVX=$(AVX) FPTYPE=$(FPTYPE)' or 'make cleanall'"; exit 1; fi
	@if [ ! -d $(LIBDIR) ]; then echo "mkdir -p $(LIBDIR)"; mkdir -p $(LIBDIR); fi
	@touch $(LIBDIR)/.build.$(TAG)
	@if [ ! -d $(BUILDDIR) ]; then echo "mkdir -p $(BUILDDIR)"; mkdir -p $(BUILDDIR); fi
	@touch $(BUILDDIR)/.build.$(TAG)

debug: OPTFLAGS = -g -O0 -DDEBUG2
debug: $(target)

$(INSTALL_INC_DIR) :
	if [ ! -d $(INSTALL_INC_DIR) ]; then mkdir $(INSTALL_INC_DIR); fi

$(INSTALL_INC_DIR)/%.h : %.h
	@if [ ! -d $(INSTALL_INC_DIR) ]; then mkdir $(INSTALL_INC_DIR); fi
	cp $< $@

# NB: cuda includes for curand.h are no longer needed in the C++ code anywhere in src!
$(BUILDDIR)/%.o : %.cc *.h
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(target): $(cxx_objects) $(cu_objects)
	@if [ ! -d $(LIBDIR) ]; then mkdir -p $(LIBDIR); fi
	$(AR) cru $@ $(cxx_objects) $(cu_objects)
	$(RANLIB) $(target)

.PHONY: clean

clean:
	rm -rf $(LIBDIR)
ifneq ($(BUILDDIR),.)
	rm -rf $(BUILDDIR)
else
	rm -f $(BUILDDIR)/.build.* $(BUILDDIR)/*.o $(BUILDDIR)/*.exe
endif
	if [ -d $(INSTALL_INC_DIR) ]; then rm -rf $(INSTALL_INC_DIR); fi

cleanall:
	@echo
	make clean
	@echo
	rm -rf build.*
