#include <iostream>
#include "hash_table.h"
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    ht_hash_table<int> ht(10);
    for(int i = 0; i < 1000; i++) {
        std::string result = "Key" + std::to_string(i + 1);
        ht.insert(result, i);
    }
    ht.remove("Key25");
    assert(ht.find("Key25") == -1);
    assert(ht.find("Key26") != -1);
    ht.insert("Key25", 999);
    assert(ht.find("Key25") != -1);
    std::cout << "ALL TESTS PASSED!" << std::endl;
    return 0;
}