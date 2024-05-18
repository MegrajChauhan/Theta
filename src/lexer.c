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
    f->filename = init_string(strlen(filename));
    if (f->filename == NULL)
    {
        free(f);
        return NULL;
    }
    if (append(f->filename, (char *)filename) == FAILURE)
    {
        destroy_string(f->filename);
        free(f);
        return NULL;
    }
    f->col = 0;
    f->file_contents = NULL;
    f->line = 0;
    return f;
}

void detroy_file(thfile *f)
{
    if (surelyF(f == NULL))
        return;
    if (surelyT(f->file_contents != NULL))
        destroy_string(f->file_contents);
    if (surelyT(f->filename != NULL))
        destroy_string(f->filename);
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

// This is called for any import statements
// ret_t read_file(thfile *f)
// {
//     // f will not be NULL
//     FILE *t = fopen(f->filename, "r");
//     if (t == NULL)
//     {
//         error_t t = (errno == EISDIR) ? FILE_A_DIR : FILE_DOESNT_EXIST;
//         // error _e = gen_new_error(LEXING, FATAL, t, "The given file")
//         return FAILURE;
//     }
// }

INTERNAL_FUNC void get_rid_of_tabs_and_spaces(thlexer *_l)
{
    if (SKIP(CURR))
    {
        while (SKIP(CURR) && !ISEOF)
        {
            NXT;
            UPCOL;
        }
    }
    if (CURR == '\n')
    {
        while (!ISEOF && CURR == '\n')
        {
            NXT;
            if (ISEOF)
                return;
            UPLINE;
        }
        COLRESET;
    }
}

INTERNAL_FUNC void get_rid_of_comments(thlexer *_l)
{
    if (ISCOMMENT)
    {
        while (ISCOMMENT && !ISEOF)
        {
            while (!ISEOF && CURR != '\n')
            {
                NXT;
            }
            get_rid_of_tabs_and_spaces(_l);
        }
        COLRESET;
    }
}

// From the current character, build a symbol
INTERNAL_FUNC char *get_current_symbol(thlexer *_l)
{
    // since buf is used temporarily, it is char*
    char *buf;
    size_t len = 0;
    char *st = _l->curr_file->curr_char;
    while (!ISEOF && !SKIP(CURR) && CURR != '\n')
    {
        len++;
        NXT;
        UPCOL;
    }
    buf = (char *)malloc(len + 1);
    if (buf == NULL)
        return NULL;
    strncpy(buf, st, len);
    buf[len] = '\0';
    return buf;
}

INTERNAL_FUNC token identify_keyword_or_identifier(char *sym)
{
    if (strcmp(sym, import) == 0)
    {
        free(sym);
        return (token){.token_value = NULL, .type = TOK_IMPORT};
    }
    return (token){
        .type = TOK_IDENTIFIER,
        .token_value = sym};
}

INTERNAL_FUNC ret_t handle_new_import(thlexer *_l)
{
    NXT;
    UPCOL;
    char *path = get_current_symbol(_l);
    if (path == NULL)
    {
        return AMBI; // indicating internal error
    }
    
}

token next_token(thlexer *_l)
{
    // If this is the input file itself, first_file_lexer_setup needs to be called
    token res;
    // firstly check if the current file being lexed has reached it's end
    // If so, exit and start lexing another
    if (CURR == _l->curr_file->end)
    {
        // we are at the end, replace the file
        // check the tracer and see if any files are left
        if (is_stack_empty(_l->file_tracer_stack))
        {
            destroy_file(_l->curr_file);
            goto err;
        }
        destroy_file(_l->curr_file); // get rid of current file
        // restore previous file
        _l->curr_file = (thfile *)pop_tracer_node(_l->file_tracer_stack);
    }
    get_rid_of_comments(_l);
    if (ISALPHA(CURR))
    {
        char *sym = get_current_symbol(_l);
        if (sym == NULL)
        {
            display_internal_error();
            goto err;
        }
        // now we have the tedious task of checking if sym is a keyword or anything else *Agh*
        res = identify_keyword_or_identifier(sym);
        if (res.type == TOK_IMPORT)
        {
            // the lexer needs to handle this itself
        }
    }
err:
    res.type = TOK_ERR;

internal:
    res.type = TOK_INTERNAL_ERROR;

ret:
    return res;
}

ret_t first_file_lexer_setup(thlexer *_l)
{
    // During this initial setup, if errors are encountered, error messages are printed and returned
    // _l will be initialized and filename will not be NULL
    char *buf;
    getcwd(buf, 0);
    if (is_file_valid(_l->curr_file->filename) == false)
    {
        fprintf("ERROR: The input file '%s/%s' is not a valid file.\n", buf, _l->curr_file->filename);
        free(buf);
        return FAILURE;
    }
    // the file is valid
    FILE *f = fopen(_l->curr_file->filename, "r");
    if (f == NULL)
    {
        if (errno == EISDIR)
        {
            fprintf("ERROR: The input file '%s/%s' is not a file but a directory instead.\n", buf, _l->curr_file->filename);
            free(buf);
            return FAILURE;
        }
        else
        {
            fprintf("ERROR: The input file '%s/%s' doesn't exist or there was an internal error.\n", buf, _l->curr_file->filename);
            free(buf);
            return FAILURE;
        }
    }
    free(buf); // we are assuming the getcwd will never fail
    // read the entire file
    size_t file_len = 0;
    fseek(f, SEEK_SET, SEEK_END);
    file_len = ftell(f);
    rewind(f);
    buf = (char *)malloc(file_len);
    if (buf == NULL)
    {
        fclose(f);
        return FAILURE;
    }
    fread(buf, 1, file_len, f); // again we assume this will not fail
    if (assign_str(_l->curr_file, buf) == FAILURE)
    {
        free(buf);
        fclose(f);
        return FAILURE;
    }
    // no need to free buf as the curr_file handles the pointer of buf
    _l->curr_file->curr_char = begin(_l->curr_file);
    _l->curr_file->end = end(_l->curr_file);
    fclose(f);
}

bool_t is_file_valid(char *filename)
{
    size_t len = strlen(filename);
    if (len < 3)
        return false;
    char *tmp = filename + (len - 3);
    if (strcmp(tmp, ".th") != 0)
        return false;
    return true;
}
