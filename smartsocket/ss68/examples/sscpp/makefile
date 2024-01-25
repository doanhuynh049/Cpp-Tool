#
# Copyright (c) 1991-2006 TIBCO Software Inc.
# All rights reserved.
# For more information, please contact:
# TIBCO Software Inc., Palo Alto, California, USA
#
# $Id: //dev/packages/examples/ss/dev/68/src/sscpp/makefile#1 $
#

CFLAGS = -g $(ARCH_FLAGS)

programs: conn_clt.x conn_srv.x mon.x msg1.x msg2.x rcvr.x rcvrcb.x rtcltrcv.x rtcltsnd.x sndr.x

conn_clt.x: conn_clt.cxx
	rtlink -cpp -o conn_clt.x conn_clt.cxx

conn_srv.x: conn_srv.cxx
	rtlink -cpp -o conn_srv.x conn_srv.cxx

mon.x: mon.cxx
	rtlink -cpp -o mon.x mon.cxx

msg1.x: msg1.cxx
	rtlink -cpp -o msg1.x msg1.cxx

msg2.x: msg2.cxx
	rtlink -cpp -o msg2.x msg2.cxx

rcvr.x: rcvr.cxx
	rtlink -cpp -o rcvr.x rcvr.cxx

rcvrcb.x: rcvrcb.cxx
	rtlink -cpp -o rcvrcb.x rcvrcb.cxx

rtcltrcv.x: rtcltrcv.cxx
	rtlink -cpp -o rtcltrcv.x rtcltrcv.cxx

rtcltsnd.x: rtcltsnd.cxx
	rtlink -cpp -o rtcltsnd.x rtcltsnd.cxx

sndr.x: sndr.cxx
	rtlink -cpp -o sndr.x sndr.cxx

clean::
	/bin/rm -f *.CKP *.ln *.BAK *.bak *.o *.x core errs ,* *~ *.a \
		.emacs_* tags make.log MakeOut \#*
