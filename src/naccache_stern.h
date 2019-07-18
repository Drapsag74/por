#ifndef NACCACHE_STERN_H
#define NACCACHE_STERN_H

#include <map>
#include <vector>
#include <givaro/givintnumtheo.h>
#include <givaro/givrnsfixed.h>

namespace SMCStrassen{
    struct pub_key{
        Givaro::Integer sigma,g,n;
    };
    
    struct priv_key{
        Givaro::Integer p, q, a, b, u, v, f1, f2, phi, k, sizeK;
        std::vector<Givaro::Integer> pk;
    };
    
    Givaro::Integer randomPrime(uint64_t size);
    void factor(std::vector<Givaro::Integer>& pi, Givaro::Integer a);
    bool contains(std::vector<Givaro::Integer> a, Givaro::Integer e);
    void keyGen(priv_key& privKey, pub_key& pubKey, int size);
    void shared_block_size_key_gen(priv_key& privKey, pub_key& pubKey, int size, std::vector<Givaro::Integer> & shared_elems);
    Givaro::Integer encryption(pub_key key, Givaro::Integer m);
    Givaro::Integer BabyStepGiantStep(Givaro::Integer alpha, Givaro::Integer beta, Givaro::Integer mod, Givaro::Integer varX);
    Givaro::Integer chinese_remainder(std::vector<Givaro::Integer> n, std::vector<Givaro::Integer> a, int len);
    Givaro::Integer CRT(std::vector<Givaro::Integer> n, std::vector<Givaro::Integer> a);
    Givaro::Integer decryption(Givaro::Integer c, pub_key pubKey, priv_key privKey);
    Givaro::Integer add_ciphers(const Givaro::Integer& c1, const Givaro::Integer& c2, const Givaro::Integer& n);
    Givaro::Integer mul_ciphers(const Givaro::Integer& c1, const Givaro::Integer& m, const Givaro::Integer& n);
    
} //namespace SMCStrassen
#endif
