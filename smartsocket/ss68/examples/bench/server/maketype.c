/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/bench/server/maketype.c#1 $
 */

/* --------------------------------------------------------------- */
/*  MAKETYPE.C                                                     */
/*  Utility program used to incorporate user-defined message types */
/*  into the standard benchmarks.                                  */
/*                                                                 */
/*  Usage: MAKETYPE <filename>                                     */
/*  filename is an ASCII file specifying the grammar for the       */
/*  message type.                                                  */
/*                                                                 */
/*  For binary and string fields, the lengths must be specified    */
/*  next to the field type in parentheses.                         */
/*  Example:  binary(100) { int2_array(10) }(5)                    */
/* ----------------------------------------------------------------*/

#include <rtworks/ipc.h>

#define T_BM_GEN_FILE "gen.c"

static T_INT4 byte_size     = 0;
static T_STR bmValidTypes[] = {
  "(",
  ")",
  "{",
  "}",
  "char",
  "int2",
  "int4",
  "int8",
  "real4",
  "real8",
  "real16",
  "str",
  "binary",
  "int2_array",
  "int4_array",
  "int8_array",
  "real4_array",
  "real8_array",
  "real16_array"
};

typedef enum {
  T_GEN_BEGIN_COUNT,
  T_GEN_END_COUNT,
  T_GEN_BEGIN_LOOP,
  T_GEN_END_LOOP,
  T_GEN_FT_CHAR,
  T_GEN_FT_INT2,
  T_GEN_FT_INT4,
  T_GEN_FT_INT8,
  T_GEN_FT_REAL4,
  T_GEN_FT_REAL8,
  T_GEN_FT_REAL16,
  T_GEN_FT_STR,
  T_GEN_FT_BINARY,
  T_GEN_FT_INT2_ARRAY,
  T_GEN_FT_INT4_ARRAY,
  T_GEN_FT_INT8_ARRAY,
  T_GEN_FT_REAL4_ARRAY,
  T_GEN_FT_REAL8_ARRAY,
  T_GEN_FT_REAL16_ARRAY,
  T_GEN_FT_INVALID
} T_GEN_FT;

#define T_BM_GEN_FILE_HEADER          0
#define T_BM_GEN_FUNC_HEADER          1
#define T_BM_GEN_CREATE_FUNC_BEGIN    2
#define T_BM_GEN_CREATE_FUNC_END      3
#define T_BM_GEN_BUILD_FUNC_BEGIN     4
#define T_BM_GEN_BUILD_FUNC_MIDDLE    5
#define T_BM_GEN_BUILD_FUNC_END       6
#define T_BM_GEN_CB_FUNC_BEGIN        7
#define T_BM_GEN_CB_FUNC_MIDDLE       8
#define T_BM_GEN_CB_FUNC_END          9
#define T_BM_GEN_SIZE_FUNC            10

