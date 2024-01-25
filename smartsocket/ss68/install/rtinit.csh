:

#
# Copyright (c) 1991-2006 TIBCO Software Inc.
# All rights reserved.
# For more information, please contact:
# TIBCO Software Inc., Palo Alto, California, USA
#
# $Id: //dev/packages/build/dev/68/src/scripts/rtinit.csh#3 $
#

#set echo
if ($?RTVERBOSE) then
  set echo
endif

#################################################################
# Skip setting mass quantities on env vars if we have already
# done so.  SMW 03/28/94
#################################################################
if ($?RTUSER_SHELL) then
  exit 0
endif

# This script is the bare-bones setup needed to run RTworks.

# Must do this first!
if ($?FORCE_RTHOME) then
  setenv RTHOME $FORCE_RTHOME
else
  setenv RTHOME /home/rtworks
endif

# Now set the architecture.
if (-e $RTHOME/bin/rtarch) then
  setenv RTARCH `$RTHOME/bin/rtarch`
  setenv RTARCH_UC `echo $RTARCH | tr '[a-z]' '[A-Z]'`
  # Set the sub arch.  SMW 10/02/94
  if (`$RTHOME/bin/rtarch -sub_arch` == tandem) then
    setenv RTTANDEM 1
  endif
else
  echo RTworks is not installed correctly in $RTHOME.
  echo The shell script $RTHOME/bin/rtarch is missing.
endif

# This bypasses VI's DVinit script.
setenv DVHOME $RTHOME/graphics/$RTARCH

# Add RTworks to the path.
set path = ($path $RTHOME/bin $RTHOME/bin/$RTARCH)

# -----------------------------------------------------------------------------
# Add RTworks to the X app-defaults path.  We use XUSERFILESEARCHPATH instead
# of XAPPLRESDIR because XUSERFILESEARCHPATH is more generic and flexible.
# If XAPPLRESDIR is set, then we tack it on to the end of XUSERFILESEARCHPATH,
# because X willl ignore XAPPLRESIDR if XUSERFILESEARCHPATH is set (see p.
# 666-667 of "X Window System Toolkit" by Asente and Swick).  Things are
# further complicated by a bug in the shell which botches environment variables
# whose names are EXACTLY 19 characters long (the Bourne shell is OK). We can
# set XUSERFILESEARCHPATH with setenv, but we cannot get at it with
# $XUSERFILESEARCHPATH, nor can we check it with $?XUSERFILESEARCHPATH. We
# therefore have to resort to using env, grep, and awk (barf).  SMW
# -----------------------------------------------------------------------------
set _app = $RTHOME/app-defaults
env | grep "^XUSERFILESEARCHPATH=" >& /dev/null
if ($status == 0) then
  set _old_path = `env | grep "^XUSERFILESEARCHPATH=" | awk -F= '{print $2}'`
  setenv XUSERFILESEARCHPATH $_old_path\:$_app/$RTARCH/%N:$_app/%N
  unset _old_path
else
  setenv XUSERFILESEARCHPATH $_app/$RTARCH/%N:$_app/%N
endif
unset _app

if ($?XAPPLRESDIR) then
  set _old_path = `env | grep "^XUSERFILESEARCHPATH=" | awk -F= '{print $2}'`
  setenv XUSERFILESEARCHPATH $_old_path\:$XAPPLRESDIR/%N
  unset _old_path
  #echo The environment variable XAPPLRESDIR is obsolete.
  #echo Please use XUSERFILESEARCHPATH instead.
endif

# -----------------------------------------------------------------------------
# On SunOS 4.1.X we have to set XKEYSYMDB and XNLSPATH to use
# our X11R5 files.  SMW 03/31/94
# -----------------------------------------------------------------------------
if ($RTARCH == sun4_sunos) then
  setenv XKEYSYMDB $RTHOME/lib/$RTARCH/XKeysymDB
  setenv XNLSPATH $RTHOME/lib/$RTARCH/nls
endif

