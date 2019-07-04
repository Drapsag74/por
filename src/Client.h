
#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"
#include "Server.h"

class Client
{
private:

    Givaro::ModularBalanced<int64_t> F;
    const Givaro::Integer & _m;
    const Givaro::Integer & _n;
    Givaro::ModularBalanced<int64_t>::Element_ptr v;
    Givaro::ModularBalanced<int64_t>::Element_ptr _u;
    Server & server;

public:
    Client(Server & server, Givaro::ModularBalanced<int64_t> & F, Givaro::ModularBalanced<int64_t>::Element_ptr M, const Givaro::Integer & m, const Givaro::Integer & n);
    
    bool audit();

    virtual ~Client(){}
};

#endif /* CLIENT_H */
