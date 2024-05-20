#ifndef _TOKENS_
#define _TOKENS_

#include <cstddef>
#include <string>
#include <unordered_map>

namespace theta
{
    namespace tokens
    {
        enum token_t
        {
            TOK_ERR,
            TOK_IMPORT,
            TOK_IDENTIFIER,
        };

        struct token_loc
        {
            size_t line;
            size_t start_col;
            size_t end_col;
            size_t file_index;

            token_loc() = default;

            token_loc(size_t l, size_t st, size_t ed, size_t ind)
            {
                line = l;
                start_col = st;
                end_col = ed;
                file_index = ind;
            }
        };

        struct token
        {
            token_loc loc;
            std::string tok_val;
            token_t type;
        };

        static std::unordered_map<std::string, token_t>
            tok_map =
                {
                    {"import", TOK_IMPORT},

        };

    };
};

#endif