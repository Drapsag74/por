#include "Client.h"
#include <iostream>


Client::Client(Server & server, Field & F, Field::Element_ptr M, size_t m, size_t n) :
server(server), F(F) {
    _m = m;
    _n = n;
    std::cout << "Creating client" << std::endl;
    SMCStrassen::keyGen(_k, _pk, 2048);
//    FFLAS::WriteMatrix(std::cout << "Matrice : " << std::endl, this->F, m, n, M,n)<< std::endl;
    v = FFLAS::fflas_new(F,m,1);
    for(size_t i = 0; i < m; i++) {
        F.init(v[i], 0);
    }
    while(FFLAS::fiszero(F, m,v, 1)) {
        std::cout << "try to create v" << std::endl;
        Field::Element_ptr u = FFLAS::fflas_new(F,n,1);
        FFPACK::RandomMatrix(F, n, 1, u, 1);
        FFPACK::RandomMatrix(F,m,1,v,1);
        //_t = v;
        FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, m, 1, n, F.one, M, n, u, 1, F.zero, v, 1);
        _u = u;
        
    }

    std::cout << "ciphering vecteur v" << std::endl;
    Givaro::Modular<Givaro::Integer> Fcipher(_pk.n);
    Givaro::Modular<Givaro::Integer>::Element_ptr w = FFLAS::fflas_new(Fcipher, m, 1);
    for(size_t i=0; i < m; i++) {
        std::cout << "progress : " << float(i)/float(m-1)*100.0 << " %\r";
        std::cout.flush();
        Fcipher.init(w[i], SMCStrassen::encryption(_pk, v[i]));
    }
    std::cout << std::endl;
    server.send(M, w, _pk.n);
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
    Givaro::Integer delta = SMCStrassen::encryption(_pk, Givaro::Integer(0));
    std::cout << "Waiting for the server" << std::endl;
    Field::Element_ptr y = server.audit(x, delta);
    std::cout << "got y" << std::endl;
//    FFLAS::WriteMatrix(std::cout << "y : " << std::endl, F, 1, _n, y, _n) << std::endl;
    Field::Element_ptr yu = FFLAS::fflas_new(F, 1, 1);
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1,1,_n, F.one, y, _n, _u,1, F.zero, yu,1);
//    FFLAS::WriteMatrix(std::cout << "Vecteur yu : " << std::endl, F, 1,1, yu,1) << std::endl;
    Field::Element_ptr xt = FFLAS::fflas_new(F, 1, 1);
    FFLAS::fgemm(F,FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1, 1,_m, F.one, x, _m, v, 1, F.zero, xt, 1);
    Field::Element xv;
    F.init(xv, SMCStrassen::decryption(delta, _pk, _k));
 //   std::cout << "xv : " << xv << std::endl;
    return xv == yu[0];
}
