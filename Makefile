# Copyright (c) 2017 Chan Beom Park <cbpark@gmail.com>
#
# This file is part of hh-optimass, which is released under the GNU General
# Public License. See file LICENSE in the top directory of this project or
# go to <http://www.gnu.org/licenses/> for full license details.

PKGNAME  := hh-optimass
SRCDIR   := src
BINDIR   := bin
LIBDIR   := lib
DESTDIR  ?= /usr/local
CXXFLAGS := -g -O2 -march=native -Wall -Wextra -std=c++11 -pedantic \
	-I$(SRCDIR) $(CXXFLAGS)
LDFLAGS  := -O2 $(LDFLAGS)
LIBS     :=
AR       := ar crs
MKDIR    := mkdir -p
RM       := rm -f

# CLHEF (https://github.com/cbpark/CLHEF)
CLHEF    ?= /usr/local
CXXFLAGS += -I$(CLHEF)/include
LDFLAGS  += -Wl,-rpath,$(CLHEF)/lib
LIBS     += -L$(CLHEF)/lib -lCLHEF

# OptiMass/alm_base
OPTIMASS ?= /usr/local
CXXFLAGS += -I$(OPTIMASS)/include
LDFLAGS  += -Wl,-rpath,$(OPTIMASS)/lib
LIBS     += -L$(OPTIMASS)/lib -lOptiMass

# ROOT (http://root.cern.ch/)
CXXFLAGS += $(shell root-config --noauxcflags --cflags)
LDFLAGS  += $(shell root-config --ldflags) \
	-Wl,-rpath,$(shell root-config --libdir)
LIBS     += $(shell root-config --noauxlibs --libs) -lMinuit2

# Targets
EXE        := $(BINDIR)/hh_optimass_parton
EXESRC     := $(patsubst $(BINDIR)/%,$(SRCDIR)/%.cc,$(EXE))
EXEOBJ     := $(EXESRC:.cc=.o)
LIB        := $(LIBDIR)/libHHOM.a
LIBSRC     := $(filter-out $(EXESRC),$(wildcard $(SRCDIR)/*.cc))
LIBOBJ     := $(LIBSRC:.cc=.o)
NOINSTHEAD := $(SRCDIR)/user_interface.h $(SRCDIR)/utils.h
HEAD       := $(filter-out $(EXESRC:.cc=.h),\
	$(filter-out $(NOINSTHEAD),$(wildcard $(SRCDIR)/*.h)))

.PHONY: all debug build install clean

all: $(EXE)

debug: CXXFLAGS += -DDEBUG
debug: all

$(BINDIR)/%: $(SRCDIR)/%.o build $(LIB)
	$(CXX) $(LDFLAGS) -o $@ $< $(LIB) $(LIBS)

$(LIB): CXXFLAGS += -fPIC
$(LIB): $(LIBOBJ)
	$(AR) $@ $^
	ranlib $@

build:
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(BINDIR)

install: all
	install -d $(DESTDIR)/bin $(DESTDIR)/lib $(DESTDIR)/include/$(PKGNAME)
	install -s -m 755 $(EXE) $(DESTDIR)/bin
	install $(LIB) $(DESTDIR)/lib
	install -m 644 $(HEAD) $(DESTDIR)/include/$(PKGNAME)

clean::
	$(RM) $(EXEOBJ) $(LIBOBJ)
	$(RM) $(EXE) $(LIB)
	$(RM) -r $(BINDIR) $(LIBDIR)
