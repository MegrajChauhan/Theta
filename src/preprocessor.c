#include "../includes/preprocessor.h"

INTERNAL_FUNC bool_t theta_add_error_message(TPreprocessor *p, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int len = vasprintf(&err_msg, fmt, args);
    if (len < 0)
    {
        p->state = _INTERNAL_ERROR_;
        return false; // treat as internal error even though it is not
    }
    va_end(args);
    return true;
}

INTERNAL_FUNC bool_t theta_add_extra_info(TPreprocessor *p, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char *temp;
    int len = vasprintf(&temp, fmt, args);
    va_end(args);
    if (len < 0)
    {
        p->state = _INTERNAL_ERROR_;
        return false; // treat as internal error even though it is not
    }
    free(err_extra_info);
    err_extra_info = temp;
    return true;
}

INTERNAL_FUNC bool_t theta_add_err_file(TPreprocessor *p)
{
    size_t len = strlen(p->reader.curr_file);
    err_file = (char *)malloc(sizeof(char) * len);
    if (err_file == NULL)
    {
        p->state = _INTERNAL_ERROR_;
        return false;
    }
    strcpy(err_file, p->reader.curr_file);
    return true;
}

INTERNAL_FUNC bool_t theta_register_error_line(TPreprocessor *p, char *line_start)
{
    size_t i = 0;
    char *temp = line_start;
    while (*line_start != '\n' && not_eof(line_start))
    {
        i++;
        line_start++;
    }
    err_line = (char *)malloc((sizeof(char) * i) + 1);
    if (err_line == NULL)
    {
        p->state = _INTERNAL_ERROR_;
        return false;
    }
    strncpy(err_line, temp, i);
    return true;
}

INTERNAL_FUNC bool_t theta_is_file_already_included(char *filename)
{
    for (size_t i = 0; i < inc_count; i++)
    {
        if (strcmp(_included_files_[i], filename) == 0)
            return true;
    }
    return false;
}

INTERNAL_FUNC bool_t theta_add_new_included_file(char *filename)
{
    _included_files_ = (char **)realloc(_included_files_, sizeof(char *) * inc_count);
    if (_included_files_ == NULL)
        return false;
    _included_files_[inc_count] = (char *)malloc(sizeof(char) * strlen(filename));
    if (_included_files_[inc_count] == NULL)
        return false;
    strcpy(_included_files_[inc_count], filename);
    inc_count++;
    return true;
}

// Init the preprocessor and the reader
TPreprocessor *theta_init_preprocessor(char *file_path)
{
    size_t len = strlen(file_path);
    if (surelyF(file_path == NULL || len == 0))
        return NULL;
    TPreprocessor *_p = (TPreprocessor *)malloc(sizeof(TPreprocessor));
    if (_p == NULL)
        return NULL;
    _p->reader.curr_file = (char *)malloc((sizeof(char) * len) + 1);
    if (_p->reader.curr_file == NULL)
    {
        free(_p);
        return NULL;
    }
    strcpy(_p->reader.curr_file, file_path); // shouldn't fail
    _p->state = _NORMAL_;                    // start normally for now
    _p->col_no = 0;
    _p->line_no = 0;
    _p->reader.file_contents = NULL;
    return _p; // everything was successful
}

void theta_destroy_preprocessor(TPreprocessor *prep)
{
    if (surelyF(prep == NULL))
        return;
    // The file_ptr would be freed before this point
    if (prep->reader.file_contents != NULL)
        free(prep->reader.file_contents);
    if (prep->reader.curr_file != NULL)
        free(prep->reader.curr_file);
    free(prep);
}

bool_t theta_process_file(TPreprocessor *p)
{
    // Fisrt we need to make some checks and validate things
    if (theta_is_file_valid(p->reader.curr_file) == false)
    {
        theta_add_err_file(p);
        return false; // invalid file
    }
    // attempt to open the file
    if ((p->reader.file_ptr = fopen(p->reader.curr_file, "r")) == NULL)
    {
        if (errno == EISDIR)
        {
            p->state = IS_A_DIRECTORY;
            theta_add_err_file(p);
            return false;
        }
        p->state = FILE_DOESNT_EXIST;
        theta_add_err_file(p);
        return false;
    }
    if (theta_read_entire_file(p) != true)
        return false;
    // now process the file
    char *it = p->reader.file_contents;
    while (not_eof(it))
    {
        if (*it == ' ' || *it == '\t')
        {
            // ignore all these
            while (*it == ' ' || *it == '\t')
            {
                it++;
                p->col_no++;
            }
        }
        if (*it == '\n')
        {
            // a new line ignore
            p->line_no++;
            p->col_no = 0;
            it++;
        }
        else if (*it == '/' && *(it + 1) == '/')
        {
            // a comment, ignore all
            while (*it != '\n' && not_eof(it))
            {
                it++;
            }
            // handled newline in next iteration
        }
        else if (is_alpha(it))
        {
            char sym[7] = {0}; // temp stack variable
            size_t i = 0;
            while (is_alpha(it) && i < 6)
            {
                sym[i] = *it;
                it++;
                p->col_no++;
                i++;
            }
            if (*(it) == ' ')
            {
                // This is a possible import statement
                if (strcmp(sym, "import") == 0)
                {
                    // an import statement
                    if (theta_handle_imports(p, &it) == false)
                        return false;
                }
                else
                {
                    while ((*it != ' ' && *it != '\n' && *it != '\t') && not_eof(it))
                    {
                        it++;
                        p->col_no++;
                    }
                }
            }
            else
            {
                // this is another symbol, maybe a variable?
                while ((*it != ' ' && *it != '\n' && *it != '\t') && not_eof(it))
                {
                    it++;
                    p->col_no++;
                }
            }
        }
        else
        {
            it++;
            p->col_no++;
        }
    }
    return true;
}

