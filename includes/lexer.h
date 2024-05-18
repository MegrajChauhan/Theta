#ifndef _LEXER_
#define _LEXER_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "str.h"
#include "tracer.h"
#include "array.h"
#include "tokens.h"
#include "error.h"
#include "../utils/types.h"

typedef struct thfile thfile;
typedef struct thlexer thlexer;

#define _TRACER_STACK_LEN_ 4
#define _FILE_ARRAY_LEN_ _TRACER_STACK_LEN_

struct thfile
{
    string *file_contents, *filename; // file's contents
    char *curr_char, *end;
    size_t line, col; // current line and column
};

struct thlexer
{
    tracer_stack *file_tracer_stack; // keep track of the current files being lexed
    thfile *curr_file;               // the current file being lexed
    array *file_array;               // keep track of which files have been included already
};

#define CURR *(_l->curr_file->curr_char)
#define NXT _l->curr_file->curr_char++
#define ISEOF _l->curr_file->curr_char == _l->curr_file->end
#define UPLINE _l->curr_file->line++
#define UPCOL _l->curr_file->col++
#define PEEK *(_l->curr_file->curr_char + 1)
#define ISCOMMENT (CURR == '/' && !ISEOF && PEEK == '/')
#define COLRESET _l->curr_file->col = 0

#define ISALPHA(x) (((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z')))
#define ISNUM(x) (x >= '0' && x <= '9')
#define SKIP(x) (x == ' ' || x == '\t') // we cannot skip newlines because we need to keep track of current line number

bool_t _custom_file_check_fnc_(void *, void *);

thfile *init_new_file(const char *);

thlexer *init_lexer(const char *);

void destroy_lexer(thlexer *);

void destroy_file(thfile *);

token next_token(thlexer *);

ret_t read_file(thfile *);

// trace back the current tracer stack to exactly pinpoint the error
char *trace_back_tracer_stack();

// Setup the first file
ret_t first_file_lexer_setup(thlexer *_l);

bool_t is_file_valid(char *filename);

#endif