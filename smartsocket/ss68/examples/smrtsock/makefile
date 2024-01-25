#
# Copyright (c) 1991-2006 TIBCO Software Inc.
# All rights reserved.
# For more information, please contact:
# TIBCO Software Inc., Palo Alto, California, USA
#
# $Id: //dev/packages/examples/ss/dev/68/src/c/makefile#1 $
#

CFLAGS = -g $(ARCH_FLAGS)

MOTIF_LIBS = -lXm -lXt -lX11

HP700_HPUX_FLAGS = -I/usr/include/Motif1.2 -I/usr/include/X11R5 -Wp,-H200000 -L/usr/lib/Motif1.2 -L/usr/lib/X11R5

# This is for Talarian's environment.  SMW 09/06/95
SUN4_SUNOS_FLAGS = -I/usr/local/x11r5/$(RTARCH)/include -L/usr/local/x11r5/$(RTARCH)/lib

SUN4_SOLARIS_FLAGS = -I/usr/openwin/include -I/usr/dt/include -L/usr/openwin/lib -L/usr/dt/lib -R/usr/openwin/lib -R/usr/dt/lib
I86_SOLARIS_FLAGS = $(SUN4_SOLARIS_FLAGS)

programs: watchsbj.x monitor.x waitsbj.x readmt.x motifipc.$(RTARCH)

watchsbj.x: watchsbj.c
	rtlink -o watchsbj.x watchsbj.c 

monitor.x: monitor.c
	rtlink -o monitor.x monitor.c 

waitsbj.x: waitsbj.c
	rtlink -o waitsbj.x waitsbj.c 

readmt.x: readmt.c
	rtlink -o readmt.x readmt.c

#------------------------------------------
# motifipc.c - OSF/Motif external process
#------------------------------------------

# This didn't work with HP-UX 9.01 make, so we now use motifipc.$(RTARCH)
# instead of motifipc_recursive.  SMW 10/19/95
#motifipc_recursive:
#	make ARCH_FLAGS="$($(RTARCH_UC)_FLAGS)" motifipc.x

# No special flags needed on some platforms.  SMW 10/19/95
motifipc.iris_irix motifipc.rs6000_aix motifipc.alpha_osf: motifipc.x

motifipc.sun4_sunos:
	make ARCH_FLAGS="$(SUN4_SUNOS_FLAGS)" motifipc.x

motifipc.sun4_solaris:
	make ARCH_FLAGS="$(SUN4_SOLARIS_FLAGS)" motifipc.x

motifipc.hp700_hpux motifipc.ia64_hpux:
	make ARCH_FLAGS="$(HP700_HPUX_FLAGS)" motifipc.x

motifipc.i86_solaris:
	make ARCH_FLAGS="$(I86_SOLARIS_FLAGS)" motifipc.x

motifipc.i86_linux motifipc.ia64_linux:

motifipc.x: motifipc.c
	rtlink $(CFLAGS) -o motifipc.x motifipc.c $(MOTIF_LIBS)
clean::
	/bin/rm -f *.CKP *.ln *.BAK *.bak *.o *.x core errs ,* *~ *.a \
		.emacs_* tags make.log MakeOut \#*
