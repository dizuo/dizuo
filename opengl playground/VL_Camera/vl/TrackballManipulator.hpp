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

#ifndef TrackballManipulator_INCLUDE_ONCE
#define TrackballManipulator_INCLUDE_ONCE

#include <vl/Camera.hpp>
#include <vl/Vector3.hpp>

namespace vl
{
  class Camera;
  class Transform;
  class Rendering;
  class SceneManager;

  //------------------------------------------------------------------------------
  //! This class lets you rotate a Camera or a Transform node using a vitual trackball.
  //! If you set a Transform node to manipulate, using the function setTransform(), this object
  //! will manipulate the given Transform. If no Transform is specified or a NULL one is
  //! passed to the function setTransform() then this object will manipulate the current camera.
  //! \note In any case, before using a TrackballManipulator you have to specify a Camera object using the function setCamera().
  //! \note The Transform is expected to contain only rotation and translation information. 
  //! Other transformations like shearing, scaling, projection, and so on can produce unspecified results.
  class TrackballManipulator : public Object
  {
  public:
    typedef enum { NoMode, RotationMode, TranslationMode, ZoomMode } ETrackballMode;
  public:
    virtual const char* className() { return "TrackballManipulator"; }
    TrackballManipulator(): 
      mMode(NoMode),
      mRotationButton(LeftButton), mTranslationButton(MiddleButton), mZoomButton(RightButton), 
      mRotationSpeed(1.0f),        mTranslationSpeed(1.0f),          mZoomSpeed(1.0f)
    {
      #ifndef NDEBUG
        mName = "TrackballManipulator";
      #endif
    }

    virtual void mouseDownEvent(EMouseButton, int x, int y);

    virtual void mouseUpEvent(EMouseButton, int x, int y);

    virtual void mouseMoveEvent(int x, int y);

    mat4 trackballRotation(int x, int y);

    vec3 computeVector(int x, int y);

    void setCamera(Camera* camera) { mCamera = camera; }

    Camera* camera() { return mCamera.get(); }

    void prepareToReconnect()
    { 
      mMode = NoMode;

      //if ( openglContext() )
      //{
      //  openglContext()->setMouseVisible(true);
      //  openglContext()->setContinuousUpdate(false);
      //}
    }

    //! Sets the center point the camera will rotate around
    void setPivot(vec3 pivot) { mPivot = pivot; }

    vec3 pivot() const { return mPivot; }

    void setTransform(Transform* tr) { mTransform = tr; }

    Transform* transform() { return mTransform.get(); }

    int rotationButton() const { return mRotationButton; }
    void setRotationButton(int mouse_button) { mRotationButton = mouse_button; }

    int zoomButton() const { return mZoomButton; }
    void setZoomButton(int mouse_button) { mZoomButton = mouse_button; }

    int translationButton() const { return mTranslationButton; }
    void setTranslationButton(int mouse_button) { mTranslationButton = mouse_button; }

    float rotationSpeed() const { return mRotationSpeed; }
    void setRotationSpeed(float speed) { mRotationSpeed = speed; }

    float zoomSpeed() const { return mZoomSpeed; }
    void setZoomSpeed(float speed) { mZoomSpeed = speed; }

    float translationSpeed() const { return mTranslationSpeed; }
    void setTranslationSpeed(float speed) { mTranslationSpeed = speed; }

    //! Returns the current trackball manipulator state.
    ETrackballMode mode() const { return mMode; }

    //! Adjusts the camera position in order to nicely see the scene. It also position the rotation pivot to the center of the AABB. See also Camera::adjustView().
    void adjustView(const AABB& aabb, const vec3& dir, const vec3& up, Real bias=1.0f);

  protected:
    ref<Camera> mCamera;
    ivec2 mMouseStart;
    mat4 mStartMatrix;
    vec3 mPivot;
    vec3 mStartCameraPos;
    vec3 mStartPivot;
    ref<Transform> mTransform;
    ETrackballMode mMode;
    int mRotationButton;
    int mTranslationButton;
    int mZoomButton;
    float mRotationSpeed;
    float mTranslationSpeed;
    float mZoomSpeed;
  };
}

#endif
