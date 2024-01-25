#
# Copyright (c) 1991-2006 TIBCO Software Inc.
# All rights reserved.
# For more information, please contact:
# TIBCO Software Inc., Palo Alto, California, USA
#
# $Id: //dev/packages/examples/ss/dev/68/src/c/manual/makefile#1 $
#

CFLAGS =
RTLINK = rtlink

all:: msg.x connserv.x connclnt.x conngmds.x conngmdc.x \
      connmtc.x connmts1.x connmts2.x rtclrcv.x rtclsnd.x \
      rtcladv.x rtclgmdr.x rtclgmds.x rtsvcc.x rtsvcp.x \
      lbsend.x lbrecv.x polltraf.x watchsbj.x guardian.x \
      udrecv.x udsend.x rtsubjcb.x rtsndsub.x request.x response.x

purify::
	$(MAKE) RTLINK="rtlink -purify -g" all

msg.x: msg.c
	$(RTLINK) $(CFLAGS) -o msg.x msg.c 

connserv.x: connserv.c
	$(RTLINK) $(CFLAGS) -o connserv.x connserv.c 

connclnt.x: connclnt.c
	$(RTLINK) $(CFLAGS) -o connclnt.x connclnt.c 

conngmds.x: conngmds.c conngmdu.c conngmd.h
	$(RTLINK) $(CFLAGS) -o conngmds.x conngmds.c conngmdu.c

conngmdc.x: conngmdc.c conngmdu.c conngmd.h
	$(RTLINK) $(CFLAGS) -o conngmdc.x conngmdc.c conngmdu.c

connmtc.x: connmtc.c
	$(RTLINK) $(CFLAGS) -o connmtc.x connmtc.c

connmts1.x: connmts1.c
	$(RTLINK) $(CFLAGS) -o connmts1.x connmts1.c

connmts2.x: connmts2.c
	$(RTLINK) $(CFLAGS) -o connmts2.x connmts2.c

rtclrcv.x: rtclrcv.c rtclutil.c rtclient.h
	$(RTLINK) $(CFLAGS) -o rtclrcv.x rtclrcv.c rtclutil.c

rtclsnd.x: rtclsnd.c rtclutil.c rtclient.h
	$(RTLINK) $(CFLAGS) -o rtclsnd.x rtclsnd.c rtclutil.c

rtcladv.x: rtcladv.c rtclient.h
	$(RTLINK) $(CFLAGS) -o rtcladv.x rtcladv.c 

rtclgmdr.x: rtclgmdr.c rtclient.h
	$(RTLINK) $(CFLAGS) -o rtclgmdr.x rtclgmdr.c 

rtclgmds.x: rtclgmds.c rtclient.h
	$(RTLINK) $(CFLAGS) -o rtclgmds.x rtclgmds.c 

rtsvcc.x: rtsvcc.c
	$(RTLINK) $(CFLAGS) -o rtsvcc.x rtsvcc.c

rtsvcp.x: rtsvcp.c
	$(RTLINK) $(CFLAGS) -o rtsvcp.x rtsvcp.c

lbsend.x: lbsend.c
	$(RTLINK) $(CFLAGS) -o lbsend.x lbsend.c

lbrecv.x: lbrecv.c
	$(RTLINK) $(CFLAGS) -o lbrecv.x lbrecv.c

polltraf.x: polltraf.c
	$(RTLINK) $(CFLAGS) -o polltraf.x polltraf.c 

watchsbj.x: watchsbj.c
	$(RTLINK) $(CFLAGS) -o watchsbj.x watchsbj.c 

guardian.x: guardian.c
	$(RTLINK) $(CFLAGS) -o guardian.x guardian.c 

udrecv.x: udrecv.c
	$(RTLINK) $(CFLAGS) -o udrecv.x udrecv.c 

udsend.x: udsend.c
	$(RTLINK) $(CFLAGS) -o udsend.x udsend.c 

rtsubjcb.x: rtsubjcb.c
	$(RTLINK) $(CFLAGS) -o rtsubjcb.x rtsubjcb.c 

rtsndsub.x: rtsndsub.c
	$(RTLINK) $(CFLAGS) -o rtsndsub.x rtsndsub.c 

request.x: request.c
	$(RTLINK) $(CFLAGS) -o request.x request.c 

response.x: response.c
	$(RTLINK) $(CFLAGS) -o response.x response.c 

clean::
	/bin/rm -f *.CKP *.ln *.BAK *.bak *.o *.x* core errs ,* *~ *.a \
		  .emacs_* tags *.msg \#*
