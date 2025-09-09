#include <iostream>
#include <vector>
#include "vector.hpp"  // Your ft::vector

// Helper to print a vector (both std and ft)
template <typename Vec>
void print_vector(const Vec &v, const std::string &name) {
    std::cout << name << " (size=" << v.size() 
              << ", capacity=" << v.capacity() << "): ";
    for (typename Vec::size_type i = 0; i < v.size(); i++)
        std::cout << v[i] << " ";
    std::cout << "\n";
}

// Compare two vectors element by element
template <typename V1, typename V2>
void compare_vectors(const V1 &v1, const V2 &v2, const std::string &label) {
    std::cout << "=== " << label << " ===\n";
    print_vector(v1, "std::vector");
    print_vector(v2, "ft::vector");

    if (v1.size() != v2.size() || v1.capacity() != v2.capacity()) {
        std::cout << "❌ Size/Capacity mismatch!\n\n";
        return;
    }

    for (size_t i = 0; i < v1.size(); i++) {
        if (v1[i] != v2[i]) {
            std::cout << "❌ Mismatch at index " << i 
                      << ": std=" << v1[i] << ", ft=" << v2[i] << "\n\n";
            return;
        }
    }
    std::cout << "✅ Vectors match!\n\n";
}

int main() {
    std::cout << "===== VECTOR TESTS =====\n\n";

    // --- 1. Fill constructor ---
    std::vector<int> std_v1(5, 42);
    ft::vector<int>  ft_v1(5, 42);
    compare_vectors(std_v1, ft_v1, "Fill constructor");

    // --- 2. push_back ---
    std_v1.push_back(100);
    ft_v1.push_back(100);
    compare_vectors(std_v1, ft_v1, "push_back(100)");

    // --- 3. pop_back ---
    std_v1.pop_back();
    ft_v1.pop_back();
    compare_vectors(std_v1, ft_v1, "pop_back()");

    // --- 4. insert single ---
    std_v1.insert(std_v1.begin() + 2, 77);
    ft_v1.insert(ft_v1.begin() + 2, 77);
    compare_vectors(std_v1, ft_v1, "insert single (77 at index 2)");

    // --- 5. insert multiple ---
    std_v1.insert(std_v1.begin() + 1, 3, 99);
    ft_v1.insert(ft_v1.begin() + 1, 3, 99);
    compare_vectors(std_v1, ft_v1, "insert multiple (3x 99 at index 1)");

    // --- 6. erase ---
    std_v1.erase(std_v1.begin() + 2);
    ft_v1.erase(ft_v1.begin() + 2);
    compare_vectors(std_v1, ft_v1, "erase index 2");

    // --- 7. assign ---
    std_v1.assign(4, 55);
    ft_v1.assign(4, 55);
    compare_vectors(std_v1, ft_v1, "assign(4, 55)");

    // --- 8. front & back ---
    std::cout << "std front: " << std_v1.front() 
              << " | back: " << std_v1.back() << "\n";
    std::cout << "ft  front: " << ft_v1.front() 
              << " | back: " << ft_v1.back() << "\n\n";

    // --- 9. clear ---
    std_v1.clear();
    ft_v1.clear();
    compare_vectors(std_v1, ft_v1, "clear()");

    // --- 10. reserve ---
    std_v1.reserve(20);
    ft_v1.reserve(20);
    compare_vectors(std_v1, ft_v1, "reserve(20)");

    // --- 11. emplace_back ---
    std_v1.emplace_back(500);
    ft_v1.emplace_back(500);
    compare_vectors(std_v1, ft_v1, "emplace_back(500)");

    // --- 12. swap ---
    std::vector<int> std_v2(3, 7);
    ft::vector<int>  ft_v2(3, 7);
    std_v1.swap(std_v2);
    ft_v1.swap(ft_v2);
    compare_vectors(std_v1, ft_v1, "swap v1 with v2 (part 1)");
    compare_vectors(std_v2, ft_v2, "swap v1 with v2 (part 2)");

    // --- 13 emplace ---
    std_v1.emplace(std_v1.begin(), 111);
    ft_v1.emplace(ft_v1.begin(), 111);
    compare_vectors(std_v1, ft_v1, "emplace at begin (111)");

    // --- 14. Relational operators ---
    std::cout << "Relational Operators:\n";
    std::cout << "std_v1 == std_v2: " << (std_v1 == std_v2) << "\n";
    std::cout << "ft_v1 == ft_v2: " << (ft_v1 == ft_v2) << "\n";    
    std::cout << "std_v1 != std_v2: " << (std_v1 != std_v2) << "\n";
    std::cout << "ft_v1 != ft_v2: " << (ft_v1 != ft_v2) << "\n";    
    std::cout << "std_v1 < std_v2: " << (std_v1 < std_v2) << "\n";
    std::cout << "ft_v1 < ft_v2: " << (ft_v1 < ft_v2) << "\n";    
    std::cout << "std_v1 <= std_v2: " << (std_v1 <= std_v2) << "\n";
    std::cout << "ft_v1 <= ft_v2: " << (ft_v1 <= ft_v2) << "\n";


    // --- 15. Iterator tests ---
    std::cout << "\nIterator Tests:\n";
    std::cout << "std_v1 using iterator: ";
    for (std::vector<int>::iterator it = std_v1.begin(); it != std_v1.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\nft_v1 using iterator: ";
    for (ft::vector<int>::iterator it = ft_v1.begin(); it != ft_v1.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";
    std::cout << "std_v1 using const_iterator: ";
    for (std::vector<int>::const_iterator it = std_v1.begin(); it !=
            std_v1.end(); ++it)
            std::cout << *it << " ";    
    std::cout << "\nft_v1 using const_iterator: ";
    for (ft::vector<int>::const_iterator it = ft_v1.begin(); it !=
            ft_v1.end(); ++it)
            std::cout << *it << " ";
    std::cout << "\n";  
    compare_vectors(std_v1, ft_v1, "Final comparison after iterator tests");


    // --- 16. Exception safety (out_of_range) ---
    try {
        std::cout << "\nAccessing out-of-bounds element in std::vector: ";
        std::cout << std_v1.at(100) << "\n"; // Should throw
    } catch (const std::out_of_range &e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
    try {
        std::cout << "Accessing out-of-bounds element in ft::vector: ";
        std::cout << ft_v1.at(100) << "\n"; // Should throw
    } catch (const std::out_of_range &e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
    std::cout << "\n===== TESTS COMPLETE =====\n";


    return 0;
}
