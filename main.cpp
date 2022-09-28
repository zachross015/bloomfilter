#include <string>
#include <vector>
#include <bitset>

template <typename T>
class hash_function {

    virtual long hash(T elem) = 0;

    public:
    long operator()(T elem) { return hash(elem); };

};

class sfold : public hash_function<std::string> {

    int seed;

    virtual long hash(std::string elem) {
        int size = elem.size() / 4;
        long sum = 0;
        for(int i = 0; i < size; i++) {
            const char* c = elem.substr(i * 4, 4).c_str();
            long mult = 1;
            for(int j = 0; j < sizeof(c) / sizeof(char); j++) {
                sum += c[j] * mult;
                mult *= seed;
            }
        }
        return sum;
    }

    public:

    sfold(int seed) {
        this->seed = seed; 
    }

};

typedef s_vec std::vector<std::string>;
typedef shash hash_function<std::string>;
typedef shash_vec std::vector<s_hash>;


template <size_t N>
class bloom_filter {

    private:
        std::bitset<N> set;
        shash_vec hashers;


    public:
        void insert(std::string str) {
            for(auto hash : hashers) {
                long i = hash(str) % N;
                set[i] = 1;
            }
        }

        bool test_membership(std::string str) {
            for(auto hash : hashers) {
                long i = hash(str) % size;
                if(!set.test(i)) {
                    return false;
                }
            }
            return true;
        }

        void reset() { set.reset(); };

        bloom_filter(shash_vec hashers) :  hashers(hashers) {};
}


int main() {

    sfold hash(101);
    std::cout << hash("aaaabbbb") << std::endl;
    return 0;

}
