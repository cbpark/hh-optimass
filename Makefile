# Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
#
# This file is part of hh-optimass, which is released under the GNU General
# Public License. See file LICENSE in the top directory of this project or
# go to <http://www.gnu.org/licenses/> for full license details.

SRCDIR   := src
BINDIR   := bin
LIBDIR   := lib
CXXFLAGS := -g -O2 -march=native -Wall -Wextra -std=c++11 -pedantic \
	-I$(SRCDIR) $(CXXFLAGS)
LDFLAGS  := -O2 $(LDFLAGS)
LIBS     :=
AR       := ar crs
MKDIR    := mkdir -p
RM       := rm -f

# CLHEF (https://github.com/cbpark/CLHEF)
CLHEF    := /usr/local
CXXFLAGS += -I$(CLHEF)/include
LDFLAGS  += -Wl,-rpath,$(CLHEF)/lib
LIBS     += -L$(CLHEF)/lib -lCLHEF

# Targets
EXE    := $(BINDIR)/hh_optimass_parton
EXESRC := $(patsubst $(BINDIR)/%,$(SRCDIR)/%.cc,$(EXE))
EXEOBJ := $(EXESRC:.cc=.o)
LIB    := $(LIBDIR)/libHHOM.a
LIBSRC := $(filter-out $(EXESRC),$(wildcard $(SRCDIR)/*.cc))
LIBOBJ := $(LIBSRC:.cc=.o)

.PHONY: all build clean

all: $(EXE)

$(BINDIR)/%: $(SRCDIR)/%.o build $(LIB)
	$(CXX) $(LDFLAGS) -o $@ $< $(LIB) $(LIBS)

$(LIB): CXXFLAGS += -fPIC
$(LIB): $(LIBOBJ)
	$(AR) $@ $^
	ranlib $@

build:
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(BINDIR)

clean::
	$(RM) $(EXEOBJ) $(LIBOBJ)
	$(RM) $(EXE) $(LIB)
	$(RM) -r $(BINDIR) $(LIBDIR)
