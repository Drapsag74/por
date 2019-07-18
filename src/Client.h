
#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"
#include "Server.h"
#include "naccache_stern.h"

class Client
{
    typedef Givaro::ModularBalanced<int64_t> Field;
private:

    Field F;
    size_t _m;
    size_t _n;
    Field::Element_ptr v;
    Field::Element_ptr _u;
    Field::Element_ptr _t;
    Server & server;
    SMCStrassen::pub_key _pk;
    SMCStrassen::priv_key _k;

public:
    Client(Server & server, Field & F, Field::Element_ptr M, size_t  m, size_t n);
    
    bool audit();

    virtual ~Client(){}
};

#endif /* CLIENT_H */