// check the extension
bool_t theta_is_file_valid(char *file_path)
{
    // don't check if valid since checked when initializing
    size_t len = strlen(file_path);
    if (len < 3)
        return false;
    // must at least be .th
    char *it = (file_path + len) - 3;
    LOCAL const char *check_with = ".th";
    if (strcmp(it, check_with) == 0)
        return true;
    return false; // not a valid file
}

bool_t theta_read_entire_file(TPreprocessor *prep)
{
    // dest will be unintialized
    size_t file_len;
    fseek(prep->reader.file_ptr, SEEK_SET, SEEK_END);
    file_len = ftell(prep->reader.file_ptr); // get the length
    if (file_len == 0)
    {
        prep->state = EMPTY_FILE;
        theta_add_err_file(prep);
        return false; // [Terminate further execution since there is nothing to do]
    }
    rewind(prep->reader.file_ptr);
    prep->reader.file_contents = (char *)malloc((sizeof(char) * file_len) + 1);
    if (prep->reader.file_contents == NULL)
    {
        prep->state = _INTERNAL_ERROR_;
        return false;
    }
    fread(prep->reader.file_contents, sizeof(char), file_len, prep->reader.file_ptr); // should work[Again, a big mistake]
    prep->reader.file_contents[file_len] = '\0';
    prep->reader.len = file_len;
    fclose(prep->reader.file_ptr);
    return true;
}

bool_t theta_handle_imports(TPreprocessor *prep, char **it)
{
    // 'it' points to right after import
    char path[256] = {0}; // the path cannot be longer than this
    char *temp = *it - 6;
    while (*(*it) == ' ' || *(*it) == '\t')
    {
        (*it)++;
        prep->col_no++;
    }
    if (*(*it) == '\n')
    {
        // we needed a path to start here, not a newline
        prep->state = EXPECT_PATH_GOT_NL;
        theta_add_error_message(prep, "Expected a path to include got a '\n' instead."); // we aren't handling the return here cause we don't need to
        theta_register_error_line(prep, temp);
        theta_add_err_file(prep);
        err_line_no = prep->line_no;
        err_col = prep->col_no;
        return false;
    }
    size_t i = 0;
    while (*(*it) != '\n' && *(*it) != '\t' && *(*it) != ' ' && not_eof(*it) && i < 255)
    {
        path[i] = *(*it);
        (*it)++;
        prep->col_no++;
        i++;
    }
    // we have our path
    /*
       This may sound like the worst idea but we will ignore the rest of the line
       so if the program contains something like thousand characters, we will ignore everything else
       This limit may be removed or increased in future.
       Since, if everything goes correctly, we replace the entire line with a hint for the lexer.
       So we "forgive" the user by not considering the remaining given path.
    */
    size_t x = 0;
    while (*(*it) != '\n' && not_eof(*it))
    {
        (*it)++;
        x++;
        prep->col_no++;
    }
    if (theta_is_file_already_included(path) == false)
    {
        if (theta_add_new_included_file(path) == false)
        {
            prep->state = _INTERNAL_ERROR_;
            return false;
        }
        TPreprocessor *_p = theta_init_preprocessor(path);
        if (_p == NULL)
        {
            prep->state = _INTERNAL_ERROR_;
            return false;
        }
        if (theta_process_file(_p) == false)
        {
            // error message is already added. So is err_line and err_col
            prep->state = _p->state; // propagate the state though
            theta_add_extra_info(prep, "%s\nfrom \'%s\'", err_extra_info, prep->reader.curr_file);
            theta_destroy_preprocessor(_p);
            return false;
        }
        // everything went smoothly
        // we have to append the file contents of _p into the
        char *_t = prep->reader.file_contents; // this could be considered unsafe as we are reallocating file_contents in the next line
        size_t new_len = prep->reader.len + _p->reader.len - x - 4;
        prep->reader.file_contents = (char *)calloc(sizeof(char), (new_len));
        if (prep->reader.file_contents == NULL)
        {
            prep->state = _INTERNAL_ERROR_;
            theta_destroy_preprocessor(_p);
            return false;
        }
        size_t _to_copy = temp - _t;
        if (_to_copy > 0)
            strncpy(prep->reader.file_contents, _t, (temp - _t));
        // strcpy((path + 1))
        char _just_temp[i + 3];
        _just_temp[0] = '#';
        strncpy(_just_temp + 1, path, i);
        _just_temp[i + 1] = '\n';
        _just_temp[i + 2] = '\0';
        strcat(prep->reader.file_contents, _just_temp);
        strcat(prep->reader.file_contents, _p->reader.file_contents);
        strcat(prep->reader.file_contents, "\n~\0");
        *it += x;
        strcat(prep->reader.file_contents, *it);
        prep->reader.len = new_len;
        free(_t);
        *it = prep->reader.file_contents + _to_copy + i + 5 + _p->reader.len;
        theta_destroy_preprocessor(_p);
    }else{
        // get rid of the import statement
        *temp = '/';
        *(temp + 1) = '/';
    }
    return true;
}
