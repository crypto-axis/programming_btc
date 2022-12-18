//
// Created by cc1 on 15/12/22.
//

#include "secp256k1.h"

FieldElement::FieldElement() {
    this->num = 1;
    this->prime =0;
    this->infinity = true;
}

FieldElement::FieldElement(const bmp::uint256_t& num, const bmp::uint256_t& prime) {
    try{
        if (num >= prime) throw std::string("num not in field range:(");
        this->num = num;
        this->prime = prime;
        this->infinity = false;
    }
    catch(std::string const& e){std::cout << e << num << ", " << prime << ")" << std::endl;}
}

FieldElement::~FieldElement() = default;

std::string FieldElement::str() const {
    std::string out = "FieldElement_" + boost::lexical_cast<std::string>(prime) + "{" + boost::lexical_cast<std::string>(num) + "}";
    return out;
}

bool FieldElement::operator==(const FieldElement &other) const {
    if (this->prime == other.prime) {
        if (this->num == other.num) return true;
        if (this->is_infinity() and other.is_infinity()) return true;
    }
    return false;

}

bool FieldElement::operator!=(const FieldElement &other) const {
    if (*this == other) return false;
    else return true;

}

FieldElement FieldElement::operator+(const FieldElement &other) const {
    try{
        if (this->prime != other.prime) throw std::string("primes not match");
        return {(this->num + other.num)%this->prime, this->prime};
    }

    catch(std::string const& e) { std::cerr << e ;}

    return {};
}

FieldElement FieldElement::operator-(const FieldElement &other) const {
    try{
        if (this->prime != other.prime) throw std::string("primes not match!");
        bmp::int512_t a,b,c;
        a = bmp::int512_t(this->num);
        b = bmp::int512_t(other.num);
        c = bmp::int512_t(this->prime);

        bmp::int512_t n = (a - b)%c;
        if (n<0) n = n + this->prime;

        auto out = bmp::uint256_t(n);

        return {out, this->prime};
    }
    catch(std::string const& e){std::cerr << e; return {};}

}

FieldElement FieldElement::operator*(const FieldElement &other) const {
    try{
        if (this->prime != other.prime) throw std::string("primes not match!");
        return {(this->num * other.num)%this->prime, this->prime};
    }
    catch(std::string const& e){std::cerr << e;return {};}

}

FieldElement FieldElement::operator*(const boost::multiprecision::uint256_t &coefficient) const {
    bmp::uint256_t _num = (this->num * coefficient) % this->prime;
    return {_num, this->prime};
}
FieldElement FieldElement::pow(const bmp::int512_t &exponent) const {

    bmp::int512_t exp = exponent;
    while (exp < 0){
        exp += this->prime -1;
    }
    bmp::uint256_t _num = bmp::powm(this->num,bmp::uint256_t(exp),this->prime);
    return {_num, this->prime};
}

FieldElement FieldElement::operator/(const FieldElement &other) const {
    try{
            if (this->prime != other.prime) throw std::string("primes not match!");
            bmp::uint256_t _num = (this->num * bmp::powm(other.num, this->prime - 2,
                                                         this->prime)% this->prime);
            return {_num, this->prime};
    }
    catch(std::string const& e){std::cerr << e;return {};}
}

bool FieldElement::is_infinity() const {
    return this->infinity & this->num == this->prime +1;
}

void FieldElement::set_infinity(bool b) {
    this->infinity = b;
    if (b) this->num = this->prime +1;
}

void FieldElement::set_infinity() {
    this->set_infinity(true);
}


S256Field::S256Field(const boost::multiprecision::uint256_t &num) {
    try{
        if (num >= P) throw std::string("num not in field range:(");
        this->num = num;
        this->prime = P;
        this->infinity = false;
    }
    catch(std::string const& e){std::cout << e << num << ", " << prime << ")" << std::endl;}

}


