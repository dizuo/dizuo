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

#ifndef VEC4_H
#define VEC4_H

//#include <gtl/gtl.hpp>
#include "gtl.hpp"

namespace gtl
{
    /*!
    \class Vec4 Vec4.hpp geometry/Vec4.hpp
    \brief 4 dimensional vector.
    \ingroup base

    This class is used by many other classes.

    \sa Vec3  
    */
    template<typename Type>
    class Vec4
    {
    public:
        //! The default constructor.The vector will be null.
        Vec4()
        {
            setValue(0, 0, 0, 0);
        }

        //! Constructs an instance with initial values from \a v.
        Vec4(const Type v[4])
        {
            setValue(v);
        }

        //! Constructs an instance with the initial values from \a a_x, \a a_y, \a a_z and \a a_w.
        Vec4(Type a_x, Type a_y, Type a_z, Type a_w)
        {
            setValue(a_x, a_y, a_z, a_w);
        }
        
        //! Constructs an instance with initial values from \a a_vec.
        Vec4(const Vec4<Type> & a_vec)
        {
            setValue(a_vec[0], a_vec[1], a_vec[2], a_vec[3]);
        }

        //! Default destructor does nothing.
        virtual ~Vec4(){}

        //! Set new x, y, z and w values for the vector. Returns reference to self.
        Vec4<Type> &  setValue(const Type v[4])
        {
            m_xyzw[0] = v[0];
            m_xyzw[1] = v[1];
            m_xyzw[2] = v[2];
            m_xyzw[3] = v[3];

            return *this;
        }

        //! Set new x, y, z and w values for the vector. Returns reference to self.
        Vec4<Type> &  setValue(Type a_x, Type a_y, Type a_z, Type a_w)
        {
            m_xyzw[0] = a_x;
            m_xyzw[1] = a_y;
            m_xyzw[2] = a_z;
            m_xyzw[3] = a_w;

            return *this;
        }

        //! Returns a pointer to an array containing the coordinates of the vector.
        const Type * getValue() const
        {
            return &m_xyzw[0];
        }

        //! Returns the x,y,z and w coordinates of the vector.
        void getValue(Type & x, Type & y, Type & z, Type & w) const
        {
            x = m_xyzw[0];
            y = m_xyzw[1];
            z = m_xyzw[2];
            w = m_xyzw[3];
        }

        //! Calculates and returns the dot product of this vector with \a a_vec.
        Type dot(const Vec4<Type> & a_vec) const
        {
            return (m_xyzw[0]*a_vec[0] + m_xyzw[1]*a_vec[1] + m_xyzw[2]*a_vec[2] + m_xyzw[3]*a_vec[3]);
        }

        //! Return length of vector.
        Type length() const
        {
            return (Type)std::sqrt( sqrLength() );
        }

        //! Return squared length of vector.
        Type sqrLength() const
        {
            return (m_xyzw[0]*m_xyzw[0])+(m_xyzw[1]*m_xyzw[1])+(m_xyzw[2]*m_xyzw[2])+(m_xyzw[3]*m_xyzw[3]);
        }

        //! Normalize the vector to unit length. Return value is the original length of the vector before normalization.
        Type normalize()
        {
            Type magnitude = length();

            if(magnitude != 0.0)
                (*this) *= (Type)(1.0 / magnitude);
            else setValue(0.0, 0.0, 0.0, 0.0);

            return magnitude;
        }

        //! Negate the vector (i.e. point it in the opposite direction).
        void negate()
        {
            setValue(-m_xyzw[0], -m_xyzw[1], -m_xyzw[2], -m_xyzw[3]);
        }

        //! Return modifiable x value.
        Type & x()
        {  
            return m_xyzw[0]; 
        }

        //! Return modifiable y value.
        Type & y()
        {  
            return m_xyzw[1]; 
        }

        //! Return modifiable z value.
        Type & z()
        {  
            return m_xyzw[2]; 
        }

        //! Return modifiable w value.
        Type & w()
        {  
            return m_xyzw[3]; 
        }

        //! Return x value.
        const Type & x()const
        {  
            return m_xyzw[0]; 
        }

        //! Return y value.
        const Type & y()const
        {  
            return m_xyzw[1]; 
        }

        //! Return z value.
        const Type & z()const
        {  
            return m_xyzw[2]; 
        }

        //! Return w value.
        const Type & w()const
        {  
            return m_xyzw[3]; 
        }

        //! Index operator. Returns modifiable x, y, z or w value.
        Type &  operator[](int i) 
        { 
            return m_xyzw[i]; 
        }

        //! Index operator. Returns x, y, z or w value.
        const Type & operator[](int i) const 
        { 
            return m_xyzw[i]; 
        }

        //! Multiply components of vector with value \a d. Returns reference to self.
        Vec4<Type> & operator *=(const Type d)
        {
            m_xyzw[0] *= d;
            m_xyzw[1] *= d;
            m_xyzw[2] *= d;
            m_xyzw[3] *= d;

            return *this;
        }

        //! Divides components of vector with value \a d. Returns reference to self.
        Vec4<Type> & operator /=(const Type d)
        {
            Type inv = 1.0f/d;

            m_xyzw[0] *= inv;
            m_xyzw[1] *= inv;
            m_xyzw[2] *= inv;
            m_xyzw[3] *= inv;

            return *this;
        }

