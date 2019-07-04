#include "Server.h"
#include <iostream>

Server::Server(Givaro::ModularBalanced<int64_t> & F, const Givaro::Integer & m, const Givaro::Integer & n):
F(F), m(m), n(n) {
    
}

int Server::send(Givaro::ModularBalanced<long int>::Element_ptr M) {
    this->M = M;

}


Givaro::ModularBalanced<long int>::Element_ptr Server::audit(Givaro::ModularBalanced<long int>::Element_ptr x) {
    Givaro::ModularBalanced<long int>::Element_ptr y = FFLAS::fflas_new(F, 1, m);
    std::cout << "Bar";
    FFLAS::fgemm(F, FFLAS::FflasTrans, FFLAS::FflasNoTrans, m, 1, n, F.one, x, 1, M, n, F.zero, y, 1);
    std::cout << "Fooo";
    return y;
}
