#include <iostream>
#include <ranges>
#include <locale>


std::string_view stripTailPunct(std::string_view s)
{
    for (size_t i = 0; i < s.length(); i++) {
        if (std::ispunct(s.at(i))) {
            s.remove_suffix(s.length() - i);
            break;
        }
    }
    // std::cout << "Trimmed: " << s << '\n';
    
    return s;
}


void repl()
{
    std::string input {};
    std::cout << "~ ";
    while (std::cin >> input) {
        std::cout << "spellcheck: " << input << "\n~ ";
        stripTailPunct(input);
    }
}

int main()
{
    repl();
}