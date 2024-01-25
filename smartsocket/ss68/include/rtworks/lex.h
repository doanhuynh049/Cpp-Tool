/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/lex.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                    lex.h -- lexical analyzer                        */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_LEX_H_
#define _RTWORKS_LEX_H_

/* ----------------------------------------------------- */
/* Syntax Tokens.  Note that T_TOK_ID *MUST* be zero and */
/* T_TOK_PATTERN *MUST* be -1!!!  If you change any of   */
/* these defines, you should update utLexSyntaxTable in  */
/* util/lex.c also.  SMW                                 */
/* ----------------------------------------------------- */
#define T_TOK_ID                   0           /* identifier */
#define T_TOK_PATTERN             (-1)         /* named pattern (id?id?id) */
#define T_TOK_PLUS                (-2)         /* +  */
#define T_TOK_PLUS_PLUS           (-3)         /* ++ */
#define T_TOK_PLUS_EQ             (-4)         /* += */
#define T_TOK_MINUS               (-5)         /* -  */
#define T_TOK_MINUS_MINUS         (-6)         /* -- */
#define T_TOK_MINUS_EQ            (-7)         /* -= */
#define T_TOK_TIMES               (-8)         /* *  */
#define T_TOK_TIMES_EQ            (-9)         /* *= */
#define T_TOK_DIV                (-10)         /* /  */
#define T_TOK_DIV_EQ             (-11)         /* /= */
#define T_TOK_EQUAL              (-12)         /* =  */
#define T_TOK_NOT_EQ             (-13)         /* <> */
#define T_TOK_LESS               (-14)         /* <  */
#define T_TOK_LESS_EQ            (-15)         /* <= */
#define T_TOK_GREATER            (-16)         /* >  */
#define T_TOK_GREATER_EQ         (-17)         /* >= */
#define T_TOK_ASSIGNMENT         (-18)         /* := */
#define T_TOK_LPAREN             (-19)         /* (  */
#define T_TOK_RPAREN             (-20)         /* )  */
#define T_TOK_LBRACKET           (-21)         /* [  */
#define T_TOK_RBRACKET           (-22)         /* ]  */
#define T_TOK_LBRACE             (-23)         /* {  */
#define T_TOK_RBRACE             (-24)         /* }  */
#define T_TOK_COLON              (-25)         /* :  */
#define T_TOK_PERIOD             (-26)         /* .  */
#define T_TOK_COMMA              (-27)         /* ,  */
#define T_TOK_SEMICOLON          (-28)         /* ;  */
#define T_TOK_EXCLAMPT           (-29)         /* !  */
#define T_TOK_DOLLAR             (-30)         /* $  */
#define T_TOK_CARET              (-31)         /* ^  */
#define T_TOK_AT                 (-32)         /* @  */
#define T_TOK_POUND              (-33)         /* #  */
#define T_TOK_PERCENT            (-34)         /* %  */
#define T_TOK_AND                (-35)         /* &  */
#define T_TOK_TILDE              (-36)         /* ~  */
#define T_TOK_BAR                (-37)         /* |  */
#define T_TOK_UNUM               (-38)         /* unsigned numeric constant */
#define T_TOK_DQUOTESTR          (-39)         /* double quoted "string" */
#define T_TOK_BACKSLASH          (-40)         /* \ */
#define T_TOK_WHITE_SPACE        (-41)         /* white space or comment */
#define T_TOK_EOF                (-42)         /* end of file */
#define T_TOK_ERROR              (-43)         /* unknown character */

#define T_TOK_OCTETSTR (-44)	/* Octet string constant */

/* ---------------------------- */
/* Attributes for TutLexCreate. */
/* ---------------------------- */
typedef enum {
  T_LEX_DONE, /* dummy (needs to be 0) */
  T_LEX_INPUT_FNAME, /* file name */
  T_LEX_INPUT_FILE, /* lexer file pointer */
  T_LEX_INPUT_STRING, /* lexer string */
  T_LEX_ERROR_FUNC, /* error function */
  T_LEX_ERROR_ARG, /* argument to pass to error function */
  T_LEX_WHITE_SPACE_FLAG, /* if set, treat white space as a token */
  T_LEX_RETAIN_CHAR_CASE /* if set, retain case for identifiers */
} T_LEX_ATT;

#define T_LEX_INPUT_BUF_SIZE  512

/* ------------------------------------------------------------- */
/* I would rather use T_LEX_ERROR_FUNC here, but that is already */
/* taken.  SMW 12/17/92                                          */
/* Changed to T_ENTRY_VA1 * T_ENTRY_VA2 since we moved to        */
/* __stdcall on WIN32. T_ENTRY_VA1 and T_ENTRY_VA2 are defined   */
/* __cdecl are varargs function pointers and prototypes need to  */
/* be __cdecl on WIN32. BM 06/21/98                              */
/* ------------------------------------------------------------- */
typedef void (T_ENTRY_VA1 * T_ENTRY_VA2 T_EXPORT T_LEX_ERROR_FUNCTION)
        T_TYPEDEF((
	           T_LEX /*lexer*/, 
		   T_STR /*format_str*/,
		   va_list /*var_arg_ptr*/,
		   T_PTR /*error_arg*/
		  ));

