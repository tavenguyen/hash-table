#include <string>

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
    public:  
        ht_hash_table(int _size) {
            capacity = _size;
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
                    delete items[i];
                }
            }

            delete [] items;
            size = 0;
            capacity = 0;
            std::cout << "Destructor called!" << std::endl;
        }
        int hash_function(const std::string& key, int prime, int m) {
            long long hash_code = 0;
            int n = key.size();
            for(int i = 1; i <= n; i++) {
                hash_code += (long long)power(prime, n - i) * s[i - 1];
                hash_code %= (long long)m;
            }
            return (int)hash_code;
        }
        // void reserve(int new_capacity)
        // void add_item(const ht_item<T>* const item);
};