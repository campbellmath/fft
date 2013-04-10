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
static unsigned long int add(unsigned long int x, unsigned long int y, unsigned int bit_length)
{
    return fixed(x+y, bit_length);
}
/*===========================================================================*/
static unsigned long int sub(unsigned long int x, unsigned long int y, unsigned int bit_length)
{
    return add(x, minus(y, bit_length), bit_length);
}
/*===========================================================================*/
static unsigned long int mul(unsigned long int x, unsigned long int y, unsigned int bit_length)
{
    return fixed(x*y, bit_length);
}
/*===========================================================================*/
static unsigned long int signedExtend(unsigned long int x, unsigned int original_bit_length, unsigned int final_bit_length)
{
    for (unsigned i = original_bit_length; i < final_bit_length; i++) {
        x |= ((x>>(original_bit_length-1))&0x1l)<<i;
    }

    return x;
}
/*===========================================================================*/
// default constructor
FixedPoint::FixedPoint(unsigned long int value, unsigned long int bit_length) : m_value(value), m_bit_length(bit_length)
{
    this->m_value = fixed(this->getValue(), this->getBitLength());
}
// destructor
FixedPoint::~FixedPoint() {}

FixedPoint FixedPoint::operator - ()
{
    FixedPoint tmp(minus(this->getValue(), this->getBitLength()), this->getBitLength());

    return tmp;
}
/*===========================================================================*/
FixedPoint FixedPoint::operator + (const FixedPoint & rhs)
{
    unsigned int bit_length = std::max(this->getBitLength(), rhs.getBitLength());

    FixedPoint tmp(add(signedExtend(this->getValue(), this->getBitLength(), bit_length),
                signedExtend(  rhs.getValue(),   rhs.getBitLength(), bit_length), bit_length), bit_length);

    return tmp;
}
/*===========================================================================*/
FixedPoint FixedPoint::operator - (const FixedPoint & rhs)
{
    unsigned int bit_length = std::max(this->getBitLength(), rhs.getBitLength());

    FixedPoint tmp(sub(signedExtend(this->getValue(), this->getBitLength(), bit_length),
                signedExtend(  rhs.getValue(),   rhs.getBitLength(), bit_length), bit_length), bit_length);

    return tmp;
}
/*===========================================================================*/
FixedPoint FixedPoint::operator * (const FixedPoint & rhs)
{
    unsigned int bit_length = std::max(this->getBitLength(), rhs.getBitLength());

    FixedPoint tmp(mul(signedExtend(this->getValue(), this->getBitLength(), bit_length),
                signedExtend(  rhs.getValue(),   rhs.getBitLength(), bit_length), 2*bit_length), bit_length);

    return tmp;
}
/*===========================================================================*/
std::ostream& operator << (std::ostream & out, const FixedPoint & point)
{
    out<<"0x"<<std::hex<<point.getValue()<<std::dec;

    return out;
}
/*===========================================================================*/
