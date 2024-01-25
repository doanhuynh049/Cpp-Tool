/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/modules/db.h#2 $
 */

/* =================================================================== */
/*                                                                     */
/*         db.h -- opaque data structures for Tdb routines             */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_DB_H_
#define _RTWORKS_DB_H_

#ifndef T_SDB_API
#ifndef T_OS_WIN32
#define T_SDB_API
#else
#ifndef T_STATIC_LINK
#ifndef T_SDB_SOURCE
#define T_SDB_API __declspec(dllimport)
#else
#define T_SDB_API __declspec(dllexport)
#endif
#else
#define T_SDB_API
#endif /* T_STATIC_LINK */
#endif /* T_OS_WIN32 */
#endif

typedef struct T_DB_DRIVER_STRUCT T_DB_DRIVER_STRUCT, *T_DB_DRIVER;
typedef struct T_DB_DATABASE_STRUCT T_DB_DATABASE_STRUCT, *T_DB_DATABASE;
typedef struct T_DB_COLUMN_STRUCT T_DB_COLUMN_STRUCT, *T_DB_COLUMN;
typedef struct T_DB_TABLE_STRUCT T_DB_TABLE_STRUCT, *T_DB_TABLE;
typedef struct T_DB_RECORD_STRUCT T_DB_RECORD_STRUCT, *T_DB_RECORD;
typedef struct T_DB_SELECTION_STRUCT T_DB_SELECTION_STRUCT, *T_DB_SELECTION;
typedef struct T_SCHEMA_INDEX_STRUCT T_SCHEMA_INDEX_STRUCT, *T_DB_SCHEMA_INDEX;
typedef struct T_SCHEMA_TABLE_STRUCT T_SCHEMA_TABLE_STRUCT, *T_DB_SCHEMA_TABLE;

typedef enum {
  T_DB_LOCK_UNUSED,
  T_DB_LOCK_SHARED,
  T_DB_LOCK_EXCLUSIVE
} T_DB_LOCK;

typedef enum {
  T_DB_SELECT_FIRST,
  T_DB_SELECT_NEXT,
  T_DB_SELECT_PREVIOUS,
  T_DB_SELECT_LAST
} T_DB_SELECT_OP;

typedef enum {
  T_DB_INDEX_UNIQUE,
  T_DB_INDEX_DUPLS,
  T_DB_INDEX_MODE_UNKNOWN
} T_DB_INDEX_MODE;

typedef enum {
  T_DB_ORDER_ASCEND,
  T_DB_ORDER_DESCEND
} T_DB_ORDER_MODE;

#define T_DB_LOCK_STD_TIMEOUT		((double) 10)

/* --------------------------------------------------------------------- */
/* Defines the smallest value whereby if the absolute value              */
/* of the difference between two EPSILON values is less than this value, */
/* then they are considered equal.                                       */
/* --------------------------------------------------------------------- */

/* ------------------------------------------------------------------- */
/* The IRIX compiler doesn't like MINDOUBLE to be put into a double    */
/* constant.   It looks like a bug to me, but Scott and I decided just */
/* to punt on this whole issue and compare the doubles directly.       */
/* DCG.  9/8/93                                                        */
/* ------------------------------------------------------------------- */
#if 0
#ifdef T_OS_VMS
#define T_DB_EPSILON_SMALL		DBL_MIN
#else
#define T_DB_EPSILON_SMALL		MINDOUBLE
#endif
#endif

/*------------------------------------------*/
/* Define the columns in the variable table */
/*------------------------------------------*/
#define T_DB_COLNAME_VARIABLE		"variable"
#define T_DB_COLNAME_DESC		"description"
#define T_DB_COLNAME_TYPE		"type"
#define T_DB_COLNAME_SUBJECT		"subjects"
#define T_DB_COLNAME_EPSILON		"epsilon"
#define T_DB_COLNAME_SLOT		"slot"
#define T_DB_COLNAME_SENDALWAYS		"send_always"
#define T_DB_COLNAME_INITIAL		"initial_value"
#define T_DB_COLNAME_FORMULA		"formula"
#define T_DB_COLNAME_SCOPE		"scope"
#define T_DB_COLNAME_SIM_INITIAL	"sim_initial_value"
#define T_DB_COLNAME_SIM_FORMULA	"sim_formula"

