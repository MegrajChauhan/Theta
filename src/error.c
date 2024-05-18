#include "../includes/error.h"

ret_t init_error_hdlr(size_t init_cap)
{
    hdlr.errs = (error *)malloc(sizeof(error) * init_cap);
    if (hdlr.errs == NULL)
        return FAILURE;
    hdlr.cap = init_cap;
    hdlr.err_count = 0;
    return SUCCESS;
}

// The error handler itself can fail
ret_t register_error(error _e)
{
    if (hdlr.err_count == hdlr.cap)
    {
        // it is full
        error *tmp = hdlr.errs;
        size_t new_len = hdlr.cap + (hdlr.cap / 2);
        hdlr.errs = (error *)realloc(hdlr.errs, sizeof(error) * new_len);
        if (hdlr.errs == NULL)
        {
            hdlr.errs = tmp;
            return FAILURE;
        }
    }
    hdlr.errs[hdlr.err_count] = _e;
    hdlr.err_count++;
    return SUCCESS;
}

error gen_new_error(error_context _c, error_lvl _l, error_t _t, string *msg, size_t line, size_t col, string *filename, string *extra_info, string *err_line)
{
    error _e;
    _e.col = col;
    _e.context = _c;
    _e.err_msg = msg;
    _e.error_line = err_line;
    _e.extra_error_info = extra_info;
    _e.filename = filename;
    _e.line = line;
    _e.lvl = _l;
    _e.type = _t;
    return _e;
}

// The task of freeing the extra info, error line and message can be left to the hdlr
void rid_all_errors()
{
    if (hdlr.err_count == 0)
        return;
    for (size_t i = 0; i < hdlr.err_count; i++)
    {
        // An error may have NULL extra info but other things are present
        if (hdlr.errs[i].extra_error_info != NULL)
            free(hdlr.errs[i].extra_error_info);
        free(hdlr.errs[i].err_msg);
        free(hdlr.errs[i].error_line);
    }
    hdlr.err_count = 0;
    free(hdlr.errs);
}

void display_all_errors()
{
    // implement error displaying here
}

void display_internal_error()
{
    printf("INTERNAL ERROR: It is not your fault.\n");
}
