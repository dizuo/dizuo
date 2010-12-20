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

#ifndef math3D_INCLUDE_ONCE
#define math3D_INCLUDE_ONCE

#include <vl/GLSLmath.hpp>
#include <vl/Plane.hpp>
#include <cstdio>
#include <memory.h>
#include <vector>

namespace vl
{
  /** 
   * Returns a random number N that between 'min' and 'max' included.
   */
  Real randomMinMax(Real min, Real max);

  /** 
   * Returns a number N that is a power of 2 and that is equal to or greater than 'n'.
   */
  int greaterEqualPow2(int n);

  /** 
   * Returns a number N that is a power of 2 and that is equal to or smaller than 'n'.
   */
  int smallerEqualPow2(int n);

  /**
   * Extracts the 6 frustum planes for the given model-view-projection matrix.
   */
  void extractPlanes( Plane* planes, const mat4& modelviewproj );
}

#endif
