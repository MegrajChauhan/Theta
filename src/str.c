#include "../includes/str.h"

string *init_string(size_t len)
{
    string *str = (string *)malloc(sizeof(string));
    if (str == NULL)
        return NULL;
    str->_str = (char *)malloc(len);
    if (str->_str == NULL)
    {
        free(str);
        return NULL;
    }
    str->len = 0;
    str->cap = len;
    return str;
}

void destroy_string(string *str)
{
    if (str == NULL)
        return;
    if (str->_str != NULL)
        free(str->_str);
    free(str);
}

char *begin(string *str)
{
    return &str->_str[0];
}

char *end(string *str)
{
    return &(str->_str[str->len]); // remember this returns an address that is not _str's
}

ret_t concat(string *dest, string *src)
{
    char *tmp = dest->_str;
    size_t new_len = dest->len + src->len;
    dest->_str = (char *)realloc(dest->_str, new_len);
    if (dest->_str == NULL)
    {
        dest->_str = tmp;
        return FAILURE;
    }
    strncat(dest->_str, src->_str, src->len);
    dest->cap = new_len;
    dest->len = new_len;
    // We do not meddle with src
    return SUCCESS;
}

ret_t append(string *dest, char *src)
{
    char *tmp = dest->_str;
    size_t src_len = strlen(src);
    size_t new_len = dest->len + src_len;
    dest->_str = (char *)realloc(dest->_str, new_len);
    if (dest->_str == NULL)
    {
        dest->_str = tmp;
        return FAILURE;
    }
    strncat(dest->_str, src, src_len);
    dest->cap = new_len;
    dest->len = new_len;
    return SUCCESS;
}

ret_t assign_str(string *dest, char *src)
{
    // src is a C-string
    size_t src_len = strlen(src);
    size_t new_len = src_len;
    if (dest->_str != NULL)
        free(dest->_str); // free first
    dest->_str = (char *)malloc(new_len);
    if (dest->_str == NULL)
        return FAILURE;
    strncat(dest->_str, src, src_len);
    dest->cap = new_len;
    dest->len = new_len;
    return SUCCESS;
}