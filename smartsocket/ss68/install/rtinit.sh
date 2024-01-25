:

#
# Copyright (c) 1991-2006 TIBCO Software Inc.
# All rights reserved.
# For more information, please contact:
# TIBCO Software Inc., Palo Alto, California, USA
#
# $Id: //dev/packages/build/dev/68/src/scripts/rtinit.sh#2 $
#

# Fix for bug 3932 -Korn shell fails with unknown env vars.
USET=0
echo $- | grep u > /dev/null 2>&1
if [ $? -ne 1 ]; then
  USET=1
  set +u
fi

#set -x
if [ $RTVERBOSE ]; then
  set -x
fi

# This script is the bare-bones setup needed to run TIBCO SmartSockets.

# Must do this first!
if [ $FORCE_RTHOME ]; then
  RTHOME=$FORCE_RTHOME
else
  RTHOME=/home/rtworks
fi
export RTHOME

# Now set the architecture.
if [ -x $RTHOME/bin/rtarch ]; then
  RTARCH=`$RTHOME/bin/rtarch`
  export RTARCH
  RTARCH_UC=`echo $RTARCH | tr '[a-z]' '[A-Z]'`
  export RTARCH_UC
  # Set the sub arch.  SMW 10/02/94
else
  echo SmartSockets is not installed correctly in $RTHOME.
  echo The shell script $RTHOME/bin/rtarch is missing.
fi

# This bypasses VI's DVinit script.
DVHOME=$RTHOME/graphics/$RTARCH
export DVHOME

# Add SmartSockets to the path.
PATH=$PATH:$RTHOME/bin:$RTHOME/bin/$RTARCH
export PATH

# -----------------------------------------------------------------------------
# Add RTworks to the X app-defaults path.  We use XUSERFILESEARCHPATH instead
# of XAPPLRESDIR because XUSERFILESEARCHPATH is more generic and flexible.
# If XAPPLRESDIR is set, then we tack it on to the end of XUSERFILESEARCHPATH,
# because X willl ignore XAPPLRESIDR if XUSERFILESEARCHPATH is set (see p.
# 666-667 of "X Window System Toolkit" by Asente and Swick).
# -----------------------------------------------------------------------------
_app=$RTHOME/app-defaults
if [ $XUSERFILESEARCHPATH ]; then
  XUSERFILESEARCHPATH=$XUSERFILESEARCHPATH:$_app/$RTARCH/%N:$_app/%N
  export XUSERFILESEARCHPATH
else
  XUSERFILESEARCHPATH=$_app/$RTARCH/%N:$_app/%N
  export XUSERFILESEARCHPATH
fi

if [ $XAPPLRESDIR ]; then
  XUSERFILESEARCHPATH=$XUSERFILESEARCHPATH:$XAPPLRESDIR/%N
  #echo The environment variable XAPPLRESDIR is obsolete.
  #echo Please use XUSERFILESEARCHPATH instead.
fi

