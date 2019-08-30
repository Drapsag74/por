#include "Client.h"
#include <iostream>
#include <time.h>
#include "Server.h"
#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"
#include "MerkleTree.h"
#include "PorTools.h"

int main(int argc, char *argv[])
{
//    PorTools::createFile(2<<30);
    typedef Givaro::ModularBalanced<double> Field;
    MerkleTree t();
    Givaro::Integer p(2719307);
    Field F(p);
    //1 gbit of data
    size_t m = 1704353;
    size_t n = 30;
    Server s(F, m,n,p);
    Field::Element_ptr M = FFLAS::fflas_new(F,m,n);
    typename Field::RandIter G(F,0);
    FFPACK::RandomMatrix(F, m, n, M, n,G);
//    FFLAS::WriteMatrix(std::cout << "Matrice M : " << std::endl, F, m,n,M,n) << std::endl;
    clock_t t1 = clock();
    Client client = Client(s, F, M, m, n, p);
    clock_t t2 = clock();
    std::cout << "time for init : " << (t2 - t1)/(CLOCKS_PER_SEC) << std::endl;
    t1 = clock();
    std::cout << client.audit() << std::endl;
    t2 = clock();
    std::cout << "time for audit 1 : " << (t2 - t1)/(CLOCKS_PER_SEC) << std::endl;
    t1 = clock();
    std::cout << client.audit() << std::endl;
    t2 = clock();
    std::cout << "time for audit 2 : " << (t2 - t1)/(CLOCKS_PER_SEC) << std::endl;

    return 0;
}
