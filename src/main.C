#include "Client.h"
#include <iostream>
#include "Server.h"
#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"



int main(int argc, char *argv[])
{
    Givaro::ModularBalanced<int64_t> F(21);
    Givaro::Integer m(100);
    Givaro::Integer n(100);
    Server s(F, m,n);
    Givaro::ModularBalanced<int64_t>::Element_ptr M = FFLAS::fflas_new(F,10,10);
    typename Givaro::Modular<int64_t>::RandIter G(F);
    FFPACK::RandomMatrix(F, 10, 10, M, 10,G);
    Client client = Client(s, F, M, m, n);
    std::cout << client.audit() << std::endl;
    return 0;
}
