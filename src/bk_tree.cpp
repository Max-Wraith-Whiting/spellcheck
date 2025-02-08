#include "bk_tree.hpp"

BKTree::BKTree(std::istream &stream)
{
    stream >> m_value;

    std::string s{};
    while (stream >> s)
        this->addChild(s); // "this->" is purely to be explicit.
}

BKTree::~BKTree()
{
    for (BKTree *child : m_children)
        delete child;
}

void BKTree::addChild(std::string value)
{
    int distance{levDistance(m_value, value)};

    for (BKTree *child : m_children) {
        if (distance == child->m_lDistance)
        {
            child->addChild(value);
            return;
        }
    }

    BKTree *newChild = new BKTree(value);
    newChild->m_lDistance = levDistance(m_value, newChild->m_value);
    m_children.emplace_back(newChild);
}

std::vector<std::string_view> BKTree::query(std::string_view query, const int accuracy)
{
    std::vector<std::string_view> output{};
    output.reserve(16);

    std::queue<BKTree*> q {{this}};
    static int counter {0};
    
    while (!q.empty()) {
        // Pull a node from the queue.
        BKTree* node {q.front()};
        q.pop();
        counter++;

        // If it is a known word, return just that.
        if (node->m_value == query) {
            output.clear();
            output.push_back(node->m_value);
            return output;
        }

        // Get the distance between the query and the m_value.
        int distance {levDistance(node->m_value, query)};
        int upperBound {distance + accuracy};
        int lowerBound {distance - accuracy};

        // If the current word is within range, add it to the output vector.
        if (distance <= accuracy)
            output.push_back(node->m_value);

        // If the node has children that are within the range of accuracy.
        for (BKTree* child : node->m_children)
            if (child->m_lDistance <= upperBound && child->m_lDistance >= lowerBound)
                q.push(child);
    }
    std::cout << counter << " nodes checked to find " << query << '\n';
    counter = 0;
    return output;
}

void BKTree::print()
{
    std::queue<BKTree *> q{};
    q.push(this);
    while (!q.empty()) {
        BKTree *current{q.front()};
        q.pop();
        std::cout << current->m_value << '\n';
        for (BKTree *child : current->m_children)
            q.push(child);
    }
}

void BKTree::populateFromStream(std::istream &stream)
{
    std::string s{};
    while (stream >> s) {
        this->addChild(s);
    }
}

int BKTree::levDistance(const std::string_view a, const std::string_view b)
{
    // Not a super efficent implementation from a C++ standpoint.
    const size_t m{a.length() + 1};
    const size_t n{b.length() + 1};

    std::vector<int> v0(n);
    std::vector<int> v1(n, 0);

    for (size_t i{0}; i < n; i++)
        v0.at(i) = static_cast<int>(i);

    for (size_t i{0}; i < m - 1; i++) {
        v1.at(0) = static_cast<int>(i) + 1;

        for (size_t j{0}; j < n - 1; j++) {

            int deleteCost{v0.at(j + 1) + 1};
            int insertCost{v1.at(j) + 1};
            int subCost{a.at(i) == b.at(j) ? v0.at(j) : v0.at(j) + 1};

            v1.at(j + 1) = std::min({deleteCost, insertCost, subCost});
        }

        v0.swap(v1);
    }

    return v0.at(n - 1);
}