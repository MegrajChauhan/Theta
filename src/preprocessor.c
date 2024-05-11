#include "../includes/preprocessor.h"

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
    _p->_included_files_ = NULL;
    _p->state = _NORMAL_; // start normally for now
    _p->error_message = NULL;
    _p->col_no = 0;
    _p->line_no = 0;
    return _p; // everything was successful
}

void theta_destroy_preprocessor(TPreprocessor *prep)
{
    if (surelyF(prep == NULL))
        return;
    if (prep->error_message != NULL)
        free(prep->error_message);
    if (prep->_included_files_ != NULL)
    {
        for (size_t i = 0; i < prep->inc_count; i++)
        {
            if (surelyT(prep->_included_files_[i] != NULL))
                free(prep->_included_files_[i]);
        }
        free(prep->_included_files_);
    }
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
        return false; // invalid file
    // attempt to open the file
    if ((p->reader.file_ptr = fopen(p->reader.curr_file, "r")) == NULL)
    {
        size_t err;
        if (errno == EISDIR)
        {
            p->state = IS_A_DIRECTORY;
            return false;
        }
        p->state = FILE_DOESNT_EXIST;
        return false;
    }
    if (theta_read_entire_file(p) != true)
        return false;
    // now process the file
    char *it = p->reader.file_contents;
    while (*it != '\0')
    {
        
    }
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
    static const char *check_with = ".th";
    if (strcmp(it, check_with) == 0)
        return true;
    return false; // not a valid file
}

bool_t theta_read_entire_file(TPreprocessor *prep)
{
    // dest will be unintialized
    size_t file_len;
    fseek(prep->reader.file_ptr, SEEK_END, SEEK_SET);
    file_len = ftell(prep->reader.file_ptr); // get the length
    if (file_len == 0)
    {
        prep->state = EMPTY_FILE;
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
    return true;
}
