#include <string>

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