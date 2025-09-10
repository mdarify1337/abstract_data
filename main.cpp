#include <iostream>
#include <vector>
#include "vector.hpp"  
#include <list>
#include "list.hpp"

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


// ---------- LIST HELPERS ----------
template <typename List>
void print_list(const List &lst, const std::string &name) {
    std::cout << name << " (size=" << lst.size() << "): ";
    for (typename List::const_iterator it = lst.begin(); it != lst.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";
}

template <typename L1, typename L2>
void compare_lists(const L1 &l1, const L2 &l2, const std::string &label) {
    std::cout << "=== " << label << " ===\n";
    print_list(l1, "std::list");
    print_list(l2, "ft::list");

    if (l1.size() != l2.size()) {
        std::cout << "❌ Size mismatch!\n\n";
        return;
    }

    typename L1::const_iterator it1 = l1.begin();
    typename L2::const_iterator it2 = l2.begin();
    for (; it1 != l1.end() && it2 != l2.end(); ++it1, ++it2) {
        if (*it1 != *it2) {
            std::cout << "❌ Mismatch: std=" << *it1 << ", ft=" << *it2 << "\n\n";
            return;
        }
    }
    std::cout << "✅ Lists match!\n\n";
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


    std::cout << "\n===== LIST TESTS =====\n\n";

    // --- 1. Fill constructor ---
    std::list<int> std_l1(5, 42);
    ft::list<int>  ft_l1(5, 42);
    compare_lists(std_l1, ft_l1, "Fill constructor");

    // --- 2. push_back ---
    std_l1.push_back(100);
    ft_l1.push_back(100);
    compare_lists(std_l1, ft_l1, "push_back(100)");

    // --- 3. push_front ---
    std::cout << std_l1.front() << std::endl;
    std::cout << ft_l1.front() << std::endl;
    compare_lists(std_l1, ft_l1, "push_front(7)");

    // --- 4. pop_back / pop_front ---
    std_l1.pop_back();
    ft_l1.pop_back();
    compare_lists(std_l1, ft_l1, "pop_back()");
    std_l1.pop_front();
    ft_l1.pop_front();
    compare_lists(std_l1, ft_l1, "pop_front()");

    // --- 5. front & back ---
    std::cout << "std front: " << std_l1.front() 
              << " | back: " << std_l1.back() << "\n";
    std::cout << "ft  front: " << ft_l1.front() 
              << " | back: " << ft_l1.back() << "\n\n";

    // --- 6. max size ---
    std::cout << "std max_size: " << std_l1.max_size() << "\n";
    std::cout << "ft  max_size: " << ft_l1.max_size() << "\n\n";

    // --- 7. assign ---
    std_l1.assign(3, 88);
    ft_l1.assign(3, 88);
    compare_lists(std_l1, ft_l1, "assign(3, 88)");

    int myints[]={1776,7,4};
    ft_l1.assign(myints,myints+3);  
    std_l1.assign(myints,myints+3);
    compare_lists(std_l1, ft_l1, "assign from array");

    std::list<int> std_l3;
    std_l3.assign(5, 100);

    ft_l1.assign(std_l3.begin(), std_l3.end());
    std_l1.assign(std_l3.begin(), std_l3.end());
    compare_lists(std_l1, ft_l1, "assign from another list");

    // ---- 8.emplace-> front && back
    ft_l1.emplace_front(125);
    std_l1.emplace_front(125);

    ft_l1.emplace_back(1337);
    std_l1.emplace_back(1337);
    compare_lists(std_l1, ft_l1, "emplace back and front");

    // // --- 5. insert ---
    // std::list<int>::iterator std_it = std_l1.begin();
    // ft::list<int>::iterator  ft_it  = ft_l1.begin();
    // std_l1.insert(std_it, 77);
    // ft_l1.insert(ft_it, 77);
    // compare_lists(std_l1, ft_l1, "insert(77 at begin)");

    // // --- 6. erase ---
    // std_it = std_l1.begin();
    // ft_it  = ft_l1.begin();
    // std_l1.erase(std_it);
    // ft_l1.erase(ft_it);
    // compare_lists(std_l1, ft_l1, "erase first element");


    // // --- 8. swap ---
    // std::list<int> std_l2(2, 9);
    // ft::list<int>  ft_l2(2, 9);
    // std_l1.swap(std_l2);
    // ft_l1.swap(ft_l2);
    // compare_lists(std_l1, ft_l1, "swap part 1");
    // compare_lists(std_l2, ft_l2, "swap part 2");

    std::cout << "\n===== TESTS COMPLETE =====\n";
    return 0;


    return 0;
}
