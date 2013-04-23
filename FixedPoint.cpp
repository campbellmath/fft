#include "FixedPoint.h"
#include <iostream>
#define BIT(N, x) (((x)>>(N))&0x1)
/*===========================================================================*/
static UINT64 clearUpperBits(UINT64 value, UINT64 n)
{
    UINT64 mask = 0;
    printf("------------------------------------------------\n");
    printf("n= %16llx \n", n);
    printf("mask = %16llx \n", mask);
    for (UINT64 idx = 0; idx < n ; idx++) {
        mask |= (0x1 << idx);
    }
    printf("mask = %16llx \n", mask);

    printf("value = %16llx \n", value);
    printf("value&mask = %16llx \n", value&mask);
    printf("------------------------------------------------\n");
    return (value&mask);
}
/*===========================================================================*/
static UINT64 fixed(UINT64 x, unsigned int bit_length)
{
    UINT64 value = 0;

    for (unsigned int i = 0; i < bit_length; i++) {
        value |= (x&(0x1<<i));
    }

    return value;
}
/*===========================================================================*/
static UINT64 add(
        UINT64 x,
        UINT64 y,
        unsigned int bit_length)
{
    UINT64 value = clearUpperBits(x+y, bit_length);

    return fixed(value, bit_length);
}
/*===========================================================================*/
static UINT64 sub(
        UINT64 x,
        UINT64 y,
        unsigned int bit_length)
{
    y = clearUpperBits(y, bit_length);
    y = ~y+1;
    y = clearUpperBits(y, bit_length);

    return add(x, y, bit_length);
}
/*===========================================================================*/
static UINT64 signedExtend(
        UINT64 x,
        unsigned int original_bit_length,
        unsigned int final_bit_length)
{
    UINT64 mask = 0;
    for (unsigned int i = 0; i < final_bit_length; i++) {
        mask |= 0x1<<i;
    }
    // set high bits to 0 ( > final_bit_length)
    x &= mask;;
    for (unsigned int i = original_bit_length; i < final_bit_length; i++) {
        //                            -1 for 2's complement MSB
        x |= ((x>>(original_bit_length-1))&0x1l)<<i;
    }

    return x;
}
/*===========================================================================*/
FixedPoint & FixedPoint::setValue(UINT64 value)
{
    this->m_value = fixed(value, this->m_bit_length);

    this->m_value = clearUpperBits(this->m_value, this->m_bit_length);
    return *this;
}
/*===========================================================================*/
FixedPoint & FixedPoint::setBitLength(UINT64 bit_length)
{
    if (bit_length > this->m_bit_length) {
        this->m_value = signedExtend(this->m_value, this->m_bit_length, bit_length);
    } else {
        this->m_value = fixed(this->m_value, bit_length);
    }

    this->m_bit_length = bit_length;

    this->m_value = clearUpperBits(this->m_value, this->m_bit_length);

    return *this;
}
/*===========================================================================*/
// default constructor
FixedPoint::FixedPoint(UINT64 value, UINT64 bit_length) :
    m_value(value),
    m_bit_length(bit_length)
{
    this->m_value = fixed(this->getValue(), this->getBitLength());
}
// destructor
FixedPoint::~FixedPoint() {}
/*===========================================================================*/
FixedPoint FixedPoint::operator + (const FixedPoint & rhs)
{
    unsigned int bit_length = std::max(this->getBitLength(), rhs.getBitLength());

    UINT64 l =
        signedExtend(this->getValue(), this->getBitLength(), bit_length);
    UINT64 r =
        signedExtend(  rhs.getValue(),   rhs.getBitLength(), bit_length);

    FixedPoint tmp(add(l, r, bit_length), bit_length);

    return tmp;
}
/*===========================================================================*/
FixedPoint FixedPoint::operator - (const FixedPoint & rhs)
{
    unsigned int bit_length = std::max(this->getBitLength(), rhs.getBitLength());

    UINT64 l =
        signedExtend(this->getValue(), this->getBitLength(), bit_length);
    UINT64 r =
        signedExtend(  rhs.getValue(),   rhs.getBitLength(), bit_length);

    FixedPoint tmp(sub(l, r, bit_length), bit_length);

    return tmp;
}
/*===========================================================================*/
FixedPoint FixedPoint::operator * (const FixedPoint & rhs)
{
    UINT64 bit_length = this->getBitLength()+rhs.getBitLength();

    UINT64 l = this->getValue();
    UINT64 r =   rhs.getValue();
    UINT64 mul = 0L;

    // if negative,  change to positive 
    if ( BIT(this->getBitLength()-1, this->getValue()) == 1) {
        l = clearUpperBits(l, this->getBitLength());
        l = ~l+1UL;
        l = clearUpperBits(l, this->getBitLength());
    }
    if ( BIT(rhs.getBitLength()-1, rhs.getValue()) == 1) {
        r = clearUpperBits(r, rhs.getBitLength());
        r = ~r+1UL;
        r = clearUpperBits(r, rhs.getBitLength());
    }

    // if one of l and r is negative, change to negative
    mul = ((( BIT(this->getBitLength()-1, this->getValue()) == 1)^
                ( BIT(  rhs.getBitLength()-1,   rhs.getValue()) == 1) )==1) ? -(l*r) : l*r ;

    printf("mul = %16llx bit_length = %d \n", mul, bit_length);
    printf("this->getBitLength()= %d\n", this->getBitLength());
    printf("rhs.getBitLength() = %d\n", rhs.getBitLength());
    mul = clearUpperBits(mul, bit_length);
    printf("mul = %16llx bit_length = %d \n", mul, bit_length);

    return FixedPoint(mul, bit_length);

}
/*===========================================================================*/
FixedPoint FixedPoint::operator << (const int & shift)
{
    FixedPoint tmp(this->m_value, this->m_bit_length);
    tmp.m_value<<=shift;
    tmp.m_bit_length+=shift;
    tmp.m_value = clearUpperBits(tmp.m_value, tmp.m_bit_length);;

    return tmp;
}
/*===========================================================================*/
FixedPoint FixedPoint::operator >> (const int & shift)
{
    FixedPoint tmp(this->m_value, this->m_bit_length);
    tmp.m_value>>=shift;
    tmp.m_bit_length-=shift;
    tmp.m_value = clearUpperBits(tmp.m_value, tmp.m_bit_length);;

    return tmp;
}
/*===========================================================================*/
#undef BIT
/*===========================================================================*/
std::ostream& operator << (std::ostream & out, const FixedPoint & point)
{
    out<<"0x"<<std::hex<<point.getValue()<<std::dec;

    return out;
}
/*===========================================================================*/
