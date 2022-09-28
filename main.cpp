#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <effolkronium/random.hpp>


/// Random generation library which works without seed boilerplate.
using rand_gen = effolkronium::random_static;


/// Abstract template for different hash implementations.
template <typename T>
class hash_function {

    /** Class abstraction for a hash definition. Should return a hash for a
     * given type of element.
     *
     * \param  elem An element of type T which this class should hash.
     * \return      The hash code for the given element. 
     */
    virtual size_t hash(T elem) = 0;

    public:

    /** Function overload for this class. 
     *
     * The only purpose of this is to call the private definition for `hash`.
     *
     * \param  elem An element of type T which this class should hash.
     * \return      The hash code for the given element. 
     */
    size_t operator()(T elem) { return hash(elem); };

};


/// Short hand for hash functions involving a string
typedef hash_function<std::string> str_hash;


/// Class implementation for the string folding hash function with a given seed.
class sfold : public str_hash {

    // The seed used for incrementing the multiplier in this hash.
    size_t seed;

    /** Hash function for the sfold, which operates by handling every 4 bytes of
     * a string and adding them to a sum. Bytes are offset by the `seed`.
     */
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

    /** Public constructor for the sfold hash function. Requires a seed for
     * projection.
     *
     * \param seed The seed for projecting the string.
     */
    sfold(size_t seed) {
        this->seed = seed; 
    }

};



/// Class implementation for the bloom filter data structure.
class bloom_filter {

    private:

        // Bit set for checking if an element occupies the set.
        std::vector<bool> set;
        
        // Array of hashers initialized at construction which are used for
        // storing and testing membership.
        str_hash** hashers;


        // Number of hashers in `hashers`.
        size_t k;


    public:

        /** Inserts an element into the bloom filter by running it through each
         * stored hasher and setting all those bits to 1 in the bit set.
         *
         * \param str The string to insert into the bloom filter
         */
        void insert(std::string str) {
            for(size_t i = 0; i < k; i++) {
                str_hash* hash = hashers[i];
                size_t j = (*hash)(str) % set.size();
                set[j] = 1;
            }
        }

        /** Tests if a string is in the bloom filter by running it through each
         * stored hasher and checking all those bits are 1 in the bit set.
         *
         * \param  str The string to test for membership
         * \return     True if the string is in the set, false otherwise.
         */
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

        /** Resets the bitset to all 0's.  */
        void reset() { std::fill(set.begin(), set.end(), 0); };

        /** Initializes the bloom filter.
         *
         * \param size        The maximum size of the bitset and corresponding 
         *                    index.
         * \param num_hashers The number of hashers being used.
         * \param hashers     An array containing pointers to all the hashers we
         *                    are using. We use an array of pointers since the
         *                    underlying base class is abstract. 
         *
         */
        bloom_filter(size_t size, size_t num_hashers, str_hash** hashers) {
            for(size_t i = 0; i < size; i++) {
                this->set.push_back(0);
            }
            this->k = num_hashers;
            this->hashers = hashers;
        };

        /** Deconstructs the bloom filter. The array of hashers should be freed
         * from memory seperately.
         */
        ~bloom_filter() {
            for(size_t i = 0; i < this->k; i++) {
                this->hashers[i] = 0;
            }
        }
};


/** Constructs sfold hashs from a given list of seeds.
 *
 * \param k     The number of sfold hashs that should be constructed.
 * \param seeds The list of seeds that the k sfold hashs should choose from
 *              wihtout replacement.
 * \return      Array of pointers to sfold hashs. They are returned this way
 *              since the underlying class, hash_function, is abstract.
 */
str_hash** construct_random_sfold_hashs(size_t k, std::vector<size_t> seeds) {

    str_hash* v[k];

    // Rather than run a choice funciton (which this library doesn't have), we
    // equivalently just shuffle all the elements of the vector and just take
    // the first k.
    rand_gen::shuffle(seeds);
    for(size_t i = 0; i < k; i++) {
        v[i] = new sfold(seeds[i]);
    }
    return v;
}

/** Generates all primes less than n using the Sieve of Erastosthenes method.
 *
 * \param n The upper limit for the primes.
 * \return  A vector containing all the primes between 2 and n.
 */
std::vector<size_t> gen_primes(size_t n) {
    std::vector<size_t> p;
    std::vector<bool> p_test;
    for(size_t i = 0; i < n; i++) {
        p_test.push_back(true);
    }
    for(size_t i = 2; (i * i) < n; i++) {
        if(p_test[i]) {
            p.push_back(i);
            for(size_t j = (i * i); j < n; j += i) {
                p_test[j] = false;
            } 
        }
    }
    return p;
}

int main() {

    size_t k = 4;
    size_t n = 1000;

    std::vector<size_t> primes = gen_primes(n);
    str_hash** hashers = construct_random_sfold_hashs(k, primes);

    return 0;
}
