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

void theta::error::add_to_tree(size_t includer, size_t included)
{
    include_tree[included] = includer;
}

std::unordered_map<size_t, size_t> theta::error::get_inc_tree()
{
    return include_tree;
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
        case _FILE_A_DIR:
        case _FILE_NOT_AVAI:
        case _IMPORT_PATH_NOT_SPECIFIED:
        case _IMPORT_PATH_INVALID:
        {
            auto filetree = error::gen_error_info(err.loc.file_index);
            print_error(err.loc.line, err.loc.start_col, err.loc.end_col, files[err.loc.file_index], err.msg, filetree, err.err_line);
            fatal = true;
            break;
        }
        }
    }
    if (fatal)
        exit(EXIT_FAILURE);
}

void theta::error::print_error(size_t line, size_t colst, size_t colend, std::string file, std::string errmsg, std::stack<size_t> filetree, std::string errline)
{
    std::cout << file << ":" << line + 1 << ":" << colst << ": ";
    if (filetree.size() > 0)
    {
        std::cout << "This file, Included from:\n";
        for (size_t i = 0; i < filetree.size(); i++)
        {
            std::cout << "\tFrom: \"" << files[filetree.top()] << "\"\n";
            filetree.pop();
        }
    }
    std::cout << errmsg << "\n";
    std::cout << "-->\t" << errline << "\n   \t";
    for (size_t i = 0; i < colst; i++)
    {
        putchar(' ');
    }
    for (size_t i = colst; i < colend; i++)
    {
        putchar('^');
    }
    putchar(10);
}