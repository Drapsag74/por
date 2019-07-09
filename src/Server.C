#include "Server.h"
#include <iostream>

Server::Server(Field & F, const Givaro::Integer & m, const Givaro::Integer & n):
F(F), _m(m), _n(n) {
    
}

int Server::send(Field::Element_ptr M) {
    this->M = M;

}


Server::Field::Element_ptr Server::audit(Field::Element_ptr x) {
    Field::Element_ptr y = FFLAS::fflas_new(F, 1, _n);
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1, _n, _m, F.one, x, _m, M, _n, F.zero, y, _n);

    return y;
}
