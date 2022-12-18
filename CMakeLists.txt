cmake_minimum_required(VERSION 3.24)
project(programming_btc)

set(CMAKE_CXX_STANDARD 23)

add_executable(programming_btc AVLTree.cpp AVLTree.h uint256.h secp256k1.h secp256k1.cpp main.cpp test_main.cpp test_main.h AVLTree.h)

