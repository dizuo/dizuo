/*****************************************************************************/
/*                                                                           */
/*  Visualization Library                                                    */
/*  http://www.visualizationlibrary.com                                      */
/*                                                                           */
/*  Copyright 2005-2009 Michele Bosi, all rights are reserved                */
/*                                                                           */
/*  This file is part of Visualization Library.                              */
/*  It may only be used, modified, and distributed under the terms of the    */
/*  license specified in the LICENSE.TXT file. By continuing to use,         */
/*  modify or distribute this file you indicate that you have read the       */
/*  license and understand and accept it fully.                              */
/*                                                                           */
/*  Visualization Library is distributed in the hope that it will be useful, */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the LICENSE.TXT */
/*  file for more details.                                                   */
/*                                                                           */
/*****************************************************************************/

#ifndef Vector2_INCLUDE_ONCE
#define Vector2_INCLUDE_ONCE

#include <glew.h>
#include <cmath>

namespace vl
{
  // trigonometric constants

  //! Greek Pi constant using \p double precision.
  const double dPi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093845;
  //! Constant to convert degrees into radians using \p double precision.
  const double dDEG_TO_RAD = dPi / 180.0;
  //! Constant to convert radians into degrees using \p double precision.
  const double dRAD_TO_DEG = 180.0 / dPi;

  //! Greek Pi constant using \p float precision.
  const float fPi = (float)3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093845;
  //! Constant to convert degrees into radians using \p float precision.
  const float fDEG_TO_RAD = float(dPi / 180.0);
  //! Constant to convert radians into degrees using \p float precision.
  const float fRAD_TO_DEG = float(180.0 / dPi);

  // fast square root

  #if VL_FAST_SQUARE_ROOTS == 1
    #define VL_FLOAT_SQRT(x)    fast_sqrt(x)
    #define VL_FLOAT_INVSQRT(x) fast2_inversesqrt(x)
  #else
    #define VL_FLOAT_SQRT(x)    ((float)::sqrt(x))
    #define VL_FLOAT_INVSQRT(x) (1.0f/(float)::sqrt(x))
  #endif

  // fast square root functions, see Dave Eberly's paper and http://www.beyond3d.com/content/articles/8/

  inline float fast1_inversesqrt(float x)
  {
    float xhalf = 0.5f*x;
    unsigned int i = *((unsigned int*)&x);
    i = 0x5f3759df - (i>>1);
    x = *((float*)&i);
    x = x*(1.5f - xhalf*x*x); // single iteration, very quick, but very poor precision
    return x;
  }
  inline float fast2_inversesqrt(float x)
  {
    float xhalf = 0.5f*x;
    unsigned int i = *((unsigned int*)&x);
    i = 0x5f3759df - (i>>1);
    x = *((float*)&i);
    x = x*(1.5f - xhalf*x*x);
    x = x*(1.5f - xhalf*x*x); // the trackball shows that one iteration is not enough so we do two
    return x;
  }
  inline float fast_sqrt(float x) { if (x == 0.0f) return 0.0f; else return x * fast2_inversesqrt(x); }

  /**
   * The Vector2 class is a template class that implements a generic 2 components vector, see also vl::fvec2, vl::dvec2, vl::uvec2, vl::ivec2, vl::svec2, vl::usvec2, vl::bvec2, vl::ubvec2.
   * \sa Vector4, Vector3, Matrix4, Matrix3, Matrix2
   */
  template<typename scalar_type>
  class Vector2
  {
  public:
    static const int scalar_count = 2;
    Vector2(const Vector2& other) { *this = other; }
    Vector2() { x() = y() = 0; }

    template<class T>
    explicit Vector2(const T& other)
    {
      x() = (scalar_type)other.x();
      y() = (scalar_type)other.y();
    }

    explicit Vector2(scalar_type x, scalar_type y)
    {
      mScalar[0] = x;
      mScalar[1] = y;
    }

    scalar_type* ptr() { return mScalar; }
    const scalar_type* ptr() const { return mScalar; }

    const scalar_type& x() const { return mScalar[0]; }
    const scalar_type& y() const { return mScalar[1]; }

    scalar_type& x() { return mScalar[0]; }
    scalar_type& y() { return mScalar[1]; }

    const scalar_type& r() const { return mScalar[0]; }
    const scalar_type& g() const { return mScalar[1]; }

    scalar_type& r() { return mScalar[0]; }
    scalar_type& g() { return mScalar[1]; }

    const scalar_type& s() const { return mScalar[0]; }
    const scalar_type& t() const { return mScalar[1]; }

