#ifndef _STR_
#define _STR_

#include <string.h>
#include <stdlib.h>
#include "../utils/config.h"
#include "../utils/types.h"

// The initial capacity of the string
#ifndef _INIT_CAP_
#define _INIT_CAP_ 32
#endif

typedef struct string string;

struct string
{
    char *_str; // We don't need the '\0' character here
    size_t len;
    size_t cap;
};

#define str_len(str) str->len
#define str_cap(str) str->cap
#define str_empty(str) str->len == 0
#define str_full(str) str->cap == str->len

string *init_string(size_t len);

void destroy_string(string *str);

// Iterators
char *begin(string *str);

// end returns one past the last character without giving a damn if there is data or not
// len's value may be messed with but we will make sure not to do so
char *end(string *str);

ret_t concat(string *dest, string *src);

// src in this case is classic C-string
ret_t append(string *dest, char *src);

ret_t assign_str(string *dest, char *src);

#endif