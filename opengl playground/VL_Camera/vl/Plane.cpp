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

#include <vl/Plane.hpp>
#include <vl/AABB.hpp>

using namespace vl;

//-----------------------------------------------------------------------------
// Plane
//-----------------------------------------------------------------------------
Real Plane::distance(const vec3 &v) const 
{
  return dot(v, mNormal) - mOrigin;
}
//-----------------------------------------------------------------------------
bool Plane::isOutside(const AABB& aabb) const
{
  vec3 pt;
  pt.x() = mNormal.x() >= 0 ? aabb.minCorner().x() : aabb.maxCorner().x();
  pt.y() = mNormal.y() >= 0 ? aabb.minCorner().y() : aabb.maxCorner().y();
  pt.z() = mNormal.z() >= 0 ? aabb.minCorner().z() : aabb.maxCorner().z();
  return distance(pt) >= 0;
}
//-----------------------------------------------------------------------------
int Plane::classify(const AABB& aabb) const
{
  vec3 corner[] = 
  {
    vec3(aabb.minCorner().x(), aabb.minCorner().y(), aabb.minCorner().z()),
    vec3(aabb.minCorner().x(), aabb.minCorner().y(), aabb.maxCorner().z()),
    vec3(aabb.minCorner().x(), aabb.maxCorner().y(), aabb.minCorner().z()),
    vec3(aabb.minCorner().x(), aabb.maxCorner().y(), aabb.maxCorner().z()),
    vec3(aabb.maxCorner().x(), aabb.minCorner().y(), aabb.minCorner().z()),
    vec3(aabb.maxCorner().x(), aabb.minCorner().y(), aabb.maxCorner().z()),
    vec3(aabb.maxCorner().x(), aabb.maxCorner().y(), aabb.minCorner().z()),
    vec3(aabb.maxCorner().x(), aabb.maxCorner().y(), aabb.maxCorner().z())
  };

  int left  = 0;
  int right = 0;
  Real const NEPS = -0.0001f;
  Real const PEPS = +0.0001f;

  for(int i=0 ;i<8; ++i)
  {
    if ( distance(corner[i]) < NEPS ) 
      left++;
    else 
    if ( distance(corner[i]) > PEPS ) 
      right++;
    // else -> we don't count the points on the plane

    if(left && right) // its clearly intersecting the plane
      return 0;
  }

  if (left)
    return -1;
  else 
  if (right)
    return +1;
  else // all the points were on the plane
    return 0;
}
//-----------------------------------------------------------------------------
