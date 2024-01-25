/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/burst/readme.txt#1 $
 */

TipcBurst Example Library
-------------------------

This directory has the source files to the TipcBurst API. The API is mainly intended for clients
that are publishing smaller messages and intend to coalesce these smaller message into one
large messsage and then to be disassembled at the receiver side. This greatly helps in reducing
the message overhead.


1. burst.c  -- has the c code for the TipcBurst API's
2. burstp.h -- private header file that the BURST data structures
3. burst.h  -- public header file that needs to be included by client programs
4. burstpr.h -- prototype file that gets included if burst.h is included in the client program
5. includes.h -- used to build the TipcBurst Library
6. makefile.unix -- a makefile for unix to build the shared library

Building the TipcBurst Library

Win32
----
Building the debug library
-------------------------------
cl.exe -c /nologo /GX /YX /MDd /GD /Zi burst.c
link.exe /debug /subsystem:windows /incremental:no /dll /nologo /machine:I386 /entry:"_CRT_INIT@12" burst.obj /out:tipcburstd.dll tipcg.lib tutilg.lib

Building the Optimized Library
-----------------------------
cl -c /GX /YX /MD /GD /O2 burst.c
link.exe /subsystem:windows /incremental:no /dll /machine:I386 /entry:"_CRT_INIT@12" burst.obj /out:tipcburst.dll tipc.lib tutil.lib

Unix:
----
make -f makefile.unix
