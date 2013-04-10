#ifndef __FIXEDPOINT_H__
#define __FIXEDPOINT_H__

#include <ostream>

/*===========================================================================*/
class FixedPoint {
    public:
        // default constructor
        FixedPoint(unsigned long int value = 0UL, unsigned long int bit_length=0U);
        // destructor
        virtual ~FixedPoint();

        FixedPoint operator - ();
        FixedPoint operator + (const FixedPoint & rhs);
        FixedPoint operator - (const FixedPoint & rhs);
        FixedPoint operator * (const FixedPoint & rhs);

        inline FixedPoint & setValue(unsigned long int value = 0UL) { this->m_value = value; return *this; }
        inline FixedPoint & setBitLength(unsigned long int bit_length= 0UL) { this->m_bit_length = bit_length; return *this; }
        inline unsigned long int getValue(unsigned long int value = 0UL) const { return this->m_value; }
        inline unsigned int getBitLength(unsigned long int bit_length= 0UL) const { return this->m_bit_length; }

    private:
        unsigned long int m_value;
        unsigned int m_bit_length;

}; /* end of class FixedPoint */
/*===========================================================================*/
std::ostream& operator << (std::ostream & out, const FixedPoint & point);
/*===========================================================================*/
#endif /* end of __FIXEDPOINT_H__ */
