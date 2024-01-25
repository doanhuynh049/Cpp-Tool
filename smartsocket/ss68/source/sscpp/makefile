#
# Copyright (c) 1991-2006 TIBCO Software Inc.
# All rights reserved.
# For more information, please contact:
# TIBCO Software Inc., Palo Alto, California, USA
#
# $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/linux.mk#1 $
#

##------------------------------------------------------------------------
## This is a sample makefile to build the SmartSockets C++ Class 
## Library using the g++ compiler on Linux.  Several make macros are 
## defined so that this file can be tailored to an individual's needs.
## 
## The following targets are supported:
##
## all       -- builds a C++ class library named $(LIBNAME) in a directory
##              named $(LIBDIR).
## clean     -- clean all the files in $(CLEAN_FILES) from this source 
##              directory.
## install   -- is synonymous with the all target
## uninstall -- removes the library named $(LIBNAME) from $(LIBDIR). 
##


##------------------------------------------------------------------------
## Suffix list for implicit rules
##

.SUFFIXES: .cxx .o .a .c 


##------------------------------------------------------------------------
## Shell commands used by this makefile.
##
## RM -- remove file command
## MV -- move/rename file command
## AR -- archive/create library command

RM = rm -f
MV = mv
AR = ar cq


##------------------------------------------------------------------------
## Environment used by this makefile.
##
## CLEAN_FILES -- files to delte with the clean target
## LIBDIR      -- directory to store created object library
## LIBNAMAE    -- name of object library
## DEFINES     -- C++ Defines
## RT_INCLUDES -- directory where rtworks include file directory is located

CLEAN_FILES = *.o
DEFINES = -D T_OS_LINUX
LIBDIR = $(RTHOME)/lib/$(RTARCH)
LIBNAME = libsscpp.a
RT_INCLUDES = -I$(RTHOME)/include


##------------------------------------------------------------------------
## C++ compiler used in this makefile.
CC = g++

C_DEBUG_FLAGS = -g

MISC_CFLAGS = 

MISC_INCLUDES = 

ALL_INCLUDES    = $(RT_INCLUDES) $(MISC_INCLUDES)
ALL_DEFINES     = $(OS_DEFINES) $(DEFINES)
ALL_MISC_CFLAGS = $(MISC_CFLAGS) $(C_DEBUG_FLAGS)

CFLAGS = $(ALL_INCLUDES) $(ALL_DEFINES) $(ALL_MISC_CFLAGS)

.cxx.o:
	$(CC) -c $(CFLAGS) $(%CDEBUGFLAGS) $< -o $@

##----------------[ DO NOT EDIT BELOW THIS LINE ]-----------------

CXX_IPC_OBJS = \
	tconn.o \
	tmsg.o \
	tsrv.o \
	tevent.o \
	texnames.o \
	tmsgsize.o \
	tmsgname.o \
	tutil.o \
	tdisp.o \
	tex.o \
	tmon.o \
	tmsgfile.o \
	tmt.o \
	toption.o \
	tsystem.o \
	txml.o \
	tscache.o \
	tcallbck.o

CXX_IPC_SRCS = $(CXX_IPC_OBJS:.o=.cxx)

install all:: $(LIBDIR)/$(LIBNAME)

$(LIBDIR)/$(LIBNAME): $(CXX_IPC_OBJS)
	$(RM) $@ || $(MV) $@ $@~
	$(AR) $@ $(CXX_IPC_OBJS)

clean::
	$(RM) $(CLEAN_FILES)

uninstall::
	$(RM) $(LIBDIR)/$(LIBNAME)






