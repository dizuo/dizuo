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

#ifndef Viewport_INCLUDE_ONCE
#define Viewport_INCLUDE_ONCE

#include <vl/Object.hpp>
#include <vl/Rect.hpp>
#include <vl/Vector4.hpp>
#include <vl/vlnamespace.hpp>

namespace vl
{
  //-----------------------------------------------------------------------------
  // Viewport
  //-----------------------------------------------------------------------------
  /**
   * Implements the viewport and clearing settings associated to a Camera.
   * Supports glClearColor, glClearColorIiEXT and glClearColorIuiEXT (see GL_EXT_texture_integer).
   * \sa Camera, Rendering, Renderer
  */
  class Viewport: public Object
  {
  public:
    virtual const char* className() { return "Viewport"; }
    Viewport();
    Viewport(int x, int y, int w, int h);

    void activate() const;

    bool null() { return mWidth == 0 || mHeight == 0; }

    void set(int x, int y, int w, int h) { mX = x; mY = y; mWidth = w; mHeight = h; }
    void setX(int x) { mX = x; }
    int x() const { return mX; }
    void setY(int y) { mY = y; }
    int y() const { return mY; }
    void setWidth(int width) { mWidth = width; }
    int width() const { return mWidth; }
    void setHeight(int height) { mHeight = height; }
    int height() const { return mHeight; }
    fvec2 center() const { return fvec2(mX + mWidth / 2.0f, mY + mHeight / 2.0f); }

    /**
     * Returns the rectangular area that defines the viewport computed as RectI(x(), y(), x()+width()-1, y()+height()-1).
     */
    RectI rect() const { return RectI(x(),y(),width(),height()); }

    void setClearColor(float r, float g, float b, float a) { mClearColor = fvec4(r,g,b,a); }
    void setClearColor(const fvec4& color) { mClearColor = color; }
    const fvec4& clearColor() const { return mClearColor; }

    void setClearColorInt(int r, int g, int b, int a) { mClearColorInt = ivec4(r,g,b,a); }
    void setClearColorInt(const ivec4& color) { mClearColorInt = color; }
    const ivec4& clearColorInt() const { return mClearColorInt; }

    void setClearColorUInt(unsigned int r, unsigned int g, unsigned int b, unsigned int a) { mClearColorUInt = uvec4(r,g,b,a); }
    void setClearColorUInt(const uvec4& color) { mClearColorUInt = color; }
    const uvec4& clearColorUInt() const { return mClearColorUInt; }

    void setClearStencil(int stencil) { mClearStencil = stencil; }
    int clearStencil() const { return mClearStencil; }

    void setClearDepth(Real depth) { mClearDepth = depth; }
    Real clearDepth() const { return mClearDepth; }

    void setClearFlags(EClearFlags clear_flags) { mClearFlags = clear_flags; }
    EClearFlags clearFlags() const { return mClearFlags; }

    void setClearColorMode(EClearColorMode mode) { mClearColorMode = mode; }
    EClearColorMode clearColorMode() const { return mClearColorMode; }

    /**
     * Returns true if the given point is inside the Viewport
    */
    bool isPointInside(int x, int y, int render_target_height) const;

  protected:
    fvec4 mClearColor;
    ivec4 mClearColorInt;
    uvec4 mClearColorUInt;

    Real mClearDepth;
    int mClearStencil;
    EClearFlags mClearFlags;
    EClearColorMode mClearColorMode;
    int mX;
    int mY;
    int mWidth;
    int mHeight;
  };
}

#endif
