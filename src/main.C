#include "Client.h"
#include <iostream>
#include "Server.h"
#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"



int main(int argc, char *argv[])
{
    typedef Givaro::ModularBalanced<int64_t> Field;
    Field F(2719307);
    Givaro::Integer m(2<<14);
    Givaro::Integer n(2<<10);
    Server s(F, m,n);
    Field::Element_ptr M = FFLAS::fflas_new(F,m,n);
    typename Field::RandIter G(F,0);
    FFPACK::RandomMatrix(F, m, n, M, n,G);
//    FFLAS::WriteMatrix(std::cout << "Matrice M : " << std::endl, F, m,n,M,n) << std::endl;
    Client client = Client(s, F, M, m, n);
    std::cout << client.audit() << std::endl;
    return 0;
}
