#include "../includes/error.h"

void add_error(token_location tok_info, char *msg, error_t type)
{
    error *tmp = e;
    if (error_count == 0)
    {
        e = (error *)malloc(sizeof(error));
        if (e == NULL)
            return;
    }
    else
    {
        // reallocate the memory
        // if failed, we will keep the previous errors but simply return
        // This way, we will show what previous errors were already their
        // A thing called "Silently failing" but in this case, there won't be complete failure and we will continue
        e = (error *)realloc(e, (sizeof(error) * error_count) + 1);
        if (e == NULL)
        {
            e = tmp;
            return;
        }
    }
    // finally append the new error
    e[error_count].msg = (char *)malloc(strlen(msg) + 1);
    if (e[error_count].msg == NULL)
    {
        // we will simply return here
        return;
    }
    strcpy(e[error_count].msg, msg);
    e[error_count].err_tok = tok_info; // tokens are freed only after the entire compilation is done so we can store a copy of it's address
    e[error_count].type = type;
    error_count++; // done
}

void free_errors()
{
    if (e == NULL)
        return;
    for (size_t i = 0; i < error_count; i++)
    {
        free(e[i].msg);
    }
    free(e);
}