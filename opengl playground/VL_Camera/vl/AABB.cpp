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

#include <vl/AABB.hpp>

using namespace vl;

//-----------------------------------------------------------------------------
// AABB
//-----------------------------------------------------------------------------
AABB::AABB() 
{
  setNull();
}
//-----------------------------------------------------------------------------
AABB::AABB( const vec3& center, Real radius ) 
{
  mMax = center + radius;
  mMin = center - radius;
}
//-----------------------------------------------------------------------------
AABB::AABB( const vec3& pt1, const vec3& pt2, Real displace) 
{
  mMax = mMin = pt1;
  if ( mMax.x() < pt2.x() ) mMax.x() = pt2.x();
  if ( mMax.y() < pt2.y() ) mMax.y() = pt2.y();
  if ( mMax.z() < pt2.z() ) mMax.z() = pt2.z();
  if ( mMin.x() > pt2.x() ) mMin.x() = pt2.x();
  if ( mMin.y() > pt2.y() ) mMin.y() = pt2.y();
  if ( mMin.z() > pt2.z() ) mMin.z() = pt2.z();

  mMax = mMax + displace;
  mMin = mMin - displace;
}
//-----------------------------------------------------------------------------
void AABB::enlarge(Real displace) {
  if ( isNull() )
    return;

  mMax = mMax + displace;
  mMin = mMin - displace;
}
//-----------------------------------------------------------------------------
bool AABB::intersects(const AABB& bb) const
{
  if (isNull() || bb.isNull())
    return false;

  if (mMax.x() < bb.mMin.x())
    return false;

  if (mMax.z() < bb.mMin.z())
    return false;

  if (mMin.x() > bb.mMax.x())
    return false;

  if (mMin.z() > bb.mMax.z())
    return false;

  return true;
}
//-----------------------------------------------------------------------------
vec3 AABB::clip(const vec3& v, bool clipx, bool clipy, bool clipz) const 
{
  if (isNull())
    return v;

  vec3 tmp = v;

  if (clipx) {
      if (v.x() > mMax.x())
        tmp.x() = mMax.x();
      if (v.x() < mMin.x())
        tmp.x() = mMin.x();
  }

  if (clipy) {
      if (v.y() > mMax.y())
        tmp.y() = mMax.y();
      if (v.y() < mMin.y())
        tmp.y() = mMin.y();
    }

  if (clipz) {
      if (v.z() > mMax.z())
        tmp.z() = mMax.z();
      if (v.z() < mMin.z())
        tmp.z() = mMin.z();
    }
    return tmp;
}
//-----------------------------------------------------------------------------
bool AABB::isInside(const vec3& v, bool clipx, bool clipy, bool clipz) const 
{
  vec3 t = v;
  return v == clip(t, clipx, clipy, clipz);
}
//-----------------------------------------------------------------------------
bool AABB::isInside(const vec3& v) const 
{
  return v.x() >= minCorner().x() && v.x() <= maxCorner().x() &&
         v.y() >= minCorner().y() && v.y() <= maxCorner().y() &&
         v.z() >= minCorner().z() && v.z() <= maxCorner().z() ;
}
//-----------------------------------------------------------------------------
void AABB::addPoint(const vec3& v, Real radius) 
{
  if (isNull())
  {
    mMax = v;
    mMin = v;
    return;
  }

  if ( mMax.x() < v.x() + radius) mMax.x() = v.x() + radius;
  if ( mMax.y() < v.y() + radius) mMax.y() = v.y() + radius;
  if ( mMax.z() < v.z() + radius) mMax.z() = v.z() + radius;
  if ( mMin.x() > v.x() - radius) mMin.x() = v.x() - radius;
  if ( mMin.y() > v.y() - radius) mMin.y() = v.y() - radius;
  if ( mMin.z() > v.z() - radius) mMin.z() = v.z() - radius;
}
//-----------------------------------------------------------------------------
Real AABB::width() const {
  if (isNull())
    return 0;
  else
    return mMax.x() - mMin.x();
}
//-----------------------------------------------------------------------------
Real AABB::height() const {
  if (isNull())
    return 0;
  else
    return mMax.y() - mMin.y();
}
//-----------------------------------------------------------------------------
Real AABB::depth() const {
  if (isNull())
    return 0;
  else
    return mMax.z() - mMin.z();
}
//-----------------------------------------------------------------------------
AABB AABB::operator+(const AABB& aabb) const 
{
  if(isNull())
    return aabb;
  if (aabb.isNull())
    return *this;
  AABB tmp = aabb;
  tmp.addPoint( mMin );
  tmp.addPoint( mMax );
  return tmp;
}
//-----------------------------------------------------------------------------
vec3 AABB::center() const
{
  vec3 c = (minCorner() + maxCorner()) / 2.0f;
  return c;
}
//-----------------------------------------------------------------------------
