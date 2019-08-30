#include "naccache_stern.h"


using namespace Givaro;

//#include "param.h"

namespace SMCStrassen{
    /* Returns a prime number of ``size`` bits */
Integer randomPrime(uint64_t size) {
    IntPrimeDom IPD;
    IntPrimeDom::Element m;
    Integer rand;
    Integer::random_exact_2exp(rand, size); 
    IPD.nextprime (m,rand);
    return m;
}

    /* Stores in ``pi`` the prime factorization of ``a`` */
void factor(std::vector<Integer>& pi, Integer a){
	pi.clear();
	IntFactorDom<> IP;
	Integer tmp = a;
	Integer p;
	while(tmp!=1){
		IP.factor(p, tmp);
		pi.push_back(p);
		tmp = tmp/p;
	}
}

bool contains(std::vector<Integer> a, Integer e){
    for(unsigned int i=0; i<a.size(); i++){
        if(a.at(i)==e){
            return true;
        }
    }
    return false;
}

    /* Generates a public key, a private key bounded by ``size`` and 
     * respectively stores them in ``pub_key``, ``priv_key`` */
void keyGen(priv_key& privKey, pub_key& pubKey, int size){
    privKey.k = 20;
    privKey.sizeK = 16;
    if((privKey.k/2)*privKey.sizeK>size){
        std::cout << "Error! k NB_BLOCKS or BLOCK_SIZE too large wrt size" << std::endl;
    }
    IntPrimeDom::Element m;
    IntPrimeDom IP;

    privKey.pk.clear();
    for(int i=1; i<=privKey.k; i++){
        Integer tmp = randomPrime(privKey.sizeK);
        if(!contains(privKey.pk, tmp)){
            privKey.pk.push_back(tmp);
        }
        else {i--;}
    }
	
    privKey.u = 1;
    for(int i=1; i<=privKey.k/2; i++){
        privKey.u *= privKey.pk.at(i-1);
    }
    privKey.v = 1;
    for(int i=(privKey.k/2)+1; i<=privKey.k; i++){
        privKey.v *= privKey.pk.at(i-1);
    }
    pubKey.sigma = 1;
    pubKey.sigma *= privKey.u;
    pubKey.sigma *= privKey.v;
    privKey.f1 = 2;
    privKey.f2 = 2;
	
    privKey.a = randomPrime(size-logtwo(privKey.u));
    privKey.p = 1;
    privKey.p *= privKey.f1;
    privKey.p *= privKey.a;
    privKey.p *= privKey.u;
    while(!IP.isprime(privKey.p+1, m)){
        privKey.f1 += 2;
        privKey.p = 1;
        privKey.p *= privKey.f1;
        privKey.p *= privKey.a;
        privKey.p *= privKey.u;
    }
    privKey.p++;
	
    privKey.b = randomPrime(size-logtwo(privKey.v));
    privKey.q = 1;
    privKey.q *= privKey.f2;
    privKey.q *= privKey.b;
    privKey.q *= privKey.v;
    while(!IP.isprime(privKey.q+1, m)){
        privKey.f2 += 2;
        privKey.q = 1;
        privKey.q *= privKey.f2;
        privKey.q *= privKey.b;
        privKey.q *= privKey.v;
    }
    privKey.q++;
	
    pubKey.n = 1;
    pubKey.n *= privKey.p;
    pubKey.n *= privKey.q;
    privKey.phi = (privKey.p-1)*(privKey.q-1);
	
	IntNumTheoDom<> IP2;
    // p-1 factorization
	Integer gp;
    std::vector<Integer> pi;
    factor(pi, privKey.f1);
    pi.push_back(privKey.a);
    if(privKey.u!=1){pi.push_back(privKey.u);}
    IP2.prim_root_of_prime(gp, pi, privKey.p-1, privKey.p);
    
    // q-1 factorization
	Integer gq;
    std::vector<Integer> qi;
    factor(qi, privKey.f2);
    pi.push_back(privKey.b);
    pi.push_back(privKey.v);
    IP2.prim_root_of_prime(gq, qi, privKey.q-1, privKey.q);
    
	powmod(gp, gp, privKey.f1, privKey.p);
	powmod(gq, gq, privKey.f2, privKey.q);
	
	Integer pm1;
	powmod(pm1, privKey.p, -1, privKey.q);
	pubKey.g = gp + (privKey.p*pm1*(gq-gp))%pubKey.n;
	if(pubKey.g<0){pubKey.g+=pubKey.n;}
}