static T_STR bmGeneratedFile[] = {
  "\
/*                                                 \n\
 * Copyright (c) 1991-2004 TIBCO Software Inc.     \n\
 * All rights reserved.                            \n\
 * For more information, please contact:           \n\
 * TIBCO Software Inc., Palo Alto, California, USA \n\
 *                                                 \n\
 */                                                \n\n\
#include <rtworks/ipc.h>\n\
#include \"srvbench.h\"\n\n\
static T_INT4 bmMsgCount = -1;\n\n\
",
  "/* ======================================\
=================================== */\n",
  "\
/*..TbmCreateGeneratedMt -- Create the generated message type */\n\
T_BOOL T_ENTRY TbmCreateGeneratedMt\n\
(\n\
void\n\
)\n\
{\n\
  T_IPC_MT mt;\n\
  T_BOOL   status;\n\n\
  mt = TipcMtCreate(\"bm_generated_user_type\",\n\
        T_BM_MT_GENERATED_USER_TYPE,\
",
  "\
);\n\
  T_ASSERT(mt != NULL);\n\
  status = TipcSrvLogAddMt(T_IPC_SRV_LOG_INTERNAL, mt);\n\
  T_EXPECT(status == TRUE);\n\n\
  return T_TRUE;\n\
} /* TbmCreateGeneratedMt */\n\n\
",
  "\
/*..TbmBuildGeneratedMsg -- Build the generated message */\n\
T_BOOL T_ENTRY TbmBuildGeneratedMsg\n\
(\n\
T_IPC_MSG msg,\n\
T_INT4 iterations\n\
)\n\
{\n\
  T_BOOL status;\n\
  T_BOOL outer_counter;\n\
",
  "\
  T_CHECK_NULL(msg, T_FALSE);\n\
  T_CHECK_EXPR(iterations > 0, T_FALSE);\n\n\
  for(outer_counter = 0; outer_counter < iterations; outer_counter++) {\n\
",
  "\
  } /* iteration for */\n\n\
  return T_TRUE;\n\
} /* TbmBuildGeneratedMsg */\n\n\
",
  "\
/*..TbmCbProcessGenerated -- Callback for the user defined message type. */\n\
void T_ENTRY TbmCbProcessGenerated\n\
(\n\
 T_IPC_CONN conn,\n\
 T_IPC_CONN_PROCESS_CB_DATA data,\n\
 T_CB_ARG arg\n\
)\n\
{\n\
  T_INT4 *param = (T_INT4 *)arg;\n\
  T_BOOL status;\n\
  T_INT4 outer_counter;\n\
  T_IPC_MSG msg;\n\
",
  "\
  bmMsgCount++;\n\
  if (0 == bmMsgCount % param[T_BM_PARAM_ACK_RATE]) {\n\
    /* ------------------------------------------------- */\n\
    /* Send an acknowledgment message back to the sender */\n\
    /* ------------------------------------------------- */\n\
    status = TipcSrvMsgWrite(T_BM_SUBJECT_SEND,\n\
           TipcMtLookupByNum(T_BM_MT_ACK),\n\
           FALSE,\n\
           T_IPC_FT_INT4, bmMsgCount,\n\
           NULL);\n\
    T_ASSERT(status == TRUE);\n\
    status = TipcSrvFlush();\n\
    T_ASSERT(status == TRUE);\n\
  } \n\n\
  msg = data->msg;\n\
  status = TipcMsgSetCurrent(msg, 0);\n\
  T_ASSERT(status == TRUE);\n\n\
  if (param[T_BM_PARAM_TRAVERSE]) {\n\n\
    for(outer_counter = 0; \n\
  outer_counter < param[T_BM_PARAM_ITERATIONS]; \n\
  outer_counter++) {\n\
",
  "\
    } /* iteration for */\n\
  } /* if traverse */\n\n\
  return;\n\
} /* TbmCbProcessGenerated */\n\n\
",
  "\
/*..TbmGeneratedMsgDataSize -- returns the size of the msg data in bytes */\n\
T_INT4 T_ENTRY TbmGeneratedMsgDataSize()\n\
{\n\
  /*\n\
   * The size of the messages data is generated\n\
   */\n\
  return %d;\n\
} /* TbmGeneratedMsgDataSize */\n\
"
};

/* ========================================================================= */
/*..bmGetFieldSize - get the size of a field from the grammar */
static T_BOOL bmGetFieldSize
(
T_INT4 *grammar,
T_INT4 field_pos,
T_INT4 g_size,
T_INT4 *f_size
)
{
  T_INT4 counter = field_pos;

  counter++;
  if (g_size <= counter ||
      T_GEN_BEGIN_COUNT != grammar[counter]) {
    TutOut("Parse error: expected '(' following <%s>\n",
           bmValidTypes[grammar[field_pos]]); 
    return T_FALSE;
  }

  counter++;
  if (g_size <= counter) {
    TutOut("Parse error: expected field size for field <%s>\n",
           bmValidTypes[grammar[field_pos]]); 
    return T_FALSE;
  }
  else {
    if (grammar[counter] < 1) {
      TutOut("Parse error: field size <%d> for field <%s> not valid\n", 
             grammar[counter], bmValidTypes[grammar[field_pos]]); 
      return T_FALSE;
    }
    else {
      *f_size = grammar[counter];
    } 
  } 

  counter++;
  if (g_size <= counter ||
      T_GEN_END_COUNT != grammar[counter]) {
    TutOut("Parse error: expected ')' following field size, for field <%s>\n",
           bmValidTypes[grammar[field_pos]]); 
    return T_FALSE;
  }

  return T_TRUE;
}

