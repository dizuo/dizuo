/*
_______________________________________________________________________
__________________________ G E O M E T R Y ____________________________
|
| THIS FILE IS PART OF THE GEOMETRY TEMPLATE LIBRARY.
| USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     
| GOVERNED BY A BSD-STYLE SOURCE LICENSE.
| PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       
_______________________________________________________________________
_______________________________________________________________________
*/

#ifndef RAY_H
#define RAY_H

#include "gtl.hpp"
#include "vec3.hpp"
#include "plane.hpp"
#include "box3.hpp"

namespace gtl
{
    /*!
    \class Ray Ray.hpp geometry/Ray.hpp
    \brief Represents a directed ray in 3D.
    \ingroup base

    This class is used by many other classes.

    \sa Vec3  
    */
    template<typename Type>
    class Ray
    {
    public:
        //! Constructs an instance with initial values from \a a_ray.
        Ray(const Ray<Type> & a_ray)
        {
            m_origin = a_ray.m_origin;
            m_direction = a_ray.m_direction;
        }

        //! Create a ray from a_origin with the direction a_direction. If \a normalize is true, \a a_direction will be normalized.
        Ray(const Vec3<Type> & a_origin, const Vec3<Type> & a_direction, bool normalize = true)
        {
            setValue(a_origin, a_direction, normalize);
        }

        //! Default destructor does nothing.
        virtual ~Ray(){}

        //! Set position and direction of the ray. If \a normalize is true, \a a_direction will be normalized.
        void setValue(const Vec3<Type> & a_origin, const Vec3<Type> & a_direction, bool normalize = true)
        {
            m_origin = a_origin;
            m_direction = a_direction;
            if(normalize) m_direction.normalize();
        }

        //! Return the ray origin.
        const Vec3<Type> & getOrigin() const
        { 
            return m_origin; 
        }

        //! Return the ray normalized direction.
        const Vec3<Type> & getDirection() const
        { 
            return m_direction; 
        }

        //! Get point on ray at t
        Vec3<Type> getValue(Type t) const
        {
            return Vec3<Type>(m_origin + t*m_direction);
        }

        //! Distance point line
        Type getDistance(const Vec3<Type> & pt) const
        {
            return std::sqrt(getSqrDistance(pt));
        }

        //! Squared distance point line
        Type getSqrDistance(const Vec3<Type> & pt) const
        {
            Type t = (pt - m_origin).dot(m_direction);

            return (getValue(t) - pt).sqrLength();
        }

        //! Reflects about \a normal and return a new ray with origin at \a newOrigin.
        Ray<Type> reflect(const Vec3<Type>& newOrigin, const Vec3<Type>& normal) const
        {
            return Ray<Type>(newOrigin, m_direction - 2.0f * (m_direction.dot(normal)) * normal);
        }

        //! Project the given point on the ray.
        Vec3<Type> project(const Vec3<Type> & pt) const
        {
            Type numerator = (pt - m_origin).dot(m_direction);
            Type denumerator = m_direction.length();

            return (m_origin + m_direction * (numerator / denumerator));
        }

		/*!	V0，V1，V2三角形三顶点
			I为交点
			返回值	-1：没有交点
					0：	交点在三角形外
					1：交点在三角形内部
		*/

		int	intersect( const gtl::Vec3<Type>& V0, const gtl::Vec3<Type>& V1, const gtl::Vec3<Type>& V2, gtl::Vec3<Type>& I ) const
		{
			Vec3<Type>    u, v, n;             // triangle vectors
			Vec3<Type>    dir, w0, w;          // ray vectors
			Type     r, a, b;             // params to calc ray-plane intersect

			// get triangle edge vectors and plane normal
			u = V1 - V0;
			v = V2 - V0;
			n = u.cross(v);

			if (n == Vec3<Type>(0, 0, 0))            // triangle is degenerate
				return -1;                 // do not deal with this case

			dir = getDirection();             // ray direction vector
			w0 = getOrigin() - V0;

			a = -( n.dot(w0) );
			b = n.dot(dir);

			if (fabs(b) < 0.00000001) {     // ray is parallel to triangle plane
				if (a == 0)                // ray lies in triangle plane
					return 2;
				else return 0;             // ray disjoint from plane
			}

			// get intersect point of ray with triangle plane
			r = a / b;
			if (r < 0.0)                   // ray goes away from triangle
				return 0;                  // => no intersect
			// for a segment, also test if (r > 1.0) => no intersect

			I = getOrigin() + r * dir;           // intersect point of ray and plane

			//// is I inside T?
			Type    uu, uv, vv, wu, wv, D;

			uu = u.dot(u);
			uv = u.dot(v);
			vv = v.dot(v);
			w = I - V0;
			wu = w.dot(u);
			wv = w.dot(v);
			D = uv * uv - uu * vv;

			// get and test parametric coords
			Type s, t;
			s = (uv * wv - vv * wu) / D;
			if (s < 0.0 || s > 1.0)        // I is outside T
				return 0;
			t = (uv * wu - uu * wv) / D;
			if (t < 0.0 || (s + t) > 1.0)  // I is outside T
				return 0;

			return 1;                      // I is in T
		}


