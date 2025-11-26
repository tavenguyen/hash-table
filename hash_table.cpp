#include <iostream>
#include "hash_table.h"
#include <vector>
#include <algorithm>

static const unsigned long prime_list[] = {
    53, 97, 193, 389, 769,
    1543, 3079, 6151, 12289, 24593,
    49157, 98317, 196613, 393241, 786433,
    1572869, 3145739, 6291469, 12582917, 25165843,
    50331653, 100663319, 201326611, 402653189, 805306457,
    1610612741, 3221225473, 4294967291
};

int main() {
    ht_hash_table<int> ht(10);
    auto it = std::upper_bound(prime_list, prime_list + sizeof(prime_list) / sizeof(prime_list[0]), 189);
    std::cout << *it;
    return 0;
}