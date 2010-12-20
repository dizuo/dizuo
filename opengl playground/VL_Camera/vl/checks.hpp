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

#ifndef CHECK_INCLUDED
#define CHECK_INCLUDED

#include <cassert>

namespace vl
{
  void log_failed_check(const char*, const char*, int);

  // Compile-time check
  #define VL_CHECK_EXPR(a,expr,b) typedef char compile_time_assert[ ((a) expr (b)) ? 1 : -1];

  #if defined(_DEBUG) || !defined(NDEBUG) || VL_FORCE_CHECKS == 1

    // Visual Studio
    #if defined(_MSC_VER) 
      #define VL_TRAP() __debugbreak(); /*{ __asm {int 3} }*/
    // GNU GCC
    #elif defined(__GNUG__) || defined(__MINGW32__) 
      #define VL_TRAP() { asm("int $0x3"); }
    // Others: fixme?
    #else 
      #define VL_TRAP() {}
    #endif

	#define VL_CHECK(expr) {assert(expr);}
    //#define VL_CHECK(expr) { if(!(expr)) { ::vl::log_failed_check(#expr,__FILE__,__LINE__); VL_TRAP() } }
    //#define VL_WARN(expr)  { if(!(expr)) { ::vl::log_failed_check(#expr,__FILE__,__LINE__); } }

    // MSDN checked iterators
    // #define _SECURE_SCL 1
  #else
    #define VL_WARN(expr) {}
    #define VL_CHECK(expr) {}
    #define VL_TRAP() {}

    // MSDN checked iterators
    // #define _SECURE_SCL 0
  #endif
}

#endif
