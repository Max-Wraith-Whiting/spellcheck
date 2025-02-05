#include "input_parser.hpp"

InputParser::InputParser (int& argc, char** argv)
{
    // this->m_tokens {std::vector<std::string>()}}
    for (int i {1}; i < argc; ++i) {
        this->m_tokens.push_back(std::string(argv[i]));
    }
}

const std::string& InputParser::getOption(const std::string& option) const
{
    std::vector<std::string>::const_iterator iter {
        std::find(this->m_tokens.begin(), this->m_tokens.end(), option)};
    
    if (iter != this->m_tokens.end() && ++iter != this->m_tokens.end()) {
        return *iter;
    } else {
        static const std::string empty("");
        return empty;
    }
}

bool InputParser::isOption(std::string_view option) const
{
    return std::find(this->m_tokens.begin(), this->m_tokens.end(), option) != this->m_tokens.end();
}