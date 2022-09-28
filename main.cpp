#include <string>
#include <vector>
#include <bitset>
#include <iostream>
//#include <effolkronium/random.hpp>

//using random = effolkronium::random_static;

template <typename T>
class hash_function {

    virtual size_t hash(T elem) = 0;

    public:
    size_t operator()(T elem) { return hash(elem); };

};

class sfold : public hash_function<std::string> {

    size_t seed;

    virtual size_t hash(std::string elem) {
        size_t size = elem.size() / 4;
        size_t sum = 0;
        for(size_t i = 0; i < size; i++) {
            const char* c = elem.substr(i * 4, 4).c_str();
            size_t mult = 1;
            for(size_t j = 0; j < sizeof(c) / sizeof(char); j++) {
                sum += c[j] * mult;
                mult *= seed;
            }
        }
        return sum;
    }

    public:

    sfold(size_t seed) {
        this->seed = seed; 
    }

};

typedef hash_function<std::string> str_hash;


class bloom_filter {

    private:
        std::vector<bool> set;
        
        str_hash** hashers;
        size_t k;


    public:
        void insert(std::string str) {
            for(size_t i = 0; i < k; i++) {
                str_hash* hash = hashers[i];
                size_t j = (*hash)(str) % set.size();
                set[j] = 1;
            }
        }

        bool test_membership(std::string str) {
            for(size_t i = 0; i < k; i++) {
                str_hash* hash = hashers[i];
                size_t j = (*hash)(str) % set.size();
                if(!set[j]) {
                    return false;
                }
            }
            return true;
        }

        void reset() { std::fill(set.begin(), set.end(), 0); };

        bloom_filter(size_t size, size_t num_hashers, str_hash** hashers) {
            for(size_t i = 0; i < size; i++) {
                this->set.push_back(0);
            }
            this->k = num_hashers;
            this->hashers = hashers;
        };
};


str_hash** contruct_random_sfold_hashs(size_t k) {
    str_hash* v[k];
    for(size_t i = 0; i < k; i++) {
        size_t seed = random::get(1, N);
        v[i] = new sfold(seed);
    }
    return v;
}

int main() {

    size_t k = 4;
    str_hash** hashers = construct_random_sfold_hashs(k);
    bloom_filter filter(10, k, hashers);

    return 0;
}
