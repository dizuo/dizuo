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

#ifndef Matrix2_INCLUDE_ONCE
#define Matrix2_INCLUDE_ONCE

#include <vl/checks.hpp>
#include <cstring> // memcpy

namespace vl
{
  //-----------------------------------------------------------------------------
  // Matrix2
  //-----------------------------------------------------------------------------
  /**
   * The Matrix2 class is a template class that implements a generic 2x2 matrix, see also vl::dmat2, vl::fmat2, vl::umat2, vl::imat2.
   * \sa Vector4, Vector3, Vector2, Matrix4, Matrix3
   */
  template<typename scalar_type>
  class Matrix2
  {
  public:
    //-----------------------------------------------------------------------------
    template<typename T>
    explicit Matrix2(const T& m)
    {
      e(0,0) = (scalar_type)m.e(0,0); e(0,1) = (scalar_type)m.e(0,1);
      e(1,0) = (scalar_type)m.e(1,0); e(1,1) = (scalar_type)m.e(1,1);
    }
    //-----------------------------------------------------------------------------
    Matrix2()
    {
      static const scalar_type I2d[] = { 
        (scalar_type)1, (scalar_type)0,
        (scalar_type)0, (scalar_type)1
      };
      memcpy( mVec,I2d,sizeof(scalar_type)*4 );
    }
    //-----------------------------------------------------------------------------
    Matrix2(scalar_type n)
    {
      static const scalar_type I2d[] = { 
        (scalar_type)1, (scalar_type)0,
        (scalar_type)0, (scalar_type)1
      };
      memcpy( mVec,I2d,sizeof(scalar_type)*4 );
      e(0,0) = n; e(1,1) = n;
    }
    //-----------------------------------------------------------------------------
    explicit Matrix2( scalar_type e00, scalar_type e01,
                    scalar_type e10, scalar_type e11  )
    {
      e(0,0) = e00; e(0,1) = e01;
      e(1,0) = e10; e(1,1) = e11;
    }
    //-----------------------------------------------------------------------------
    const Vector2<scalar_type>& operator[](unsigned int i) const { VL_CHECK(i<2); return mVec[i]; }
    //-----------------------------------------------------------------------------
    Vector2<scalar_type>& operator[](unsigned int i) { VL_CHECK(i<2); return mVec[i]; }
    //-----------------------------------------------------------------------------
    bool operator==(const Matrix2& m) const 
    {
      return memcmp(m.mVec, mVec, sizeof(scalar_type)*4) == 0;
    }
    //-----------------------------------------------------------------------------
    bool operator!=(const Matrix2& m) const 
    {
      return !(*this == m);
    }
    //-----------------------------------------------------------------------------
    Matrix2& operator=(const Matrix2& m) 
    {
      memcpy(mVec, m.mVec, sizeof(scalar_type)*4);
      return *this;
    }
    //-----------------------------------------------------------------------------
    Matrix2 operator+(const Matrix2& m) const
    {
      Matrix2 t;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          t[i][j] = e(i,j) + m[i][j];

      return t;
    }
    //-----------------------------------------------------------------------------
    Matrix2& operator+=(const Matrix2& m)
    {
      return *this = *this + m;
    }
    //-----------------------------------------------------------------------------
    Matrix2 operator-(const Matrix2& m) const
    {
      Matrix2 t;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          t[i][j] = e(i,j) - m[i][j];

      return t;
    }
    //-----------------------------------------------------------------------------
    Matrix2& operator-=(const Matrix2& m)
    {
      return *this = *this - m;
    }
    //-----------------------------------------------------------------------------
    Matrix2& operator*=(const Matrix2& m)
    {
      return *this = *this * m;
    }
    //-----------------------------------------------------------------------------
    Matrix2 operator-() const
    {
      Matrix2 t;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          t[i][j] = -e(i,j);

      return t;
    }
    //-----------------------------------------------------------------------------
    Matrix2 operator+(scalar_type d) const
    {
      Matrix2 t;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          t[i][j] = e(i,j) + d;

      return t;
    }
    //-----------------------------------------------------------------------------
    Matrix2& operator+=(scalar_type d)
    {
      return *this = *this + d;
    }
    //-----------------------------------------------------------------------------
    Matrix2 operator-(scalar_type d) const
    {
      Matrix2 t;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          t[i][j] = e(i,j) - d;

      return t;
    }
    //-----------------------------------------------------------------------------
    Matrix2& operator-=(scalar_type d)
    {
      return *this = *this - d;
    }
    //-----------------------------------------------------------------------------
    Matrix2 operator*(scalar_type d) const
    {
      Matrix2 t;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          t[i][j] = e(i,j) * d;

      return t;
    }
    //-----------------------------------------------------------------------------
    Matrix2& operator*=(scalar_type d)
    {
      Matrix2 t;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          e(i,j) *= d;

      return *this;
    }
    //-----------------------------------------------------------------------------
    Matrix2 operator/(scalar_type d) const
    {
      d = (scalar_type)1 / d;
      Matrix2 t;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          t[i][j] = e(i,j) * d;

      return t;
    }
    //-----------------------------------------------------------------------------
    Matrix2& operator/=(scalar_type d)
    {
      d = (scalar_type)1 / d;
      Matrix2 t;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          e(i,j) *= d;

      return *this;
    }
    //-----------------------------------------------------------------------------
    bool isIdentity() const
    {
      Matrix2 i;
      return memcmp( ptr(), i.ptr(), sizeof(scalar_type)*4 ) == 0;
    }
    //-----------------------------------------------------------------------------
    scalar_type* ptr()
    {
      return &e(0,0);
    }
    //-----------------------------------------------------------------------------
    const scalar_type* ptr() const
    {
      return &e(0,0);
    }
    //-----------------------------------------------------------------------------
    Matrix2& transpose()
    {
      Matrix2 m;
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j) {
          m.mVec[i][j] = mVec[j][i];
        }
      memcpy(mVec, m.mVec, sizeof(scalar_type)*4);
      return *this;
    }
    //-----------------------------------------------------------------------------
    bool isNull() const
    {
      for(int i=0; i<2; ++i)
        for(int j=0; j<2; ++j)
          if( mVec[j][i] != 0 )
            return false;
      return true;
    }
    //-----------------------------------------------------------------------------
    void setIdentity()
    {
      *this = Matrix2();
    }
    //-----------------------------------------------------------------------------
    Matrix2 inverse(bool *invertible=NULL) const
    {
      const scalar_type& a11 = e(0,0); 
      const scalar_type& a12 = e(0,1); 
      const scalar_type& a21 = e(1,0); 
      const scalar_type& a22 = e(1,1); 

      scalar_type DET = a11*a22-a12*a21;

      if (invertible)
      {
        if (DET == 0)
          *invertible = false;
        else
          *invertible = true;
      }
      
      Matrix2 m(0,0,0,0);

      if (DET == 0)
        return m;

      m = Matrix2(+a22, -a12, -a21, +a11) / DET;

      return m;
    }
    //-----------------------------------------------------------------------------
    Matrix2& invert(bool *invertible=NULL)
    {
      *this = inverse(invertible);
      return *this;
    }
    //-----------------------------------------------------------------------------
    const scalar_type& e(int i, int j) const { return mVec[i][j]; }
    //-----------------------------------------------------------------------------
    scalar_type& e(int i, int j) { return mVec[i][j]; }
    //-----------------------------------------------------------------------------

