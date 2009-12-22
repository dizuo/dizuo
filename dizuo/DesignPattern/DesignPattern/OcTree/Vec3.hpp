#ifndef VEC3_H
#define VEC3_H

#include <limits> 
/*!
\class Vec3 Vec3.hpp geometry/Vec3.hpp
\brief 3 dimensional vector.
\ingroup base

This class is used by many other classes.

\sa Vec2  
*/
template<typename Type>
class Vec3
{
public:
    //! The default constructor.The vector will be null.
    Vec3()
    {
        setValue(0, 0, 0);
    }

    //! Constructs an instance with initial values from \a v.
    Vec3(const Type v[3])
    {
        setValue(v);
    }

    //! Constructs an instance with the initial values from \a a_x, \a a_y and \a a_z.
    Vec3(Type a_x, Type a_y, Type a_z)
    {
        setValue(a_x, a_y, a_z);
    }

    //! Constructs an instance with initial values from \a a_vec.
    Vec3(const Vec3<Type> & a_vec)
    {
        setValue(a_vec[0], a_vec[1], a_vec[2]);
    }

    //! Default destructor does nothing.
    virtual ~Vec3(){}

    //! Set new x, y and z values for the vector. Returns reference to self.
    Vec3<Type> & setValue(const Type v[3])
    {
        m_xyz[0] = v[0];
        m_xyz[1] = v[1];
        m_xyz[2] = v[2];

        return *this;
    }

    //! Set new x, y and z values for the vector. Returns reference to self.
    Vec3<Type> & setValue(Type a_x, Type a_y, Type a_z)
    {
        m_xyz[0] = a_x;
        m_xyz[1] = a_y;
        m_xyz[2] = a_z;

        return *this;
    }

    //! Returns a pointer to an array containing the coordinates of the vector.
    const Type * getValue() const
    {
        return &m_xyz[0];
    }

    //! Calculates and returns the dot product of this vector with \a a_vec.
    Type dot(const Vec3<Type> & a_vec) const
    {
        return (m_xyz[0]*a_vec[0] + m_xyz[1]*a_vec[1] + m_xyz[2]*a_vec[2]);
    }

    //! Return length of vector.
    Type length() const
    {
        return (Type)std::sqrt( sqrLength() );
    }

    //! Return squared length of vector.
    Type sqrLength() const
    {
        return (m_xyz[0]*m_xyz[0])+(m_xyz[1]*m_xyz[1])+(m_xyz[2]*m_xyz[2]);
    }

    //! Normalize the vector to unit length. Return value is the original length of the vector before normalization.
    Type normalize()
    {
        Type magnitude = length();

        if(magnitude != 0.0)
            (*this) *= (Type)(1.0 / magnitude);
        else setValue(0.0, 0.0, 0.0);

        return magnitude;
    }

    //! Returns the cross product of this vector with \a a_vec.
    Vec3<Type> cross(const Vec3<Type> & a_vec) const
    {
        return Vec3<Type>(m_xyz[1] * a_vec[2] - a_vec[1] * m_xyz[2],
                          m_xyz[2] * a_vec[0] - a_vec[2] * m_xyz[0],
                          m_xyz[0] * a_vec[1] - a_vec[0] * m_xyz[1]);
    }

    //! Negate the vector (i.e. point it in the opposite direction).
    void negate()
    {
        setValue(-m_xyz[0], -m_xyz[1], -m_xyz[2]);
    }

    //! Return modifiable x value.
    Type & x(){  return m_xyz[0]; }

    //! Return modifiable y value.
    Type & y(){  return m_xyz[1]; }

    //! Return modifiable z value.
    Type & z(){  return m_xyz[2]; }

    //! Return x value.
    const Type & x()const{  return m_xyz[0]; }

    //! Return y value.
    const Type & y()const{  return m_xyz[1]; }

    //! Return z value.
    const Type & z()const{  return m_xyz[2]; }

    //! Index operator. Returns modifiable x, y or z value.
    Type &  operator[](int i) { return m_xyz[i]; }

    //! Index operator. Returns x, y or z value.
    const Type & operator[](int i) const { return m_xyz[i]; }

    //! Multiply components of vector with value \a d. Returns reference to self.
    Vec3<Type> & operator *=(const Type d)
    {
        m_xyz[0] *= d;
        m_xyz[1] *= d;
        m_xyz[2] *= d;

        return *this;
    }