# -----------------------------------------------------------------------------
# Set LD_LIBRARY_PATH to include our shared libraries.
# -----------------------------------------------------------------------------
switch ($RTARCH)
  case iris_irix:
  case alpha_osf:
  case ia64_linux:
  case i86_linux:
  case i86_solaris:
  case sun4_solaris:
    if ($?LD_LIBRARY_PATH) then
      setenv LD_LIBRARY_PATH $RTHOME/lib/$RTARCH\:$LD_LIBRARY_PATH
    else
      # LD_LIBRARY_PATH not yet set.
      setenv LD_LIBRARY_PATH $RTHOME/lib/$RTARCH
    endif
    switch ($RTARCH)
      case i86_linux:
        setenv LD_LIBRARY_PATH $RTHOME/lib/$RTARCH/lib64:$LD_LIBRARY_PATH
        breaksw
      case i86_solaris:
        setenv LD_LIBRARY_PATH $RTHOME/lib/$RTARCH/amd64:$LD_LIBRARY_PATH
        breaksw
      case sun4_solaris:
        setenv LD_LIBRARY_PATH $RTHOME/lib/$RTARCH/sparcv9:$LD_LIBRARY_PATH
        breaksw
    endsw
    breaksw
  # ---------------------------------------------------------------------------
  # (HP-UX) Set SHLIB_PATH to include our shared libraries.
  # ---------------------------------------------------------------------------
  case hp700_hpux:
  case ia64_hpux:
    if ($?SHLIB_PATH) then
      setenv SHLIB_PATH $SHLIB_PATH\:$RTHOME/lib/$RTARCH
    else
      # SHLIB_PATH not yet set.
      setenv SHLIB_PATH $RTHOME/lib/$RTARCH
    endif
    switch ($RTARCH)
      case hp700_hpux:
        setenv SHLIB_PATH $SHLIB_PATH\:$RTHOME/lib/$RTARCH/pa20_64
        breaksw
      case ia64_hpux:
        setenv SHLIB_PATH $SHLIB_PATH\:$RTHOME/lib/$RTARCH/hpux64
        breaksw
    endsw
    #--------------------------------------------------------------------------
    # Set these to vary the memory arena size and the small block allocator.
    # This has shown to improve performance (see www.hp.docs.com for details).
    #--------------------------------------------------------------------------
    setenv _M_SBA_OPTS 1024:128:16
    setenv _M_ARENA_OPTS 8:128
    breaksw

  # ---------------------------------------------------------------------------
  # (AIX) Set LIBPATH to include our shared libraries.
  # Also seem to need /usr/lib if a dir in $LIBPATH not used.  SMW 08/25/94
  # ---------------------------------------------------------------------------
  case rs6000_aix:
    if ($?LIBPATH) then
      setenv LIBPATH $LIBPATH\:$RTHOME/lib/$RTARCH
    else
      # LIBPATH not yet set.
      setenv LIBPATH $RTHOME/lib/$RTARCH\:/usr/lib
    endif
    # On AIX there is no way to nicely set a 64-bit LIBPATH
    breaksw
  case tandem_oss:
    if ($?_RLD_LIB_PATH) then
      setenv _RLD_LIB_PATH $_RLD_LIB_PATH\:$RTHOME/lib/$RTARCH
    else
      setenv _RLD_LIB_PATH $RTHOME/lib/$RTARCH\:/usr/lib
    endif
    breaksw
endsw

if ($?PT_KIT) then
  setenv RT_FATAL_ERROR_ABORT 1
endif

# -----------------------------------------------------------------------------
# On OS/390 or zOS running under Unix System Services we need to
# set the following for the calls to the compilers so that rtlink
# will work as expected.
# -----------------------------------------------------------------------------
if ($RTARCH == os390_mvsoe) then
  setenv _C89_CCMODE 1
  setenv _CXX_CCMODE 1
  setenv _CXX_CXXSUFFIX cxx
endif

# -----------------------------------------------------------------------------
# We need to set UID path for rtmongdi
# -----------------------------------------------------------------------------
set _new_uidpath = ${RTHOME}/lib/${RTARCH}/%U
if ($?UIDPATH) then
  setenv UIDPATH ${UIDPATH}:${_new_uidpath}
else
  setenv UIDPATH ${_new_uidpath}
  # On DEC UNIX 3.2 and 4.0, we need to add /usr/lib/X11/uid to get
  # the old dxterm program to find its .uid file (bug 5995).  We tried
  # to look through the Motif 1.2.4 source to find a nice, generic solution,
  # but the DEC libXm.so is hard to decipher, so just put in the
  # minimal directory change (determined empirically).  SMW BM 10/09/97
  if ($RTARCH == alpha_osf) then
    setenv UIDPATH ${UIDPATH}:/usr/lib/X11/uid/%N%S
  endif
endif
unset _new_uidpath

