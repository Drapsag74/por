#include "Client.h"
#include <iostream>
#include <givaro/givintnumtheo.h>
#include <time.h>

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
        if(i%(100) == 0) {
            std::cout << "progress : " << float(i)/float(m-1)*100.0 << " %\r";
            std::cout.flush();
        }
        Givaro::Integer vi;
        F.convert(vi,v[i]);
        if(vi<0) {
            Fcipher.init(w[i], SMCStrassen::encryption(_pk,vi + _p));
        } else {
            Fcipher.init(w[i], SMCStrassen::encryption(_pk,vi));
        }
    }
    std::cout << std::endl;
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
    clock_t t1 = clock();
    Field::Element r;
    typename Field::RandIter G(F,0);
    F.random(G, r);
//    FFLAS::WriteMatrix(std::cout << "vecteur x : " << std::endl,F, 1, _m, x, _m) << std::endl;
    Givaro::Integer delta = SMCStrassen::encryption(_pk, Givaro::Integer(0));
    clock_t t2 = clock();
    clock_t elapsed_other = t2 - t1;
    std::cout << "Waiting for the server" << std::endl;
    Field::Element_ptr y = server.audit(r, delta);
    std::cout << "got y" << std::endl;
//    FFLAS::WriteMatrix(std::cout << "y : " << std::endl, F, 1, _n, y, _n) << std::endl;
    t1 = clock();
    Field::Element_ptr yu = FFLAS::fflas_new(F, 1, 1);
    FFLAS::fgemm(F, FFLAS::FflasNoTrans, FFLAS::FflasNoTrans, 1,1,_n, F.one, y, _n, _u,1, F.zero, yu,1);
//    FFLAS::WriteMatrix(std::cout << "Vecteur yu : " << std::endl, F, 1,1, yu,1) << std::endl;
    Field::Element xv;
    F.init(xv, SMCStrassen::decryption(delta, _pk, _k));
    Field::Element rtau;
    F.mul(rtau, r, tau);
    Field::Element xt;
    F.init(xt, rtau);
//    std::cout << "rtau : " << F.convert(rtau) << " r : " << r << " tau : " << tau << std::endl;
    Field::Element tmp;
    Field::Element rtaui;
    F.convert(rtaui, rtau);
    F.add(tmp, F.mOne, Givaro::powmod(rtaui, _m%(_p-1),_p));
    F.mulin(xt, tmp);
    F.add(tmp, F.mOne, rtau);
    F.divin(xt, tmp);
//    std::cout << "xt : " <<   << " yu : " << yu[0] << std::endl;
    F.addin(yu[0],xt);
//    F.init(yu[0], F.addin(yu[0] + rtau*((1-Givaro::pow(rtau, _m))/(1-rtau)));

 //   std::cout << "xv : " << xv << " yu : " << yu[0] << std::endl;
 //   std::cout << "xv : " << xv << std::endl;

    Givaro::Integer ri, taui, xti;
    F.convert(ri, r);
    F.convert(taui, tau);
    F.convert(xti,xt);
    t2 = clock();
    elapsed_other += t2 - t1;
    std::cout << "time for other : " << elapsed_other/(CLOCKS_PER_SEC) << std::endl;
    std::cout << "r : " << ri << " tau : " << taui << " rtau : " << rtaui << " xt : " << xti << std::endl;
    return xv == yu[0];
}

//void Client::update(size_t i, size_t j) {
//    coef = Server::request(i,j);
//
//}
