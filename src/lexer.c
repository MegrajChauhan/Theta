#include "../includes/lexer.h"

lexer *init_lexer(char *inpfilename)
{
    lexer *l = (lexer *)malloc(sizeof(lexer));
    if (l == NULL)
        return NULL;
    file_names = (char **)malloc(sizeof(char *));
    if (file_names == NULL)
    {
        free(l);
        return NULL;
    }
    file_names[0] = (char *)malloc(strlen(inpfilename) + 1);
    if (file_names[0] == NULL)
    {
        free(file_names);
        free(l);
        return NULL;
    }
    strcpy(file_names[0], inpfilename);
    l->file_contents = NULL;
    l->tokens = NULL;
    return l;
}

void destroy_lexer(lexer *_l)
{
    if (_l == NULL)
        return;
    if (_l->file_contents != NULL)
        free(_l->file_contents);
    if (file_names != NULL)
    {
        for (size_t i = 0; i <= filecount; i++)
        {
            free(file_names[i]);
        }
        free(file_names);
    }
    if (_l->tokens != NULL)
        free(_l->tokens);
    free(_l);
}

static bool is_file_valid(lexer *_l, size_t pos)
{
    size_t len = strlen(file_names[pos]);
    if (len < 3)
        return false;
    if (strcmp(file_names[pos] + len - 3, ".th") == 0)
        return true;
    return false;
}

static bool allocate_for_another_file(lexer *_l, size_t len)
{
    _l->file_contents = (char *)malloc(len);
    if (_l->file_contents == NULL)
        return false;
    return true;
}

bool add_token(lexer *_l, token tok)
{
    // If we fail, what to do?
    // Add an INTERNAL ERROR message and then return false to stop further lexing
    token *tmp = _l->tokens;
    _l->tokens = (token *)realloc(_l->tokens, sizeof(token) * _l->tok_count);
    if (_l->tokens == NULL)
    {
        _l->tokens = tmp;
        return false;
    }
    _l->tokens[_l->tok_count] = tok;
    _l->tok_count++;
    return true;
}

bool lex_all(lexer *_l)
{
    // perform initial setup
    _l->tok_count = 0;
    if (is_file_valid(_l, 0) != true)
    {
        add_error(empty_loc(), "The given input file is not a valid input file.", _INVALID_FILE);
        return false;
    }
    FILE *f = fopen(file_names[0], "r");
    if (f == NULL)
    {
        if (errno == EISDIR)
            add_error(empty_loc(), "The given input file is not a input file but a directory instead", _FILE_A_DIR);
        else
            add_error(empty_loc(), "The given input file doesn't exist.", _FILE_NOT_FOUND);
        return false;
    }
    size_t len;
    fseek(f, SEEK_SET, SEEK_END);
    len = ftell(f);
    if (len == 0)
        return true;
    if (allocate_for_another_file(_l, len + 1) == false)
    {
        add_error(empty_loc(), "Unable to read the input file: It is not your fault.", _INTERNAL_ERROR);
        return false;
    }
    rewind(f);
    fread(_l->file_contents, 1, len, f);
    fclose(f);

    size_t this_index = filecount;
    filecount++; // only then can we say we have read the first file
    char *curr = _l->file_contents;
    _l->col = 0;
    _l->line = 0;
    while (!iseof(*curr))
    {
        switch (*curr)
        {
        case ' ':
        case '\t':
        {
            while (skip(*curr) && !iseof(*curr))
            {
                curr++;
                _l->col++;
            }
            break;
        }
        case '\n':
        {
            curr++;
            _l->line++;
            _l->col = 0;
            break;
        }
        case '/':
        {
            curr++;
            _l->col++;
            switch (*curr)
            {
            case '/':
            {
                // skip comments
                while (*curr != '\n' && !iseof(*curr))
                {
                    curr++;
                }
                _l->line++;
                _l->col = 0;
                break;
            }
            default:
                // if none of these then probably the divide token
                if (add_token(_l, build_tok(NULL, TOK_DIV, _l->line, _l->col, this_index)) == false)
                {
                    return false; // false is returned in the case of internal errors only
                }
                curr++;
                _l->col++;
                break;
            }
        }
        default:
            if (isalpha(*curr))
            {
            }
            else if (isnum(*curr))
            {
            }
            else
            {
                add_error(loc(_l->line, _l->col, this_index), "Cannot construct a token from this", TOK_INVALID);
                curr++;
                _l->col++;
            }
        }
    }
}