/* ------------------------------------------------------- */
/* Information kept about each input source we are lexing. */
/* ------------------------------------------------------- */
/* typedef struct T_LEX_STRUCT T_LEX_STRUCT; */ /* now in <rtworks/util.h> */
struct T_LEX_STRUCT {
  T_INT4 token; /* current token */
  T_STR token_buffer; /* string rep of current token */
  T_STR token_ptr; /* last character in token_buffer */
  T_INT4 token_size; /* number of characters in token */
  T_INT4 max_token_size; /* number of bytes allocated for token_buffer */

  /* token attributes */
  T_ID id_buffer; /* token attribute for ID_SYM */
  T_SAFE_STR_STRUCT string_buffer; /* token attribute for DQUOTESTR_SYM */
  T_REAL8 num_val; /* token attribute for UNUM_SYM */

  /* error handling */
  T_LEX_ERROR_FUNCTION error_func;
  T_PTR error_arg; /* arg to pass to error_func (should be void *) */

  /* other stuff */
  T_STR input_fname; /* name of input file (NULL if N/A) */
  FILE *input_file; /* input file (NULL if lexing a string) */
  T_STR input_string; /* input string (NULL if lexing a file) */
  T_INT4 line_num; /* current line number of source file */
  T_INT4 input_char; /* current input character */
  T_STR input_ptr; /* the character we are currently processing */
  T_CHAR input_buffer[T_LEX_INPUT_BUF_SIZE + 1];/* buffer of input characters */
  T_BOOL white_space_flag; /* TRUE iff we should use T_TOK_WHTESPACE */
  T_BOOL retain_char_case_flag;	/* TRUE iff char case for IDs retained */
  T_BOOL octet_str_flag;	/* TRUE iff we should use T_TOK_OCTETSTR */
  T_CB_LIST token_cb_list;	/* token accept callbacks */
};

/* --------------------------------- */
/* T_GLEX macros -- get lexer field. */
/* --------------------------------- */
#define T_GLEX_TOKEN(lexer)             ((lexer)->token)
#define T_GLEX_TOKEN_BUFFER(lexer)      ((lexer)->token_buffer)
#define T_GLEX_TOKEN_PTR(lexer)         ((lexer)->token_ptr)
#define T_GLEX_TOKEN_SIZE(lexer)        ((lexer)->token_size)
#define T_GLEX_MAX_TOKEN_SIZE(lexer)    ((lexer)->max_token_size)
#define T_GLEX_ID(lexer)                ((lexer)->id_buffer)
#define T_GLEX_NUM(lexer)               ((lexer)->num_val)
#define T_GLEX_STR(lexer)               ((lexer)->string_buffer.str)
#define T_GLEX_ERROR_FUNC(lexer)        ((lexer)->error_func)
#define T_GLEX_ERROR_ARG(lexer)         ((lexer)->error_arg)
#define T_GLEX_LINE_NUM(lexer)          ((lexer)->line_num)
#define T_GLEX_INPUT_CHAR(lexer)        ((lexer)->input_char)
#define T_GLEX_INPUT_FNAME(lexer)       ((lexer)->input_fname)
#define T_GLEX_WHITE_SPACE_FLAG(lexer)  ((lexer)->white_space_flag)
#define T_GLEX_TOKEN_CB_LIST(lexer)     ((lexer)->token_cb_list)

/* -------------------------------- */
/* T_SLEX macros -- set lexer field */
/* -------------------------------- */
#define T_SLEX_TOKEN(lexer, val) (T_GLEX_TOKEN(lexer) = (val))
#define T_SLEX_TOKEN_BUFFER(lexer, val) (T_GLEX_TOKEN_BUFFER(lexer) = (val))
#define T_SLEX_TOKEN_PTR(lexer, val) (T_GLEX_TOKEN_PTR(lexer) = (val))
#define T_SLEX_TOKEN_SIZE(lexer, val) (T_GLEX_TOKEN_SIZE(lexer) = (val))
#define T_SLEX_NUM(lexer, val) (T_GLEX_NUM(lexer) = (val))
#define T_SLEX_WHITE_SPACE_FLAG(lexer, val) \
            (T_GLEX_WHITE_SPACE_FLAG(lexer) = (val))
#define T_SLEX_ERROR_FUNC(lexer, val) (T_GLEX_ERROR_FUNC(lexer) = (val))
#define T_SLEX_ERROR_ARG(lexer, val) (T_GLEX_ERROR_ARG(lexer) = (val))

#define TutLexInitFileName(fname) \
  TutLexCreate(T_LEX_INPUT_FNAME, (fname), 0)
#define TutLexInitFilePtr(file) \
  TutLexCreate(T_LEX_INPUT_FILE, (file), 0)
#define TutLexInitString(str) \
  TutLexCreate(T_LEX_INPUT_STRING, (str), 0)

#endif /* _RTWORKS_LEX_H_ */
