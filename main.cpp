//
// Created by cc1 on 16/12/22.
//
#include "secp256k1.h"
#include "test_main.h"

int main(){

    test_main();

    PVKey priv = {"0x483ada7726a3c4655da4fbfc0e1108a8fd1715896548ffb10d4b8"};

    std::cout << priv.pb_compressed() << std::endl;
    std::cout << priv.pb_uncompressed() << std::endl;

    PVKey test = PVKey::from_uncompressed("0415649eda6359123fbb2d97d11faa1c2b59be640653213a117dccc8eaed5d2da79c87abfd8e853c41ee6ad666df7948b7d8c182e4fc625e047ac4c5208d9b91ea");

    std::cout << "------------" << std::endl;

    std::cout << test.pb_uncompressed() << std::endl;
    std::cout << test.pb_compressed() << std::endl;





    return 0;
}
