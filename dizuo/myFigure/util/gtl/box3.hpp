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

#ifndef BOX3_H
#define BOX3_H

#include "gtl.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "plane.hpp"
#include "matrix4.hpp"

namespace gtl
{
    // forward declaration	
    template<typename Type> class Plane;
    template<typename Type> class Ray;

    /*!
    \class Box3 Box3.hpp geometry/Box3.hpp
    \brief Axis-Aligned 3D Bounding Box Class..
    \ingroup base

    This box class is used by many other classes.

    \sa XfBox3
    */
    template<typename Type>
    class Box3
    {
    public:
        //! The default constructor makes an empty box.
        Box3()
        {
            makeEmpty();
        }

        //!	Constructs a box with the given corners.
        Box3(const Vec3<Type> & a_min, const Vec3<Type> & a_max)
        {
            setBounds(a_min, a_max);
        }

        //! Default destructor does nothing.
        virtual ~Box3(){}

        //! Reset the boundaries of the box with the given corners.
        void setBounds(const Vec3<Type> & a_min, const Vec3<Type> & a_max)
        {
            m_min = a_min;
            m_max = a_max;
        }

        //! Check if this has been marked as an empty box. \sa makeEmpty().
        bool isEmpty() const
        {
            return (m_max[0] < m_min[0] || 
                    m_max[1] < m_min[1] ||
                    m_max[2] < m_min[2]);
        }

        //! Marks this as an empty box.	\sa isEmpty().
        void makeEmpty()
        {
            m_min =  Vec3<Type>::max();
            m_max = -Vec3<Type>::max();
        }

        //! Returns the lower left corner of the box. \sa getCenter(), getMax().
        const Vec3<Type> & getMin() const
        { 
            return m_min; 
        }

        //! Returns the upper right corner of the box. \sa getMin().
        const Vec3<Type> & getMax() const
        { 
            return m_max; 
        }

        //! Returns width, height and depth of box.
        Vec3<Type> getSize() const
        {
            return m_max - m_min;
        }

        //! Returns the center point of the box.
        Vec3<Type> getCenter() const
        {
            return Vec3<Type>((m_max[0] + m_min[0]) * 0.5f,
                              (m_max[1] + m_min[1]) * 0.5f,
                              (m_max[2] + m_min[2]) * 0.5f);
        }

        //! Extend the boundaries of the box by the given point.
        void extendBy(const Vec3<Type> & a_point)
        {
            if(isEmpty()){
                setBounds(a_point, a_point);
            }else{
                if(a_point[0] < m_min[0]) m_min[0] = a_point[0];
                if(a_point[1] < m_min[1]) m_min[1] = a_point[1];
                if(a_point[2] < m_min[2]) m_min[2] = a_point[2];

                if(a_point[0] > m_max[0]) m_max[0] = a_point[0];
                if(a_point[1] > m_max[1]) m_max[1] = a_point[1];
                if(a_point[2] > m_max[2]) m_max[2] = a_point[2];
            }
        }

        //! Extend the boundaries of the box by the given \a a_box parameter.
        void extendBy(const Box3<Type> & a_box)
        {
            if(isEmpty()){
                *this = a_box;
            }else{
                extendBy(a_box.getMin());
                extendBy(a_box.getMax());
            }
        }

        //! Give the volume of the box (0 for an empty box)
        Type getVolume() const
        {
            if(isEmpty()) return 0.0;

            return (m_max[0] - m_min[0]) * (m_max[1] - m_min[1]) * (m_max[2] - m_min[2]);
        }

        //! Transforms Box3 by matrix, enlarging Box3 to contain result.
        void transform(const Matrix4<Type> & m)
        {
            // a transformed empty box is still empty
            if(isEmpty()) return;

            Vec3<Type> corners[8];
            corners[0]    = m_min;
            corners[1][0] = m_min[0]; corners[1][1] = m_max[1]; corners[1][2] = m_min[2];
            corners[2][0] = m_max[0]; corners[2][1] = m_max[1]; corners[2][2] = m_min[2];
            corners[3][0] = m_max[0]; corners[3][1] = m_min[1]; corners[3][2] = m_min[2];            
            corners[4]    = m_max;
            corners[5][0] = m_min[0]; corners[5][1] = m_max[1]; corners[5][2] = m_max[2];
            corners[6][0] = m_min[0]; corners[6][1] = m_min[1]; corners[6][2] = m_max[2];
            corners[7][0] = m_max[0]; corners[7][1] = m_min[1]; corners[7][2] = m_max[2];

            Box3<Type> newbox;
            for(int i = 0; i < 8; ++i){
                m.multVecMatrix(corners[i], corners[i]);
                newbox.extendBy(corners[i]);
            }

            setBounds(newbox.m_min, newbox.m_max);
        }

