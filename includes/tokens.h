#ifndef _TOKENS_
#define _TOKENS_

#include <stdlib.h>

typedef enum token_t token_t;
typedef struct token token;

enum token_t
{
    TOK_ERR,
    TOK_EOF,
    TOK_INTERNAL_ERROR, // special signaling token to halt any process
    TOK_IDENTIFIER,
    TOK_IMPORT,
};

struct token
{
    token_t type;
    char *token_value; // not all types will have value
};

#define import "import"


#endif