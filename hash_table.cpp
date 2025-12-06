#include <iostream>
#include "hash_table.h"
#include <vector>
#include <algorithm>

int main() {
    ht_hash_table<int> ht(10);
    ht.insert("User1", 100);
    ht.insert("User2", 200);
    ht.insert("User3", 300);
    std::cout << *ht.get("User2") << std::endl;
    ht.insert("User1", 500);
    std::cout << *ht.get("User1") << std::endl;
    return 0;
}