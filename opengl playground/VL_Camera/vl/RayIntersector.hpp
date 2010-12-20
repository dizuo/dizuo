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

#ifndef RayIntersector_INCLUDE_ONCE
#define RayIntersector_INCLUDE_ONCE

#include <vl/Ray.hpp>
#include <vl/Actor.hpp>
#include <vl/Geometry.hpp>
#include <vl/Vector4.hpp>
#include <vl/Matrix4.hpp>

namespace vl
{
  class SceneManager;
  //-----------------------------------------------------------------------------
  // RayIntersection
  //-----------------------------------------------------------------------------
  /** The RayIntersection encapsulates all the information relative to a Ray/Actor intersection.
   */
  class RayIntersection: public Object
  {
  public:
    virtual const char* className() { return "RayIntersection"; }
    RayIntersection(): mActor(NULL), mDistance(0.0f) 
    {
      #ifndef NDEBUG
        mName = className();
      #endif
    }

    //! The intersected Actor
    void setActor(Actor* a) { mActor = a; }
    //! The intersected Actor
    const Actor* actor() const { return mActor; }
    //! The intersected Actor
    Actor* actor() { return mActor; }

    //! The intersection point, in world coordinates
    const vec3& intersectionPoint() const { return mIntersectionPoint; }
    //! The intersection point, in world coordinates
    void setIntersectionPoint(const vec3& v) { mIntersectionPoint = v; }

    //! The intersection distance
    float distance() const { return mDistance; }
    //! The intersection distance
    void setDistance(float dist) { mDistance = dist; }

  protected:
    vec3 mIntersectionPoint;
    Actor* mActor;
    float mDistance;
  };
  //-----------------------------------------------------------------------------
  // RayIntersectionGeometry
  //-----------------------------------------------------------------------------
  /** The RayIntersectionGeometry encapsulates all the information relative to a Ray/Actor intersection, 
      providing also extra information relative to the intersection on the Geometry in use by the Actor.
   */
  class RayIntersectionGeometry: public RayIntersection
  {
  public:
    virtual const char* className() { return "RayIntersectionGeometry"; }
    RayIntersectionGeometry(): mGeometry(NULL), mPrimitives(NULL), mPrimitiveIndex(-1) 
    {
      #ifndef NDEBUG
        mName = className();
      #endif
    }

    //! The intersected Geometry
    Geometry* geometry() { return mGeometry; }
    //! The intersected Geometry
    const Geometry* geometry() const { return mGeometry; }
    //! The intersected Primitives
    Primitives* primitives() { return mPrimitives; }
    //! The intersected Primitives
    const Primitives* primitives() const { return mPrimitives; }
    //! The starting index of the intersected primitive inside primitives()
    int primitiveIndex() const { return mPrimitiveIndex; }

    //! The intersected Geometry
    void setGeometry(Geometry* g) { mGeometry = g; }
    //! The intersected Primitives
    void setPrimitives(Primitives* p) { mPrimitives = p; }
    //! The starting index of the intersected primitive inside primitives()
    void setPrimitiveIndex(int p_idx) { mPrimitiveIndex = p_idx; }

  protected:
    vec3 mIntersectionPoint;
    Geometry* mGeometry;
    Primitives* mPrimitives;
    int mPrimitiveIndex;
    float mDistance;
  };
  //-----------------------------------------------------------------------------
  // RayIntersector
  //-----------------------------------------------------------------------------
  /** The RayIntersector class is used to detect the intersection points between a Ray and a set of Actor[s]
   */
  class RayIntersector: public Object
  {
  public:
    virtual const char* className() { return "RayIntersector"; }
    RayIntersector()
    {
      #ifndef NDEBUG
        mName = className();
      #endif
      mActors = new ActorCollection;
    }

    //! The Actors against which the intersection test is performed.
    const ActorCollection* actors() const { return mActors.get(); }
    //! The Actors against which the intersection test is performed.
    ActorCollection* actors() { return mActors.get(); }

    //! The ray in world coordinates to be intersected with the actors()
    const Ray& ray() const { return mRay; }
    //! The ray in world coordinates to be intersected with the actors()
    void setRay(const Ray& ray) { mRay = ray; }

    //! The frustum in world coordinates used to cull the objects.
    const Frustum& frustum() const { return mFrustum; }
    //! The frustum in world coordinates used to cull the objects.
    void setFrustum(const Frustum& frustum) { mFrustum = frustum; }

    //! The intersection points detected by the last intersect() call sorted according to their distance (the first one is the closest).
    const std::vector< ref<RayIntersection> >& intersections() const { return mIntersections; }

    /** Executes the intersection test.
     * \note Before calling this function the transforms and the bounding volumes of the Actor[s] to be intersected must be updated, in this order.
     * \note All the intersections are mande on the Actor's LOD level #0.
     */
    void intersect();

    /** Computes the intersections between the given ray and the Actor[s] contained in the given scene manager.
      * This is an utility function equivalent to:
      * \code
      * intersector->actors()->clear();
      * scene_manager->appendActors( *intersector->actors() );
      * intersector->setRay(ray);
      * intersector->intersect();
      * \endcode
      */
    void intersect(const Ray& ray, SceneManager* scene_manager);

  protected:
    static bool sorter(const ref<RayIntersection>& a, const ref<RayIntersection>& b) { return a->distance() < b->distance(); }

    void intersect(Actor* act);
    void intersectGeometry(Actor* act, Geometry* geom);

    void intersectTriangle(const fvec3& a, const fvec3& b, const fvec3& c, Actor*, Geometry* geom, Primitives* prim, int prim_idx);

    void intersectPolygon(const std::vector<fvec3>& polygon, Actor*, Geometry* geom, Primitives* prim);

    void intersectQuad(const fvec3& a, const fvec3& b, const fvec3& c, const fvec3& d, Actor*, Geometry* geom, Primitives* prim, int prim_idx);

  protected:
    Frustum mFrustum;
    std::vector< ref<RayIntersection> > mIntersections;
    ref<ActorCollection> mActors;
    Ray mRay;
  };
}

#endif
