//
// Created by cc1 on 16/12/22.
//
#include "secp256k1.h"
#include <cassert>

void test_fieldelement(){

    // == !=
    FieldElement a = {2,31};
    FieldElement b = {2,31};
    FieldElement c = {15,31};
    FieldElement d = {15,31};


    assert(a == b);
    assert(a != c);
    assert(!(a != b));

    // add
    a = {2,31};
    b = {15,31};

    assert((a + b) == FieldElement(17,31));

    a = {17,31};
    b = {21,31};

    assert((a + b) == FieldElement(7,31));

    a = {29,31};
    b = {4,31};

    // sub
    assert((a - b) == FieldElement(25,31));

    a = {15,31};
    b = {30,31};

    assert((a - b) == FieldElement(16,31));

    //mul
    a = {24,31};
    b = {19,31};

    assert((a * b) == FieldElement(22,31));

    // pow
    a = {17,31};

    assert((a.pow(3)) == FieldElement(15,31));

    a = {5,31};
    b = {18,31};

    assert((a.pow(5) * b) == FieldElement(16,31));

    //div
    a = {3,31};
    b = {24,31};
    assert((a / b) == FieldElement(4,31));

    a = {17,31};
    assert((a.pow(-3)) == FieldElement(29,31));

    a = {4,31};
    b = {11,31};
    assert((a.pow(-4) * b) == FieldElement(13,31));

    std::cout << "test_field_element passed!" << std::endl;

}

void test_point() {

    bmp::uint256_t prime = 223;

    FieldElement a = {0, prime};
    FieldElement b = {7, prime};

    // point on curve
    FieldElement x = {192, prime};
    FieldElement y = {105, prime};
    Point p = {x, y, a, b};

    x = {17, prime};
    y = {56, prime};
    p = {x, y, a, b};

    x = {17, prime};
    y = {56, prime};
    p = {x, y, a, b};

    x = {1, prime};
    y = {193, prime};
    p = {x, y, a, b};

}

void test_point_not_on_curve(){
    bmp::uint256_t prime = 223;

    FieldElement a = {0, prime};
    FieldElement b = {7, prime};

    // point on curve
    FieldElement x = {192, prime};
    FieldElement y = {105, prime};
    Point p = {x, y, a, b};
    // point not on curve
    try{
        x = {200, prime};
        y = {119, prime};
        p = {x, y, a, b};

        throw 0;
    }

    catch(int i){
        if ( i == -1) i = -2;
        if ( i == 0) {
            std::cerr << "point_not_on_curve fail!" << std::endl;
            throw "point_not_on_curve fail!"; }

    }

    try{
        x = {42, prime};
        y = {99, prime};
        p = {x, y, a, b};

        throw 0;
    }

    catch(int i){
        if ( i == -1) i = -2;
        if ( i == 0) {
            std::cerr << "point_not_on_curve fail!" << std::endl;
            throw "point_not_on_curve fail!"; }

    }

    //addition


    std::cout << "test_point passed!" << std::endl;


}

void test_point_add(){

    bmp::uint256_t prime = 223;
    FieldElement a ={0, prime};
    FieldElement b ={7, prime};

    std::vector<std::vector<bmp::uint256_t>> v;
    v.push_back(std::vector<bmp::uint256_t>{192, 105, 17, 56, 170, 142});
    v.push_back(std::vector<bmp::uint256_t>{47, 71, 117, 141, 60, 139});
    v.push_back(std::vector<bmp::uint256_t>{143, 98, 76, 66, 47, 71});

    FieldElement x1 = {};
    FieldElement y1= {};

    FieldElement x2= {};
    FieldElement y2= {};

    FieldElement x3= {};
    FieldElement y3= {};

    FieldElement infinity = {0,prime};
    infinity.set_infinity();

    Point p1 = {a,b};
    Point p2 = {a,b};
    Point p3 = {a,b};

    for (auto i: v){

        x1 = {i[0], prime} ;
        y1 = {i[1], prime} ;
        p1 = {x1,y1,a,b};

        x2 = {i[2], prime} ;
        y2 = {i[3], prime} ;
        p2 = {x2,y2,a,b};

        x3 = {i[4], prime} ;
        y3 = {i[5], prime} ;
        p3 = {x3,y3,a,b};

        assert( p1 + p2 == p3);

    }
    std::cout << "test_point_add passed!" << std::endl;

}

void test_point_mult(){

    bmp::uint256_t prime = 223;
    FieldElement a ={0, prime};
    FieldElement b ={7, prime};

    std::vector<std::vector<bmp::uint256_t>> v;
    v.push_back(std::vector<bmp::uint256_t>{2,192,105,49,71});
    v.push_back(std::vector<bmp::uint256_t>{2,143,98,64,168});
    v.push_back(std::vector<bmp::uint256_t>{2, 47, 71, 36, 111});
    v.push_back(std::vector<bmp::uint256_t>{4, 47, 71, 194, 51});
    v.push_back(std::vector<bmp::uint256_t>{8, 47, 71, 116, 55});
    v.push_back(std::vector<bmp::uint256_t>{21, 47, 71,0,0});

    FieldElement x1 = {};
    FieldElement y1= {};
    FieldElement x2= {};
    FieldElement y2= {};
    FieldElement infinity = {0,prime};
    infinity.set_infinity();

    Point p1 = {a,b};
    Point p2 = {a,b};

    for (auto i: v){
//        std::cout << i[0] << "-" << i[1] << "-" << i[2] << "-" << i[3] << "-" << i[4] << "-" << std::endl;
        bmp::uint256_t s = i[0];

        x1 = {i[1], prime} ;
        y1 = {i[2], prime} ;
        p1 = {x1,y1,a,b};

        if (i[3] != 0){
            x2 = {i[3], prime} ;
            y2 = {i[4], prime} ;
            p2 = {x2,y2,a,b};
        }
        else p2 = {infinity,infinity,a,b};

//        std::cout << (p1 * s).str() << " - " << p2.str() << std::endl;

        assert((p1 * s == p2));
//        std::cout << "---------------" << std::endl;

    }
    std::cout << "test_point_mult passed!" << std::endl;

}

int test_main(){
    test_fieldelement();
    test_point();
    test_point_not_on_curve();
    test_point_add();
    test_point_mult();

    std::cout << "All test passed successfully!" << std::endl;
    return 0;
}
