#ifndef BK_TREE_H 
#define BK_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>

class BKTree {
public:
    BKTree() = delete;
    BKTree(std::string value) : m_value{value}{};
    BKTree(std::istream &stream); // Use a stream to initialise and populate the BK-Tree.
    ~BKTree();

    /// @brief Adds a child node to the BKTree.
    /// @param value A string value copy. (No need to worry about references or lifetimes etc.)
    void addChild(std::string value);

    /// @brief Queries the dataset and 
    /// @param query A given word to query against the dataset to find similar words.
    /// @param accuracy The acceptable difference (Levenshtein) between the query and the returned words. 
    /// @return Vector of strings within the supplied accuracy.
    std::vector<std::string_view> query(std::string_view query, const int accuracy);

    void print();

    /// @brief Adds the contents of a stream by standard string extraction from a given stream. (I.e., stream >> s)
    /// @param stream A standard `std::istream` reference.
    void populateFromStream(std::istream &stream);

private:
    int lDistance{0};
    std::string m_value{};
    std::vector<BKTree *> m_children{};

    void queryRecursive(std::vector<std::string_view> &output, std::string_view query, const int accuracy);

    /// @brief Calculates the Levenshtein distance between two strings.
    /// Does not care about whether a or b is longer.
    /// @param a First string to compare.
    /// @param b Second string to compare.
    /// @return Integer value for the Levenshtein distance.
    int levDistance(const std::string_view a, const std::string_view b);
};

#endif 