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

#ifndef PLANE_H
#define PLANE_H

#include "gtl.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "matrix4.hpp"


namespace gtl
{
    // forward declaration	
    template<typename Type> class Ray;

    /*!
    \class Plane Plane.hpp geometry/Plane.hpp
    \brief Represents an oriented plane in 3D.
    \ingroup base

    This box class is used by many other classes.

    \sa Box3
    */
    template<typename Type>
    class Plane
    {
    public:
        //! Default constructor. Does nothing.
        Plane(){}

        //! Construct a Plane from a normal and a distance from coordinate system origin.
        Plane(const Vec3<Type> & a_normal, Type a_distance)
        {
            m_normal = a_normal;
            m_normal.normalize();
            m_distance = a_distance;
        }

        //! Construct a Plane from a normal and a point laying in the plane. \a normal must not be null.
        Plane(const Vec3<Type> & a_normal, const Vec3<Type> & a_pt)
        {
            m_normal = a_normal;
            m_normal.normalize();
            m_distance = m_normal.dot(a_pt);	
        }

        //! Construct a Plane with three points laying in the plane.
        Plane(const Vec3<Type> & a_pt1, const Vec3<Type> & a_pt2, const Vec3<Type> & a_pt3)
        {
            m_normal = (a_pt2 - a_pt1).cross(a_pt3 - a_pt1);
            m_normal.normalize();
            m_distance = m_normal.dot(a_pt1);
        }

        //! Computing the Plane Equation of a Polygon.
        Plane(const std::vector< Vec3<Type> > & points)
        {
            if(points.empty()) return;

            // "Newell's Method for Computing the Plane Equation of a Polygon",
            // Graphics Gems III, Academic Press (1992), pp.231-232.
            m_normal.setValue(0,0,0);
            Vec3<Type> refpt(0,0,0);

            // Compute the polygon normal and a reference point on
            // the plane. Note that the actual reference point is
            // refpt / nverts
            for(unsigned int i=0; i<points.size(); i++){
                const Vec3<Type> & u = points[i];
                const Vec3<Type> & v = points[(i+1)%points.size()];

                m_normal[0] += (u[1] - v[1]) * (u[2] + v[2]);
                m_normal[1] += (u[2] - v[2]) * (u[0] + v[0]);
                m_normal[2] += (u[0] - v[0]) * (u[1] + v[1]);

                refpt += u;
            }

            // Compute the distance from origin to the plane equation
            m_distance = -refpt.dot(m_normal) / (m_normal.length() * points.size());
        }

        //! Default destructor does nothing.
        virtual ~Plane(){}

        //! Set the Plane normal. \sa getNormal().
        void setNormal(const Vec3<Type> & a_normal)
        { 
            m_normal = a_normal; 
        }

        //! Return the plane's normal vector.
        const Vec3<Type> & getNormal() const
        { 
            return m_normal; 
        }

        //! Set the distance from coordinate system origin to the plane..
        void setDistance(Type a_distance)
        { 
            m_distance = a_distance; 
        }

        //! Set the plane equation. ( ax + by + cz + w = 0 )
        void setValue(Type a, Type b, Type c, Type w)
        {
            m_normal.setValue(a,b,c);
            m_normal.normalize();

            m_distance = -w;
        }

        //! Return distance from coordinate system origin to the plane. \sa setDistance(Type a_distance).
        Type getDistanceFromOrigin() const
        { 
            return m_distance; 
        }

        //! Return the distance from \a a_point to plane. Positive distance means the point is in the plane's half space.
        Type getDistance(const Vec3<Type> & a_point) const
        {
            return a_point.dot(m_normal) - m_distance;
        }

        //! Check if the given point lies in the halfspace of the plane which the plane normal vector is pointing.
        bool isInHalfSpace(const Vec3<Type> & a_point) const
        {
            return (a_point.dot(m_normal) >= m_distance);
        }

        //! Transform the plane by the given matrix.
        void transform(const Matrix4<Type> & a_matrix)
        {
            // Find the point on the plane along the normal from the origin
            Vec3<Type> point = m_distance * m_normal;

            // Transform the plane normal by the matrix to get the new normal.
            // Use the inverse transpose of the matrix so that normals are not scaled incorrectly.
            Matrix4<Type> invTran = a_matrix.inverse().transpose();
            invTran.multDirMatrix(m_normal, m_normal);
            m_normal.normalize();

            // Transform the point by the matrix
            a_matrix.multVecMatrix(point, point);

            // The new distance is the projected distance of the vector to the
            // transformed point onto the (unit) transformed normal. This is
            // just a dot product.
            m_distance = point.dot(m_normal);
        }

