#include "Client.h"
#include <iostream>
#include <givaro/givintnumtheo.h>

Client::Client(Server & server, Field & F, Field::Element_ptr M, size_t m, size_t n, Givaro::Integer p) :
server(server), F(F), _p(p) {
    _m = m;
    _n = n;
    _u = FFLAS::fflas_new(F, _n, 1);
    std::cout << "Creating client" << std::endl;
    SMCStrassen::keyGen(_k, _pk, 2048);
//    FFLAS::WriteMatrix(std::cout << "Matrice : " << std::endl, this->F, m, n, M,n)<< std::endl;
    v = FFLAS::fflas_new(F,m,1);
    for(size_t i = 0; i < m; i++) {
        F.init(v[i], 0);
    }

    typename Field::RandIter G(F,0);
    F.random(G, tau);
    Field::Element_ptr t = FFLAS::fflas_new(F, _m, 1);
    F.init(t[0], tau);
    for(size_t i = 1; i < _m; i++) {
        Field::Element tmp;
        F.mul(tmp, t[i-1], tau);
        F.init(t[i],tmp);
    }

    while(FFLAS::fiszero(F, m,v, 1)) {
        std::cout << "try to create v" << std::endl;
        FFPACK::RandomMatrix(F, n, 1, _u, 1);
        //FFPACK::RandomMatrix(F,m,1,v,1);
        //v=Mu
        FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, _m, 1, _n, F.one, M, _n, _u, 1, F.zero, v, 1);
        // v+=t
        FFLAS::fadd(F, _m, 1, v, 1, F.one, t, 1, v, 1);
    }

    std::cout << "ciphering vecteur v" << std::endl;
    Givaro::Modular<Givaro::Integer> Fcipher(_pk.n);
    Givaro::Modular<Givaro::Integer>::Element_ptr w = FFLAS::fflas_new(Fcipher, _m, 1);
    for(size_t i=0; i < _m; i++) {
        std::cout << "progress : " << float(i)/float(m-1)*100.0 << " %\r";
        std::cout.flush();
        if(v[i] < 0) {
            Fcipher.init(w[i], SMCStrassen::encryption(_pk,v[i]  + p));
        } else {
            Fcipher.init(w[i], SMCStrassen::encryption(_pk, v[i]));
        }
    }
//    std::cout << std::endl;
//    for(size_t i=0; i < _m; i++) {
//        std::cout << SMCStrassen::decryption(w[i], _pk, _k) << std::endl;
//    }
//    
    server.send(M, w, _pk.n);
//    FFLAS::WriteMatrix(std::cout << "vecteur u : " << std::endl, F, n,1, _u, 1) << std::endl; 
//    FFLAS::WriteMatrix(std::cout << "vecteur v : " << std::endl, F, m,1, v, 1) << std::endl;
    std::cout << "Client created" << std::endl;
    
}


bool Client::audit() {
    std::cout << "Begin audit" << std::endl;
    Field::Element r;
    typename Field::RandIter G(F,0);
    F.random(G, r);
    
//    FFLAS::WriteMatrix(std::cout << "vecteur x : " << std::endl,F, 1, _m, x, _m) << std::endl;
    Givaro::Integer delta = SMCStrassen::encryption(_pk, Givaro::Integer(0));
    std::cout << "Waiting for the server" << std::endl;
    Field::Element_ptr y = server.audit(r, delta);
    std::cout << "got y" << std::endl;
//    FFLAS::WriteMatrix(std::cout << "y : " << std::endl, F, 1, _n, y, _n) << std::endl;
    Field::Element_ptr yu = FFLAS::fflas_new(F, 1, 1);
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1,1,_n, F.one, y, _n, _u,1, F.zero, yu,1);
//    FFLAS::WriteMatrix(std::cout << "Vecteur yu : " << std::endl, F, 1,1, yu,1) << std::endl;
    Field::Element xv;
    F.init(xv, SMCStrassen::decryption(delta, _pk, _k));
    Field::Element xt;
    Givaro::Integer rc, tauc;
    F.convert(rc, r);
    F.convert(tauc, tau);
    Givaro::Integer rtau = rc*tauc;
    std::cout << "rtau : " << rtau << " r : " << r << " tau : " << tau << std::endl;
    //F.div(xt, (1 -Givaro::pow(rtau, _m)), 1 - rtau);
    //F.mul(xt, xt, rtau);
   // std::cout << "xt : " << rtau*((1-Givaro::pow(rtau, _m))/(1-rtau))  << " yu : " << yu[0] << std::endl;
    F.init(yu[0], yu[0] + rtau*((1-Givaro::pow(rtau, _m))/(1-rtau)));

 //   std::cout << "xv : " << xv << " yu : " << yu[0] << std::endl;
    Field::Element_ptr x = FFLAS::fflas_new(F, 1, _m); 
    F.init(x[0], r);
    for(size_t i = 1; i < _m; i++) {
        Field::Element tmp;
        F.mul(tmp, x[i-1], r);
        F.init(x[i],tmp);
    }
 //   std::cout << "xv : " << xv << std::endl;
    return xv == yu[0];
}

//void Client::update(size_t i, size_t j) {
//    coef = Server::request(i,j);
//
//}
