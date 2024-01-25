/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/burst/includes.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                  includes.h -- included in all modules              */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_BURST_INCLUDES_H_
#define _RTWORKS_BURST_INCLUDES_H_

#define T_BURST_SOURCE
#include <rtworks/common.h>  /* required for VxWorks */
#include "burstp.h"
#include "burst.h"

#if defined(T_OS_WIN16_PT)  || defined(T_OS_NETWARE)
#include <rtworks/getstat.h>
#endif


#endif /* _RTWORKS_BURST_INCLUDES_H_ */
