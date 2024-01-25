#
# Copyright (c) 1991-2006 TIBCO Software Inc.
# All rights reserved.
# For more information, please contact:
# TIBCO Software Inc., Palo Alto, California, USA
#
# $Id: //dev/packages/examples/ss/dev/68/src/cxx/makefile#1 $
#

CFLAGS = -g $(ARCH_FLAGS)

programs: conn_clt.x conn_srv.x mon.x msg1.x msg2.x rcvr.x rtcltrcv.x rtcltsnd.x sndr.x

conn_clt.x: conn_clt.cxx
	rtlink -cxx -o conn_clt.x conn_clt.cxx

conn_srv.x: conn_srv.cxx
	rtlink -cxx -o conn_srv.x conn_srv.cxx

mon.x: mon.cxx
	rtlink -cxx -o mon.x mon.cxx

msg1.x: msg1.cxx
	rtlink -cxx -o msg1.x msg1.cxx

msg2.x: msg2.cxx
	rtlink -cxx -o msg2.x msg2.cxx

rcvr.x: rcvr.cxx
	rtlink -cxx -o rcvr.x rcvr.cxx

rtcltrcv.x: rtcltrcv.cxx rtclient.hxx
	rtlink -cxx -o rtcltrcv.x rtcltrcv.cxx

rtcltsnd.x: rtcltsnd.cxx rtclient.hxx
	rtlink -cxx -o rtcltsnd.x rtcltsnd.cxx

sndr.x: sndr.cxx
	rtlink -cxx -o sndr.x sndr.cxx

clean::
	/bin/rm -f *.CKP *.ln *.BAK *.bak *.o *.x core errs ,* *~ *.a \
		  .emacs_* tags make.log MakeOut \#*
