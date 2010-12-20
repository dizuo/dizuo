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

#include <vl/RayIntersector.hpp>
#include <vl/SceneManager.hpp>

using namespace vl;

//-----------------------------------------------------------------------------
void RayIntersector::intersect(const Ray& ray, SceneManager* scene_manager)
{
  actors()->clear();
  scene_manager->appendActors( *actors() );
  setRay(ray);
  intersect();
}
//-----------------------------------------------------------------------------
void RayIntersector::intersect()
{
  mIntersections.clear();
  for(int i=0; i<actors()->size(); ++i)
  {
    if (!frustum().cull(actors()->at(i)->boundingBox()))
    {
      intersect(actors()->at(i));
    }
  }

  std::sort( mIntersections.begin(), mIntersections.end(), sorter );
}
//-----------------------------------------------------------------------------
void RayIntersector::intersect(Actor* act)
{
  Geometry* geom = dynamic_cast<Geometry*>(act->lod(0).get());
  if (geom)
    intersectGeometry(act, geom);
}
//-----------------------------------------------------------------------------
void RayIntersector::intersectGeometry(Actor* act, Geometry* geom)
{
  fmat4 matrix = act->transform() ? (fmat4)act->transform()->worldMatrix() : fmat4();
  ArrayFVec3* vert3f = dynamic_cast<ArrayFVec3*>(geom->vertexArray());
  for(int i=0; i<geom->primitives()->size(); ++i)
  {
    Primitives* prim = geom->primitives()->at(i);
    if (prim->primitiveType() == vl::PT_TRIANGLES)
    {
      for(unsigned itri=0; itri<prim->indexCount(); itri+=3)
      {
        fvec3 a = vert3f->at(prim->index(itri+0));
        fvec3 b = vert3f->at(prim->index(itri+1));
        fvec3 c = vert3f->at(prim->index(itri+2));
        if (act->transform())
        {
          a = matrix * a;
          b = matrix * b;
          c = matrix * c;
        }
        intersectTriangle(a,b,c, act,geom,prim,itri);
      }
    }
    else
    if (prim->primitiveType() == vl::PT_TRIANGLE_FAN)
    {
      for(unsigned itri=1; itri<prim->indexCount()-1; ++itri)
      {
        fvec3 a = vert3f->at(prim->index(0));
        fvec3 b = vert3f->at(prim->index(itri));
        fvec3 c = vert3f->at(prim->index(itri+1));
        if (act->transform())
        {
          a = matrix * a;
          b = matrix * b;
          c = matrix * c;
        }
        intersectTriangle(a,b,c, act,geom,prim,itri);
      }
    }
    else
    if (prim->primitiveType() == vl::PT_TRIANGLE_STRIP)
    {
      for(unsigned itri=0; itri<prim->indexCount()-2; ++itri)
      {
        fvec3 a = vert3f->at(prim->index(itri));
        fvec3 b = vert3f->at(prim->index(itri+1));
        fvec3 c = vert3f->at(prim->index(itri+2));
        if (act->transform())
        {
          a = matrix * a;
          b = matrix * b;
          c = matrix * c;
        }
        intersectTriangle(a,b,c, act,geom,prim,itri);
      }
    }
    else
    if (prim->primitiveType() == vl::PT_POLYGON)
    {
      std::vector<fvec3> polygon;
      for(unsigned itri=0; itri<prim->indexCount(); ++itri)
      {
        fvec3 a = vert3f->at(prim->index(itri));
        if (act->transform())
          a = matrix * a;
        polygon.push_back(a);
      }
      VL_CHECK(polygon.size() >= 3)
      intersectPolygon(polygon, act,geom,prim);
    }
    else
    if (prim->primitiveType() == vl::PT_QUADS)
    {
      for(unsigned iquad=0; iquad<prim->indexCount(); iquad+=4)
      {
        fvec3 a = vert3f->at(prim->index(iquad+0));
        fvec3 b = vert3f->at(prim->index(iquad+1));
        fvec3 c = vert3f->at(prim->index(iquad+2));
        fvec3 d = vert3f->at(prim->index(iquad+3));
        if (act->transform())
        {
          a = matrix * a;
          b = matrix * b;
          c = matrix * c;
          d = matrix * d;
        }
        intersectQuad(a,b,c,d, act,geom,prim,iquad);
      }
    }
    else
    if (prim->primitiveType() == vl::PT_QUAD_STRIP)
    {
      for(unsigned iquad=0; iquad<prim->indexCount()-3; iquad+=2)
      {
        fvec3 a = vert3f->at(prim->index(iquad+0));
        fvec3 b = vert3f->at(prim->index(iquad+1));
        fvec3 c = vert3f->at(prim->index(iquad+2));
        fvec3 d = vert3f->at(prim->index(iquad+3));
        if (act->transform())
        {
          a = matrix * a;
          b = matrix * b;
          c = matrix * c;
          d = matrix * d;
        }
        intersectQuad(a,b,c,d, act,geom,prim,iquad);
      }
    }
  }
}
//-----------------------------------------------------------------------------
void RayIntersector::intersectTriangle(const fvec3& a, const fvec3& b, const fvec3& c, Actor* act, Geometry* geom, Primitives* prim, int prim_idx)
{
  fvec3 v1 = b-a;
  fvec3 v2 = c-a;
  fvec3 n = cross(v1,v2).normalize();
  float det = dot(n,(fvec3)ray().direction());
  if(det == 0.0f)
    return;
  float t = dot(n, a-(fvec3)ray().origin()) / det;
  if (t<0)
    return;
  vec3  rp = ray().origin() + ray().direction()*t;
  fvec3 fp = (fvec3)rp;
  fvec3 pts[] = { a, b, c, a };
  for(int i=0; i<3; ++i)
  {
    fvec3 bi_norm = -cross(pts[i+1]-pts[i],n).normalize();
    if (dot(fp-pts[i],bi_norm) < 0)
      return;
  }
  ref<RayIntersectionGeometry> record = new vl::RayIntersectionGeometry;
  record->setIntersectionPoint( rp );
  record->setPrimitiveIndex(prim_idx);
  record->setActor(act);
  record->setGeometry(geom);
  record->setPrimitives(prim);
  record->setDistance( t );
  mIntersections.push_back(record);
}
//-----------------------------------------------------------------------------
void RayIntersector::intersectPolygon(const std::vector<fvec3>& polygon, Actor* act, Geometry* geom, Primitives* prim)
{
  fvec3 v1 = polygon[1]-polygon[0];
  fvec3 v2 = polygon[2]-polygon[0];
  fvec3 n = cross(v1,v2).normalize();
  float det = dot(n,(fvec3)ray().direction());
  if(det == 0.0f)
    return;
  float t = dot(n, polygon[0]-(fvec3)ray().origin()) / det;
  if (t<0)
    return;
  vec3  rp = ray().origin() + ray().direction()*t;
  fvec3 fp = (fvec3)rp;
  for(unsigned i=0; i<polygon.size(); ++i)
  {
    int i2 = (i+1) % polygon.size();
    fvec3 bi_norm = -cross(polygon[i2]-polygon[i],n).normalize();
    if (dot(fp-polygon[i],bi_norm) < 0)
      return;
  }
  ref<RayIntersectionGeometry> record = new vl::RayIntersectionGeometry;
  record->setIntersectionPoint( rp );
  record->setPrimitiveIndex(0);
  record->setActor(act);
  record->setGeometry(geom);
  record->setPrimitives(prim);
  record->setDistance( t );
  mIntersections.push_back(record);
}
//-----------------------------------------------------------------------------
void RayIntersector::intersectQuad(const fvec3& a, const fvec3& b, const fvec3& c, const fvec3& d, Actor* act, Geometry* geom, Primitives* prim, int prim_idx)
{
  fvec3 v1 = b-a;
  fvec3 v2 = c-a;
  fvec3 n = cross(v1,v2).normalize();
  float det = dot(n,(fvec3)ray().direction());
  if(det == 0.0f)
    return;
  float t = dot(n, a-(fvec3)ray().origin()) / det;
  if (t<0)
    return;
  vec3  rp = ray().origin() + ray().direction()*t;
  fvec3 fp = (fvec3)rp;
  fvec3 pts[] = {a,b,c,d,a};
  for(int i=0; i<4; ++i)
  {
    fvec3 bi_norm = -cross(pts[i+1]-pts[i],n).normalize();
    if (dot(fp-pts[i],bi_norm) < 0)
      return;
  }
  ref<RayIntersectionGeometry> record = new vl::RayIntersectionGeometry;
  record->setIntersectionPoint( rp );
  record->setPrimitiveIndex(prim_idx);
  record->setActor(act);
  record->setGeometry(geom);
  record->setPrimitives(prim);
  record->setDistance( t );
  mIntersections.push_back(record);
}
//-----------------------------------------------------------------------------
