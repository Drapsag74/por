#include "Server.h"
#include <iostream>
#include "naccache_stern.h"

Server::Server(Field & F, size_t m, size_t n):
F(F), _m(m), _n(n) {
    
}

int Server::send(Field::Element_ptr M, Givaro::Modular<Givaro::Integer>::Element_ptr w, Givaro::Integer & n) {
    this->M = M;
    this->w = w;
    this->cipherSize = Givaro::Integer(n);
}


Server::Field::Element_ptr Server::audit(Field::Element_ptr x, Givaro::Integer & delta) {
    Field::Element_ptr y = FFLAS::fflas_new(F, 1, _n);
    for (size_t i = 0; i < _m; i++) {
        delta = SMCStrassen::add_ciphers(delta, SMCStrassen::mul_ciphers(w[i], x[i], cipherSize), cipherSize);
    }
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1, _n, _m, F.one, x, _m, M, _n, F.zero, y, _n);

    return y;
}
