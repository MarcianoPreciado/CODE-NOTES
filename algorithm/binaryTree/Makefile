# Generic Makefile for compiling a simple executable.
#
CC := ~/.vim/bin/cc_args.py gcc
CXX := ~/.vim/bin/cc_args.py g++
#CC := gcc
#CXX := g++
SRCDIR := .
BUILDDIR := ./build
INCPATH := -I. -I/usr/include
#DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
LINK = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu #-lQtGui -lQtCore -lpthread 
TARGET := program
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.deps)
 
$(TARGET): $(OBJECTS) 
	@echo " Linking..."; $(CXX) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
 
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) 
	@mkdir -p $(BUILDDIR) 
	@echo " CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $<
 
clean: 
	@echo " Cleaning..."; $(RM) -r $(BUILDDIR) $(OBJECTS)
 
distclean: 
	@echo " Dist Cleaning..."; $(RM) -r $(BUILDDIR) $(OBJECTS) $(TARGET)

-include $(DEPS)
 
.PHONY: clean
