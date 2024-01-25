$ !
$ ! Copyright (c) 1991-2006 TIBCO Software Inc.
$ ! All rights reserved.
$ ! For more information, please contact:
$ ! TIBCO Software Inc., Palo Alto, California, USA
$ !
$ ! $Id: //dev/packages/examples/ss/dev/68/src/bench/server/startsrv.com#2 $
$ !
$
$ ! startsrv.com -- start benchmark RTserver
$
$ ! Use a remote RTserver if BENCH_SERVER_NODE is set.
$ if (f$trnlnm("BENCH_SERVER_NODE") .nes. "")
$ then
$   ! Getting the directory right can be tricky, so allow it to be set.
$   if (f$trnlnm("BENCH_DIR") .nes. "")
$   then
$     bench_dir = f$trnlnm("BENCH_DIR")
$   else
$     ! Try to support UNIX<-->VMS too.  SMW 11/20/94
$     if (f$trnlnm("BENCH_SERVER_UNIX") .nes. "")
$     then
$       bench_dir = "$RTHOME/examples/bench/server"
$     else
$       bench_dir = "rthome:[examples.bench.server]"
$     endif
$   endif
$   if (f$trnlnm("BENCH_SERVER_UNIX") .nes. "")
$   then
$     ! May need to use rtrsh/output= here if NFS delays cause problems.
$     rtrsh/output=server.out 'f$trnlnm("BENCH_SERVER_NODE") -
        rtserver -no_daemon -threads 'num_rtserver_threads' -
        -command 'bench_dir'/rtserver.cmd
$   else
$     ! MCR is an undocumented VMS command that combines RUN and foreign
$     ! commmands in one step.  SMW 11/20/94
$     rtrsh/output=server.out 'f$trnlnm("BENCH_SERVER_NODE") -
        mcr rtserver -no_daemon -threads 'num_rtserver_threads' -
        -command 'bench_dir'rtserver.cmd
$   endif
$ else
$   rtserver -no_daemon > server.out
$ endif