case $RTARCH in
  # ---------------------------------------------------------------------------
  # Set LD_LIBRARY_PATH to include our shared libraries.
  # ---------------------------------------------------------------------------
  iris_irix|alpha_osf|ia64_linux|i86_linux|i86_solaris|sun4_solaris)
    if [ "$LD_LIBRARY_PATH" ]; then
      LD_LIBRARY_PATH=$RTHOME/lib/$RTARCH:$LD_LIBRARY_PATH
    else
      # LD_LIBRARY_PATH not yet set.
      LD_LIBRARY_PATH=$RTHOME/lib/$RTARCH
    fi
    case $RTARCH in
      i86_linux)
        LD_LIBRARY_PATH=$RTHOME/lib/$RTARCH/lib64:$LD_LIBRARY_PATH;;
      i86_solaris)
        LD_LIBRARY_PATH=$RTHOME/lib/$RTARCH/amd64:$LD_LIBRARY_PATH;;
      sun4_solaris)
        LD_LIBRARY_PATH=$RTHOME/lib/$RTARCH/sparcv9:$LD_LIBRARY_PATH;;
    esac
    export LD_LIBRARY_PATH
    ;;

  # ---------------------------------------------------------------------------
  # (HP-UX) Set SHLIB_PATH to include our shared libraries.
  # ---------------------------------------------------------------------------
  hp700_hpux|ia64_hpux)
    if [ $SHLIB_PATH ]; then
      SHLIB_PATH=$SHLIB_PATH:$RTHOME/lib/$RTARCH
    else
      # SHLIB_PATH not yet set.
      SHLIB_PATH=$RTHOME/lib/$RTARCH
    fi
    case $RTARCH in
      hp700_hpux)
        SHLIB_PATH=$SHLIB_PATH:$RTHOME/lib/$RTARCH/pa20_64;;
      ia64_hpux)
        SHLIB_PATH=$SHLIB_PATH:$RTHOME/lib/$RTARCH/hpux64;;
    esac
    export SHLIB_PATH
    #--------------------------------------------------------------------------
    # Set these to vary the memory arena size and the small block allocator.
    # This has shown to improve performance (see www.hp.docs.com for details).
    #--------------------------------------------------------------------------
    _M_SBA_OPTS=1024:128:16
    _M_ARENA_OPTS=8:128
    export _M_SBA_OPTS
    export _M_ARENA_OPTS
    ;;

  # ---------------------------------------------------------------------------
  # (AIX) Set LIBPATH to include our shared libraries.
  # Also seem to need /usr/lib if a dir in $LIBPATH not used.  SMW 08/25/94
  # ---------------------------------------------------------------------------
  rs6000_aix)
    if [ $LIBPATH ]; then
      LIBPATH=$LIBPATH:$RTHOME/lib/$RTARCH
    else
      # LIBPATH not yet set.
      LIBPATH=$RTHOME/lib/$RTARCH:/usr/lib
    fi
    # On AIX there is no way to nicely set a 64-bit LIBPATH
    export LIBPATH
    ;;
  tandem_oss)
    if [ $_RLD_LIB_PATH ]; then
      _RLD_LIB_PATH=$_RLD_LIB_PATH:$RTHOME/lib/$RTARCH
    else
      _RLD_LIB_PATH=$RTHOME/lib/$RTARCH:/usr/lib
    fi
    export _RLD_LIB_PATH
    ;;
esac

if $PT_KIT
then
  RT_FATAL_ERROR_ABORT=1
  export RT_FATAL_ERROR_ABORT
fi

# -----------------------------------------------------------------------------
# On OS/390 or zOS running under Unix System Services we need to
# set the following for the calls to the compilers so that rtlink
# will work as expected.
# -----------------------------------------------------------------------------
if [ $RTARCH = os390_mvsoe ]; then
  export _C89_CCMODE=1
  export _CXX_CCMODE=1
  export _CXX_CXXSUFFIX=cxx
fi

# -----------------------------------------------------------------------------
# We need to set UID path for rtmongdi
# -----------------------------------------------------------------------------
_new_uidpath=${RTHOME}/lib/${RTARCH}/%U
if [ $UIDPATH ] ; then
  UIDPATH=${UIDPATH}:${_new_uidpath}
else
  UIDPATH=${_new_uidpath}
  # On DEC UNIX 3.2 and 4.0, we need to add /usr/lib/X11/uid to get
  # the old dxterm program to find its .uid file (bug 5995).  We tried
  # to look through the Motif 1.2.4 source to find a nice, generic solution,
  # but the DEC libXm.so is hard to decipher, so just put in the
  # minimal directory change (determined empirically).  SMW BM 10/09/97
  if [ $RTARCH = alpha_osf ]; then
    UIDPATH=${UIDPATH}:/usr/lib/X11/uid/%N%S
  fi
fi
export UIDPATH
unset _new_uidpath

if [ $USET -eq 1 ]; then
  set -u
fi
