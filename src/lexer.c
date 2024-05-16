#include "../includes/lexer.h"

thlexer *init_lexer(const char *inp_filename)
{
    thlexer *_l = (thlexer *)malloc(sizeof(thlexer));
    if (_l == NULL)
        return NULL;
    // We do not push the active file onto the tracer stack
    if ((_l->file_tracer_stack = init_tracer_stack(_TRACER_STACK_LEN_)) == NULL)
    {
        free(_l);
        return NULL;
    }
    if ((_l->file_array = init_array(_FILE_ARRAY_LEN_)) == NULL)
    {
        destroy_tracer_stack(_l->file_tracer_stack);
        free(_l);
        return NULL;
    }
    if ((_l->curr_file = init_new_file(inp_filename)) == NULL)
    {
        destroy_tracer_stack(_l->file_tracer_stack);
        destroy_array(_l->file_array);
        free(_l);
        return NULL;
    }
    // This shouldn't fail
    push_to_array(_l->file_array, (void *)(_l->curr_file->filename));
    return _l;
}

thfile *init_new_file(const char *filename)
{
    thfile *f = (thfile *)malloc(sizeof(thfile));
    if (f == NULL)
        return NULL;
    if (f->filename == NULL)
    {
        free(f);
        return NULL;
    }
    strcpy(f->filename, filename);
    f->col = 0;
    f->file_contents = NULL;
    f->filename = (char *)malloc(strlen(filename));
    f->line = 0;
    return f;
}

void detroy_file(thfile *f)
{
    if (surelyF(f == NULL))
        return;
    if (surelyT(f->file_contents != NULL))
        free(f->file_contents);
    if (surelyT(f->filename != NULL))
        free(f->filename);
    free(f);
}

void destroy_lexer(thlexer *_l)
{
    if (surelyF(_l == NULL))
        return;
    if (surelyT(_l->curr_file != NULL))
        destroy_file(_l);
    if (surelyT(_l->file_array != NULL))
    {
        destroy_elements(_l->file_array);
        destroy_array(_l->file_array);
    }
    if (surelyT(_l->file_tracer_stack != NULL))
        destroy_tracer_stack(_l->file_tracer_stack);
    free(_l);
}