        //! Check if \a a_point lies within the boundaries of this box.
        bool intersect(const Vec3<Type> & a_point) const
        {
            return !(a_point[0] < m_min[0] || a_point[0] > m_max[0] ||
                     a_point[1] < m_min[1] || a_point[1] > m_max[1] ||
                     a_point[2] < m_min[2] || a_point[2] > m_max[2]);
        }

        //! Check if the given box lies wholly or partly within the boundaries of this box.
        bool intersect(const Box3<Type> & a_box) const
        {
            if ((m_max[0] < a_box.m_min[0]) || (m_min[0] > a_box.m_max[0]) ||
                (m_max[1] < a_box.m_min[1]) || (m_min[1] > a_box.m_max[1]) ||
                (m_max[2] < a_box.m_min[2]) || (m_min[2] > a_box.m_max[2])){
                    return false;
            }
            return true;
        }

        //! Check if the given ray intersect the box.
        bool intersect(const Ray<Type> & a_ray, Type & tmin, Type & tmax) const
        {
            // Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
            // "An Efficient and Robust Ray-Box Intersection Algorithm"
            //  Journal of graphics tools, 10(1):49-54, 2005
            if( isEmpty() ) return false;

            Vec3<Type> inv_direction(1/a_ray.getDirection()[0], 1/a_ray.getDirection()[1], 1/a_ray.getDirection()[2]);

            Vec3<int> sign( inv_direction[0] < 0, inv_direction[1] < 0, inv_direction[2] < 0);

            tmin  = ((sign[0] ? m_max : m_min).x() - a_ray.getOrigin().x()) * inv_direction.x();
            tmax  = (((1-sign[0]) ? m_max : m_min).x() - a_ray.getOrigin().x()) * inv_direction.x();

            Type tymin = ((sign[1] ? m_max : m_min).y() - a_ray.getOrigin().y()) * inv_direction.y();
            Type tymax = (((1-sign[1]) ? m_max : m_min).y() - a_ray.getOrigin().y()) * inv_direction.y();

            if( (tmin > tymax) || (tymin > tmax) ) return false;

            if(tymin > tmin) tmin = tymin;
            if(tymax < tmax) tmax = tymax;

            Type tzmin = ((sign[2] ? m_max : m_min).z() - a_ray.getOrigin().z()) * inv_direction.z();
            Type tzmax = (((1-sign[2]) ? m_max : m_min).z() - a_ray.getOrigin().z()) * inv_direction.z();

            if( (tmin > tzmax) || (tzmin > tmax) ) return false;

            if (tzmin > tmin) tmin = tzmin;
            if (tzmax < tmax) tmax = tzmax;

            return (tmin >= 0 && tmax >= 1);
        }

        //! Check if the given plane intersect the box.
        bool intersect(const Plane<Type> & a_plane) const
        {
            // Empty boxes can cause problems.
            if( isEmpty() ) return false;

            const Vec3<Type> & pnorm = a_plane.getNormal();

            // Use separating axis theorem to test overlap.
            Vec3<Type> min3( pnorm[0] > 0.0 ? m_min[0] : m_max[0],
                             pnorm[1] > 0.0 ? m_min[1] : m_max[1],
                             pnorm[2] > 0.0 ? m_min[2] : m_max[2]);

            Vec3<Type> max3( pnorm[0] > 0.0 ? m_max[0] : m_min[0],
                             pnorm[1] > 0.0 ? m_max[1] : m_min[1],
                             pnorm[2] > 0.0 ? m_max[2] : m_min[2]);

            if(a_plane.isInHalfSpace(vmin)) return false;
            if(a_plane.isInHalfSpace(vmax)) return true;

            return false;
        }

        //! Check \a b1 and \a b2 for equality.
        friend bool operator ==(const Box3<Type> & b1, const Box3<Type> & b2)
        { 
            return b1.getMin() == b2.getMin() && b1.getMax() == b2.getMax(); 
        }

        //! Check \a b1 and \a b2 for inequality.
        friend bool operator !=(const Box3<Type> & b1, const Box3<Type> & b2)
        { 
            return !(b1 == b2); 
        }

    private:
        Vec3<Type> m_min;
        Vec3<Type> m_max;
    };

    typedef Box3<int>    Box3i;
    typedef Box3<float>  Box3f;
    typedef Box3<double> Box3d;
} // namespace gtl

#endif
