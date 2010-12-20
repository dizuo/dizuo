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

#ifndef ClipPlane_INCLUDE_ONCE
#define ClipPlane_INCLUDE_ONCE

#include <vl/Plane.hpp>
#include <vl/Camera.hpp>
#include <vl/RenderState.hpp>

namespace vl
{
  //-----------------------------------------------------------------------------
  // ClipPlane
  //-----------------------------------------------------------------------------
  /**
   * Wraps the OpenGL function glClipPlane().
   *
   * \sa Shader, Effect, Actor
  */
  class ClipPlane: public RenderState
  {
  public:
    ClipPlane(int plane_index, Real o=0.0f, vec3 n=vec3(0,0,0))
    { 
      #ifndef NDEBUG
        mName = "ClipPlane";
      #endif
      mPlaneIndex = plane_index;
      mPlane.setNormal(n); 
      mPlane.setOrigin(o); 
    }

    ClipPlane(int plane_index, const vec3& o, const vec3& n)
    {
      #ifndef NDEBUG
        mName = "ClipPlane";
      #endif
      mPlaneIndex = plane_index;
      mPlane.setNormal(n); 
      mPlane.setOrigin(dot(o, n)); 
    }

    virtual const char* className() { return "ClipPlane"; }

    virtual void disable() const { VL_CHECK(planeIndex()>=0 && planeIndex()<6); glDisable(GL_CLIP_PLANE0 + planeIndex()); }
    virtual void enable () const { VL_CHECK(planeIndex()>=0 && planeIndex()<6); glEnable (GL_CLIP_PLANE0 + planeIndex()); }

    virtual ERenderState type() const { return (ERenderState)(RS_ClipPlane0 + planeIndex()); }
    virtual void apply(const Camera* camera) const
    // void applyClipPlane(const mat4& mat, int index = -1)
    {
      VL_CHECK(planeIndex()>=0 && planeIndex()<6); 
      // we do our own transformations

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();

      glLoadIdentity();

      mat4 mat;
      if ( followedTransform() )
        mat = camera->viewMatrix() * followedTransform()->worldMatrix();

      vec3 pt1 = mPlane.normal() * mPlane.origin();
        vec3 pt2 = mPlane.normal() * mPlane.origin() + mPlane.normal();

      pt1 = mat * pt1;
      pt2 = mat * pt2;

      vec3 n = pt2 - pt1;
      Real orig = dot(n, pt1);

      double equation[] = 
      {
        (double)n.x(),
        (double)n.y(),
        (double)n.z(),
        -(double)orig
      };

      glClipPlane(GL_CLIP_PLANE0 + planeIndex(), equation);

      glPopMatrix();
    }

    void followTransform(Transform* transform) { mFollowedTransform = transform; }

    Transform* followedTransform() { return mFollowedTransform.get(); }
    const Transform* followedTransform() const { return mFollowedTransform.get(); }

    void setPlaneIndex(int plane_index) { mPlaneIndex = plane_index; }
    int planeIndex() const { return mPlaneIndex; }

    const Plane& plane() const { return mPlane; }
    void setPlane(const Plane& plane) { mPlane = plane; }

  protected:
    int mPlaneIndex;
    ref<Transform> mFollowedTransform;
    Plane mPlane;
  };
  //-----------------------------------------------------------------------------
}

#endif
