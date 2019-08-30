#include <stdint.h>
#include <iostream>
#include <fstream>  
#include <string>

namespace PorTools {
    
    void createFile(uint64_t size) {
        std::cout << "creating random data " << std::endl;
        uint64_t nbfile = size /(2<<23);
        uint64_t i = 0;
        while(i < nbfile) {
            std::cout << "fichier " << i << std::endl;
            std::ofstream outfile("./data/"+std::to_string(i));           
            for(uint64_t k = 0; k < (2<<23); ++k) {
                uint64_t r = std::rand();
                outfile << r;
            }
            outfile.close();
            ++i;
        }
        nbfile = size < (2<<23) || size%(2<<23) != 0 ? 1 : 0;
        if(nbfile == 1) {
            std::cout << "fichier " << i << std::endl;
            std::ofstream outfile("./data/"+std::to_string(i));           
            for(uint64_t k = 0; k < (size%(2<<23)); ++k) {
                uint64_t r = std::rand();
                outfile << r;
            }
            outfile.close();
        }
    }

}