    /* Key Generation when small primes and the related products are shared */
void shared_block_size_key_gen(priv_key& privKey, pub_key& pubKey, int size, std::vector<Integer> & shared_elems){
    size_t nb_blocks = 20;
    privKey.k = nb_blocks;
    privKey.sizeK = 16;
    Givaro::Integer u = shared_elems[nb_blocks];
    Givaro::Integer v = shared_elems[nb_blocks+1];
    Givaro::Integer sigma = shared_elems[nb_blocks+2];
    if((privKey.k/2)*privKey.sizeK>size){
        std::cout << "Error! k NB_BLOCKS or BLOCK_SIZE too large wrt size" << std::endl;
    }
    IntPrimeDom::Element m;
    IntPrimeDom IP;
	
    privKey.f1 = 2;
    privKey.f2 = 2;
	
    privKey.a = randomPrime(size-logtwo(u));
    privKey.p = 1;
    privKey.p *= privKey.f1;
    privKey.p *= privKey.a;
    privKey.p *= u;
    while(!IP.isprime(privKey.p+1, m)){
        privKey.f1 += 2;
        privKey.p = 1;
        privKey.p *= privKey.f1;
        privKey.p *= privKey.a;
        privKey.p *= u;
    }
    privKey.p++;
	
    privKey.b = randomPrime(size-logtwo(v));
    privKey.q = 1;
    privKey.q *= privKey.f2;
    privKey.q *= privKey.b;
    privKey.q *= v;
    while(!IP.isprime(privKey.q+1, m)){
        privKey.f2 += 2;
        privKey.q = 1;
        privKey.q *= privKey.f2;
        privKey.q *= privKey.b;
        privKey.q *= v;
    }
    privKey.q++;
	
    pubKey.n = 1;
    pubKey.n *= privKey.p;
    pubKey.n *= privKey.q;
    privKey.phi = (privKey.p-1)*(privKey.q-1);
	
	IntNumTheoDom<> IP2;
    // p-1 factorization
	Integer gp;
    std::vector<Integer> pi;
    factor(pi, privKey.f1);
    pi.push_back(privKey.a);
    if(u!=1){pi.push_back(u);}
    IP2.prim_root_of_prime(gp, pi, privKey.p-1, privKey.p);
    
    // q-1 factorization
	Integer gq;
    std::vector<Integer> qi;
    factor(qi, privKey.f2);
    pi.push_back(privKey.b);
    pi.push_back(v);
    IP2.prim_root_of_prime(gq, qi, privKey.q-1, privKey.q);
    
	powmod(gp, gp, privKey.f1, privKey.p);
	powmod(gq, gq, privKey.f2, privKey.q);
	
	Integer pm1;
	powmod(pm1, privKey.p, -1, privKey.q);
	pubKey.g = gp + (privKey.p*pm1*(gq-gp))%pubKey.n;
	if(pubKey.g<0){pubKey.g+=pubKey.n;}
}


    /* Returns ``m`` ciphered using ``key`` */
Integer encryption(pub_key key, Integer m){
    Integer t; //t.seeding();
    Integer x;
    IntegerDom::Element k;
    x = t.random_between((Integer)2,(Integer)key.n-1);
    Integer xSigma = powmod(k,x, key.sigma, key.n);
    Integer gm = powmod(k,key.g, m, key.n);
    return (xSigma*gm)%key.n;
}

    /* Discrete log computation using Baby Step, Giant Step strategy */
Integer BabyStepGiantStep(Integer alpha, Integer beta, Integer mod, Integer varX) {
    Integer S = ceil(::sqrt(varX));
    Modular<Integer>::Element k;
    Integer an = powmod(k, alpha, S, mod);
    Integer alphaInv = powmod(k, alpha, -1, mod);
    std::map<Integer,Integer> vals;
    Integer cur=an;
    for (Integer i=1; i<=S; i++) {
        vals[cur] = i;
        cur = (cur * an) % mod;
    }
    cur = beta;
    for (Integer i=0; i<=S; i++) {
        if (vals.count(cur)) {
            return (vals[cur]*S + i)%varX;
        }
        cur = (cur * alphaInv) % mod;
    }
    return -1;
}

Integer chinese_remainder(std::vector<Integer> n, std::vector<Integer> a, int len){
    Integer p, prod = 1, sum = 0;
    int i;
    for (i = 0; i < len; i++) {prod = prod * n.at(i);}
    for (i = 0; i < len; i++) {
        p = prod / n.at(i);
        Givaro::Integer inv;
        Givaro::inv(inv, n.at(i), p);
        sum = sum + a.at(i) * inv * p;
    }
    return sum % prod;
}

Integer CRT(std::vector<Integer> n, std::vector<Integer> a){
    typedef RNSsystemFixed<Integer> CRTSystemFixed;
    CRTSystemFixed CRT(n);
    Integer ret;
    CRT.RnsToRing(ret, a);
    return ret;
}

    /* returns the plain text associate with ``c`` using the provided key ring
     * ``pubKey``, ``privKey`` */
Integer decryption(Integer c, pub_key pubKey, priv_key privKey){
    std::vector<Integer> mi;
    std::vector<Integer> pi;
    Modular<Integer>::Element k;
    for(int i=0; i<privKey.k; i++){
        Integer ci = powmod(k, c, (privKey.phi/privKey.pk.at(i)), pubKey.n);
        Integer gi = powmod(k, pubKey.g, (privKey.phi/privKey.pk.at(i)), pubKey.n);
        Integer cur = gi%pubKey.n;
        Integer j = BabyStepGiantStep(gi, ci, pubKey.n, privKey.pk.at(i));
        mi.push_back(j);
        pi.push_back(privKey.pk.at(i));
    }
    return CRT(pi, mi);
}

Integer mul_ciphers(const Integer& c1, const Integer& m, const Integer& n) {
    Integer r;
    powmod(r,c1,m,n);
    return r;
}

Integer add_ciphers(const Integer& c1, const Integer& c2, const Integer& n) {
    Modular<Integer> Zn(n);
    Integer r;
    return Zn.mul(r,c1,c2);
}
} //namespace SMCStrassen
