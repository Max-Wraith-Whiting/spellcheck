#include <iostream>
#include <ranges>
#include <vector>
#include <numeric>
#include <algorithm>


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

int main()
{
    // repl();
    int d {levDistance("sittmg", "setting")};
    std::cout << "diff = " << d << '\n';

}