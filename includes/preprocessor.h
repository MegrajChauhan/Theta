#ifndef _PREPROCESSOR_
#define _PREPROCESSOR_

#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../utils/config.h"
#include "../utils/types.h"

typedef struct TReader TReader;
typedef struct TPreprocessor TPreprocessor;

struct TReader
{
    FILE *file_ptr;
    char *file_contents;
    char *curr_file;
};

struct TPreprocessor
{
    TReader reader;
    STATE state;
    size_t col_no, line_no;
    char **_included_files_; // all included files
    size_t inc_count;
    char *error_message;
};

TPreprocessor *theta_init_preprocessor(char *file_path);

// process the file
bool_t theta_process_file(TPreprocessor *p);

void theta_destroy_preprocessor(TPreprocessor *prep);

// utilities
bool_t theta_is_file_valid(char *file_path);

bool_t theta_read_entire_file(TPreprocessor *prep);

#endif