/*-------------------------------------------*/
/* Maintain compatibility with RTworks 3.5   */
/*-------------------------------------------*/
#ifdef T_RTWORKS35_COMPAT
#define T_DB_COLNAME_DG 		"datagroups"
#endif

#define T_DB_MAX_DRIVER_NAME		255
#define T_DB_MAX_USER_NAME		255
#define T_DB_MAX_DATABASE_NAME	       	255
#define T_DB_MAX_TABLE_NAME		255
#define T_DB_MAX_COLUMN_NAME		255
#define T_DB_MAX_INDEX_NAME		255
#define T_DB_MAX_HEADING		255
#define T_DB_MAX_ALIAS			255
#define T_DB_NO_TRUNC			((T_INT4) -987)

/*--------------------------------*/
/* Typedefs for driver functions  */
/*--------------------------------*/
#define T_DRV_FAILURE 			((T_PTR) -1)

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_COLUMN_BIND) T_PROTO((T_PTR, T_STR, T_TYPE, T_INT4, T_BOOL, T_BOOL, T_BOOL, T_PTR, T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_COLUMN_GET_VALUE) T_PROTO((T_PTR, T_STR, T_TYPE, T_INT4, T_BOOL, T_BOOL, T_BOOL, T_PTR, T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_COLUMN_RELEASE) T_PROTO((T_STR, T_TYPE, T_INT4, T_BOOL, T_BOOL, T_BOOL, T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_COLUMN_SET_VALUE) T_PROTO((T_PTR, T_STR, T_TYPE, T_INT4, T_BOOL, T_BOOL, T_BOOL, T_PTR, T_PTR));
typedef T_STR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_DATABASE_LOOKUP) T_PROTO((T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_DATABASE_SET_CURRENT) T_PROTO((T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_DRIVER_CLOSE) T_PROTO((void));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_DRIVER_OPEN) T_PROTO((T_STR, T_BOOL));
typedef T_STR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_GET_USER_NAME) T_PROTO((T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_INDEX_ADD_COLUMN) T_PROTO((T_STR, T_PTR, T_STR, T_PTR, T_STR, T_PTR, T_DB_ORDER_MODE));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_INDEX_BUILD) T_PROTO((T_STR, T_PTR, T_STR, T_PTR, T_DB_INDEX_MODE));
typedef T_PTR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_INDEX_CREATE_SCHEMA) T_PROTO((T_STR, T_PTR, T_STR, T_DB_INDEX_MODE));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_INDEX_DROP) T_PROTO((T_STR, T_PTR, T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_INDEX_EXISTS) T_PROTO((T_STR, T_PTR, T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_INDEX_RELEASE) T_PROTO((T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_OPTION_PARSE_VARIABLE_TABLE) T_PROTO((T_STR, T_STR*, T_STR*));
typedef T_PTR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_RECORD_CREATE) T_PROTO((T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_RECORD_DELETE) T_PROTO((T_PTR));
typedef T_PTR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_RECORD_FETCH) T_PROTO((T_PTR, T_DB_SELECT_OP));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_RECORD_INSERT) T_PROTO((T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_RECORD_RELEASE) T_PROTO((T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_RECORD_UPDATE) T_PROTO((T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_SELECTION_ADD_COLUMN) T_PROTO((T_PTR, T_STR, T_TYPE, T_INT4, T_PTR, T_STR, T_PTR, T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_SELECTION_ADD_TABLE) T_PROTO((T_PTR, T_STR, T_PTR, T_STR));
typedef T_PTR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_SELECTION_CREATE) T_PROTO((T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_SELECTION_EXECUTE) T_PROTO((T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_SELECTION_RELEASE) T_PROTO((T_PTR));
typedef T_PTR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_ADD_COLUMN) T_PROTO((T_PTR, T_STR table_name, T_STR column_name, T_TYPE column_type, T_INT4 size, T_BOOL nulls, T_BOOL unique, T_BOOL primary_key));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_BREAK_LOCK) T_PROTO((T_STR, T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_BUILD) T_PROTO((T_PTR, T_STR, T_PTR_ARY));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_CLOSE) T_PROTO((T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_COLUMN_RELEASE) T_PROTO((T_PTR));
typedef T_PTR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_CREATE_SCHEMA) T_PROTO((T_STR table_name, T_INT4 nrecords));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_DROP) T_PROTO((T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_EXISTS) T_PROTO((T_STR table_name));
typedef T_STR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_GET_LOCKER) T_PROTO((T_PTR, T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_GET_NUM_RECORDS) T_PROTO((T_PTR, T_INT4 *));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_LOCK) T_PROTO((T_PTR, T_DB_LOCK, T_STR));
typedef T_PTR  (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_OPEN) T_PROTO((T_STR, T_STR, T_STR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_OPERATION) T_PROTO((T_PTR, T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_RELEASE) T_PROTO((T_PTR));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_DB_FUNC_TABLE_UNLOCK) T_PROTO((T_PTR, T_STR));

typedef T_PTR  (T_ENTRY1 * T_ENTRY2 T_DB_COLUMN_TRAV_FUNC)
     T_PROTO((T_PTR         /*column_name*/,
              T_DB_COLUMN   /*col*/,
              T_PTR         /*arg*/));

/*  For drivers only  */
typedef void (T_ENTRY1 * T_ENTRY2 T_DRV_FUNC_PRIMARY_KEY_TRAVERSE)
     T_PROTO((T_STR,
              T_TYPE,
              T_INT4,
              T_PTR,
              T_PTR,
              T_PTR));

/*----------------------------------*/
/* Prototypes for public functions  */
/*----------------------------------*/
#if !defined(T_GENERATE_PROTOTYPES)

#ifdef __cplusplus
extern "C" {
#endif

/* apipublic.c */
T_SDB_API T_DB_DRIVER T_ENTRY T_EXPORT TdbDriverLookup T_PROTO((
	T_STR /*name*/
	));
T_SDB_API T_STR T_ENTRY T_EXPORT TdbDriverGetName T_PROTO((
	T_DB_DRIVER /*driver*/,
	T_STR /*location*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDriverOpen T_PROTO((
	T_DB_DRIVER /*driver*/,
	T_PTR /*arg*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDriverClose T_PROTO((
	T_DB_DRIVER /*driver*/
	));
T_SDB_API T_STR T_ENTRY T_EXPORT TdbGetUserName T_PROTO((
	T_STR /*username*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableBreakLock T_PROTO((
	T_STR /*tablename*/,
	T_STR /*username*/
	));
T_SDB_API T_DB_DATABASE T_ENTRY T_EXPORT TdbDatabaseLookup T_PROTO((
	T_DB_DRIVER /*d*/,
	T_STR /*name*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDatabaseSetCurrent T_PROTO((
	T_DB_DATABASE /*db*/
	));
T_SDB_API T_DB_DATABASE T_ENTRY T_EXPORT TdbDatabaseGetCurrent T_PROTO((void));
T_SDB_API T_STR T_ENTRY T_EXPORT TdbDatabaseGetName T_PROTO((
	T_DB_DATABASE /*db*/,
	T_STR /*location*/
	));
T_SDB_API T_DB_DRIVER T_ENTRY T_EXPORT TdbDatabaseGetDriver T_PROTO((
	T_DB_DATABASE /*db*/
	));
T_SDB_API T_DB_TABLE T_ENTRY T_EXPORT TdbTableOpen T_PROTO((
	T_STR /*name*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableAddColumn T_PROTO((
	T_DB_SCHEMA_TABLE /*table_schema*/,
	T_STR /*column_name*/,
	T_TYPE /*column_type*/,
	T_INT4 /*size*/,
	T_BOOL /*nulls*/,
	T_BOOL /*unique*/,
	T_BOOL /*primary_key*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableBuild T_PROTO((
	T_DB_SCHEMA_TABLE /*table_schema*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableClose T_PROTO((
	T_DB_TABLE /*table*/
	));
T_SDB_API T_DB_SCHEMA_TABLE T_ENTRY T_EXPORT TdbTableCreateSchema T_PROTO((
	T_STR /*table_name*/,
	T_INT4 /*nrecords*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableDrop T_PROTO((
	T_DB_TABLE /*table*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableExists T_PROTO((
	T_STR /*table_name*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableGetNumRecords T_PROTO((
	T_DB_TABLE /*table*/,
	T_INT4 * /*size*/
	));
T_SDB_API T_STR T_ENTRY T_EXPORT TdbTableGetName T_PROTO((
	T_DB_TABLE /*table*/,
	T_STR /*location*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableOperate T_PROTO((
	T_DB_TABLE /*table*/,
	T_PTR /*arg*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableReleaseSchema T_PROTO((
	T_DB_SCHEMA_TABLE /*table_schema*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableLock T_PROTO((
	T_DB_TABLE /*table*/,
	T_DB_LOCK /*mode*/,
	T_REAL8 /*timeout*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbTableUnlock T_PROTO((
	T_DB_TABLE /*table*/
	));
T_SDB_API T_STR T_ENTRY T_EXPORT TdbTableGetLocker T_PROTO((
	T_DB_TABLE /*table*/,
	T_STR /*username*/
	));
T_SDB_API T_DB_SELECTION T_ENTRY T_EXPORT TdbSelectionCreate T_PROTO((
	T_STR /*clause*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbSelectionAddTable T_PROTO((
	T_DB_SELECTION /*sel*/,
	T_DB_TABLE /*table*/,
	T_STR /*alias*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbSelectionAddColumn T_PROTO((
	T_DB_SELECTION /*sel*/,
	T_DB_COLUMN /*column*/,
	T_STR /*heading*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbSelectionExecute T_PROTO((
	T_DB_SELECTION /*sel*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbSelectionRelease T_PROTO((
	T_DB_SELECTION /*sel*/
	));
T_SDB_API T_DB_RECORD T_ENTRY T_EXPORT TdbRecordFetch T_PROTO((
	T_DB_SELECTION /*sel*/,
	T_DB_SELECT_OP /*operation*/
	));
T_SDB_API T_DB_RECORD T_ENTRY T_EXPORT TdbRecordCreate T_PROTO((
	T_DB_TABLE /*table*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbRecordInsert T_PROTO((
	T_DB_RECORD /*record*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbRecordUpdate T_PROTO((
	T_DB_RECORD /*record*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbRecordRelease T_PROTO((
	T_DB_RECORD /*record*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbRecordDelete T_PROTO((
	T_DB_RECORD /*record*/
	));
T_SDB_API T_DB_COLUMN T_ENTRY T_EXPORT TdbColumnLookup T_PROTO((
	T_DB_TABLE /*table*/,
	T_STR /*name*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbColumnBind T_PROTO((
	T_DB_SELECTION /*sel*/,
	T_DB_COLUMN /*column*/,
	T_PTR /*variable*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbColumnGetValue T_PROTO((
	T_DB_RECORD /*record*/,
	T_DB_COLUMN /*column*/,
	T_PTR /*variable*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbColumnSetValue T_PROTO((
	T_DB_RECORD /*record*/,
	T_DB_COLUMN /*column*/,
	T_PTR /*variable*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbColumnGetSize T_PROTO((
	T_DB_COLUMN /*column*/,
	T_INT4 * /*size*/
	));
T_SDB_API T_STR T_ENTRY T_EXPORT TdbColumnGetName T_PROTO((
	T_DB_COLUMN /*column*/,
	T_STR /*location*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbColumnGetType T_PROTO((
	T_DB_COLUMN /*column*/,
	T_TYPE * /*type*/
	));
T_SDB_API T_PTR T_ENTRY T_EXPORT TdbColumnTraverse T_PROTO((
	T_DB_TABLE /*table*/,
	T_DB_COLUMN_TRAV_FUNC /*func*/,
	T_PTR /*arg*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbIndexExists T_PROTO((
	T_DB_TABLE /*table*/,
	T_STR /*idxname*/
	));
T_SDB_API T_DB_SCHEMA_INDEX T_ENTRY T_EXPORT TdbIndexCreateSchema T_PROTO((
	T_DB_TABLE /*table*/,
	T_STR /*idxname*/,
	T_DB_INDEX_MODE /*unique*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbIndexAddColumn T_PROTO((
	T_DB_SCHEMA_INDEX /*index*/,
	T_DB_COLUMN /*column*/,
	T_DB_ORDER_MODE /*order*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbIndexBuild T_PROTO((
	T_DB_SCHEMA_INDEX /*index*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbIndexDrop T_PROTO((
	T_DB_TABLE /*table*/,
	T_STR /*idxname*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbIndexReleaseSchema T_PROTO((
	T_DB_SCHEMA_INDEX /*index*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbSetTableLocking T_PROTO((
	T_BOOL /*lockflag*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDriverCreateStandard T_PROTO((void));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDriverCloseAll T_PROTO((void));

/*  For internal drivers  */
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDriverCreateOracle T_PROTO((void));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDriverCreateSybase T_PROTO((void));

/*  For external drivers  */
T_SDB_API T_STR T_ENTRY T_EXPORT TdbDrvSaveUserName T_PROTO((
	T_STR /*username*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDrvTraversePrimaryKey T_PROTO((
	T_DRV_FUNC_PRIMARY_KEY_TRAVERSE /*traverse_func*/,
	T_PTR /*arg*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDrvSaveColumn T_PROTO((
	T_STR /*column_name*/,
	T_TYPE /*type*/,
	T_INT4 /*size*/,
	T_BOOL /*nulls*/,
	T_BOOL /*unique*/,
	T_BOOL /*primkey*/,
	T_PTR /*driverdata*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDrvPrimaryKeys T_PROTO((
	T_BOOL /*primkey_required*/,
	T_BOOL /*primkey_implied*/,
	T_BOOL /*primkey_index*/,
	T_BOOL /*primkey_nulls_allowed*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDrvTruncSizes T_PROTO((
	T_INT4 /*database_size*/,
	T_INT4 /*table_size*/,
	T_INT4 /*column_size*/,
	T_INT4 /*index_size*/,
	T_INT4 /*username_size*/,
	T_INT4 /*lockid_size*/,
	T_INT4 /*heading_size*/,
	T_INT4 /*alias_size*/
	));
T_SDB_API T_BOOL T_ENTRY T_EXPORT TdbDrvLwrCaseFold T_PROTO((
	T_BOOL /*database_lc_flag*/,
	T_BOOL /*table_lc_flag*/,
	T_BOOL /*column_lc_flag*/,
	T_BOOL /*index_lc_flag*/,
	T_BOOL /*username_lc_flag*/,
	T_BOOL /*lockid_lc_flag*/,
	T_BOOL /*heading_lc_flag*/,
	T_BOOL /*alias_lc_flag*/
	));
T_SDB_API T_DB_DRIVER T_ENTRY T_EXPORT TdbDrvCreate T_PROTO((
    T_STR /*name*/,
    T_DB_FUNC_COLUMN_BIND /*column_bind_func*/,
    T_DB_FUNC_COLUMN_GET_VALUE /*column_get_value_func*/,
    T_DB_FUNC_COLUMN_RELEASE /*column_release_func*/,
    T_DB_FUNC_COLUMN_SET_VALUE /*column_set_value_func*/,
    T_DB_FUNC_DATABASE_LOOKUP /*database_lookup_func*/,
    T_DB_FUNC_DATABASE_SET_CURRENT /*database_set_current_func*/,
    T_DB_FUNC_DRIVER_CLOSE /*driver_close_func*/,
    T_DB_FUNC_DRIVER_OPEN /*driver_open_func*/,
    T_DB_FUNC_INDEX_ADD_COLUMN /*index_add_column_func*/,
    T_DB_FUNC_INDEX_BUILD /*index_build_func*/,
    T_DB_FUNC_INDEX_CREATE_SCHEMA /*index_create_schema_func*/,
    T_DB_FUNC_INDEX_DROP /*index_drop_func*/,
    T_DB_FUNC_INDEX_EXISTS /*index_exists_func*/,
    T_DB_FUNC_INDEX_RELEASE /*index_release_func*/,
    T_DB_FUNC_OPTION_PARSE_VARIABLE_TABLE /*option_parse_variable_table_func*/,
    T_DB_FUNC_RECORD_CREATE /*record_create_func*/,
    T_DB_FUNC_RECORD_DELETE /*record_delete_func*/,
    T_DB_FUNC_RECORD_FETCH /*record_fetch_func*/,
    T_DB_FUNC_RECORD_INSERT /*record_insert_func*/,
    T_DB_FUNC_RECORD_RELEASE /*record_release_func*/,
    T_DB_FUNC_RECORD_UPDATE /*record_update_func*/,
    T_DB_FUNC_SELECTION_ADD_COLUMN /*selection_add_column_func*/,
    T_DB_FUNC_SELECTION_ADD_TABLE /*selection_add_table_func*/,
    T_DB_FUNC_SELECTION_CREATE /*selection_create_func*/,
    T_DB_FUNC_SELECTION_EXECUTE /*selection_execute_func*/,
    T_DB_FUNC_SELECTION_RELEASE /*selection_release_func*/,
    T_DB_FUNC_TABLE_ADD_COLUMN /*table_add_column_func*/,
    T_DB_FUNC_TABLE_BREAK_LOCK /*table_break_lock_func*/,
    T_DB_FUNC_TABLE_BUILD /*table_build_func*/,
    T_DB_FUNC_TABLE_CLOSE /*table_close_func*/,
    T_DB_FUNC_TABLE_COLUMN_RELEASE /*table_column_release_func*/,
    T_DB_FUNC_TABLE_CREATE_SCHEMA /*table_create_schema_func*/,
    T_DB_FUNC_TABLE_DROP /*table_drop_func*/,
    T_DB_FUNC_TABLE_EXISTS /*table_exists_func*/,
    T_DB_FUNC_TABLE_GET_LOCKER /*table_get_locker_func*/,
    T_DB_FUNC_TABLE_GET_NUM_RECORDS /*get_num_records_func*/,
    T_DB_FUNC_TABLE_LOCK /*table_lock_func*/,
    T_DB_FUNC_TABLE_OPEN /*table_open_func*/,
    T_DB_FUNC_TABLE_OPERATION /*table_operation_func*/,
    T_DB_FUNC_TABLE_RELEASE /*table_release_func*/,
    T_DB_FUNC_TABLE_UNLOCK /*table_unlock_func*/
    ));

#ifdef __cplusplus
}
#endif

#endif /* !defined(T_GENERATE_PROTOTYPES) */

#endif /* _RTWORKS_DB_H_ */

/*==========================================================================*/
