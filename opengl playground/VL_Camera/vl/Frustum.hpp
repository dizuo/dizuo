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

#ifndef Frustum_INCLUDE_ONCE
#define Frustum_INCLUDE_ONCE

#include <vl/Plane.hpp>
#include <vl/AABB.hpp>
#include <vl/Sphere.hpp>

namespace vl
{
  //-----------------------------------------------------------------------------
  // Frustum
  //-----------------------------------------------------------------------------
  /**
   * A set of planes defining a frustum used for culling purposes (frustum culling).
   *
   * \sa Camera, Viewport
  */
  class Frustum: public Object
  {
  public:
    virtual const char* className() { return "Frustum"; }

    std::vector<Plane>& planes() { return mPlanes; }
    const std::vector<Plane>& planes() const { return mPlanes; }

    void setPlane(unsigned i, const Plane& plane) { VL_CHECK(i<mPlanes.size()); mPlanes[i] = plane; }
    const Plane& plane(unsigned i) const { VL_CHECK(i<mPlanes.size()); return mPlanes[i]; }

    bool cull(const Sphere& sphere) const
    {
      // null spheres are always visible
      if (sphere.isNull())
        return false;
      for(unsigned i=0; i<planes().size(); ++i)
      {
        if ( plane(i).distance(sphere.center()) > sphere.radius() )
          return true;
      }
      return false;
    }

    bool cull(const AABB& aabb) const
    {
      if (aabb.isNull())
        return false;
      for(unsigned i=0; i<planes().size(); ++i)
      {
        if ( plane(i).isOutside(aabb) )
          return true;
      }
      return false;
    }

    bool cull(const std::vector<fvec3>& points) const
    {
      for(unsigned i=0; i<planes().size(); ++i)
      {
        unsigned j=0;
        for(; j<points.size(); ++j)
          if ( plane(i).distance((vec3)points[j]) <= 0 )
            break;
        if(j == points.size())
          return true;
      }
      return false;
    }

  protected:
    std::vector<Plane> mPlanes;
  };
}

#endif