/* ========================================================================= */
/*..bmGenerateCbFuncDecl - generate some function declarations for the cb. */
static T_BOOL bmGenerateCbFuncDecl
(
FILE *fp,
T_INT4 *grammar,
T_INT4 size
)
{
  T_INT4 counter;
  T_HASH_TABLE declared;
  T_BOOL array = T_FALSE;

  declared = TutHashCreateInt(4);
  T_EXPECT(T_NULL != declared);

  for (counter = 0; counter < size; counter++) {
    if (!array &&
        T_GEN_FT_STR < grammar[counter]) {
      array = T_TRUE;
      fputs("  T_INT4 size;\n", fp);
    }

    if (T_NULL == TutHashLookup(declared, (T_PTR)grammar[counter])) {
      switch (grammar[counter]) {
        case T_GEN_BEGIN_LOOP:
          fputs("  T_INT4 counter;\n", fp);
          break;
        case T_GEN_FT_CHAR:
          fputs("  T_CHAR char_val;\n", fp);
          break;
        case T_GEN_FT_INT2:
          fputs("  T_INT2 int2_val;\n", fp);
          break;
        case T_GEN_FT_INT4:
          fputs("  T_INT4 int4_val;\n", fp);
          break;
        case T_GEN_FT_INT8:
          fputs("  T_INT8 int8_val;\n", fp);
          break;
        case T_GEN_FT_REAL4:
          fputs("  T_REAL4 real4_val;\n", fp);
          break;
        case T_GEN_FT_REAL8:
          fputs("  T_REAL8 real8_val;\n", fp);
          break;
        case T_GEN_FT_REAL16:
          fputs("  T_REAL16 real16_val;\n", fp);
          break;
        case T_GEN_FT_STR:
          fputs("  T_STR str_val;\n", fp);
          break;
        case T_GEN_FT_BINARY:
          fputs("  T_PTR binary_val;\n", fp);
          break;
        case T_GEN_FT_INT2_ARRAY:
          fputs("  T_INT2 *int2_p_val;\n", fp);
          break;
        case T_GEN_FT_INT4_ARRAY:
          fputs("  T_INT4 *int4_p_val;\n", fp);
          break;
        case T_GEN_FT_INT8_ARRAY:
          fputs("  T_INT8 *int8_p_val;\n", fp);
          break;
        case T_GEN_FT_REAL4_ARRAY:
          fputs("  T_REAL4 *real4_p_val;\n", fp);
          break;
        case T_GEN_FT_REAL8_ARRAY:
          fputs("  T_REAL8 *real8_p_val;\n", fp);
          break;
        case T_GEN_FT_REAL16_ARRAY:
          fputs("  T_REAL16 *real16_p_val;\n", fp);
          break;
        default:
          break;
      } /* switch */

      TutHashInsert(declared, (T_PTR)grammar[counter], "true");
    }
    /*
     * if the field has a count, skip past it.
     */
    if (T_GEN_FT_REAL16 < grammar[counter]) {
      counter += 3;
    }
    /* we must end with a loop */
    if (T_GEN_END_LOOP == grammar[counter]) {
      break;
    }
  } 
  fputs("\n", fp);
  TutHashDestroy(declared, T_NULL_FUNC, T_NULL);  

  return T_TRUE;
}

/* ========================================================================= */
/*..bmDeclarationTableDestroy -- destroy each entry in the has table used in */
/*..                             bmGenerateBuildFuncDecl. */
static void T_ENTRY bmDeclarationTableDestroy
(
T_PTR key,
T_PTR ft,
T_PTR arg
)
{
  T_PTR_ARY field_type = (T_PTR_ARY)ft;
  T_BOOL status;

  status = TutPtrAryDestroy(field_type, T_NULL_FUNC, T_NULL);
  T_EXPECT(T_TRUE == status);

  return;
}

