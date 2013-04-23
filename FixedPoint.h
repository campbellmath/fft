#ifndef __FIXEDPOINT_H__
#define __FIXEDPOINT_H__

#include <ostream>

typedef unsigned long long int UINT64; 
/*===========================================================================*/
class FixedPoint {
    public:
        // default constructor
        FixedPoint(UINT64 value = 0UL, UINT64 bit_length=0UL);
        // destructor
        virtual ~FixedPoint();

        FixedPoint operator + (const FixedPoint & rhs);
        FixedPoint operator - (const FixedPoint & rhs);
        FixedPoint operator * (const FixedPoint & rhs);
        FixedPoint operator << (const int & shift);
        FixedPoint operator >> (const int & shift);

        FixedPoint & setValue(UINT64 value = 0UL);
        FixedPoint & setBitLength(UINT64 bit_length= 0UL);
        inline UINT64 getValue() const { return this->m_value; }
        inline unsigned int getBitLength() const { return this->m_bit_length; }

    private:
        UINT64 m_value;
        unsigned int m_bit_length;

}; /* end of class FixedPoint */
/*===========================================================================*/
std::ostream& operator << (std::ostream & out, const FixedPoint & point);
/*===========================================================================*/
#endif /* end of __FIXEDPOINT_H__ */
