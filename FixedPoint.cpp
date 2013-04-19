#include "FixedPoint.h"
#include <iostream>

/*===========================================================================*/
static unsigned long int fixed(unsigned long int x, unsigned int bit_length)
{
    unsigned int value = 0;

    for (unsigned int i = 0; i < bit_length; i++) {
        value |= (x&(0x1<<i));
    }

    return value;
}
/*===========================================================================*/
static unsigned long int minus(unsigned long int x, unsigned int bit_length)
{
    return fixed(-x, bit_length);
}
/*===========================================================================*/
static unsigned long int add(
        unsigned long int x,
        unsigned long int y,
        unsigned int bit_length)
{
    return fixed(x+y, bit_length);
}
/*===========================================================================*/
static unsigned long int sub(
        unsigned long int x,
        unsigned long int y,
        unsigned int bit_length)
{
    return add(x, minus(y, bit_length), bit_length);
}
/*===========================================================================*/
static unsigned long int mul(
        unsigned long int x,
        unsigned long int y,
        unsigned int bit_length)
{
    return fixed(x*y, bit_length);
}
/*===========================================================================*/
static unsigned long int signedExtend(
        unsigned long int x,
        unsigned int original_bit_length,
        unsigned int final_bit_length)
{
    for (unsigned i = original_bit_length; i < final_bit_length; i++) {
        //                            -1 for 2's complement MSB
        x |= ((x>>(original_bit_length-1))&0x1l)<<i;
    }

    return x;
}
/*===========================================================================*/
#define GET_BIT(N, X) (((X)>>N)&0x1)
FixedPoint & FixedPoint::setValue(unsigned long int value)
{
    this->m_value = fixed(value, this->m_bit_length);

    return *this;
}
/*===========================================================================*/
FixedPoint & FixedPoint::setBitLength(unsigned long int bit_length)
{
    if (bit_length > this->m_bit_length) {
        this->m_value = signedExtend(this->m_value, this->m_bit_length, bit_length);
    } else {
        this->m_value = fixed(this->m_value, bit_length);
    }


    this->m_bit_length = bit_length;

    return *this;
}
#undef GET_BIT
/*===========================================================================*/
// default constructor
FixedPoint::FixedPoint(unsigned long int value, unsigned long int bit_length) :
    m_value(value),
    m_bit_length(bit_length)
{
    this->m_value = fixed(this->getValue(), this->getBitLength());
}
// destructor
FixedPoint::~FixedPoint() {}
/*===========================================================================*/
FixedPoint FixedPoint::operator - ()
{
    unsigned long int value = minus(this->getValue(), this->getBitLength());
    FixedPoint tmp(value, this->getBitLength());

    return tmp;
}
/*===========================================================================*/
FixedPoint FixedPoint::operator + (const FixedPoint & rhs)
{
    unsigned int bit_length = std::max(this->getBitLength(), rhs.getBitLength());

    unsigned long int l =
        signedExtend(this->getValue(), this->getBitLength(), bit_length);
    unsigned long int r =
        signedExtend(  rhs.getValue(),   rhs.getBitLength(), bit_length);

    FixedPoint tmp(add(l, r, bit_length), bit_length);

    return tmp;
}
/*===========================================================================*/
FixedPoint FixedPoint::operator - (const FixedPoint & rhs)
{
    unsigned int bit_length = std::max(this->getBitLength(), rhs.getBitLength());

    unsigned long int l =
        signedExtend(this->getValue(), this->getBitLength(), bit_length);
    unsigned long int r =
        signedExtend(  rhs.getValue(),   rhs.getBitLength(), bit_length);

    FixedPoint tmp(sub(l, r, bit_length), bit_length);

    return tmp;
}
/*===========================================================================*/
FixedPoint FixedPoint::operator * (const FixedPoint & rhs)
{
    // unsigned int bit_length = std::max(this->getBitLength(), rhs.getBitLength());
    unsigned int bit_length = this->getBitLength()+rhs.getBitLength();

    unsigned long int l =
        signedExtend(this->getValue(), this->getBitLength(), bit_length);
    unsigned long int r =
        signedExtend(  rhs.getValue(),   rhs.getBitLength(), bit_length);

    FixedPoint tmp(mul(l ,r , bit_length), bit_length);

    return tmp;
}
/*===========================================================================*/
std::ostream& operator << (std::ostream & out, const FixedPoint & point)
{
    out<<"0x"<<std::hex<<point.getValue()<<std::dec;

    return out;
}
/*===========================================================================*/
