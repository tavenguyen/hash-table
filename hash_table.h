#include <string>
#include <algorithm>
#include <cmath>

static const unsigned long prime_list[] = {
    53, 97, 193, 389, 769,
    1543, 3079, 6151, 12289, 24593,
    49157, 98317, 196613, 393241, 786433,
    1572869, 3145739, 6291469, 12582917, 25165843,
    50331653, 100663319, 201326611, 402653189, 805306457,
    1610612741, 3221225473, 4294967291
};

template <typename T>
struct ht_item {
    std::string key;
    T value;

    ht_item(const std::string& _key, const T& _value) : key(_key), value(_value) {}
    std::string getKey() const {return key;} 
};

template <typename T>
class ht_hash_table {
    private:
        ht_item<T>** items;
        int size;
        int capacity;

        static ht_item<int>* TOMBSTONE;
        int get_next_prime(int min_capacity) {
            return *(std::upper_bound(prime_list, prime_list + sizeof(prime_list) / sizeof(prime_list[0]), min_capacity));
        }
    public:  
        ht_hash_table(int _size) {
            capacity = get_next_prime(_size);
            size = 0;
            items = new ht_item<T>* [capacity];
            for(int i = 0; i < capacity; i++) {
                items[i] = nullptr;
            }
            std::cout << "Constructor called!" << std::endl;
        }
        ~ht_hash_table() {
            for(int i = 0; i < size; i++) {
                if(items[i] != nullptr) {
                    // Nếu có nhiều index cùng trỏ vào TOMBSTONE mà ta lại xoá thì nó sẽ gây ra UB.
                    if(items[i] == TOMBSTONE) {
                        continue;
                    }

                    delete items[i];
                }
            }

            delete [] items;
            delete TOMBSTONE;
            size = 0;
            capacity = 0;
            std::cout << "Destructor called!" << std::endl;
        }
        int hash_function(const std::string& key, int prime, int capacity) {
            long long hash_code = 0;
            for(auto c : key) {
                // Horner's method
                hash_code = (hash_code * prime + c) % capacity;
            }
            return (int)hash_code;
        }
        int get_hash(const std::string& key, int attempt) {
            int hash_1 = hash_function(key, 31, capacity);
            int hash_2 = hash_function(key, 53, capacity - 1);
            return (hash_1 + (long long)attempt * (hash_2 + 1)) % capacity;
        }
        void insert(const std::string& key, const T& value);
        int find(const std::string& key);
        void remove(const std::string& key);
};

template<typename T>
ht_item<int>* ht_hash_table<T>::TOMBSTONE = new ht_item("DELETED", 0);

template<typename T>
void ht_hash_table<T>::insert(const std::string& key, const T& value) {
    int index = get_hash(key, 0);
    int i = 1;
    ht_item<T>* current_pointer = items[index];
    while(current_pointer != nullptr) {
        // Đã tồn tại key, cập nhật lại giá trị
        if(current_pointer->key == key) {
            current_pointer->value = value;
            return;
        }

        index = get_hash(key, i++);
        current_pointer = items[index];
    }
    current_pointer = new ht_item(key, value);
    items[index] = current_pointer;
    size++;
}

template <typename T>
int ht_hash_table<T>::find(const std::string& key) {
    if(size == 0) {
        return (-1);
    }

    int index = get_hash(key, 0);
    int i = 1;
    ht_item<T>* current_pointer = items[index];
    while(current_pointer != nullptr) {
        if(current_pointer->key == key) {
            return index;
        }

        index = get_hash(key, i++);
        current_pointer = items[index];
    }
    return (-1);
}

template <typename T>
void ht_hash_table<T>::remove(const std::string& key) {
    if(size == 0) {
        return;
    }

    int index = get_hash(key, 0);
    int i = 1;
    ht_item<T>* current_pointer = items[index];
    while(current_pointer != nullptr) {
        if(current_pointer->key == key) {
            delete current_pointer;
            current_pointer = TOMBSTONE;
            size--;
            return;
        }

        index = get_hash(key, i++);
        current_pointer = items[index];
    }
}