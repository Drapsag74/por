#include "Client.h"

#include <iostream>


Client::Client(Server & server, Givaro::ModularBalanced<int64_t> & F, Givaro::ModularBalanced<int64_t>::Element_ptr M, const Givaro::Integer & m, const Givaro::Integer & n) :
server(server), F(F), _m(m), _n(n) {
//    FFLAS::WriteMatrix(std::cout << "Matrice : " << std::endl, this->F, m, n, M,m);
    v = FFLAS::fflas_new(F,m,1);
    while(FFLAS::fiszero(F, m,v, 1)) {
        Givaro::ModularBalanced<int64_t>::Element_ptr u = FFLAS::fflas_new(F,n,1);
        FFPACK::RandomMatrix(F, m, 1, u, 1);
        FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, m, 1, n, F.one, M, m, u, 1, F.zero, v, 1);
    _u = u;

    }

    server.send(M);
    FFLAS::WriteMatrix(std::cout << "vecteur v : " << std::endl, F, m,1, v, 1) << std::endl;
    
}


bool Client::audit() {
    std::cout << "Début de l'audit : " << std::endl;
    Givaro::ModularBalanced<int64_t>::Element_ptr x = FFLAS::fflas_new(F, _m, 1);
    typename Givaro::ModularBalanced<int64_t>::RandIter G(F,0);
    FFPACK::NonZeroRandomMatrix(F, _m, 1, x, 1, G);
//    FFLAS::WriteMatrix(std::cout << "vecteur x : " << std::endl,F, 1, _m, x, _m) << std::endl;
    std::cout << "En attente du serveur" << std::endl;
    Givaro::ModularBalanced<int64_t>::Element_ptr y = server.audit(x);
    std::cout << "y reçus" << std::endl;
    Givaro::ModularBalanced<int64_t>::Element_ptr yu = FFLAS::fflas_new(F, 1, 1);
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1,1,_n, F.one, y, _n, _u,1, F.zero, yu,1);
    FFLAS::WriteMatrix(std::cout << "Vecteur yu : " << std::endl, F, 1,1, yu,1) << std::endl;
    Givaro::ModularBalanced<int64_t>::Element_ptr xv = FFLAS::fflas_new(F, _m, 1);
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1,1,_m, F.one, x, _m, v,1, F.zero, xv,1);
    FFLAS::WriteMatrix(std::cout << "Vecteur xv : " << std::endl, F, 1,1, xv,1) << std::endl;
        
    return yu[0] == xv[0];
}
