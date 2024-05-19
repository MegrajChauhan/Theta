#ifndef _ERROR_
#define _ERROR_

#include <stdlib.h>
#include "token.h"
#include <string.h>
#include "../utils/types.h"

typedef struct error error;
typedef enum error_t error_t;

enum error_t
{
    _NONE,

    _INVALID_FILE,
    _FILE_A_DIR,
    _FILE_NOT_FOUND,
    _INTERNAL_ERROR,
};

struct error
{
    token_location err_tok;
    char *msg;
    error_t type;
};

static error *e = NULL;
static size_t error_count = 0;

void add_error(token_location tok_info, char *msg, error_t type);

void free_errors();

#endif