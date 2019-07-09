#ifndef SERVER_H
#define SERVER_H

#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"

class Server
{
    typedef Givaro::ModularBalanced<int64_t> Field;
private:
    Field::Element_ptr M;    
    Field F;
    const Givaro::Integer & _m;
    const Givaro::Integer & _n;
public:
    Server(Field & F, const Givaro::Integer & m, const Givaro::Integer & n);
    Server(Server & serv) : F(serv.F), _m(serv._m), _n(serv._n) {
        
    }

    int send(Field::Element_ptr M);    
    Field::Element_ptr audit(Field::Element_ptr x);

    virtual ~Server(){}
};

#endif /* SERVER_H */
