
#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"
#include "Server.h"

class Client
{
    typedef Givaro::ModularBalanced<int64_t> Field;
private:

    Field F;
    const Givaro::Integer & _m;
    const Givaro::Integer & _n;
    Field::Element_ptr v;
    Field::Element_ptr _u;
    Server & server;

public:
    Client(Server & server, Field & F, Field::Element_ptr M, const Givaro::Integer & m, const Givaro::Integer & n);
    
    bool audit();

    virtual ~Client(){}
};

#endif /* CLIENT_H */
