#include "input_parser.hpp"

InputParser::InputParser (int& argc, char** argv)
{
    for (int i {1}; i < argc; ++i) {
        m_tokens.push_back(std::string(argv[i]));
    }
}

const std::string& InputParser::getOption(const std::string& option) const
{
    std::vector<std::string>::const_iterator iter {
        std::find(m_tokens.begin(), m_tokens.end(), option)};
    
    if (iter != m_tokens.end() && ++iter != m_tokens.end()) {
        return *iter;
    } else {
        static const std::string empty("");
        return empty;
    }
}

bool InputParser::isOption(std::string_view option) const
{
    return std::find(m_tokens.begin(), m_tokens.end(), option) != m_tokens.end();
}