/* ========================================================================= */
/*..bmGenerateBuildFuncDecl - generate some function decl for the build. */
static T_BOOL bmGenerateBuildFuncDecl
(
FILE *fp,
T_INT4 *grammar,
T_INT4 size
)
{
  T_INT4 counter;
  T_INT4 f_size;
  T_BOOL status;
  T_HASH_TABLE declared;
  T_PTR_ARY ft;
  T_BOOL array = T_FALSE;
  T_STR var = (T_STR)T_NULL;

  declared = TutHashCreateInt(4);
  T_EXPECT(T_NULL != declared);

  for (counter = 0; counter < size; counter++) {

    if (T_GEN_FT_REAL16 < grammar[counter]) {
      if (T_NULL == (ft = (T_PTR_ARY)TutHashLookup(declared, 
                                                   (T_PTR)grammar[counter]))) {
        ft = TutPtrAryCreate(4);
        T_EXPECT(T_NULL != ft);
        TutHashInsert(declared, (T_PTR)grammar[counter], ft);
      }

      if (!bmGetFieldSize(grammar, counter, size, &f_size)) {
        TutHashDestroy(declared, bmDeclarationTableDestroy, T_NULL);  
        return T_FALSE;
      }

      if (!TutPtrAryItemExists(ft, (T_PTR)f_size)) {
        status = TutPtrAryAppend(ft, (T_PTR)f_size, (T_INT4 *)T_NULL);
        T_EXPECT(T_TRUE == status);

        switch (grammar[counter]) {
          case T_GEN_FT_STR:
            var = "T_CHAR char";
            break;
          case T_GEN_FT_BINARY:
            var = "T_CHAR bin";
            break;
          case T_GEN_FT_INT2_ARRAY:
            var = "T_INT2 int2";
            break;
          case T_GEN_FT_INT4_ARRAY:
            var = "T_INT4 int4";
            break;
          case T_GEN_FT_INT8_ARRAY:
            var = "T_INT8 int8";
            break;
          case T_GEN_FT_REAL4_ARRAY:
            var = "T_REAL4 real4";
            break;
          case T_GEN_FT_REAL8_ARRAY:
            var = "T_REAL8 real8";
            break;
          case T_GEN_FT_REAL16_ARRAY:
            var = "T_REAL16 real16";
            break;
          default:
            var = (T_STR)T_NULL;
            break;
        } /* switch */
        if (T_NULL != var) {
          /*
           * we make the variable static, because some compilers don't
           * allow the aggragate init on automatic variable :-(
           */
          fprintf(fp, "  static %s_%d_val[%d] = { 42 };\n", 
                  var, f_size, f_size);
        }
        if (T_GEN_FT_STR == grammar[counter]) {
          fprintf(fp, "  T_STR str_%d_val = char_%d_val;\n", f_size, f_size);
        }
      }
      /*
       * the field has a count, skip past it.
       */
      counter += 3;
    }
    else {
      if (T_GEN_BEGIN_LOOP == grammar[counter]) {
        fputs("  T_INT4 counter;\n", fp);
      }
      /* we must end with a loop */
      if (T_GEN_END_LOOP == grammar[counter]) {
        break;
      }
    } 
  } 

  fputs("\n", fp);
  TutHashDestroy(declared, bmDeclarationTableDestroy, T_NULL);  

  return T_TRUE;
}

/* ========================================================================= */
/*..TbmGenerateCreateMt - generates the function TbmCreateGeneratedMt */
T_BOOL TbmGenerateCreateMt
(
FILE *fp,
T_INT4 *grammar,
T_INT4 size
)
{
  T_INT4 counter;

  T_CHECK_NULL(fp, T_FALSE);
  T_CHECK_NULL(grammar, T_FALSE);

  fputs(bmGeneratedFile[T_BM_GEN_FUNC_HEADER], fp);
  fputs(bmGeneratedFile[T_BM_GEN_CREATE_FUNC_BEGIN], fp);

  for (counter = 0; counter < size; counter++) {
    if (T_GEN_BEGIN_COUNT == grammar[counter]) {
      counter += 2;
    }
    else {
      fprintf(fp, "\n       \"%s \"", bmValidTypes[grammar[counter]]);
    } 
  } 

  fputs(bmGeneratedFile[T_BM_GEN_CREATE_FUNC_END], fp);

  return T_TRUE;
}

