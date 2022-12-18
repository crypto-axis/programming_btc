//
// Created by cc1 on 15/12/22.
//

#ifndef SECP256K1_H
#define SECP256K1_H

#include <string>
#include <iostream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/lexical_cast.hpp>
#include <crypto/sha256.h>

namespace bmp = boost::multiprecision;
namespace ba = boost::algorithm;

class FieldElement {

public:
    FieldElement();
    FieldElement(const bmp::uint256_t & num, const bmp::uint256_t & prime);
    ~FieldElement();

    bool operator==(FieldElement const& other) const;
    bool operator!=(FieldElement const& other) const;
    FieldElement operator+(FieldElement const& other) const;
    FieldElement operator-(FieldElement const& other) const;
    FieldElement operator*(FieldElement const& other) const;
    FieldElement operator*(bmp::uint256_t const& coefficient) const;
    FieldElement operator/(FieldElement const& other) const;
    FieldElement pow(bmp::int512_t const& exponent) const;
    std::string str() const;
    bool is_infinity() const;
    void set_infinity();
    void set_infinity(bool b);

    bmp::uint256_t num;
    bmp::uint256_t prime;

protected:
    bool infinity;
};

class S256Field : public FieldElement {
public:
    S256Field(const bmp::uint256_t & num);
    std::string str() const;

    const bmp::uint256_t P = bmp::uint256_t("115792089237316195423570985008687907853269984665640564039457584007908834671663");
};

class Point{
public:
    Point(const FieldElement& x, const FieldElement& y, const FieldElement& a, const FieldElement& b);
    Point(const FieldElement& a, const FieldElement& b);
    Point();
    bool operator==(Point const& other) const;
    bool operator!=(Point const& other) const;
    Point operator+(Point const& other);
    Point operator*(bmp::int512_t const& coefficient);
    std::string str() const;


    FieldElement a;
    FieldElement b;
    FieldElement x;

    FieldElement y;
};

class S256Point : public Point {
public:
    S256Point();
    S256Point(const S256Field& x, const S256Field& y);
    Point operator*(bmp::int512_t const& coefficient);

    const bmp::uint256_t A = 0;
    const bmp::uint256_t B = 7;
    const bmp::uint256_t N = bmp::uint256_t("0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");

};


#endif //SECP256K1_H
