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

#ifndef Ray_INCLUDE_ONCE
#define Ray_INCLUDE_ONCE

#include <vl/Vector4.hpp>

namespace vl
{
  /** The Ray class defines a ray as an origin and direction using Real precision.
   */
  class Ray
  {
  public:
    const vec3& direction() const { return mDirection; }
    const vec3& origin() const { return mOrigin; }
    void setDirection(const vec3& dir) { mDirection = dir; }
    void setOrigin(const vec3& orig) { mOrigin = orig; }

  protected:
    vec3 mDirection;
    vec3 mOrigin;
  };
}

#endif
