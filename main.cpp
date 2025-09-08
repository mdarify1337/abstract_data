#include <iostream>
#include "container.hpp"

int main() {
    ft::vector<int> v1;
    std::cout << "v1 size: " << v1.size() << "\n";

    ft::vector<int> v2(5, 42);
    std::cout << "v2: ";
    for (size_t i = 0; i < v2.size(); i++) std::cout << v2[i] << " ";
    std::cout << "\n";

    ft::vector<int> v3(v2);
    std::cout << "v3 (copy of v2): ";
    for (size_t i = 0; i < v3.size(); i++) std::cout << v3[i] << " ";
    std::cout << "\n";

    v1 = v3;
    std::cout << "v1 (after assignment from v3): ";
    for (size_t i = 0; i < v1.size(); i++) std::cout << v1[i] << " ";
    std::cout << "\n";

    return 0;
}
