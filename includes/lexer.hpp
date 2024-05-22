#ifndef _LEXER_
#define _LEXER_

#include <vector>
#include <filesystem>
#include <fstream>
#include "error.hpp"
#include "tokens.hpp"

#define ISALPHA(x) (*x >= 'a' && *x <= 'z') || (*x >= 'A' && *x <= 'Z')
#define ISNUM(x) (*x >= '0' && *x <= '9')

// Not having those data structures and standard library like C++s' made me wanna switch to C++ for this project for finishing it quickly

namespace theta
{
    namespace lexer
    {
        // for propagating state
        enum state
        {
            NORMAL,
            FILE_NOT_AVAI,
            FILE_A_DIR,
            FILE_INVALID,
            LEX_FAILURE,
            INIT_FAILURE,
            HANDLE_IMPORT, // a temporary state
        };

        class Lexer
        {
            std::vector<tokens::token> toks;
            std::string file_contents;
            std::string::iterator curr_char, end;
            std::string file;
            std::filesystem::path path;
            std::string path_in_str;
            size_t ind = 0;
            size_t line = 0, col = 0;
            state s = NORMAL;

        public:
            Lexer(std::string);

            bool init_lexer();

            bool is_valid_file();

            bool file_exists();

            bool file_not_a_dir();

            bool read_file();

            bool lex_all();

            void get_current_character_group_token();

            std::string get_import_path();

            void handle_imports();

            std::string get_current_character_group();

            void add_token(size_t, std::string, tokens::token_t);

            std::string interpret_the_state();

            auto get_tokens() { return toks; }

            std::string get_current_line();
        };
    };
};

#endif