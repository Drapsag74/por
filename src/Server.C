#include "Server.h"
#include <iostream>

Server::Server(Givaro::ModularBalanced<int64_t> & F, const Givaro::Integer & m, const Givaro::Integer & n):
F(F), _m(m), _n(n) {
    
}

int Server::send(Givaro::ModularBalanced<long int>::Element_ptr M) {
    this->M = M;

}


Givaro::ModularBalanced<long int>::Element_ptr Server::audit(Givaro::ModularBalanced<long int>::Element_ptr x) {
    Givaro::ModularBalanced<long int>::Element_ptr y = FFLAS::fflas_new(F, 1, _n);
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1, _n, _m, F.one, x, _m, M, _n, F.zero, y, _n);

    return y;
}