		/*! Intersect the ray with the given triangle defined by vert0,vert1,vert2.
        Return true if there is an intersection.
        If there is an intersection, a vector a_tuv is returned, where t is the 
        distance to the plane in which the triangle lies and (u,v) represents the 
        coordinates inside the triangle. 
        */
        //bool intersect(const Vec3<Type> & vert0, const Vec3<Type> & vert1, const Vec3<Type> & vert2, Vec3<Type> & a_tuv) const
        //{
        //    // Tomas Moller and Ben Trumbore.
        //    // Fast, minimum storage ray-triangle intersection.
        //    // Journal of graphics tools, 2(1):21-28, 1997

        //    // find vectors for two edges sharing vert0 
        //    Vec3<Type> edge1 = vert1 - vert0;
        //    Vec3<Type> edge2 = vert2 - vert0;

        //    // begin calculating determinant - also used to calculate U parameter 
        //    Vec3<Type> pvec = m_direction.cross(edge2);

        //    // if determinant is near zero, ray lies in plane of triangle 
        //    Type det = edge1.dot(pvec);

        //    if (det < EPS)
        //        return false;

        //    // calculate distance from vert0 to ray origin 
        //    Vec3<Type> tvec = m_origin - vert0;

        //    // calculate U parameter and test bounds
        //    a_tuv[1] = tvec.dot(pvec);

        //    if (a_tuv[1] < 0.0 || a_tuv[1] > det)
        //        return false;

        //    // prepare to test V parameter
        //    Vec3<Type> qvec = tvec.cross(edge1);

        //    // calculate V parameter and test bounds
        //    a_tuv[2] = m_origin.dot(qvec);

        //    if (a_tuv[2] < 0.0 || a_tuv[1] + a_tuv[2] > det)
        //        return false;

        //    // calculate t, scale parameters, ray intersects triangle
        //    a_tuv[0] = edge2.dot(qvec);

        //    Type inv_det = (Type)1.0 / det;

        //    a_tuv[0] *= inv_det;
        //    a_tuv[1] *= inv_det;
        //    a_tuv[2] *= inv_det;

        //    return true;
        //}

		/*!Intersect the ray with the given plane defined by the normal vector and any one point
		return the value in the result vector
		*/	
		bool intersect(const gtl::Vec3<Type> & plane_norm, const gtl::Vec3<Type> & plane_pnt, gtl::Vec3<Type>& result)
		{
			if (plane_norm.normalize())
				plane_norm.normalize();

			// Check if the ray is parallel to the plane.
			Type denom = plane_norm.dot(getDirection());

			if(denom == (Type)0.0) return false;

			//plane equation: A(x-x0) + B(y-y0) + C(z-z0) = 0
			//general equation: Ax + By + Cz + D = 0
			//A = norm[0];
			//B = norm[1];
			//C = norm[2];
			//x0 = center_[0];
			//z0 = center_[1];
			//y0 = center_[2];			
		}

        //! The shortest line between two lines in 3D
        bool intersect(const Ray<Type> & a_ray, Type & mua, Type & mub) const
        {
            // Based on code from Paul Bourke
            // http://astronomy.swin.edu.au/~pbourke
            Vec3<Type> p1 = m_origin;
            Vec3<Type> p2 = m_origin + 1.0f * m_direction;
            Vec3<Type> p3 = a_ray.m_origin;
            Vec3<Type> p4 = a_ray.m_origin + 1.0f * a_ray.m_direction;

            Vec3<Type> p13 = p1 - p3;
            Vec3<Type> p43 = p4 - p3;

            if(std::abs(p43[0]) < EPS && std::abs(p43[1]) < EPS && std::abs(p43[2]) < EPS) return false;

            Vec3<Type> p21 = p2 - p1;

            if(std::abs(p21[0]) < EPS && std::abs(p21[1]) < EPS && std::abs(p21[2]) < EPS) return false;

            Type d1343 = p13.dot(p43);
            Type d4321 = p43.dot(p21);
            Type d1321 = p13.dot(p21);
            Type d4343 = p43.dot(p43);
            Type d2121 = p21.dot(p21);

            Type denom = d2121 * d4343 - d4321 * d4321;

            if(std::abs(denom) < EPS) return false;

            Type numer = d1343 * d4321 - d1321 * d4343;

            mua = numer / denom;
            mub = (d1343 + d4321 * mua) / d4343;

            return true;
        }

        //! Check the two given ray for equality.
        friend bool operator ==(const Ray<Type> & r1, const Ray<Type> & r2)
        { 
            return(r1.m_origin == r2.m_origin && r1.m_direction == r2.m_direction); 
        }

        //! Check the two given ray for inequality.
        friend bool operator !=(const Ray<Type> & r1, const Ray<Type> & r2)
        { 
            return !(r1 == r2); 
        }

    private:
        Vec3<Type> m_origin;	//!< Ray origin
        Vec3<Type> m_direction; //!< Normalized direction
    };


    typedef Ray<int>    Rayi;
    typedef Ray<float>  Rayf;
    typedef Ray<double> Rayd;
} // namespace gtl

#endif
