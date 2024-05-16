#ifndef _LEXER_
#define _LEXER_

#include <stdlib.h>
#include <string.h>
#include "tracer.h"
#include "array.h"
#include "../utils/types.h"

typedef struct thfile thfile;
typedef struct thlexer thlexer;

#define _TRACER_STACK_LEN_ 4
#define _FILE_ARRAY_LEN_ _TRACER_STACK_LEN_

struct thfile
{
    char *file_contents, *filename; // file's contents
    size_t line, col;               // current line and column
};

struct thlexer
{
    tracer_stack *file_tracer_stack; // keep track of the current files being lexed
    thfile *curr_file;               // the current file being lexed
    array *file_array;               // keep track of which files have been included already
};

bool_t _custom_file_check_fnc_(void *, void *);

thfile *init_new_file(const char *);

thlexer *init_lexer(const char *);

void destroy_lexer(thlexer *);

void destroy_file(thfile *);



#endif