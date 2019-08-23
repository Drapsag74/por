#include "Client.h"
#include <iostream>
#include "Server.h"
#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"
#include "MerkleTree.h"


int main(int argc, char *argv[])
{
    typedef Givaro::ModularBalanced<double> Field;
    MerkleTree t();
    Givaro::Integer p(2719307);
    Field F(p);
    size_t m = 2<<10;
    size_t n = 10;
    Server s(F, m,n,p);
    Field::Element_ptr M = FFLAS::fflas_new(F,m,n);
    typename Field::RandIter G(F,0);
    FFPACK::RandomMatrix(F, m, n, M, n,G);
//    FFLAS::WriteMatrix(std::cout << "Matrice M : " << std::endl, F, m,n,M,n) << std::endl;
    Client client = Client(s, F, M, m, n, p);
    std::cout << client.audit() << std::endl;
    std::cout << client.audit() << std::endl;
    return 0;
}