Point::Point(const FieldElement& x, const FieldElement& y, const FieldElement& a, const FieldElement& b) {
    this->x = x;
    this->y = y;
    this->a = a;
    this->b = b;

    if (x.is_infinity() and y.is_infinity()) return;

    // check the point is on the curve
    try{
        if ((y*y) != (x*x*x) + (a * x) + b) throw std::string("except");
    }
    catch (std::string){
        std::cout << "Point not on curve! => " ;
        std::cout << this->str() << std::endl;
        throw -1;
     }

}

Point::Point(const FieldElement& a, const FieldElement& b) {

    this->x = {0,a.prime};
    this->y = {1,a.prime};
    this->a = a;
    this->b = b;

}

Point::Point()= default;

bool Point::operator==(const Point &other) const {

    // point have no curve param
    if (this->a.is_infinity() or this->b.is_infinity()) return false;
    if (other.a.is_infinity() or other.b.is_infinity()) return false;

    if(this->x == other.x and this->y == other.y and this->a == other.a and this->b == other.b )
        return true;
    else return false;
}

bool Point::operator!=(const Point &other) const {
    // point have no curve param
    if (this->a.is_infinity() or this->b.is_infinity()) return true;
    if (other.a.is_infinity() or other.b.is_infinity()) return true;
    if (*this == other) return false;
    else return true;
}

std::string Point::str() const {
    std::string out = "Point(x=" + this->x.str() + ", y="
            + this->y.str() + ") [a="
            + this->a.str() + ") , b="
            + this->b.str() + "]" ;
    return out;
}

Point Point::operator+(const Point &other) {

    // point have no curve param
    if (this->a.is_infinity() or this->b.is_infinity()) return {};
    if (other.a.is_infinity() or other.b.is_infinity()) return {};

    FieldElement inf = {0, this->x.prime};
    inf.set_infinity();

    try{
        // case 0.0
        if (this->a != other.a or this->b != other.b) throw std::string("a or b not match!");

        //case 0.1
        if (this->x.is_infinity()) return other;
        if (other.x.is_infinity()) return *this;

        //case 1
        if (this->x == other.x and this->y != other.y) {

            return {inf, inf, this->a, this->b};
        }

        //case2
        if (this->x != other.x){
            FieldElement _s = (other.y - this->y) / (other.x - this->x);
            FieldElement _x = _s.pow(2) - this-> x - other.x;
            FieldElement _y = _s * (this->x - _x) - this->y;
            return {_x, _y, this->a, this->b};
        }

        //case3
        if (*this == other){

            FieldElement _s = ( this->x.pow(2) * 3 + this->a) / (this->y * 2);
            FieldElement _x = _s.pow(2) -  this->x * 2;
            FieldElement _y = _s * (this->x - _x) - this->y;
            return {_x, _y, this->a, this->b};
        }

        //case4
        if (*this == other and this->y == this->x * 0)return {inf,inf,this->a,this->b};

    }
    catch(std::string const& e){std::cerr << e;return {};}

}

Point Point::operator*(const bmp::int512_t &coefficient) {

    // point have no curve param
    if (this->a.is_infinity() or this->b.is_infinity()) return *this;

    bmp::int512_t coef = coefficient;

    Point current = *this;
    FieldElement _x = {0, this->a.prime};
    _x.set_infinity();
    FieldElement _y = {0, this->a.prime};
    _y.set_infinity();

    Point result = {_x, _y, this->a, this->b};

    while(coef){
        if (coef & 1)result = result +  current;
        current = current + current;
        coef = coef >> 1;
    }
    return result;

}


S256Point::S256Point() = default;

S256Point::S256Point(const S256Field &x, const S256Field &y) {
    this->x = x;
    this->y = y;
    this->a = S256Field(A);
    this->b = S256Field(B);

    if (x.is_infinity() and y.is_infinity()) return;

    // check the point is on the curve
    try{
        if ((y*y) != (x*x*x) + (a * x) + b) throw std::string("except");
    }
    catch (std::string){
        std::cout << "Point not on curve! => " ;
        std::cout << this->str() << std::endl;
        throw -1;
    }


}
