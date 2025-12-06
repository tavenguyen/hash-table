#include <string>
#include <algorithm>
#include <cmath>

constexpr double LOAD_FACTOR_HASH_TABLE = 0.75;

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
            //std::cout << "Constructor called! [capacity: " << capacity << "]" << std::endl; 
        }
        ~ht_hash_table() {
            ht_del_hash_table();
            //std::cout << "Destructor called!" << std::endl;
        }
        void ht_del_hash_table();
        void resize(int _capacity);
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
        bool valid() const {return capacity;} 
        void insert(const std::string& key, const T& value);
        int find(const std::string& key);
        void remove(const std::string& key);
        T* get(const std::string& key);
};

template <typename T>
void ht_hash_table<T>::ht_del_hash_table() {
    for(int i = 0; i < capacity; i++) {
        if(items[i] != nullptr) {
            // Nếu có nhiều index cùng trỏ vào TOMBSTONE mà ta lại xoá thì nó sẽ gây ra UB.
            if(items[i] == TOMBSTONE) {
                continue;
            }

            delete items[i];
        }
    }

    delete [] items;
}

template <typename T>
void ht_hash_table<T>::resize(int _capacity) { 
    ht_hash_table<T> new_ht_table(_capacity);
    for(int i = 0; i < capacity; i++) {
        if(items[i] == nullptr || items[i] == TOMBSTONE) {
            continue;
        }

        new_ht_table.insert(items[i]->key, items[i]->value);
    }

    std::swap(this->items, new_ht_table.items);
    std::swap(this->capacity, new_ht_table.capacity);
    std::swap(this->size, new_ht_table.size);
}

template<typename T>
ht_item<int>* ht_hash_table<T>::TOMBSTONE = new ht_item("DELETED", 0);

template<typename T>
void ht_hash_table<T>::insert(const std::string& key, const T& value) {
    if(!valid()) {
        std::cerr << "[DEBUG] Invalid hash_table to insert an element!" << std::endl;
        return;
    }
    
    if(float(size) / float(capacity) >= LOAD_FACTOR_HASH_TABLE) {
        resize(capacity + 1);
    }

    int index = get_hash(key, 0);
    int first_deleted_index = -1;
    int i = 1;
    ht_item<T>* current_pointer = items[index];
    while(current_pointer != nullptr) {
        if(current_pointer == TOMBSTONE) {
            if(first_deleted_index == -1) {
                first_deleted_index = index;
            }
        }
        else if(current_pointer != TOMBSTONE && current_pointer->key == key) {
            current_pointer->value = value;
            return;
        }

        index = get_hash(key, i++);
        current_pointer = items[index];
    }

    ht_item<T>* new_item = new ht_item(key, value);
    if(first_deleted_index != -1) {
        items[first_deleted_index] = new_item;
    } else {
        items[index] = new_item;
    }
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
        if(current_pointer != TOMBSTONE) {
            if(current_pointer->key == key) {
                return index;
            }
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
        if(current_pointer != TOMBSTONE) { 
            if(current_pointer->key == key) {
                delete current_pointer;
                items[index] = TOMBSTONE;
                size--;
                return;
            }
        }

        index = get_hash(key, i++);
        current_pointer = items[index];
    }
}

template <typename T>
T* ht_hash_table<T>::get(const std::string& key) {    
    if(size == 0) {
        return nullptr;
    }

    int index = find(key);
    if(index == -1) {
        return nullptr;
    }

    return &(items[index]->value);
}