    scalar_type& s() { return mScalar[0]; }
    scalar_type& t() { return mScalar[1]; }

    Vector2 operator+(const Vector2& other) const
    {
      return Vector2(x()+other.x(), y()+other.y());
    }
    Vector2 operator-(const Vector2& other) const
    {
      return Vector2(x()-other.x(), y()-other.y());
    }
    Vector2 operator*(const Vector2& other) const
    {
      return Vector2(x()*other.x(), y()*other.y());
    }
    Vector2 operator/(const Vector2& other) const
    {
      return Vector2(x()/other.x(), y()/other.y());
    }
    Vector2 operator+(scalar_type val) const
    {
      return Vector2(x()+val, y()+val);
    }
    Vector2 operator-(scalar_type val) const
    {
      return Vector2(x()-val, y()-val);
    }
    Vector2 operator*(scalar_type val) const
    {
      return Vector2(x()*val, y()*val);
    }
    Vector2 operator/(scalar_type val) const
    {
      return Vector2(x()/val, y()/val);
    }
    Vector2 operator-() const
    {
      return Vector2(-x(), -y());
    }
    Vector2& operator+=(const Vector2& other)
    {
      *this = *this + other;
      return *this;
    }
    Vector2& operator-=(const Vector2& other)
    {
      *this = *this - other;
      return *this;
    }
    Vector2& operator*=(const Vector2& other)
    {
      *this = *this * other;
      return *this;
    }
    Vector2& operator/=(const Vector2& other)
    {
      *this = *this / other;
      return *this;
    }
    Vector2& operator+=(scalar_type val)
    {
      *this = *this + val;
      return *this;
    }
    Vector2& operator-=(scalar_type val)
    {
      *this = *this - val;
      return *this;
    }
    Vector2& operator*=(scalar_type val)
    {
      *this = *this * val;
      return *this;
    }
    Vector2& operator/=(scalar_type val)
    {
      *this = *this / val;
      return *this;
    }
    Vector2& operator=(const Vector2& other)
    {
      x() = other.x();
      y() = other.y();
      return *this;
    }
    Vector2& operator=(scalar_type val)
    {
      x() = y() = val;
      return *this;
    }
    bool operator==(const Vector2& other) const
    {
      return x() == other.x() && y() == other.y();
    }
    bool operator!=(const Vector2& other) const
    {
      return !operator==(other);
    }
    bool operator<(const Vector2& other) const
    {
      if (x() != other.x())
        return x() < other.x();
      else
        return y() < other.y();
    }
    scalar_type& operator[](unsigned i) { return mScalar[i]; }
    const scalar_type& operator[](unsigned i) const { return mScalar[i]; }
    scalar_type length() const { return ::sqrt(x()*x()+y()*y()); }
    scalar_type lengthSquared() const { return x()*x()+y()*y(); }
    bool isNull() const { return !x() && !y(); }
    const Vector2& normalize()
    {
      scalar_type l = length();
      if (l)
        *this *= (scalar_type)(1.0/l); 
      return *this; 
    }

  protected:
    scalar_type mScalar[scalar_count];
  };

  template<typename T>
  inline const Vector2<T> operator*(T val, const Vector2<T>& v)
  {
    return v * val;
  }

  //! A 2 components vector with \p GLint precision.
  typedef Vector2<GLint> ivec2;
  //! A 2 components vector with \p GLuint precision.
  typedef Vector2<GLuint> uvec2;
  //! A 2 components vector with \p GLfloat precision.
  typedef Vector2<GLfloat> fvec2;
  //! A 2 components vector with \p GLdouble precision.
  typedef Vector2<GLdouble> dvec2;
  //! A 2 components vector with \p GLbyte precision.
  typedef Vector2<GLbyte> bvec2;
  //! A 2 components vector with \p GLubyte precision.
  typedef Vector2<GLubyte> ubvec2;
  //! A 2 components vector with \p GLshort precision.
  typedef Vector2<GLshort> svec2;
  //! A 2 components vector with \p GLushort precision.
  typedef Vector2<GLushort> usvec2;

  #if VL_PIPELINE_PRECISION == 2
    //! Defined as: \p 'typedef \p dvec2 \p vec2'. See also \ref VL_PIPELINE_PRECISION.
    typedef dvec2 vec2;
  #else
    //! Defined as: \p 'typedef \p fvec2 \p vec2'. See also \ref VL_PIPELINE_PRECISION.
    typedef fvec2 vec2;
  #endif
}

#endif
