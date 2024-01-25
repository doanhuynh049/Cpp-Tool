/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/utlic.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                 utlic.h -- serial number licensing                  */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_UTLIC_H
#define _RTWORKS_UTLIC_H

/* ------------------------------------------------------- */
/* string constants to match these T_PRODUCT and T_LICENSE */
/* numeric constants provided in LICDEFSP_FILENAME         */
/* ------------------------------------------------------- */
typedef enum {
  T_PRODUCT_INVALID,
  T_PRODUCT_SMARTSOCKETS,
  T_PRODUCT_SMARTMQ,
  T_PRODUCT_GATEWAY,
  T_PRODUCT_WEB,
  T_PRODUCT_SNMP,
  T_PRODUCT_MULTICAST,
  T_PRODUCT_SC
} T_LICENSE_PRODUCTS;

#define T_SERIAL_NUMBER_LENGTH (16)
typedef T_UCHAR T_SERIAL_NUMBER_CELL;
typedef T_SERIAL_NUMBER_CELL T_SERIAL_NUMBER_DATA[ T_SERIAL_NUMBER_LENGTH ];
/* ----------------------------------------------------------------------- */
/* the length of the string representation of the serial number is 3x the  */
/* length of the serial number data bytes: 2x for ASCII hex representation */
/* then also 1x more for the potential hyphens in the human readable       */
/* version; finally, plus one more for terminating null                    */
/* ----------------------------------------------------------------------- */
typedef T_CHAR T_SERIAL_NUMBER_STR[ (3*T_SERIAL_NUMBER_LENGTH)+1 ];
typedef struct T_LICENSE_DATA_STRUCT T_LICENSE_DATA_STRUCT, *T_LICENSE_DATA;

#endif /* _RTWORKS_UTLIC_H */
