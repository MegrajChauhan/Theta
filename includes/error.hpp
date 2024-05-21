#ifndef _ERROR_
#define _ERROR_

#include "tokens.hpp"
#include <vector>
#include <algorithm>

namespace theta
{
    namespace error
    {
        enum error_t
        {
            _FILE_INVALID,
            _FILE_A_DIR,
            _FILE_NOT_AVAI,
            _IMPORT_PATH_NOT_SPECIFIED,
            _IMPORT_PATH_INVALID,
        };

        struct Error
        {
            tokens::token_loc loc;
            std::string msg;
            error_t type;
        };

        static std::vector<Error> errors;
        static std::vector<std::string> files;
        static size_t count = -1;

        void register_new_file(std::string);

        void register_new_error(std::string, tokens::token_loc, error_t);

        bool is_file_already_registered(std::string);

        std::vector<Error> get_errors();
        std::vector<std::string> get_files();
        size_t get_count();

    };
};

#endif