        //! Check for equality with given tolerance.
        bool equals(const Plane<Type> & a_plane, const Type a_tolerance=EPS) const
        {
            return (m_normal.equal(a_plane.getNormal(),a_tolerance) && 
                (std::abs(m_distance-a_plane.getDistance()) <= a_tolerance) );
        }

        //! Return the plane equation. (ax + by + cz + w = 0)
        Vec4<Type> equation() const
        {
            return Vec4<Type>(m_normal[0],m_normal[1],m_normal[2], -m_distance);
        }

        //! Check the two given planes for equality.
        friend bool operator ==(const Plane<Type> & p1, const Plane<Type> & p2)
        { 
            return(p1.getDistanceFromOrigin() == p2.getDistanceFromOrigin()&& 
                p1.getNormal() == p2.getNormal()); 
        }

        //! Check the two given planes for inequality.
        friend bool operator !=(const Plane<Type> & p1, const Plane<Type> & p2)
        { 
            return !(p1 == p2); 
        }

        /*! Ray-plane intersection. 
            Return true if there is an intersection. 
            \a t is the distance from ray origin to plane.
         */
        bool intersect(const Ray<Type> & a_ray, Type & t) const
        {
            // Check if the ray is parallel to the plane.
            Type denom = m_normal.dot(a_ray.getDirection());

            if(denom == (Type)0.0) return false;

            t = (m_distance - m_normal.dot(a_ray.getOrigin())) / denom;

            return true;
        }

        /*! The intersection of two planes. 
            Return true if there is an intersection. 
            \a a_ray is the line of intersection.
        */
        bool intersect(const Plane<Type> & a_plane, Ray<Type> & a_ray) const
        {
            // Based on code from Graphics Gems III, Plane-to-Plane Intersection
            // by Priamos Georgiades
            const Vec3<Type> & pl1n = m_normal;
            const Vec3<Type> & pl2n = a_plane.m_normal;
            const Type pl1w = -m_distance;
            const Type pl2w = -a_plane.m_distance;

            Vec3<Type> xpt;
            Vec3<Type> xdir = pl1n.cross(pl2n);

            // holds the squares of the coordinates of xdir
            Vec3<Type> dir2 = xdir * xdir;

            if (dir2[2] > dir2[1] && dir2[2] > dir2[0] && dir2[2] > EPS) {
                // then get a point on the XY plane
                xpt.setValue(pl1n[1] * pl2w - pl2n[1] * pl1w,
                    pl2n[0] * pl1w - pl1n[0] * pl2w, 0.0f);
                xpt *= 1.0f / xdir[2];
            }
            else if (dir2[1] > dir2[0] && dir2[1] > EPS) {
                // then get a point on the XZ plane
                xpt.setValue(pl1n[2] * pl2w - pl2n[2] * pl1w, 0.0f,
                    pl2n[0] * pl1w - pl1n[0] * pl2w);
                xpt *= 1.0f / xdir[1];
            }
            else if (dir2[0] > EPS) {
                // then get a point on the YZ plane
                xpt.setValue(0.0f, pl1n[2] * pl2w - pl2n[2] * pl1w,
                    pl2n[1] * pl1w - pl1n[1] * pl2w);
                xpt *= 1.0f / xdir[0];
            }
            else // xdir is zero, then no point of intersection exists
                return false;

            xdir *= 1.0f / (Type)std::sqrt(dir2[0] + dir2[1] + dir2[2]);

            a_ray.setValue(xpt, xpt+xdir);

            return true;
        }

        /*! The intersection of three planes. 
            Return true if there is an intersection. 
            \a a_pt is the point of intersection.
        */
        bool intersect(const Plane<Type> & a_plane1, const Plane<Type> & a_plane2, Vec3<Type> & a_pt) const
        {
            Vec3<Type> N23 = a_plane1.m_normal.cross(a_plane2.m_normal);

            if(N23.sqrLength() < EPS) return false;

            Vec3<Type> N31 = a_plane2.m_normal.cross(m_normal);

            if(N31.sqrLength() < EPS) return false;

            Vec3<Type> N12 = m_normal.cross(a_plane1.m_normal);

            if(N12.sqrLength() < EPS) return false;

            Type d1 = m_distance;
            Type d2 = a_plane1.m_distance;
            Type d3 = a_plane2.m_distance;

            a_pt = (d1 * N23 + d2 * N31 + d3 * N12) / m_normal.dot(N23);

            return true;
        }

    private:
        Vec3<Type> m_normal;	//!< The normal to the plane
        Type m_distance;		//!< The distance from the origin
    };

    typedef Plane<int>    Planei;
    typedef Plane<float>  Planef;
    typedef Plane<double> Planed;
} // namespace gtl

#endif
