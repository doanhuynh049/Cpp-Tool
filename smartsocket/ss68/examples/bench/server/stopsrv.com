$ !
$ ! Copyright (c) 1991-2006 TIBCO Software Inc.
$ ! All rights reserved.
$ ! For more information, please contact:
$ ! TIBCO Software Inc., Palo Alto, California, USA
$ !
$ ! $Id: //dev/packages/examples/ss/dev/68/src/bench/server/stopsrv.com#1 $
$ !
$
$ ! stop_rtserver.com -- stop benchmark RTserver
$
$ ! Use a remote RTserver if BENCH_SERVER_NODE is set.  SMW 11/19/94
$ if (f$trnlnm("BENCH_SERVER_NODE") .nes. "")
$ then
$   rtserver -server_names 'f$trnlnm("BENCH_SERVER_NODE") -stop_clients > rtstopsrv.out
$ else
$   rtserver -stop_clients > rtstopsrv.out
$ endif
