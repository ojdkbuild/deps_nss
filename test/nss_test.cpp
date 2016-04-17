
#include <iostream>
#include <cstdio>

#include "blapi.h"
#include "secitem.h"
#include "secerr.h"
#include "keythi.h"


int main() {

    SECStatus init_status = SECOID_Init();
    std::cout << init_status << std::endl;

    SECStatus rng_init_status = RNG_RNGInit();
    std::cout << rng_init_status << std::endl;

    // 1.2.840.10045.3.1.7
    char oid[10];
    oid[0] = 6;
    oid[1] = 8;
    oid[2] = 42;
    oid[3] = -122;
    oid[4] = 72;
    oid[5] = -50;
    oid[6] = 61;
    oid[7] = 3;
    oid[8] = 1;
    oid[9] = 7;
    SECKEYECParams params_item;
    params_item.len = 10;
    params_item.data = reinterpret_cast<unsigned char*>(&oid);
    ECParams* ecparams = NULL;
    SECStatus decode_status = EC_DecodeParams(&params_item, &ecparams);
    std::cout << decode_status << std::endl;

    ECPrivateKey* privKey = NULL;
    SECStatus key_status = EC_NewKey(ecparams, &privKey);
    std::cout << key_status << std::endl;
    
    puts("EXIT");
    //getchar();
    return 0;
}