        //! Multiply components of vector with value \a a_vec.
        Vec4<Type> & operator *=(const Vec4<Type> & a_vec)
        {
            m_xyzw[0] *= a_vec.m_xyzw[0];
            m_xyzw[1] *= a_vec.m_xyzw[1];
            m_xyzw[2] *= a_vec.m_xyzw[2];
            m_xyzw[3] *= a_vec.m_xyzw[3];

            return *this;
        }

        //! Adds this vector and vector \a a_vec. Returns reference to self.
        Vec4<Type> & operator +=(const Vec4<Type> & a_vec)
        {
            m_xyzw[0] += a_vec.m_xyzw[0];
            m_xyzw[1] += a_vec.m_xyzw[1];
            m_xyzw[2] += a_vec.m_xyzw[2];
            m_xyzw[3] += a_vec.m_xyzw[3];

            return *this;
        }

        //! Subtracts vector \a a_vec from this vector. Returns reference to self.
        Vec4<Type> & operator -=(const Vec4<Type> & a_vec)
        {
            m_xyzw[0] -= a_vec.m_xyzw[0];
            m_xyzw[1] -= a_vec.m_xyzw[1];
            m_xyzw[2] -= a_vec.m_xyzw[2];
            m_xyzw[3] -= a_vec.m_xyzw[3];

            return *this;
        }

        //! Non-destructive negation operator.
        Vec4<Type> operator-() const
        {
            return Vec4<Type>(-m_xyzw[0], -m_xyzw[1], -m_xyzw[2], -m_xyzw[3]);
        }

        friend Vec4<Type> operator *(const Vec4<Type> & a_vec, const Type d)
        { 
            return Vec4<Type>(a_vec.m_xyzw[0] * d, 
                              a_vec.m_xyzw[1] * d, 
                              a_vec.m_xyzw[2] * d, 
                              a_vec.m_xyzw[3] * d);
        }
        friend Vec4<Type> operator *(const Type d, const Vec4<Type> & a_vec)
        { 
            return a_vec * d; 
        }
        friend Vec4<Type> operator /(const Vec4<Type> & a_vec, const Type d)
        { 
            return Vec4<Type>(a_vec.m_xyzw[0] / d, 
                              a_vec.m_xyzw[1] / d, 
                              a_vec.m_xyzw[2] / d, 
                              a_vec.m_xyzw[3] / d);
        }
        friend Vec4<Type> operator *(const Vec4<Type> & v1, const Vec4<Type> & v2)
        {	
            return Vec4<Type>(v1.m_xyzw[0] * v2.m_xyzw[0],
                              v1.m_xyzw[1] * v2.m_xyzw[1],
                              v1.m_xyzw[2] * v2.m_xyzw[2],
                              v1.m_xyzw[3] * v2.m_xyzw[3]);
        }
        friend Vec4<Type> operator +(const Vec4<Type> & v1, const Vec4<Type> & v2)
        {	
            return Vec4<Type>(v1.m_xyzw[0] + v2.m_xyzw[0],
                              v1.m_xyzw[1] + v2.m_xyzw[1],
                              v1.m_xyzw[2] + v2.m_xyzw[2],
                              v1.m_xyzw[3] + v2.m_xyzw[3]);
        }
        friend Vec4<Type> operator -(const Vec4<Type> & v1, const Vec4<Type> & v2)
        {	
            return Vec4<Type>(v1.m_xyzw[0] - v2.m_xyzw[0],
                              v1.m_xyzw[1] - v2.m_xyzw[1],
                              v1.m_xyzw[2] - v2.m_xyzw[2],
                              v1.m_xyzw[3] - v2.m_xyzw[3]);	
        }

        //! Check the two given vector for equality. 
        friend bool operator ==(const Vec4<Type> & v1, const Vec4<Type> & v2)
        { 
            return (v1.m_xyzw[0]==v2.m_xyzw[0] && 
                    v1.m_xyzw[1]==v2.m_xyzw[1] && 
                    v1.m_xyzw[2]==v2.m_xyzw[2] && 
                    v1.m_xyzw[3]==v2.m_xyzw[3]); 
        }

        //! Check the two given vector for inequality. 
        friend bool operator !=(const Vec4<Type> & v1, const Vec4<Type> & v2)
        { 
            return !(v1 == v2); 
        }

        //! Check for equality with given tolerance.
        bool equals(const Vec4<Type> & a_vec, const Type a_tolerance=1E-2) const
        {
            return ( (m_xyzw - a_vec).sqrLength() <= a_tolerance*a_tolerance );
        }

        friend std::ostream & operator<<(std::ostream & os, const Vec4<Type> & vect)
        { 
            return os << vect.x() << '\t'<< vect.y() << '\t'<< vect.z() << '\t'<< vect.w(); 
        }

        //! Largest representable vector
        static Vec4<Type> max()
        {
            return Vec4<Type>(std::numeric_limits<Type>::max(), 
                              std::numeric_limits<Type>::max(),
                              std::numeric_limits<Type>::max(),
                              std::numeric_limits<Type>::max());
        }
    private:
        Type m_xyzw[4];
    };

    typedef Vec4<int>    Vec4i; 
    typedef Vec4<float>  Vec4f; 
    typedef Vec4<double> Vec4d;
} // namespace gtl

#endif
