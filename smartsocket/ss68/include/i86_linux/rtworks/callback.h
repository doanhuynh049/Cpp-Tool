/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/callback.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                    callback.h  -- public callback header            */
/*                                                                     */
/* =================================================================== */
#ifndef _RTWORKS_UTIL_CALLBACK_H_
#define _RTWORKS_UTIL_CALLBACK_H_

#include <rtworks/modules/db.h>  /* To get T_DB_SELECTION */

typedef struct T_CB_STRUCT T_CB_STRUCT, *T_CB;
typedef struct T_CB_LIST_STRUCT T_CB_LIST_STRUCT, *T_CB_LIST;
typedef struct T_CB_TYPE_STRUCT T_CB_TYPE_STRUCT, *T_CB_TYPE;

typedef void *T_CB_ARG;       /* callback arg passed to callback func */
/* ----------------------------------------------------------------- */
/* All type specific callback data structures must start with this   */
/* structure.                                                        */
/* ----------------------------------------------------------------- */
typedef struct T_CB_DATA_STRUCT {
  T_CB cb;
} T_CB_DATA_STRUCT, *T_CB_DATA;

/* --------------------------------------------------------------- */
/* Access macros to get the fields of the callback data structure. */
/* --------------------------------------------------------------- */
#define T_GCD_CB(cd)      (((T_CB_DATA)(cd))->cb)

typedef void *T_CB_OBJ;       /* object that callback belongs to */
typedef T_INT2 T_CB_PRIORITY;

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CB_FUNC)
  T_TYPEDEF((T_CB_OBJ /*ptr*/, T_CB_DATA /*data*/, T_CB_ARG /*arg*/));
typedef void *(T_ENTRY1 * T_ENTRY2 T_EXPORT T_CBLIST_TRAV_FUNC)
  T_TYPEDEF((T_CB_LIST /*list*/, T_CB /*cb*/, T_CB_ARG/*arg*/));
typedef void *(T_ENTRY1 * T_ENTRY2 T_EXPORT T_CBTYPE_TRAV_FUNC)
  T_TYPEDEF((T_INT4 /*type_num*/, T_CB_TYPE /*cbt*/, T_CB_ARG/*arg*/));

/* ---------------------------- */
/* Values for global flag below */
/* ---------------------------- */
typedef enum {
  T_CB_GLOBAL_ONLY,   /* only global cb allowed (like frame callbacks) */
  T_CB_NO_GLOBAL,     /* Globals Not allowed */
  T_CB_GLOBAL_OK      /* Globals and specific callbacks allowed */
} T_CB_GLOBAL_FLAG;


/* --------------------------------------------- */
/* Details of the utility level callback clients */
/* --------------------------------------------- */

/* --------------------- */
/* CB Change callbacks. */
/* --------------------- */
typedef struct T_CB_CHANGE_CB_DATA_STRUCT {
  T_CB       cb;
  T_INT4 reason;
} T_CB_CHANGE_CB_DATA_STRUCT, *T_CB_CHANGE_CB_DATA;

#define T_CB_CHANGE_REASON_FUNC     1
#define T_CB_CHANGE_REASON_ARG      2
#define T_CB_CHANGE_REASON_DESTROY  3
#define T_CB_CHANGE_REASON_PRIORITY 4

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CB_CHANGE_CB_FUNC)
  T_TYPEDEF((T_CB /*cb*/, T_CB_CHANGE_CB_DATA /*data*/, T_CB_ARG /*arg*/));

/* ---------------------- */
/* Time Change callbacks. */
/* ---------------------- */
typedef struct T_TIME_CHANGE_CB_DATA_STRUCT {
  T_CB cb;
  T_TIME time;
} T_TIME_CHANGE_CB_DATA_STRUCT, *T_TIME_CHANGE_CB_DATA;

#define T_TIME_CHANGE_CB_NUM (T_LOBE_UTIL_START - 101)

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_TIME_CHANGE_CB_FUNC)
     T_TYPEDEF((T_PTR /*dummy*/,
		T_TIME_CHANGE_CB_DATA /*data*/,
		T_CB_ARG /*arg*/));

/* ----------------------- */
/* Option Change callbacks */
/* ----------------------- */
typedef struct T_OPTION_CHANGE_CB_DATA_STRUCT {
  T_CB cb;
  T_PTR value;
} T_OPTION_CHANGE_CB_DATA_STRUCT, *T_OPTION_CHANGE_CB_DATA;

