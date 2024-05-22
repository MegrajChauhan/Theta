#include "../includes/lexer.hpp"

theta::lexer::Lexer::Lexer(std::string filename)
{
    file = filename;
    path = std::filesystem::current_path() / file;
    path_in_str = path.generic_string();
}

bool theta::lexer::Lexer::file_exists()
{
    return std::filesystem::exists(path);
}

bool theta::lexer::Lexer::file_not_a_dir()
{
    return !std::filesystem::is_directory(path);
}

bool theta::lexer::Lexer::is_valid_file()
{
    return file.ends_with(".th");
}

bool theta::lexer::Lexer::read_file()
{
    std::fstream f(file, std::ios::in);
    // the file opening shouldn't fail
    if (!f.is_open())
        return false; // this shouldn't happen
    f.seekg(f.beg, f.end);
    size_t len = f.tellg();
    char fconts[len];
    f.seekg(f.beg);
    f.read(fconts, len);
    file_contents = fconts;
    file_contents += '\0';
    f.close();
    return true;
}

bool theta::lexer::Lexer::init_lexer()
{
    if (!is_valid_file())
    {
        s = FILE_INVALID;
        return false;
    }
    if (!file_exists())
    {
        s = FILE_NOT_AVAI;
        return false;
    }
    if (!file_not_a_dir())
    {
        s = FILE_A_DIR;
        return false;
    }
    if (!read_file())
    {
        s = INIT_FAILURE;
        return false;
    }
    // initialize other members
    curr_char = file_contents.begin();
    end = file_contents.end();
    error::register_new_file(path);
    ind = error::get_count();
    return true;
}

void theta::lexer::Lexer::handle_imports()
{
    while (curr_char != end && *curr_char != '\n' && (*curr_char == ' ' || *curr_char == '\t'))
    {
        curr_char++;
        col++;
    }
    if (*curr_char == '\n' || curr_char == end)
    {
        error::register_new_error("Expected a path to import not this.", get_current_line(), tokens::token_loc(line, col - 1, col, ind), error::_IMPORT_PATH_NOT_SPECIFIED);
        return;
    }
    size_t st = col;
    std::string p = get_import_path();
    // first confirm the path and make sure it exists
    Lexer _l(p);
    if (!_l.lex_all())
    {
        error::register_new_error(_l.interpret_the_state(), get_current_line(), tokens::token_loc(line, st, col, ind), error::_IMPORT_PATH_INVALID);
        return;
    }
    // just append the tokens
    toks.insert(toks.begin(), _l.toks.begin(), _l.toks.end());
    error::add_to_tree(ind); // update the tree
}

std::string theta::lexer::Lexer::get_import_path()
{
    std::string p;
    while (curr_char != end && *curr_char != ' ' && *curr_char != '\t' && *curr_char != '\n')
    {
        p += *curr_char;
        curr_char++;
        col++;
    }
    return p;
}

std::string theta::lexer::Lexer::get_current_line()
{
    std::string l;
    auto tmp = curr_char;
    size_t len = col;
    while (tmp != end && *tmp != '\n')
    {
        len++;
        tmp++;
    }
    // we have our len
    l.assign(tmp - len, tmp);
    return l;
}

bool theta::lexer::Lexer::lex_all()
{
    if (error::is_file_already_registered(path_in_str))
        return true;
    if (!init_lexer())
        return false;
    // we lex everything at once
    while (curr_char != end)
    {
        switch (*curr_char)
        {
        case ' ':
        case '\t':
            while ((*curr_char == ' ' || *curr_char == '\t') && curr_char != end)
            {
                col++;
                curr_char++;
            }
            break;
        case '\n':
            curr_char++;
            col = 0;
            line++;
            break;
        default:
            if (ISALPHA(curr_char))
            {
                get_current_character_group_token();
                if (s == HANDLE_IMPORT)
                {
                    // gotta handle it
                    handle_imports(); // don't care about errors here
                    s = NORMAL;       // return to NORMAL state
                }
            }
            else
            {
                curr_char++;
                col++;
            }
        }
    }
    return true; // even if errors were encountered while lexing the file
}

std::string theta::lexer::Lexer::get_current_character_group()
{
    // This will basically gather the entire chunk inbetween spaces, newlines or tabs
    std::string sym;
    while (curr_char != end && *curr_char != ' ' && *curr_char != '\t' && *curr_char != '\n' && (ISALPHA(curr_char) || *curr_char == '_' || ISNUM(curr_char)))
    {
        sym += *curr_char;
        col++;
        curr_char++;
    }
    return sym;
}

void theta::lexer::Lexer::get_current_character_group_token()
{
    size_t curr = col;
    std::string sym = get_current_character_group();
    if (sym == "import")
    {
        // This is a whole different thing
        s = HANDLE_IMPORT;
        return;
    }
    auto res = tokens::tok_map.find(sym);
    if (res == tokens::tok_map.end())
        add_token(curr, sym, tokens::TOK_IDENTIFIER);
    else
        add_token(curr, " ", res->second);
}

void theta::lexer::Lexer::add_token(size_t col_st, std::string val, tokens::token_t type)
{
    tokens::token t;
    t.loc.end_col = col;
    t.loc.start_col = col_st;
    t.loc.file_index = ind;
    t.loc.line = line;
    t.tok_val = val;
    t.type = type;
    toks.push_back(t);
}

std::string theta::lexer::Lexer::interpret_the_state()
{
    // This will register errors based on the state
    // Not for all the states
    // This function is useful for only the initialization stage
    switch (s)
    {
    case FILE_NOT_AVAI:
        return "The given file " + path_in_str + " is not available.";
    case FILE_A_DIR:
        return "The given file " + path_in_str + " is not file but a directory.";
    case FILE_INVALID:
        return "The given file " + path_in_str + " is not valid.";
    }
}