# Module.mk for graf module
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

MODDIR       := graf
MODDIRS      := $(MODDIR)/src
MODDIRI      := $(MODDIR)/inc

GRAFDIR      := $(MODDIR)
GRAFDIRS     := $(GRAFDIR)/src
GRAFDIRI     := $(GRAFDIR)/inc

##### libGraf #####
GRAFL1       := $(MODDIRI)/LinkDef1.h
GRAFL2       := $(MODDIRI)/LinkDef2.h
GRAFDS1      := $(MODDIRS)/G__Graf1.cxx
GRAFDS2      := $(MODDIRS)/G__Graf2.cxx
GRAFDO1      := $(GRAFDS1:.cxx=.o)
GRAFDO2      := $(GRAFDS2:.cxx=.o)
GRAFDS       := $(GRAFDS1) $(GRAFDS2)
GRAFDO       := $(GRAFDO1) $(GRAFDO2)
GRAFDH       := $(GRAFDS:.cxx=.h)

GRAFH        := $(filter-out $(MODDIRI)/LinkDef%,$(wildcard $(MODDIRI)/*.h))
GRAFHD       := $(filter-out $(MODDIRI)/TTF.h,$(GRAFH))
GRAFHD       := $(filter-out $(MODDIRI)/TText.h,$(GRAFHD))
GRAFHD       := $(filter-out $(MODDIRI)/TLatex.h,$(GRAFHD))
GRAFS        := $(filter-out $(MODDIRS)/G__%,$(wildcard $(MODDIRS)/*.cxx))
GRAFO        := $(GRAFS:.cxx=.o)

GRAFDEP      := $(GRAFO:.o=.d) $(GRAFDO:.o=.d)

GRAFLIB      := $(LPATH)/libGraf.$(SOEXT)

# used in the main Makefile
ALLHDRS     += $(patsubst $(MODDIRI)/%.h,include/%.h,$(GRAFH))
ALLLIBS     += $(GRAFLIB)

# include all dependency files
INCLUDEFILES += $(GRAFDEP)

##### local rules #####
include/%.h:    $(GRAFDIRI)/%.h
		cp $< $@

$(GRAFLIB):     $(GRAFO) $(GRAFDO) $(FREETYPELIB) $(MAINLIBS) $(GRAFLIBDEP)
		@$(MAKELIB) $(PLATFORM) $(LD) "$(LDFLAGS)" \
		   "$(SOFLAGS)" libGraf.$(SOEXT) $@ \
		   "$(GRAFO) $(GRAFDO)" "$(FREETYPELIB) $(GRAFLIBEXTRA)"

$(GRAFDS1):     $(GRAFHD) $(GRAFL1) $(ROOTCINTTMP)
		@echo "Generating dictionary $@..."
		$(ROOTCINTTMP) -f $@ -c $(GRAFHD) $(GRAFL1)
$(GRAFDS2):     $(GRAFH) $(GRAFL2) $(ROOTCINTTMP)
		@echo "Generating dictionary $@..."
		$(ROOTCINTTMP) -f $@ -c $(GRAFH) $(GRAFL2)

ifneq ($(ARCH),alphacxx6)
$(GRAFDO1):     $(GRAFDS1)
		$(CXX) $(NOOPT) $(CXXFLAGS) -I. -o $@ -c $<
$(GRAFDO2):     $(GRAFDS2) $(FREETYPELIB)
		$(CXX) $(NOOPT) -I$(FREETYPEDIRI) $(CXXFLAGS) -I. -o $@ -c $<
else
$(GRAFDO1):     $(GRAFDS1)
		$(CXX) $(DEBUGFLAGS) $(CXXFLAGS) -I. -o $@ -c $<
$(GRAFDO2):     $(GRAFDS2) $(FREETYPELIB)
		$(CXX) $(DEBUGFLAGS) -I$(FREETYPEDIRI) $(CXXFLAGS) -I. -o $@ -c $<
endif

all-graf:       $(GRAFLIB)

clean-graf:
		@rm -f $(GRAFO) $(GRAFDO)

clean::         clean-graf

distclean-graf: clean-graf
		@rm -f $(GRAFDEP) $(GRAFDS) $(GRAFDH) $(GRAFLIB)

distclean::     distclean-graf

##### extra rules ######
graf/src/TTF.o: graf/src/TTF.cxx $(FREETYPELIB)
		$(CXX) $(OPT) -I$(FREETYPEDIRI) $(CXXFLAGS) -o $@ -c $<

graf/src/TText.o: graf/src/TText.cxx $(FREETYPELIB)
		$(CXX) $(OPT) -I$(FREETYPEDIRI) $(CXXFLAGS) -o $@ -c $<

graf/src/TLatex.o: graf/src/TLatex.cxx $(FREETYPELIB)
		$(CXX) $(OPT) -I$(FREETYPEDIRI) $(CXXFLAGS) -o $@ -c $<