/* ========================================================================= */
/*..TbmGenerateBuildMsg - generates the function TbmBuildGeneratedMsg */
T_BOOL TbmGenerateBuildMsg
(
FILE *fp,
T_INT4 *grammar,
T_INT4 size
)
{
  T_INT4 counter;
  T_INT4 c_size;
  T_INT4 loop_mult = 1;
  T_STR dent = "    ";

  T_CHECK_NULL(fp, T_FALSE);
  T_CHECK_NULL(grammar, T_FALSE);

  fputs(bmGeneratedFile[T_BM_GEN_FUNC_HEADER], fp);
  fputs(bmGeneratedFile[T_BM_GEN_BUILD_FUNC_BEGIN], fp);

  if (!bmGenerateBuildFuncDecl(fp, grammar, size)) {
    return T_FALSE;
  }

  fputs(bmGeneratedFile[T_BM_GEN_BUILD_FUNC_MIDDLE], fp);

  for (counter = 0; counter < size; counter++) {
    if (T_GEN_FT_REAL16 < grammar[counter]
        && !bmGetFieldSize(grammar, counter, size, &c_size)) {
      return T_FALSE;
    }

    switch (grammar[counter]) {
      case T_GEN_BEGIN_LOOP:
        {
          T_INT4 intr;

          for (intr = counter + 1; intr < size; intr++) {
            if (T_GEN_BEGIN_LOOP == grammar[intr]) {
              TutOut("Parse error: found a recursive loop. Not allowed.\n");
              return T_FALSE;
            }
            if (T_GEN_END_LOOP == grammar[intr]) {
              break;
            }
          } 
          if (intr == size) {
            TutOut("Parse error: Did not find end of loop structure.\n");
            return T_FALSE;
          }

          if (!bmGetFieldSize(grammar, intr, size, &c_size)) {
            return T_FALSE;
          }

          fprintf(fp, "\n%sfor(counter = 0; counter < %d; counter++) {\n", 
                  dent, c_size);
          dent = "      ";
          loop_mult = c_size;
        }
        break;
      case T_GEN_END_LOOP:
        loop_mult = 1;
        dent = "    ";
        fprintf(fp, "%s} \n\n", dent);
        break;
      case T_GEN_FT_CHAR:
        byte_size += loop_mult * sizeof(T_CHAR);
        fprintf(fp, "%sstatus = TipcMsgAppendChar(msg, 'X');\n", dent);
        break;
      case T_GEN_FT_INT2:
        byte_size += loop_mult * sizeof(T_INT2);
        fprintf(fp, "%sstatus = TipcMsgAppendInt2(msg, 42);\n", dent);
        break;
      case T_GEN_FT_INT4:
        byte_size += loop_mult * sizeof(T_INT4);
        fprintf(fp, "%sstatus = TipcMsgAppendInt4(msg, 42);\n", dent);
        break;
      case T_GEN_FT_INT8:
        byte_size += loop_mult * sizeof(T_INT8);
        fprintf(fp, "%sstatus = TipcMsgAppendInt8(msg, 42);\n", dent);
        break;
      case T_GEN_FT_REAL4:
        byte_size += loop_mult * sizeof(T_REAL4);
        fprintf(fp, "%sstatus = TipcMsgAppendReal4(msg, 42.0);\n", dent);
        break;
      case T_GEN_FT_REAL8:
        byte_size += loop_mult * sizeof(T_REAL8);
        fprintf(fp, "%sstatus = TipcMsgAppendReal8(msg, 42.0);\n", dent);
        break;
      case T_GEN_FT_REAL16:
        byte_size += loop_mult * sizeof(T_REAL16);
        fprintf(fp, "%sstatus = TipcMsgAppendReal16(msg, 42.0);\n", dent);
        break;
      case T_GEN_FT_STR:
        byte_size += loop_mult * c_size;
        fprintf(fp, "%smemset(str_%d_val, 'X', %d);\n", dent, c_size, c_size);
        fprintf(fp, "%sstr_%d_val[%d] = NULL;\n", dent, c_size, c_size-1);
        fprintf(fp, "%sstatus = TipcMsgAppendStr(msg, str_%d_val);\n", 
                dent, c_size);
        break;
      case T_GEN_FT_BINARY:
        byte_size += loop_mult * c_size;
        fprintf(fp, "%smemset(bin_%d_val, 'X', %d);\n", dent, c_size, c_size);
        fprintf(fp, 
                "%sstatus = TipcMsgAppendBinary(msg, (T_PTR)bin_%d_val, %d);\n", 
                dent, c_size, c_size);
        break;
      case T_GEN_FT_INT2_ARRAY:
        byte_size += loop_mult * c_size * sizeof(T_INT2);
        fprintf(fp, "%sstatus = TipcMsgAppendInt2Array(msg, int2_%d_val, %d);\n",
                dent, c_size, c_size);
        break;
      case T_GEN_FT_INT4_ARRAY:
        byte_size += loop_mult * c_size * sizeof(T_INT4);
        fprintf(fp, "%sstatus = TipcMsgAppendInt4Array(msg, int4_%d_val, %d);\n",
                dent, c_size, c_size);
        break;
      case T_GEN_FT_INT8_ARRAY:
        byte_size += loop_mult * c_size * sizeof(T_INT8);
        fprintf(fp, "%sstatus = TipcMsgAppendInt8Array(msg, int8_%d_val, %d);\n",
                dent, c_size, c_size);
        break;
      case T_GEN_FT_REAL4_ARRAY:
        byte_size += loop_mult * c_size * sizeof(T_REAL4);
        fprintf(fp, 
                "%sstatus = TipcMsgAppendReal4Array(msg, real4_%d_val, %d);\n", 
                dent, c_size, c_size);
        break;
      case T_GEN_FT_REAL8_ARRAY:
        byte_size += loop_mult * c_size * sizeof(T_REAL8);
        fprintf(fp, 
                "%sstatus = TipcMsgAppendReal8Array(msg, real8_%d_val, %d);\n",
                dent, c_size, c_size);
        break;
      case T_GEN_FT_REAL16_ARRAY:
        byte_size += loop_mult * c_size * sizeof(T_REAL16);
        fprintf(fp, 
                "%sstatus = TipcMsgAppendReal16Array(msg, real16_%d_val, %d);\n",
                dent, c_size, c_size);
        break;
      default:
        TutOut("Parse Error: received unexpected token <%s>, expected field\n",
               bmValidTypes[grammar[counter]]);
        return T_FALSE;
    } /* switch */
    /*
     * If we generated an append, check the status
     */
    if (T_GEN_FT_CHAR < grammar[counter]) {
      fprintf(fp, "%sT_EXPECT(T_TRUE == status);\n", dent);
      /*
       * if the field has a count, skip past it.
       */
      if (T_GEN_FT_REAL16 < grammar[counter]) {
        counter += 3;
      }
    }
    /*
     * We do not support any fields after the loop, check for it, and issue
     * a warning.
     */
    if (T_GEN_END_LOOP == grammar[counter]) {
      if (size > counter + 4) {
        TutOut("Parse Warning: Fields <%s> after the loop are not allowed.\n",
               bmValidTypes[grammar[counter]]);
      }
      break;
    }
  } 

  fputs(bmGeneratedFile[T_BM_GEN_BUILD_FUNC_END], fp);
  return T_TRUE;
}

