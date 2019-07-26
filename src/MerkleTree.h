#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <iostream>
#include <vector>
#include "fflas-ffpack/fflas-ffpack.h"
#include "givaro/modular.h"
#include "Server.h"

class MerkleTree
{
private:
    unsigned char * root;
    MerkleTree & leftChild;
    MerkleTree & rightchild;
    Givaro::Integer nbleafs;

public:
    MerkleTree();
    
    void addLeafs(std::vector<Givaro::Integer> & leafs);
//    Givaro::Integer getLeaf(Givaro::Integer & i, std::vector<Givaro::Integer> & brothers);
//    void updateBloc(Givaro::Integer & val, Givaro::Integer & i);

    virtual ~MerkleTree();
};

#endif /* MERKLETREE_H */
