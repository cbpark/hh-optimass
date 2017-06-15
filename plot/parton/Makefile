SRCDIR   := src
BINDIR   := bin
CXXFLAGS := -g -O2 -Wall -Wextra -std=c++11 -pedantic -I$(SRCDIR)
LDFLAGS  :=
MKDIR    := mkdir -p
RM       := rm -f

# ROOT (http://root.cern.ch/)
CXXFLAGS += $(shell root-config --noauxcflags --cflags)
LDFLAGS  += $(shell root-config --ldflags) \
	-Wl,-rpath,$(shell root-config --libdir)
LIBS     += $(shell root-config --noauxlibs --glibs)

# Targets
EXE    := $(BINDIR)/mT2_ll $(BINDIR)/mT2_bbll $(BINDIR)/dphi_ll $(BINDIR)/dR_ll \
	$(BINDIR)/mhh_true $(BINDIR)/mhh_om $(BINDIR)/mhh_compare $(BINDIR)/delta_mhh \
	$(BINDIR)/compat_distance $(BINDIR)/cd_dphi_ll
EXESRC := $(patsubst $(BINDIR)/%,$(SRCDIR)/%.cc,$(EXE))
EXEOBJ := $(EXESRC:.cc=.o)

.PHONY: all build clean

all: $(EXE)

$(BINDIR)/%: $(SRCDIR)/%.o build
	$(CXX) $(LDFLAGS) -o $@ $< $(LIBS)

build:
	$(MKDIR) $(BINDIR)

clean::
	$(RM) $(EXEOBJ)
	$(RM) $(EXE)
	$(RM) -r $(BINDIR)
