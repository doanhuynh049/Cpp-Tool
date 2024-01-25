#
# Copyright (c) 1991-2006 TIBCO Software Inc.
# All rights reserved.
# For more information, please contact:
# TIBCO Software Inc., Palo Alto, California, USA
#
# $Id: //dev/packages/examples/ss/dev/68/src/bench/server/makefile#1 $
#

# makefile for benchmark programs

CDEBUGFLAGS =
CFLAGS = $(CDEBUGFLAGS) -DT_OPTIMIZE_CODE
EXECUTABLES = send.x receive.x srvbench.x
RTLINK = rtlink -verbose -optimized $(BENCH_EXTRA_LINK_FLAGS)
GENERATED_FILE = gen.c
GRAMMAR_FILE = grammar.dat

all:: $(EXECUTABLES)

maketype.x : maketype.c
	$(RTLINK) $(CFLAGS) maketype.c -o maketype.x

$(GENERATED_FILE): maketype.x $(GRAMMAR_FILE)
	./maketype.x $(GRAMMAR_FILE)

srvbench.x : srvbench.c $(GENERATED_FILE) srvbench.h
	$(RTLINK) $(CFLAGS) srvbench.c $(GENERATED_FILE) -o srvbench.x

send.x : send.c $(GENERATED_FILE) srvbench.h
	$(RTLINK) $(CFLAGS) send.c $(GENERATED_FILE) -o send.x

receive.x : receive.c $(GENERATED_FILE) srvbench.h
	$(RTLINK) $(CFLAGS) receive.c $(GENERATED_FILE) -o receive.x

clean::
	rm -f *.o *.x *.tim *.out gen.c
