#include <iostream>
#include <ranges>
#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>
#include <fstream>

namespace BK_Tree {

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


    
    class Node {
    private:
        int lDistance {0};
        std::string m_value {};
        std::vector<Node*> m_children {};

        void innerQuery(const std::string_view query, const int accuracy, std::vector<std::string_view>& output)
        {
            int d {levDistance(m_value, query)};

            const int upperBound {d + accuracy};
            const int lowerBound {d - accuracy};

            for (Node* child : m_children) {

                if (child->lDistance > lowerBound && child->lDistance < upperBound) {
                    output.emplace_back(child->m_value);
                    innerQuery(query, accuracy, output);
                }
            }
        }

    public:

        Node() = delete;
        Node(std::string value) :
            m_value {value}
        {
        }

        ~Node() {
            for (Node* child : m_children)
                delete child;
        }

        void addChild(std::string value) 
        {
            int distance {levDistance(m_value, value)};

            for (Node* child : m_children) {
                if (distance == child->lDistance) {
                    child->addChild(value);
                    return;
                }
            }

            Node* newChild = new Node(value);
            newChild->lDistance = levDistance(m_value, newChild->m_value);
            m_children.emplace_back(newChild);
        }

        std::vector<std::string_view> query(std::string_view query, int accuracy)
        {
            std::vector<std::string_view> output {};
            output.reserve(16);
            innerQuery(query, accuracy, output);

            return output;
        }

        void print()
        {
            std::queue<Node*> q {};
            q.push(this);
            while (!q.empty()) {
                Node* current {q.front()};
                q.pop();
                std::cout << current->m_value << '\n';
                for (Node* child : current->m_children)
                    q.push(child);
            }
        }
    
        void populate(const std::string& fileName)
        {
            std::ifstream infile(fileName);
            std::string s {};
            while (infile >> s) {
                this->addChild(s);
            }
        }

    };
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
    std::string input {};
    std::cout << "~ ";
    while (std::cin >> input) {
        std::cout << "spellcheck: " << input << "\n~ ";
        stripTailPunct(input);
    }
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

int main()
{
    BK_Tree::Node root {"book"};
    root.addChild("rook");
    root.addChild("nooks");
    root.addChild("boon");
    root.print();

    printVector(root.query("took", 0));

}