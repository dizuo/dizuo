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

#ifndef AABB_INCLUDE_ONCE
#define AABB_INCLUDE_ONCE

#include <vl/Vector3.hpp>
#include <vl/Matrix4.hpp>
#include <vl/config.hpp>

namespace vl
{
  //-----------------------------------------------------------------------------
  // AABB
  //-----------------------------------------------------------------------------
  /**
   * The AABB class implements an axis-aligned bounding box using vl::Real precision.
  */
  class AABB 
  {
  public:
    AABB();
    AABB( const vec3& center, Real radius );
    AABB( const vec3& pt1, const vec3& pt2, Real displace=0);

    void setNull() { mMin = 1; mMax = -1; }
    bool isNull()  const { return mMin.x() > mMax.x() || mMin.y() > mMax.y() || mMin.z() > mMax.z(); }
    bool isPoint() const { return mMin == mMax; }
    void enlarge(Real displace);
    void addPoint(const vec3& v, Real radius);
    bool intersects(const AABB & bb) const;
    vec3 clip(const vec3& v, bool clipx=true, bool clipy=true, bool clipz=true) const;
    bool isInside(const vec3& v, bool clipx, bool clipy, bool clipz) const;
    bool isInside(const vec3& v) const;
    Real height() const;
    Real width() const;
    Real depth() const;
    AABB operator+(const AABB& aabb) const;
    const AABB& operator+=(const AABB& other)
    {
      *this = *this + other;
      return *this;
    }
    AABB operator+(const vec3& p)
    {
      AABB aabb = *this;
      aabb += p;
      return aabb;
    }
    const AABB& operator+=(const vec3& p)
    {
      addPoint(p);
      return *this;
    }
    vec3 center() const;
    Real area() const
    {
      if (isNull())
        return 0;
      else 
        return width()*height()*depth();
    }
    Real longestSideLength() const
    {
      Real side = width();
      if (height() > side)
        side = height();
      if (depth() > side)
        side = depth();
      return side;
    }
    void addPoint(const vec3& v) 
    {
      if (isNull())
      {
        mMax = v;
        mMin = v;
        return;
      }

      if ( mMax.x() < v.x() ) mMax.x() = v.x() ;
      if ( mMax.y() < v.y() ) mMax.y() = v.y() ;
      if ( mMax.z() < v.z() ) mMax.z() = v.z() ;
      if ( mMin.x() > v.x() ) mMin.x() = v.x() ;
      if ( mMin.y() > v.y() ) mMin.y() = v.y() ;
      if ( mMin.z() > v.z() ) mMin.z() = v.z() ;
    }
    void transformed(AABB& aabb, const mat4& mat) const 
    {
      aabb.setNull();
      if ( !isNull() )
      {
        aabb.addPoint( mat * vec3(minCorner().x(), minCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3(minCorner().x(), maxCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3(maxCorner().x(), maxCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3(maxCorner().x(), minCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3(minCorner().x(), minCorner().y(), maxCorner().z()) );
        aabb.addPoint( mat * vec3(minCorner().x(), maxCorner().y(), maxCorner().z()) );
        aabb.addPoint( mat * vec3(maxCorner().x(), maxCorner().y(), maxCorner().z()) );
        aabb.addPoint( mat * vec3(maxCorner().x(), minCorner().y(), maxCorner().z()) );
      }
    }
    AABB transformed(const mat4& mat) const 
    {
      AABB aabb;
      transformed(aabb, mat);
      return aabb;
    }
    const vec3& minCorner() const { return mMin; }
    const vec3& maxCorner() const { return mMax; }
    void setMinCorner(Real x, Real y, Real z) { mMin = vec3(x,y,z); }
    void setMinCorner(const vec3& v) { mMin = v; }
    void setMaxCorner(Real x, Real y, Real z) { mMax = vec3(x,y,z); }
    void setMaxCorner(const vec3& v) { mMax = v; }
    Real volume() const { return width() * height() * depth(); }

  protected:
    vec3 mMin;
    vec3 mMax;
  };
}

#endif
