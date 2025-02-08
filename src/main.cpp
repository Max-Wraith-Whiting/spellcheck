#include <iostream>
#include <ranges>
#include <vector>
#include <queue>
#include <fstream>
#include <iomanip>
#include <exception>

#include "input_parser.hpp"
#include "bk_tree.hpp"

namespace Config {
    std::string g_dictLocation {"./dictionary/dict.txt"};
};

// DEBUG
void printVector(std::vector<std::string_view> v)
{
    std::cout << "< ";
    if (v.size() == 0) {
        std::cout << " >\n";
        return;
    } else if (v.size() == 1) {
        std::cout << v.at(0) << " >\n";
        return;
    } 
        
    for (std::string_view s : v) {
        std::cout << s << ", ";
    }

    // int size {static_cast<int>(v.size())};

    for (size_t i {0}; i < v.size(); ++i) {
        std::cout << v.at(i);
        if (i != (v.size() - 1))
            std::cout << ", ";
    }
    std::cout << " >\n";
}

// Possibly remove this as it is unecessary.
std::string_view stripTailPunct(std::string_view s)
{
    for (size_t i = 0; i < s.length(); i++)
    {
        if (std::ispunct(s.at(i)))
        {
            s.remove_suffix(s.length() - i);
            break;
        }
    }

    return s;
}

void repl(int accuracy=1)
{
    std::ifstream file("./docs/british-english.txt");

    if (!file.is_open())
        std::cout << "File failed to open!";

    // SpellCheck::BKTree spellchecker(file);
    BKTree spellchecker(file);
    std::string input{};
    std::cout << "~ ";
    while (std::cin >> input) {
        std::cout << "spellcheck: " << input << "\n~ ";
        std::vector<std::string_view> results{spellchecker.query(input, accuracy)};
        printVector(results);
    }
}

void printHelp()
{
    std::cout << "Usage: spellcheck [OPTION]\n\n"
            << std::setw(20) 
            << "-h,  --help       print this help.\n"
            << "-f,  --file       specify a file to spellcheck.\n"
            << "-a,  --accuracy   specify an accuracy for the spellchecker.\n"
            << "-r,  --repl       launch a spelling REPL.\n";
}

void handleFile(const InputParser& input, const std::string& option, const int accuracy=1)
{
    const std::string fileName{input.getOption(option)};
    if (fileName.empty())
        throw std::runtime_error("No file name provided!\n");

    std::ifstream stream(fileName);
    if (!stream.is_open())
        throw std::runtime_error("Cannot find file: " + fileName + "!\n");

    
    std::ifstream dict(Config::g_dictLocation);
    if (!dict.is_open())
        throw std::runtime_error("Cannot open dictionary! Validate spellchecker/dictionary/dict.txt\n");

    BKTree spellchecker(dict);
    std::string s {};
    while (stream >> s) {
       std::cout << s << ": ";
       printVector(spellchecker.query(s, accuracy));
    }
    
}

int main(int argc, char *argv[])
{
    InputParser input(argc, argv);

    if (input.isOption("-h") || input.isOption("--help")) {
        printHelp();
        return 0;
    }

    int accuracy {1};
    
    if (input.isOption("-f")) {
        const std::string fileName{input.getOption("-f")};
        std::cout << "File: " << fileName << '\n';
        try {
            handleFile(input, "-f", accuracy);
        } catch(const std::exception& e) {
            std::cout << "Error: " << e.what();
        }
        return 0;

    } else if (input.isOption("--file")) {
        const std::string fileName{input.getOption("--file")};
        std::cout << "File:" << fileName << '\n';
        try {
            handleFile(input, "--file", accuracy);
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what();
        }
        return 0;

    } else if (input.isOption("-r") || input.isOption("--repl")) {
        std::cout << "REPL\n";
        repl(accuracy);
        return 0;

    } else {
        std::cout << "SPELLCHECK ONE WORD\n";
    }
}