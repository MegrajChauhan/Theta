#ifndef _TOKENS_
#define _TOKENS_

#include <stdlib.h>

typedef enum token_t token_t;
typedef struct token token;

enum token_t
{
    TOK_ERR,

};

struct token
{
    token_t type;
    char *token_value; // not all types will have value
};

#endif