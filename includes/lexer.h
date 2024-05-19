#ifndef _LEXER_
#define _LEXER_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "../utils/types.h"
#include "token.h"
#include "error.h"

typedef struct lexer lexer;

struct lexer
{
    token *tokens; // all tokens
    char *file_contents;
    size_t line, col;
    size_t tok_count;
};

#define isalpha(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
#define isnum(x) (x >= '0' && x <= '9')
#define skip(x) (x == ' ' || x == '\t')
#define iseof(x) (x == '\0')
#define peek(x) *(x + 1)

#define loc(LINE, COL, IND) \
    (token_location) { .column = COL, .line = LINE, .token_file_index = IND }
#define build_tok(VAL, TYPE, LINE, COL, IND)                         \
    (token)                                                          \
    {                                                                \
        .token_value = VAL, .type = TYPE, .loc = loc(LINE, COL, IND) \
    }

#define empty_loc() loc(0, 0, 0)

static size_t filecount = 0;
static char **file_names; // all included files

lexer *init_lexer(char *inpfilename);

void destroy_lexer(lexer *_l);

// In this lexer, we will lex the entire file first before proceeding to parsing
// This is not a unique but less common approach and considered inefficient(for some reason)
bool lex_all(lexer *_l);

bool add_token(lexer *_l, token tok);

#endif