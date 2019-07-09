#include "Client.h"

#include <iostream>


Client::Client(Server & server, Field & F, Field::Element_ptr M, const Givaro::Integer & m, const Givaro::Integer & n) :
server(server), F(F), _m(m), _n(n) {
    std::cout << "Creating client" << std::endl;
    //    FFLAS::WriteMatrix(std::cout << "Matrice : " << std::endl, this->F, m, n, M,m);
    v = FFLAS::fflas_new(F,m,1);
    while(FFLAS::fiszero(F, m,v, 1)) {
        Field::Element_ptr u = FFLAS::fflas_new(F,n,1);
        FFPACK::RandomMatrix(F, n, 1, u, 1);
        FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, m, 1, n, F.one, M, n, u, 1, F.zero, v, 1);
        _u = u;

    }

    server.send(M);
//    FFLAS::WriteMatrix(std::cout << "vecteur u : " << std::endl, F, n,1, _u, 1) << std::endl; 
//    FFLAS::WriteMatrix(std::cout << "vecteur v : " << std::endl, F, m,1, v, 1) << std::endl;
    std::cout << "Client created" << std::endl;
    
}


bool Client::audit() {
    std::cout << "Begin audit" << std::endl;
    Field::Element_ptr x = FFLAS::fflas_new(F, _m, 1);
    typename Field::RandIter G(F,0);
    FFPACK::NonZeroRandomMatrix(F, _m, 1, x, 1, G);
//    FFLAS::WriteMatrix(std::cout << "vecteur x : " << std::endl,F, 1, _m, x, _m) << std::endl;
    std::cout << "Waiting for the server" << std::endl;
    Field::Element_ptr y = server.audit(x);
    std::cout << "got y" << std::endl;
    Field::Element_ptr yu = FFLAS::fflas_new(F, 1, 1);
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1,1,_n, F.one, y, _n, _u,1, F.zero, yu,1);
    FFLAS::WriteMatrix(std::cout << "Vecteur yu : " << std::endl, F, 1,1, yu,1) << std::endl;
    Field::Element_ptr xv = FFLAS::fflas_new(F, _m, 1);
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1,1,_m, F.one, x, _m, v,1, F.zero, xv,1);
    FFLAS::WriteMatrix(std::cout << "Vecteur xv : " << std::endl, F, 1,1, xv,1) << std::endl;
        
    return yu[0] == xv[0];
}