#define T_OPTION_CHANGE_CB_NUM (T_LOBE_UTIL_START - 102)

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_OPTION_CHANGE_CB_FUNC)
    T_TYPEDEF((T_OPTION /*option*/,
	       T_OPTION_CHANGE_CB_DATA /*cbdata*/,
	       T_CB_ARG /*arg*/));

typedef struct T_VAR_ROW_READ_CB_DATA_STRUCT 
  T_VAR_ROW_READ_CB_DATA_STRUCT, *T_VAR_ROW_READ_CB_DATA;

struct T_VAR_ROW_READ_CB_DATA_STRUCT {
  T_CB cb;
  T_STR var_name;
  T_TYPE var_type;
  /* array of columns values used with TdbColumnBind */
  T_PTR_ARY column_values;  
  /* companion array of the column names used in program_vars */
  T_PTR_ARY column_names; 
};

#define T_VAR_ROW_READ_CB_NUM (T_LOBE_UTIL_START - 103)

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_VAR_ROW_READ_CB_FUNC) 
     T_TYPEDEF ((T_DB_SELECTION /*sel*/,
		 T_VAR_ROW_READ_CB_DATA /*cbdata*/,
		 T_CB_ARG /*arg*/));
	
/* ---------------------- */
/* Lexer token callbacks. */
/* ---------------------- */
typedef struct T_CB_DATA_STRUCT T_LEX_TOKEN_CB_DATA_STRUCT,
  *T_LEX_TOKEN_CB_DATA;

#define T_LEX_TOKEN_CB_NUM (T_LOBE_UTIL_START - 104)

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LEX_TOKEN_CB_FUNC)
    T_TYPEDEF((T_LEX /*lexer*/,
	       T_LEX_TOKEN_CB_DATA /*data*/,
	       T_CB_ARG /*arg*/));

/*
 * Graph change callbacks.
 */
typedef struct T_GRAPH_CHANGE_CB_DATA_STRUCT {
  T_CB cb;
  /* TRUE if the graph had something added, FALSE if something removed */
  T_BOOL add_flag; 
  T_GRAPH_VERTEX vertex; /* exactly one of vertex/edge will be NULL */
  T_GRAPH_EDGE edge;
} T_GRAPH_CHANGE_CB_DATA_STRUCT, *T_GRAPH_CHANGE_CB_DATA;

#define T_GRAPH_CHANGE_CB_NUM (T_LOBE_UTIL_START - 105)

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_GRAPH_CHANGE_CB_FUNC)
     T_TYPEDEF((T_GRAPH /*graph*/,
		T_GRAPH_CHANGE_CB_DATA /*data*/,
		T_CB_ARG /*arg*/));

/*
 * Callback called before process exits.
 */
#define T_QUIT_CB_NUM (T_LOBE_UTIL_START - 106)

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_QUIT_CB_FUNC) 
     T_TYPEDEF((T_PTR /*dummy*/,
		T_CB_DATA /*cbdata */,
		T_CB_ARG /*arg*/));

/*
 * Reason codes for pointer array change callbacks
 */
typedef enum {
  T_PA_ITEM_INSERTED,
  T_PA_ITEM_REMOVED,
  T_PA_ITEM_REPLACED,
  T_PA_ALL_ITEMS_REMOVED,
  T_PA_ITEMS_SWAPPED,
  T_PA_CLEARED,
  T_PA_SORTED
} T_PA_REASON;

/*
 * Callback data argument for pointer array change callbacks
 */
typedef struct T_PTR_ARY_CHANGE_CB_STRUCT T_PTR_ARY_CHANGE_CB_STRUCT,
  *T_PTR_ARY_CHANGE_CB;

struct T_PTR_ARY_CHANGE_CB_STRUCT {
  T_CB cb;
  T_PA_REASON reason;
  T_PTR item;    /* item changed.  NULL if reason = T_PA_CLEARED */
  T_INT4 index;  /* index of item above.   -1 if item is NULL */
  T_PTR item2;   /* item changed.  NULL if reason != T_PA_ITEMS_SWAPPED */
  T_INT4 index2; /* index of item above.   -1 if item is NULL */
};

/*
 * Pointer array change callback function
 */
#define T_PTR_ARY_CHANGE_CB_NUM (T_LOBE_UTIL_START - 107)
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_PTR_ARY_CHANGE_CB_FUNC) T_TYPEDEF
((T_PTR_ARY /* ptr_ary */,
  T_PTR_ARY_CHANGE_CB /* data */,
  T_CB_ARG /* arg */));

/*
 * Ring buffer overwrite callback function
 */
#define T_RING_OVERWRITE_CB_NUM (T_LOBE_UTIL_START - 108)

#endif  /* _RTWORKS_UTIL_CALLBACK_H_ */








