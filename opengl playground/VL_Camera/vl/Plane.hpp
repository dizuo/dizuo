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

#ifndef Plane_INCLUDE_ONCE
#define Plane_INCLUDE_ONCE

#include <vl/Object.hpp>
#include <vl/GLSLmath.hpp>
#include <vl/Transform.hpp>

namespace vl
{
  class AABB;

  //-----------------------------------------------------------------------------
  // Plane
  //-----------------------------------------------------------------------------
  /**
   * The Plane class defines a plane using a normal and an origin.
  */
  class Plane: public Object
  {
  public:
    Plane( Real o=0.0f, vec3 n=vec3(0,0,0) ): mNormal(n), mOrigin(o) 
    {
      #ifndef NDEBUG
        mName = "Plane";
      #endif
    }
    Plane( const vec3& o, const vec3& n ) 
    {
      #ifndef NDEBUG
        mName = "Plane";
      #endif
      mNormal = n;
      mOrigin = dot(o, n);
    }
    virtual const char* className() { return "Plane"; }
    Real distance(const vec3 &v) const;
    //! returns 0 if the AABB intersects the plane, 1 if it's in the positive side, 
    //! -1 if it's in the negative side.
    int classify(const AABB&) const;
    bool isOutside(const AABB&) const;
    const vec3& normal() const { return mNormal; }
    Real origin() const { return mOrigin; }
    void setNormal(const vec3& normal) { mNormal = normal; }
    void setOrigin(Real origin) { mOrigin = origin; }

  protected:
    vec3 mNormal;
    Real mOrigin;
  };
}

#endif