/* ========================================================================= */
/*..TbmGenerateProcessCb -- generate the callback for the generated mt */
T_BOOL TbmGenerateProcessCb
(
FILE *fp,
T_INT4 *grammar,
T_INT4 size
)
{
  T_INT4 counter;
  T_INT4 c_size;
  T_STR dent = "      ";
  T_STR var = (T_STR)T_NULL;

  T_CHECK_NULL(fp, T_FALSE);
  T_CHECK_NULL(grammar, T_FALSE);

  fputs(bmGeneratedFile[T_BM_GEN_FUNC_HEADER], fp);
  fputs(bmGeneratedFile[T_BM_GEN_CB_FUNC_BEGIN], fp);

  if (!bmGenerateCbFuncDecl(fp, grammar, size)) {
    return T_FALSE;
  }

  fputs(bmGeneratedFile[T_BM_GEN_CB_FUNC_MIDDLE], fp);

  for (counter = 0; counter < size; counter++) {
    switch (grammar[counter]) {
      case T_GEN_BEGIN_LOOP:
        {
          T_INT4 intr;

          for (intr = counter + 1; intr < size; intr++) {
            if (T_GEN_BEGIN_LOOP == grammar[intr]) {
              TutOut("Parse error: found a recursive loop. Not allowed.\n");
              return T_FALSE;
            }
            if (T_GEN_END_LOOP == grammar[intr]) {
              break;
            }
          } 
          if (intr == size) {
            TutOut("Parse error: Did not find end of loop structure.\n");
            return T_FALSE;
          }

          if (!bmGetFieldSize(grammar, intr, size, &c_size)) {
            return T_FALSE;
          }

          fprintf(fp, "\n%sfor(counter = 0; counter < %d; counter++) {\n", 
                  dent, c_size);

          dent = "        ";
          var = (T_STR)T_NULL;
        }
        break;
      case T_GEN_END_LOOP:
        dent = "      ";
        var = (T_STR)T_NULL;
        fprintf(fp, "%s} \n", dent);
        break;
      case T_GEN_FT_CHAR:
        var = "Char(msg, &char_val)";
        break;
      case T_GEN_FT_INT2:
        var = "Int2(msg, &int2_val)";
        break;
      case T_GEN_FT_INT4:
        var = "Int4(msg, &int4_val)";
        break;
      case T_GEN_FT_INT8:
        var = "Int8(msg, &int8_val)";
        break;
      case T_GEN_FT_REAL4:
        var = "Real4(msg, &real4_val)";
        break;
      case T_GEN_FT_REAL8:
        var = "Real8(msg, &real8_val)";
        break;
      case T_GEN_FT_REAL16:
        var = "Real16(msg, &real16_val)";
        break;
      case T_GEN_FT_STR:
        var = "Str(msg, &str_val)";
        break;
      case T_GEN_FT_BINARY:
        var = "Binary(msg, &binary_val, &size)";
        break;
      case T_GEN_FT_INT2_ARRAY:
        var = "Int2Array(msg, &int2_p_val, &size)";
        break;
      case T_GEN_FT_INT4_ARRAY:
        var = "Int4Array(msg, &int4_p_val, &size)";
        break;
      case T_GEN_FT_INT8_ARRAY:
        var = "Int8Array(msg, &int8_p_val, &size)";
        break;
      case T_GEN_FT_REAL4_ARRAY:
        var = "Real4Array(msg, &real4_p_val, &size)";
        break;
      case T_GEN_FT_REAL8_ARRAY:
        var = "Real8Array(msg, &real8_p_val, &size)";
        break;
      case T_GEN_FT_REAL16_ARRAY:
        var = "Real16Array(msg, &real16_p_val, &size)";
        break;
      default:
        TutOut("Parse Error: received unexpected token <%s>, expected field\n",
               bmValidTypes[grammar[counter]]);
        return T_FALSE;
    } /* switch */
    if (T_NULL != var) {
      fprintf(fp, "%sstatus = TipcMsgNext%s;\n", dent, var);
      fprintf(fp, "%sT_EXPECT(T_TRUE == status);\n", dent);
    }
    /*
     * if we had an array, skip the size fields
     */
    if (T_GEN_FT_REAL16 < grammar[counter]) {
      counter += 3;
    }
    /*
     * We do not support any fields after the loop, check for it, and issue
     * a warning.
     */
    if (T_GEN_END_LOOP == grammar[counter]) {
      if (size > counter + 4) {
        TutOut("Parse Warning: Fields <%s> after the loop are not allowed.\n",
               bmValidTypes[grammar[counter]]);
      }
      break;
    }
  } 

  fputs(bmGeneratedFile[T_BM_GEN_CB_FUNC_END], fp);
  return T_TRUE;
}

