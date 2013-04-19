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

        FixedPoint & setValue(unsigned long int value = 0UL);
        FixedPoint & setBitLength(unsigned long int bit_length= 0UL);
        inline unsigned long int getValue() const { return this->m_value; }
        inline unsigned int getBitLength() const { return this->m_bit_length; }

    private:
        unsigned long int m_value;
        unsigned int m_bit_length;

}; /* end of class FixedPoint */
/*===========================================================================*/
std::ostream& operator << (std::ostream & out, const FixedPoint & point);
/*===========================================================================*/
#endif /* end of __FIXEDPOINT_H__ */
