#include "../includes/error.hpp"

void theta::error::register_new_file(std::string filename)
{
    files.push_back(filename);
    count++;
}

void theta::error::register_new_error(std::string msg, tokens::token_loc loc, error_t type)
{
    Error _e;
    _e.loc = loc;
    _e.msg = msg;
    _e.type = type;
    errors.push_back(_e);
}

bool theta::error::is_file_already_registered(std::string file)
{
    return (std::find(files.begin(), files.end(), file) != files.end());
}