  protected:
    Vector2<scalar_type> mVec[2];
  };

  //-----------------------------------------------------------------------------
  // OPERATORS
  //-----------------------------------------------------------------------------
  template<typename scalar_type>
  inline Matrix2<scalar_type> operator*(const Matrix2<scalar_type>& m2, const Matrix2<scalar_type>& m1)
  {
    Matrix2<scalar_type> t;
    t.e(0,0) = m1.e(0,0)*m2.e(0,0) + m1.e(0,1)*m2.e(1,0);
    t.e(1,0) = m1.e(1,0)*m2.e(0,0) + m1.e(1,1)*m2.e(1,0);

    t.e(0,1) = m1.e(0,0)*m2.e(0,1) + m1.e(0,1)*m2.e(1,1);
    t.e(1,1) = m1.e(1,0)*m2.e(0,1) + m1.e(1,1)*m2.e(1,1);

    return t;
  }
  //-----------------------------------------------------------------------------
  template<typename scalar_type>
  inline Matrix2<scalar_type> operator+(scalar_type d, const Matrix2<scalar_type>& m)
  {
    return m + d;
  }
  //-----------------------------------------------------------------------------
  template<typename scalar_type>
  inline Matrix2<scalar_type> operator*(scalar_type d, const Matrix2<scalar_type>& m)
  {
    return m * d;
  }
  //-----------------------------------------------------------------------------
  // post multiplication: matrix * column vector
  template<typename scalar_type>
  inline Vector2<scalar_type> operator*(const Matrix2<scalar_type>& m, const Vector2<scalar_type>& v)
  {
    Vector2<scalar_type> t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1);
    return t;
  }
  //-----------------------------------------------------------------------------
  // pre multiplication: row vector * matrix
  template<typename scalar_type>
  inline Vector2<scalar_type> operator*(const Vector2<scalar_type>& v, const Matrix2<scalar_type>& m)
  {
    Vector2<scalar_type> t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1);
    return t;
  }
  //-----------------------------------------------------------------------------

  //! A 2x2 matrix using \p GLdouble precision.
  typedef Matrix2<GLdouble> dmat2;
  //! A 2x2 matrix using \p GLfloat precision.
  typedef Matrix2<GLfloat>  fmat2;
  //! A 2x2 matrix using \p GLint precision.
  typedef Matrix2<GLint>    imat2;
  //! A 2x2 matrix using \p GLuint precision.
  typedef Matrix2<GLuint>  umat2;

  #if VL_PIPELINE_PRECISION == 2
    //! Defined as: \p 'typedef \p dmat2 \p mat2'. See also \ref VL_PIPELINE_PRECISION.
    typedef dmat2 mat2;
  #else
    //! Defined as: \p 'typedef \p fmat2 \p mat2'. See also \ref VL_PIPELINE_PRECISION.
    typedef fmat2 mat2;
  #endif
}

#endif
