#include "../includes/error.hpp"

void theta::error::register_new_file(std::string filename)
{
    files.push_back(filename);
    count++;
}

void theta::error::register_new_error(std::string msg, std::string line, tokens::token_loc loc, error_t type)
{
    Error _e;
    _e.loc = loc;
    _e.msg = msg;
    _e.type = type;
    _e.err_line = line;
    errors.push_back(_e);
}

bool theta::error::is_file_already_registered(std::string file)
{
    return (std::find(files.begin(), files.end(), file) != files.end());
}

std::vector<std::string> theta::error::get_files()
{
    return error::files;
}

std::vector<theta::error::Error> theta::error::get_errors()
{
    return error::errors;
}

size_t theta::error::get_count()
{
    return error::count;
}

void theta::error::add_to_tree(size_t includer)
{
    include_tree[count] = includer;
}

std::stack<size_t> theta::error::gen_error_info(size_t ind)
{
    std::stack<size_t> final_error;
    auto dets = include_tree.find(ind);
    if (dets != include_tree.end())
    {
        // this file was included by something
        // go the very first file that included it
        final_error.push(dets->second);
        while ((dets = include_tree.find(dets->second)) != include_tree.end())
        {
            final_error.push(dets->second);
        }
    }
    return final_error;
}

void theta::error::_clear_all_errors_()
{
    bool fatal = false;
    for (auto err : errors)
    {
        switch (err.type)
        {
        case _FILE_INVALID:
        {
            auto filetree = error::gen_error_info(err.loc.file_index);
            
        }
        }
    }
}

static void print_error(size_t line, size_t colst, size_t colend, std::string file, std::string errmsg, std::stack<std::string> filetree, std::string errline)
{
    std::cout << file<< ":"<<line<<":"<<colst<<": ";
    if (filetree.size() > 0)
    {
        // std::cout << ""
    }
}