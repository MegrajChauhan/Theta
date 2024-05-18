#ifndef _ERROR_
#define _ERROR_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "str.h"
#include "../utils/config.h"
#include "../utils/types.h"

typedef struct error error;           // represents a single error
typedef struct error_hdlr error_hdlr; // the error handler
typedef enum error_lvl error_lvl;
typedef enum error_context error_context;
typedef enum error_t error_t;

enum error_t
{
    FILE_A_DIR,
    INVALID_FILE_EXT,
    FILE_DOESNT_EXIST,
};

enum error_context
{
    LEXING,
    PARSING,
};

enum error_lvl
{
    FATAL,
    WARNING,
    NOTE
};

struct error
{
    error_context context;
    error_lvl lvl;
    error_t type;
    string *err_msg;
    size_t line, col;
    string *filename;
    string *extra_error_info;
    string *error_line;
};

struct error_hdlr
{
    error *errs;      // all the errors
    size_t err_count; // the number of errors registered
    size_t cap;       // the capacity of errs array
};

INTERNAL_VAR error_hdlr hdlr; // this variable is not accessible to others directly

ret_t init_error_hdlr(size_t);

ret_t register_error(error);

error gen_new_error(error_context _c, error_lvl _l, error_t _t, string *msg, size_t line, size_t col, string *filename, string *extra_info, string *err_line);

// empty the currently registered errors without printing them
// free the array too
void rid_all_errors();

// empty the currently registered errors while printing them
// free the array too
void display_all_errors();

void display_internal_error();

#endif