    //! Divides components of vector with value \a d. Returns reference to self.
    Vec3<Type> & operator /=(const Type d)
    {
        *this *= (1.0f/d);

        return *this;
    }

    //! Multiply components of vector with value \a a_vec.
    Vec3<Type> & operator *=(const Vec3<Type> & a_vec)
    {
        m_xyz[0] *= a_vec.m_xyz[0];
        m_xyz[1] *= a_vec.m_xyz[1];
        m_xyz[2] *= a_vec.m_xyz[2];

        return *this;
    }

    //! Adds this vector and vector \a a_vec. Returns reference to self.
    Vec3<Type> & operator +=(const Vec3<Type> & a_vec)
    {
        m_xyz[0] += a_vec.m_xyz[0];
        m_xyz[1] += a_vec.m_xyz[1];
        m_xyz[2] += a_vec.m_xyz[2];

        return *this;
    }

    //! Subtracts vector \a a_vec from this vector. Returns reference to self.
    Vec3<Type> & operator -=(const Vec3<Type> & a_vec)
    {
        m_xyz[0] -= a_vec.m_xyz[0];
        m_xyz[1] -= a_vec.m_xyz[1];
        m_xyz[2] -= a_vec.m_xyz[2];

        return *this;
    }

    //! Non-destructive negation operator.
    Vec3<Type> operator-() const
    {
        return Vec3<Type>(-m_xyz[0], -m_xyz[1], -m_xyz[2]);
    }

    friend Vec3<Type> operator *(const Vec3<Type> & a_vec, const Type d)
    { 
        return Vec3<Type>(a_vec.m_xyz[0] * d, a_vec.m_xyz[1] * d, a_vec.m_xyz[2] * d);
    }

    friend Vec3<Type> operator *(const Type d, const Vec3<Type> & a_vec)
    { 
        return a_vec * d; 
    }

    friend Vec3<Type> operator /(const Vec3<Type> & a_vec, const Type d)
    { 
        return Vec3<Type>(a_vec.m_xyz[0] / d, a_vec.m_xyz[1] / d, a_vec.m_xyz[2] / d);
    }

    friend Vec3<Type> operator *(const Vec3<Type> & v1, const Vec3<Type> & v2)
    {	
        return Vec3<Type>(v1.m_xyz[0] * v2.m_xyz[0],v1.m_xyz[1] * v2.m_xyz[1],v1.m_xyz[2] * v2.m_xyz[2]);
    }

    friend Vec3<Type> operator +(const Vec3<Type> & v1, const Vec3<Type> & v2)
    {	
        return Vec3<Type>(v1.m_xyz[0] + v2.m_xyz[0],v1.m_xyz[1] + v2.m_xyz[1],v1.m_xyz[2] + v2.m_xyz[2]);
    }

    friend Vec3<Type> operator -(const Vec3<Type> & v1, const Vec3<Type> & v2)
    {	
        return Vec3<Type>(v1.m_xyz[0] - v2.m_xyz[0],v1.m_xyz[1] - v2.m_xyz[1],v1.m_xyz[2] - v2.m_xyz[2]);	
    }

    //! Check the two given vector for equality. 
    friend bool operator ==(const Vec3<Type> & v1, const Vec3<Type> & v2)
    { 
        return (v1.m_xyz[0]==v2.m_xyz[0] && v1.m_xyz[1]==v2.m_xyz[1] && v1.m_xyz[2]==v2.m_xyz[2]); 
    }

    //! Check the two given vector for inequality. 
    friend bool operator !=(const Vec3<Type> & v1, const Vec3<Type> & v2)
    { 
        return !(v1 == v2); 
    }

    //! Check for equality with given tolerance.
    bool equals(const Vec3<Type> & a_vec, const Type a_tolerance=1E-2) const
    {
        return ( (m_xyz - a_vec).sqrLength() <= a_tolerance*a_tolerance );
    }

//         friend std::ostream & operator<<(std::ostream & os, const Vec3<Type> & vect)
//         { 
//             return os << vect.x() << " " << vect.y() << " " << vect.z(); 
//         }
// 
         //! Largest representable vector
     static Vec3<Type> max()
     {
         return Vec3<Type>(std::numeric_limits<Type>::max(), 
                           std::numeric_limits<Type>::max(),
                           std::numeric_limits<Type>::max());
     }
private:
    Type m_xyz[3];
};

typedef Vec3<int>    Vec3i; 
typedef Vec3<float>  Vec3f; 
typedef Vec3<double> Vec3d;


#endif
