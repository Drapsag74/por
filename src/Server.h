#ifndef SERVER_H
#define SERVER_H

#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"

class Server
{
    typedef Givaro::ModularBalanced<int64_t> Field;
private:
    Field::Element_ptr M;
    Givaro::Modular<Givaro::Integer>::Element_ptr w;
    Field F;
    size_t  _m;
    size_t _n;
    Givaro::Integer _p;
    Givaro::Integer cipherSize;

public:
    Server(Field & F, size_t  m, size_t n, Givaro::Integer & _p);
    Server(Server & serv) : F(serv.F) {
       _m = serv._m;
       _n = serv._n;
    }

    int send(Field::Element_ptr M,Givaro::Modular<Givaro::Integer>::Element_ptr w, Givaro::Integer & n); 
    Field::Element_ptr audit(Field::Element r, Givaro::Integer & delta);

    virtual ~Server(){}
};

#endif /* SERVER_H */
