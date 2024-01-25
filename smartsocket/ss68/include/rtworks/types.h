/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/types.h#1 $
 */

#ifndef _T_TYPES_HH_
#define _T_TYPES_HH_

/** @mainpage
 *
 * @section intro Introduction
 *
 * This class provides support for multiple RTserver connections, provides a
 * new paradigm for callbacks that doesn't require callback methods to be
 * static, provides a set of exception classes that are thrown when errors
 * occur, and also provides wrappers on selected Tut*() functions.
 *
 * @section install Installation
 * A compiled version of this library is shipped with the product; because not
 * all compilers have the same name mangling schemes, the source code is
 * provided.
 *
 * @section usage Using This Library
 * In order to use this library, the only file users have to include is
 * sscpp.h.
 *
 * @section Copyrights Copyright Notice
 *
 * Copyright &copy;, 1991-2006 TIBCO Software Inc. All rights reserved.
 * Unpublished -- rights reserved under the copyright laws of
 * the United States. Use of a copyright notice is precautionary
 * only and does not imply publication or disclosure.
 *
 * This documentation contains proprietary and confidential
 * information of TIBCO Software Inc. Use, disclosure, or
 * reproduction is prohibited without the prior express written
 * permission of TIBCO Software Inc.
 *
 * @subsection restrights Restricted Rights Legend
 *
 * Use, duplication, or disclosure by the Government is subject
 * to restrictions set forth in subparagraph (c)(1)(ii) of the
 * rights in Technical Data and Computer Software clause at
 * DFARS 252.227-7013.
 *
 *                     TIBCO Software Inc
 *
 *             3303 Hillview Ave, Palo Alto, CA 94304
 */

#if defined (T_OS_WIN32)
//disable VC++ warning messages about truncating debug information
#pragma warning  (disable : 4786)

//disable VC++ warning messages about bool cast performance warnings
#pragma warning  (disable : 4800)

//disable VC++ warning messages about ignoring exception specifications
#pragma warning( disable : 4290 )

#endif

#include <rtworks/config.h>
#include <rtworks/ipc.h>

namespace SmartSockets
{
  typedef T_INT1    int1;
  typedef T_UINT1   uint1;
  typedef T_INT2    int2;
  typedef T_UINT2   uint2;
  typedef T_INT4    int4;
  typedef T_UINT4   uint4;
  typedef T_INT8    int8;
  typedef T_REAL4   real4;
  typedef T_REAL8   real8;
  typedef T_REAL16  real16;
  typedef T_SOCK    sock;

} //namespace SmartSockets


#endif //_T_TYPES_HH_

