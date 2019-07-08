#ifndef SERVER_H
#define SERVER_H

#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"

class Server
{
private:
    Givaro::ModularBalanced<int64_t>::Element_ptr M;    
    Givaro::ModularBalanced<int64_t> F;
    const Givaro::Integer & _m;
    const Givaro::Integer & _n;
public:
    Server(Givaro::ModularBalanced<int64_t> & F, const Givaro::Integer & m, const Givaro::Integer & n);
    Server(Server & serv) : F(serv.F), _m(serv._m), _n(serv._n) {
        
    }

    int send(Givaro::ModularBalanced<int64_t>::Element_ptr M);    
    Givaro::ModularBalanced<int64_t>::Element_ptr audit(Givaro::ModularBalanced<long int>::Element_ptr x);

    virtual ~Server(){}
};

#endif /* SERVER_H */
