#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <vector>
#include <string>

class InputParser
{
public:
    InputParser() = delete;

    /// @brief Simple CLI input parser.
    /// @param argc A reference to argument count (argc)
    /// @param argv A pointer to argument values (argv)
    InputParser (int& argc, char** argv);

    /// @brief Checks the vector of tokens provided against the supplied option value.
    /// @param option A CLI option like "-h" or "--file".
    /// @return True if the given option was found. Otherwise false.
    bool isOption(std::string_view option) const;

    /// @brief Getter for a provided option value.
    /// @param option A CLI option like "--file".
    /// @return A reference to a string argument like "file.txt".
    const std::string& getOption(const std::string& option) const;


private:
    std::vector<std::string> m_tokens {};
};

#endif