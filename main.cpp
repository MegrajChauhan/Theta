#include <iostream>
#include "includes/error.hpp"
#include "includes/lexer.hpp"

int main()
{
    theta::lexer::Lexer _l("main.th");
    auto p = theta::error::get_inc_tree();
    if (!_l.lex_all())
    {
        std::cout << _l.interpret_the_state() << std::endl;
        return 0;
    }
    // auto x = theta::error::get_count();
    // auto y = theta::error::get_errors();
    // auto z = theta::error::get_files();
    // std::cout << "The count is " << x << '\n';
    // for (auto a : y)
    // {
    //     std::cout << a.msg << '\n';
    //     std::cout << a.type << '\n';
    //     std::cout << a.loc.start_col << " " << a.loc.end_col << " " << a.loc.line << " " << a.loc.file_index << z[a.loc.file_index] << '\n';
    //     std::cout << "\n";
    // }
    // std::cout << "\n";
    // for (auto f : z)
    // {
    //     std::cout << f << "\n";
    // }
    // std::cout << "\n";
    // for (auto t : _l.get_tokens())
    // {
    //     std::cout << t.loc.start_col << " " << t.loc.end_col << " " << t.loc.line << " " << t.loc.file_index << z[t.loc.file_index] << '\n';
    //     std::cout << t.tok_val << '\n';
    //     std::cout << t.type << '\n';
    // }
    p = theta::error::get_inc_tree();
    theta::error::_clear_all_errors_();
}