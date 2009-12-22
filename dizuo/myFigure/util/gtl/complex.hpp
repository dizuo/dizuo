/*
_______________________________________________________________________
__________________________ G E O M E T R Y ____________________________
|
| THIS FILE IS PART OF THE GEOMETRY TEMPLATE LIBRARY.
| USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     
| GOVERNED BY A BSD-STYLE SOURCE LICENSE.
| PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       
_______________________________________________________________________
_______________________________________________________________________
*/

#ifndef COMPLEX_H
#define COMPLEX_H

#include "gtl.hpp"

namespace gtl
{
    /*!
    \class Complex Complex.hpp geometry/Complex.hpp
    \brief Represents a complex number.
    \ingroup base
    */
    template<typename Type>
    class Complex
    {
    public:
        //! Constructs an instance using given real and imaginary values.
        Complex(Type real, Type imaginary)
        {
            setValue(real, imaginary);
        }

        //! Constructs an instance using values from a given complex instance
        Complex(const Complex<Type> & c)
        {
            setValue(c.m_real, c.m_imag);
        }

        //! Default destructor does nothing.
        virtual ~Complex(){}

        //! Set new real and imaginary values for the complex. Returns reference to self.
        Complex<Type> & setValue(Type real, Type imaginary)
        {
            m_real = real;
            m_imag = imaginary;
            return *this;
        }

        //! Gets the real value of the complex number.
        const Type & getReal() const
        { 
            return m_real; 
        }

        //! Sets the real value of the complex number.
        void setReal(Type real)
        { 
            m_real = real; 
        }

        //! Gets the imaginary value of the complex number.
        const Type & getImaginary() const
        { 
            return m_imag; 
        }

        //! Sets the imaginary value of the complex number.
        void setImaginary(Type imaginary)
        { 
            m_imag = imaginary; 
        }

        //! Negates the complex number.
        void negate()
        {
            setValue(-m_real, -m_imag);
        }

        //! Check for equality with given tolerance.
        bool equals(const Complex<Type> & a_complex, const Type a_tolerance=1E-2) const
        {
            return ((std::abs(m_real - a_complex.m_real) <= a_tolerance) &&
                    (std::abs(m_imag - a_complex.m_imag) <= a_tolerance));
        }

        //! Gets the modulus of the complex number.
        Type modulus() const
        {
            return (Type)std::sqrt( sqrModulus() );
        }

        //! Gets the squared modulus of the complex number.
        Type sqrModulus() const
        {
            return (m_real * m_real + m_imag * m_imag);
        }

        //! Returns the complex conjugate.
        Complex<Type> getConjugate() const
        {
            return Complex<Type>(m_real, -m_imag);
        }

        //! Normalize the complex number.
        void normalize()
        {
            Type inv_modulus = (Type)(1.0 / modulus());

            m_real	= m_real * inv_modulus;
            m_imag	= m_imag * inv_modulus;
        }

        //! Check the two given complex for equality. 
        friend bool operator ==(const Complex<Type> & c1, const Complex<Type> & c2)
        { 
            return c1.m_real==c2.m_real && c1.m_imag==c2.m_imag; 
        }

        //! Check the two given complex for inequality. 
        friend bool operator !=(const Complex<Type> & c1, const Complex<Type> & c2)
        { 
            return !(c1 == c2); 
        }

        //! Non-destructive negation operator.
        Complex<Type> operator-() const
        { 
            return Complex<Type>(-m_real, -m_imag); 
        }

        Complex<Type> & operator *=(const Complex<Type> & c)
        {
            m_real += m_real * c.m_real - m_imag * c.m_imag;
            m_imag += m_real * c.m_imag + m_imag * c.m_real;
            return *this;
        }

        Complex<Type> & operator *=(const Type & d)
        {
            m_real *= d;
            m_imag *= d;
            return *this;
        }

        Complex<Type> & operator /=(const Complex<Type> & c)
        {
            Type inv_sqr_modulus = (Type)(1.0 / sqrModulus());

            m_real = ( m_real*c.m_real + m_imag*c.m_imag ) * inv_sqr_modulus;
            m_imag = ( m_imag*c.m_real - m_real*c.m_imag ) * inv_sqr_modulus;

            return *this;
        }

        Complex<Type> & operator /=(const Type & d)
        {
            m_real /= d;
            m_imag /= d;
            return *this;
        }

        Complex<Type> & operator +=(const Complex<Type> & c)
        {
            m_real += c.m_real;
            m_imag += c.m_imag;
            return *this;
        }

        Complex<Type> & operator +=(const Type & d)
        {
            m_real += d;
            return *this;
        }

        Complex<Type> & operator -=(const Complex<Type> & c)
        {
            m_real -= c.m_real;
            m_imag -= c.m_imag;
            return *this;
        }

        Complex<Type> & operator -=(const Type & d)
        {
            m_real -= d;
            return *this;
        }

        friend Complex<Type> operator *(const Complex<Type> & c1, const Complex<Type> & c2)
        { 
            return Complex<Type>(c1.m_real * c2.m_real - c1.m_imag * c2.m_imag, c1.m_real * c2.m_imag + c1.m_imag * c2.m_real);
        }
        friend Complex<Type> operator *(const Complex<Type> & c, const Type d)
        { 
            return Complex<Type>(c.m_real * d, c.m_imag * d);
        }
        friend Complex<Type> operator *(const Type d, const Complex<Type> & c)
        { 
            return c * d; 
        }

        friend Complex<Type> operator /(const Complex<Type> & c, const Type d)
        { 
            return Complex<Type>(c.m_real / d, c.m_imag / d);
        }
        friend Complex<Type> operator /(const Complex<Type> & c1, const Complex<Type> & c2)
        { 
            return Complex<Type>(c1 * c2.getConjugate() / c2.sqrModulus()); 
        }

        friend Complex<Type> operator +(const Complex<Type> & c1, const Complex<Type> & c2)
        { 
            return Complex<Type>(c1.m_real + c2.m_real,c1.m_imag + c2.m_imag);
        }
        friend Complex<Type> operator +(const Complex<Type> & c, const Type d)
        { 
            return Complex<Type>(c.m_real + d, c.m_imag);
        }
        friend Complex<Type> operator +(const Type d, const Complex<Type> & c)
        { 
            return c + d; 
        }

        friend Complex<Type> operator -(const Complex<Type> & c1, const Complex<Type> & c2)
        { 
            return Complex<Type>(c1.m_real - c2.m_real,c1.m_imag - c2.m_imag);	
        }
        friend Complex<Type> operator -(const Complex<Type> & c, const Type d)
        { 
            return Complex<Type>(c.m_real - d, c.m_imag);
        }
        friend Complex<Type> operator -(const Type d, const Complex<Type> & c)
        { 
            return c - d; 
        }

    private:
        Type m_real;
        Type m_imag;
    };

    typedef Complex<int>    Complexi;
    typedef Complex<float>  Complexf;
    typedef Complex<double> Complexd;
}



#endif