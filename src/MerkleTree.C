#include "MerkleTree.h"

#include <openssl/evp.h>


//MerkleTree::MerkleTree() : 
//nbleafs(0) {
//    root = 0;
//    EVP_DigestInit_ex(EVP_MD_CTX_create(), EVP_sha3_256(), NULL);
//}
//
//void MerkleTree::addLeafs(std::vector<Givaro::Integer> & leafs) {
//    EVP_MD * algo = NULL;
//
//
//}

//void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len)
//{
//        EVP_MD_CTX *mdctx;
//
//            if((mdctx = EVP_MD_CTX_create()) == NULL)
//                        handleErrors();
//
//                if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
//                            handleErrors();
//
//                    if(1 != EVP_DigestUpdate(mdctx, message, message_len))
//                                handleErrors();
//
//                        if((*digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()))) == NULL)
//                                    handleErrors();
//
//                            if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
//                                        handleErrors();
//
//                                EVP_MD_CTX_destroy(mdctx);
//}
//
