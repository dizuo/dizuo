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

#ifndef VEC2_H
#define VEC2_H

#include "gtl.hpp"

namespace gtl
{
    /*!
    \class Vec2 Vec2.hpp geometry/Vec2.hpp
    \brief 2 dimensional vector.
    \ingroup base

    This class is used by many other classes.

    \sa Vec3  
    */
    template<typename Type>
    class Vec2
    {
    public:
        //! The default constructor.The vector will be null.
        Vec2()
        {
            setValue(0, 0);
        }

        //! Constructs an instance with initial values from \a v.
        Vec2(const Type v[2])
        {
            setValue(v);
        }

        //! Constructs an instance with the initial values from \a a_x and \a a_y.
        Vec2(Type a_x, Type a_y)
        {
            setValue(a_x, a_y);
        }

        //! Constructs an instance with initial values from \a a_vec.
        Vec2(const Vec2<Type> & a_vec)
        {
            setValue(a_vec[0], a_vec[1]);
        }

        //! Default destructor does nothing.
        virtual ~Vec2(){}

        //! Set new x and y coordinates for the vector. Returns reference to self.
        Vec2<Type> & setValue(const Type v[2])
        {
            m_xy[0] = v[0];
            m_xy[1] = v[1];

            return *this;
        }

        //! Set new x and y coordinates for the vector. Returns reference to self.
        Vec2<Type> & setValue(Type a_x, Type a_y)
        {
            m_xy[0] = a_x;
            m_xy[1] = a_y;

            return *this;
        }

        //! Returns a pointer to an array containing the coordinates of the vector.
        const Type * getValue() const
        {
            return &m_xy[0];
        }

        //! Calculates and returns the dot product of this vector with \a a_vec.
        Type dot(const Vec2<Type> & a_vec) const
        {
            return (m_xy[0]*a_vec[0] + m_xy[1]*a_vec[1]);
        }

        //! Return length of vector.
        Type length() const
        {
            return (Type)std::sqrt( sqrLength() );
        }

        //! Return squared length of vector.
        Type sqrLength() const
        {
            return (m_xy[0]*m_xy[0])+(m_xy[1]*m_xy[1]);
        }

        //! Normalize the vector to unit length. Return value is the original length of the vector before normalization.
        Type normalize()
        {
            Type magnitude = length();

            if(magnitude != 0.0)
                (*this) *= (Type)(1.0 / magnitude);
            else setValue(0.0, 0.0);

            return magnitude;
        }

        //! Returns the cross product of this vector with \a a_vec.
        Type cross(const Vec2<Type> & a_vec)
        {
            return (m_xy[0] * a_vec[1] - m_xy[1] * a_vec[0]);
        }

        //! Negate the vector (i.e. point it in the opposite direction).
        void negate()
        {
            setValue(-m_xy[0], -m_xy[1]);
        }

        //! Return modifiable x value.
        Type & x()
        {  
            return m_xy[0]; 
        }

        //! Return modifiable y value.
        Type & y()
        {  
            return m_xy[1]; 
        }

        //! Return x value.
        const Type & x()const
        {  
            return m_xy[0]; 
        }

        //! Return y value.
        const Type & y()const
        {  
            return m_xy[1]; 
        }

        //! Index operator. Returns modifiable x or y value.
        Type &  operator[](int i)
        { 
            return m_xy[i]; 
        }

        //! Index operator. Returns x or y value.
        const Type & operator[](int i) const
        { 
            return m_xy[i]; 
        }

        //! Multiply components of vector with value \a d. Returns reference to self.
        Vec2<Type> & operator *=(const Type d)
        {
            m_xy[0] *= d;
            m_xy[1] *= d;

            return *this;
        }

        //! Divides components of vector with value \a d. Returns reference to self.
        Vec2<Type> & operator /=(const Type d)
        {
            Type inv = 1.0f/d;

            m_xy[0] *= inv;
            m_xy[1] *= inv;

            return *this;
        }

        //! Multiply components of vector with value \a a_vec.
        Vec2<Type> & operator *=(const Vec2<Type> & a_vec)
        {
            m_xy[0] *= a_vec.m_xy[0];
            m_xy[1] *= a_vec.m_xy[1];

            return *this;
        }

        //! Adds this vector and vector \a a_vec. Returns reference to self.
        Vec2<Type> & operator +=(const Vec2<Type> & a_vec)
        {
            m_xy[0] += a_vec.m_xy[0];
            m_xy[1] += a_vec.m_xy[1];

            return *this;
        }

        //! Subtracts vector \a a_vec from this vector. Returns reference to self.
        Vec2<Type> & operator -=(const Vec2<Type> & a_vec)
        {
            m_xy[0] -= a_vec.m_xy[0];
            m_xy[1] -= a_vec.m_xy[1];

            return *this;
        }

        //! Non-destructive negation operator.
        Vec2<Type> operator-() const
        {
            return Vec2<Type>(-m_xy[0], -m_xy[1]);
        }

        friend Vec2<Type> operator *(const Vec2<Type> & a_vec, const Type d)
        { 
            return Vec2<Type>(a_vec.m_xy[0] * d, a_vec.m_xy[1] * d);
        }

        friend Vec2<Type> operator *(const Type d, const Vec2<Type> & a_vec)
        { 
            return a_vec * d; 
        }

        friend Vec2<Type> operator /(const Vec2<Type> & a_vec, const Type d)
        { 
            return Vec2<Type>(a_vec.m_xy[0] / d, a_vec.m_xy[1] / d);
        }

        friend Vec2<Type> operator *(const Vec2<Type> & v1, const Vec2<Type> & v2)
        {	
            return Vec2<Type>(v1.m_xy[0] * v2.m_xy[0],v1.m_xy[1] * v2.m_xy[1]);
        }

        friend Vec2<Type> operator +(const Vec2<Type> & v1, const Vec2<Type> & v2)
        {	
            return Vec2<Type>(v1.m_xy[0] + v2.m_xy[0],v1.m_xy[1] + v2.m_xy[1]);
        }

        friend Vec2<Type> operator -(const Vec2<Type> & v1, const Vec2<Type> & v2)
        {	
            return Vec2<Type>(v1.m_xy[0] - v2.m_xy[0], v1.m_xy[1] - v2.m_xy[1]);	
        }

        //! Check the two given vector for equality. 
        friend bool operator ==(const Vec2<Type> & v1, const Vec2<Type> & v2)
        { 
            return v1.m_xy[0]==v2.m_xy[0] && v1.m_xy[1]==v2.m_xy[1]; 
        }

        //! Check the two given vector for inequality. 
        friend bool operator !=(const Vec2<Type> & v1, const Vec2<Type> & v2)
        { 
            return !(v1 == v2); 
        }

        //! Check for equality with given tolerance.
        bool equals(const Vec2<Type> & a_vec, const Type a_tolerance=1E-2) const
        {
            return ( (m_xy - a_vec).sqrLength() <= a_tolerance*a_tolerance );
        }

        friend std::ostream & operator<<(std::ostream & os, const Vec2<Type> & vect)
        { 
            return os << vect.x() << " " << vect.y(); 
        }

        //! Largest representable vector
		// There is something wrong...
/*
        static Vec2<Type> max()
        {
            return Vec2<Type>(std::numeric_limits<Type>::max(), std::numeric_limits<Type>::max());
        }*/


    private:
        Type m_xy[2];
    };

    typedef Vec2<int>    Vec2i; 
    typedef Vec2<float>  Vec2f; 
    typedef Vec2<double> Vec2d;
} // namespace gtl

#endif
