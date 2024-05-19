#ifndef _TOKEN_
#define _TOKEN_

#include <stddef.h>

typedef struct token_location token_location;
typedef struct token token;
typedef enum token_t token_t;

struct token_location
{
    size_t line;
    size_t column;
    size_t token_file_index;
};

enum token_t
{
    TOK_EOF,
    TOK_ERR,
    TOK_DIV,
    TOK_INVALID,
};

struct token
{
    token_location loc;
    token_t type;
    char *token_value;
};

#endif