/* ========================================================================= */
/*..main -- main program */
int main
(
int argc,
char **argv
)
{
  FILE   *fp;
  T_INT4  counter;
  T_INT4  i;
  T_INT4  file_size;
  T_INT4 *grammar;
  T_BOOL  status;
  T_BOOL  white_space;
  T_CHAR  chr;
  T_STR   grammar_str;
  T_STR   tkn;

  /* 
   * check for command-line argument 
   */
  if (argc != 2) {
    TutOut("Usage: maketype <filname>\n");
    TutExit(T_EXIT_FAILURE);
  }

  /*
   * first read the grammer file into a buffer
   */
  status = TutFileGetSize(argv[1], &file_size);
  T_EXPECT(T_TRUE == status);

  if (T_NULL == (fp = fopen(argv[1], "r"))) {
    TutOut("Could not open file <%s>\n", argv[1]);
    return -1;
  }

  T_MALLOC(grammar_str, 2 * file_size, T_STR);
  T_MALLOC(grammar, file_size, T_INT4 *);

  for (counter = 0, white_space = T_TRUE, chr = fgetc(fp); !feof(fp); chr = fgetc(fp)) {
    if (isspace(chr)) {
      if (!white_space) {
        grammar_str[counter++] = ' ';
        white_space = T_TRUE;
      }
    }
    else {
      if (
         ')' == chr 
         ||
         '(' == chr 
         ||
         '}' == chr 
         ||
         '{' == chr
         ) {
        if (!white_space) {
          grammar_str[counter++] = ' ';
        }
        grammar_str[counter++] = chr;
        grammar_str[counter++] = ' ';
        white_space = T_TRUE;
      }
      else {
        white_space = T_FALSE;
        grammar_str[counter++] = chr;
      }  
    } 
  } 
  grammar_str[counter] = '\0';
  fclose(fp);

  /*
   * parse the grammar
   */
  for (counter = 0, tkn = strtok(grammar_str, " "); tkn != T_NULL; counter++, tkn = strtok(NULL, " ")) {
    for (i = 0; i < T_ARRAY_ELEMENTS(bmValidTypes); i++) {
      if (T_STREQI(bmValidTypes[i], tkn)) {
        break;
      }
    } 

    if (T_GEN_FT_INVALID == i) {
      break;
    }
    else {
      grammar[counter] = i;
    } 

    if (T_GEN_BEGIN_COUNT == i) {
      if (T_NULL != (tkn = strtok(NULL, " "))) {
        counter++;
        grammar[counter] = atoi(tkn);
      }
      else {
        tkn = "eof";
        break;
      } 
    }
  } 

  /*
   * check for a parse error
   */
  if (tkn != T_NULL) {
    TutOut("Parse error: <%s>\n", tkn);
    T_FREE(grammar_str);
    T_FREE(grammar);
    return -1;
  }

  /*
   * open the generate file
   */
  if (T_NULL == (fp = fopen(T_BM_GEN_FILE, "w"))) {
    TutOut("Could not be opened file <%s>\n", T_BM_GEN_FILE);
    return -1;
  }

  fputs(bmGeneratedFile[T_BM_GEN_FILE_HEADER], fp);

  status = T_TRUE;
  if (status && !TbmGenerateBuildMsg(fp, grammar, counter)) {
    status = T_FALSE;
  }
  if (status && !TbmGenerateCreateMt(fp, grammar, counter)) {
    status = T_FALSE;
  }
  if (status && !TbmGenerateProcessCb(fp, grammar, counter)) {
    status = T_FALSE;
  }

  fputs(bmGeneratedFile[T_BM_GEN_FUNC_HEADER], fp);
  fprintf(fp, bmGeneratedFile[T_BM_GEN_SIZE_FUNC], byte_size);

  if (!status) {
    TutOut("Parse Error: generated file <%s> may not have been completed.\n",
           T_BM_GEN_FILE);
  }

  fclose(fp);
  T_FREE(grammar_str);
  T_FREE(grammar);
  return 0;
}

