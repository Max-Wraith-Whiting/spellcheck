#include <iostream>
#include <ranges>
#include <vector>
#include <queue>
#include <fstream>

#include "input_parser.hpp"

namespace SpellCheck {

    /// @brief Calculates the Levenshtein distance between two strings.
    /// Does not care about whether a or b is longer.
    /// @param a First string to compare.
    /// @param b Second string to compare.
    /// @return Integer value for the Levenshtein distance.
    int levDistance(const std::string_view a, const std::string_view b)
    {
        const size_t m {a.length() + 1};
        const size_t n {b.length() + 1};

        std::vector<int> v0(n);
        std::vector<int> v1(n, 0);

        for (size_t i {0}; i < n; i++)
            v0.at(i) = static_cast<int>(i);

        for (size_t i {0}; i < m - 1; i++) {
            v1.at(0) = static_cast<int>(i) + 1;

            for (size_t j {0}; j < n - 1; j++) {

                int deleteCost {v0.at(j + 1) + 1};
                int insertCost {v1.at(j) + 1};
                int subCost {a.at(i) == b.at(j) ? v0.at(j) : v0.at(j) + 1};

                v1.at(j + 1) = std::min({deleteCost, insertCost, subCost});
            }

            v0.swap(v1);
        }

        return v0.at(n - 1);
    }

    class BKTree {
    private:
        int lDistance {0};
        std::string m_value {};
        std::vector<BKTree*> m_children {};

        void queryRecursive(std::vector<std::string_view>& output, std::string_view query, const int accuracy)
        {
            const int distance {levDistance(m_value, query)};
            const int upperBound {distance + accuracy};
            const int lowerBound {distance - accuracy};

            // If an exact match is found, return only that.
            if (distance == 0) {
                output.clear();
                output.push_back(m_value);
                return;
            }

            // If a word within accuracy distance is found, append it to the output.
            if (distance <= accuracy)
                output.push_back(m_value);

            // Query all the children of the current node that are within accuracy distance.
            for (BKTree* child : m_children) {
                if (child->lDistance <= upperBound && child->lDistance >= lowerBound)
                    child->queryRecursive(output, query, accuracy);
            }
        }

    public:

        BKTree() = delete;
        BKTree(std::string value) :
            m_value {value}
        {
        }

        // Use a stream to initialise and populate the BK-Tree.
        BKTree(std::istream& stream)
        {
            stream >> m_value;

            std::string s {};
            while (stream >> s)
                this->addChild(s);
        }

        ~BKTree() {
            for (BKTree* child : m_children)
                delete child;
        }

        /// @brief Adds a child node to the BKTree.
        /// @param value A string value copy. (No need to worry about references or lifetimes etc.)
        void addChild(std::string value) 
        {
            int distance {levDistance(m_value, value)};

            for (BKTree* child : m_children) {
                if (distance == child->lDistance) {
                    child->addChild(value);
                    return;
                }
            }

            BKTree* newChild = new BKTree(value);
            newChild->lDistance = levDistance(m_value, newChild->m_value);
            m_children.emplace_back(newChild);
        }

        std::vector<std::string_view> query(std::string_view query, const int accuracy)
        {
            std::vector<std::string_view> output {};
            output.reserve(16);

            queryRecursive(output, query, accuracy);

            return output;
        }

        void print()
        {
            std::queue<BKTree*> q {};
            q.push(this);
            while (!q.empty()) {
                BKTree* current {q.front()};
                q.pop();
                std::cout << current->m_value << '\n';
                for (BKTree* child : current->m_children)
                    q.push(child);
            }
        }
    
        /// @brief Adds the contents of a stream by standard string extraction from a given stream. (I.e., stream >> s)
        /// @param stream A standard `std::istream` reference. 
        void populateFromStream(std::istream& stream)
        {
            std::string s {};
            while (stream >> s) {
                this->addChild(s);
            }
        }

    };
}

// DEBUG
void printVector(std::vector<std::string_view> v)
{
    std::cout << "< ";
    for (std::string_view s : v) {
        std::cout << s << ", ";
    }
    std::cout << ">\n";
}

// Possibly remove this as it is unecessary.
std::string_view stripTailPunct(std::string_view s)
{
    for (size_t i = 0; i < s.length(); i++) {
        if (std::ispunct(s.at(i))) {
            s.remove_suffix(s.length() - i);
            break;
        }
    }
    
    return s;
}

void repl()
{
    std::ifstream file("./docs/british-english.txt");

    if (!file.is_open())
        std::cout << "File failed to open!";

    SpellCheck::BKTree spellchecker(file);
    std::string input {};
    std::cout << "~ ";
    while (std::cin >> input) {
        std::cout << "spellcheck: " << input << "\n~ ";
        std::vector<std::string_view> results {spellchecker.query(input, 1)};
        printVector(results);
    }
}

int main(int argc, char* argv[])
{
    InputParser input(argc, argv);

    if (input.isOption("-f")) {
        const std::string fileName {input.getOption("-f")};
        std::cout << "The file name is: " << fileName << "!\n";
        return 0;
    }

    if (input.isOption("-h")) {
        std::cout << "HELP WAS INVOKED\n";
        return 0;
    }

    // repl();
    std::cout << "Standard run!\n";
}