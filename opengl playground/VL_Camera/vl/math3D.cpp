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

#include <vl/math3D.hpp>
#include <vl/AABB.hpp>
#include <vl/Vector2.hpp>
#include <vl/Vector4.hpp>
#include <vl/quat.hpp>
#include <vl/Plane.hpp>
#include <vl/checks.hpp>
#include <cstdlib>

using namespace vl;

//-----------------------------------------------------------------------------
Real vl::randomMinMax(Real min, Real max)
{
  Real t = rand() / (Real)RAND_MAX;
  return min + (max-min)*t;
}
//-----------------------------------------------------------------------------
int vl::greaterEqualPow2(int n)
{
  int pow2=2;
  for(int i=0; i<20; ++i) {
    if (pow2 >= n)
      return pow2;
    pow2 = pow2 * 2;
  }
  return pow2;
}
//-----------------------------------------------------------------------------
int vl::smallerEqualPow2(int n)
{
  int pow2=2;
  for(int i=0; i<20; ++i) {
    if (pow2 > n)
      return pow2/2;
    pow2 = pow2 * 2;
  }
  return pow2;
}
//-----------------------------------------------------------------------------
void vl::extractPlanes( Plane* planes, const mat4& modelviewproj )
{
  // see also http://www2.ravensoft.com/users/ggribb/plane%20extraction.pdf
  // see also http://zach.in.tu-clausthal.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
  // the equation is a*x+b*y+x*z+d = 0
  // where <a b c> is the normal of the plane
  // the normals are pointing inside the viewfrustum

  // 1) we want the planes to point outward so we reverse them
  // 2) because of 1) "d" becomes the distance of the plane from the origin

  vec3 n;
  Real d;

  // left clipping plane
  n.x() = modelviewproj[0][3] + modelviewproj[0][0];
  n.y() = modelviewproj[1][3] + modelviewproj[1][0];
  n.z() = modelviewproj[2][3] + modelviewproj[2][0];
  d = modelviewproj[3][3] + modelviewproj[3][0];
  d /= n.length();
  n.normalize();
  planes[0] = Plane(d,-n);

  // right clipping plane
  n.x() = modelviewproj[0][3] - modelviewproj[0][0];
  n.y() = modelviewproj[1][3] - modelviewproj[1][0];
  n.z() = modelviewproj[2][3] - modelviewproj[2][0];
  d = modelviewproj[3][3] - modelviewproj[3][0];
  d /= n.length();
  n.normalize();
  planes[1] = Plane(d,-n);

  // top clipping plane
  n.x() = modelviewproj[0][3] - modelviewproj[0][1];
  n.y() = modelviewproj[1][3] - modelviewproj[1][1];
  n.z() = modelviewproj[2][3] - modelviewproj[2][1];
  d = modelviewproj[3][3] - modelviewproj[3][1];
  d /= n.length();
  n.normalize();
  planes[2] = Plane(d,-n);

  // bottom clipping plane
  n.x() = modelviewproj[0][3] + modelviewproj[0][1];
  n.y() = modelviewproj[1][3] + modelviewproj[1][1];
  n.z() = modelviewproj[2][3] + modelviewproj[2][1];
  d = modelviewproj[3][3] + modelviewproj[3][1];
  d /= n.length();
  n.normalize();
  planes[3] = Plane(d,-n);

  // near clipping plane
  n.x() = modelviewproj[0][3] + modelviewproj[0][2];
  n.y() = modelviewproj[1][3] + modelviewproj[1][2];
  n.z() = modelviewproj[2][3] + modelviewproj[2][2];
  d = modelviewproj[3][3] + modelviewproj[3][2];
  d /= n.length();
  n.normalize();
  planes[4] = Plane(d,-n);

  // far clipping plane
  n.x() = modelviewproj[0][3] - modelviewproj[0][2];
  n.y() = modelviewproj[1][3] - modelviewproj[1][2];
  n.z() = modelviewproj[2][3] - modelviewproj[2][2];
  d = modelviewproj[3][3] - modelviewproj[3][2];
  d /= n.length();
  n.normalize();
  planes[5] = Plane(d,-n);
}
