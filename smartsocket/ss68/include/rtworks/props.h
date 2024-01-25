/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/props.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*            props.h -- private interface for properties              */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_PROPS_H_
#define _RTWORKS_PROPS_H_

typedef struct T_NAME_VALUE_STRUCT T_NAME_VALUE_STRUCT, *T_NAME_VALUE;
struct T_NAME_VALUE_STRUCT {
  T_STR name;
  T_STR value;
};

typedef T_HASH_TABLE T_PROPERTIES;

/*
 * T_PROPERTY_TRAV_FUNC is for traversing property tables.
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_PROPERTY_TRAV_FUNC) T_TYPEDEF((
T_PTR,  /* key */
T_PTR,  /* value (T_NAME_VALUE) */
T_PTR   /* arg */
));

#endif /* _RTWORKS_PROPS_H_ */

