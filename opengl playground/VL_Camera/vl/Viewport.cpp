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

#include <vl/Viewport.hpp>
//#include <vl/Log.hpp>
//#include <vl/Say.hpp>

using namespace vl;

//-----------------------------------------------------------------------------
// Viewport
//-----------------------------------------------------------------------------
Viewport::Viewport()
{
  #ifndef NDEBUG
    mName = "Viewport";
  #endif
  mX = 0;
  mY = 0;
  mWidth = 0;
  mHeight = 0;
  mClearColor = fvec4(0.8f,0,0.1f,1);
  mClearDepth = 1.0f;
  mClearStencil = 0;
  mClearFlags = CF_CLEAR_COLOR_DEPTH;
  mClearColorMode = CCM_Float;
}
//-----------------------------------------------------------------------------
Viewport::Viewport(int x, int y, int w, int h)
{
  #ifndef NDEBUG
    mName = "Viewport";
  #endif
  mX = x;
  mY = y;
  mWidth  = w;
  mHeight = h;
  mClearColor = fvec4(0.8f,0,0.1f,1);
  mClearDepth = 1.0f;
  mClearStencil = 0;
  mClearFlags = CF_CLEAR_COLOR_DEPTH;
  mClearColorMode = CCM_Float;
}
//-----------------------------------------------------------------------------
void Viewport::activate() const
{
  // viewport
  int x = mX ;
  int y = mY ;
  int w = mWidth;
  int h = mHeight;

  if (w < 1) w = 1;
  if (h < 1) h = 1;

  glViewport(x, y, w, h);

  // clear viewport
  if (mClearFlags)
  {
    #ifndef NDEBUG
      if (!GLEW_EXT_texture_integer)
      {
        switch( clearColorMode() )
        {
        case CCM_Int:
        case CCM_UInt:
          //Log::bug("Viewport::activate(): GL_EXT_texture_integer not supported.\n");
          break;
        default:
          break;
        }
      }
    #endif

    // this are deliberately all initialized to true
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(GL_TRUE);

    // save scissor settings
    GLboolean scissor_on = glIsEnabled(GL_SCISSOR_TEST);
    int scissor_box[4] = {0,0,-1,-1};
    glGetIntegerv(GL_SCISSOR_BOX, scissor_box);

    // setup scissor
    glScissor(x, y, w, h);
    glEnable(GL_SCISSOR_TEST);

    switch( clearColorMode() )
    {
      case CCM_Float: glClearColor(        mClearColor.r(),     mClearColor.g(),     mClearColor.b(),     mClearColor.a()    );  break;
      case CCM_Int:   glClearColorIiEXT(   mClearColorInt.r(),  mClearColorInt.g(),  mClearColorInt.b(),  mClearColorInt.a() );  break;
      case CCM_UInt:  glClearColorIuiEXT(  mClearColorUInt.r(), mClearColorUInt.g(), mClearColorUInt.b(), mClearColorUInt.a() ); break;
    }

    glClearDepth( mClearDepth );
    glClearStencil( mClearStencil );

    glClear(mClearFlags);

    // restore scissor settings
    if (!scissor_on)
      glDisable(GL_SCISSOR_TEST);
    glScissor(scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]); 
  }
}
//-----------------------------------------------------------------------------
bool Viewport::isPointInside(int x, int y, int render_target_height) const
{
  // set x/y relative to the viewport
  x -= this->x();
  y -= render_target_height - 1 - (this->y() + height() -1);

  // check that the click is in the viewport

  if (x<0 || y<0 || x>=this->width() || y>=this->height())
    return false;
  else
    return true;
}
//-----------------------------------------------------------------------------
