#ifndef _PREPROCESSOR_
#define _PREPROCESSOR_

#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <stdio.h>

#include "error.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "../utils/config.h"
#include "../utils/types.h"

#define not_eof(x) (*x != '\0')
#define is_alpha(x) ((*x >= 'a' && *x <= 'z') || (*x >= 'A' && *x <= 'Z'))

#define sym_lim 1024 // the limit of a symbol i.e a symbol, could be a variable name, file path in import statement etc

typedef struct TReader TReader;
typedef struct TPreprocessor TPreprocessor;

struct TReader
{
    FILE *file_ptr;
    char *file_contents;
    char *curr_file;
    size_t len;
};

struct TPreprocessor
{
    TReader reader;
    STATE state;
    size_t col_no, line_no;
};

INTERNAL_VAR size_t det_len = 0;

// for error handling
INTERNAL_VAR size_t err_line_no, err_col;
INTERNAL_VAR char *err_msg, *err_line, *err_extra_info, *err_file; // these are freed by the error handler after printing the details
INTERNAL_VAR char **_included_files_;                              // all included files
INTERNAL_VAR size_t inc_count;

TPreprocessor *theta_init_preprocessor(char *file_path);

// process the file
bool_t theta_process_file(TPreprocessor *p);

void theta_destroy_preprocessor(TPreprocessor *prep);

bool_t theta_handle_imports(TPreprocessor *prep, char **it);

// utilities
bool_t theta_is_file_valid(char *file_path);

bool_t theta_read_entire_file(TPreprocessor *prep);

#endif