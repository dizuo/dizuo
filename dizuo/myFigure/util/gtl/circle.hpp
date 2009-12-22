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

#ifndef CIRCLE_H
#define CIRCLE_H

#include "gtl.hpp"
#include "vec2.hpp"
#include <assert.h>

namespace gtl
{
    /*!
    \class Circle circle.hpp gtl/circle.hpp
    \brief 2 dimensional circle.
    \ingroup base

    This class is used by many other classes.

    \sa   
    */
    template<typename Type>
    class Circle
    {
    public:
        //! The default constructor. Does nothing.
        Circle(){}

        //! Constructs an instance with initial values from \a a_center and \a a_radius.
        Circle(const Vec2<Type> & a_center, Type a_radius)
        {
            setValue(a_center, a_radius);
        }

        //! Constructs an instance with initial values from \a a_circle.
        Circle(const Circle<Type> & a_circle)
        {
            setValue(a_circle.m_center, a_circle.m_radius);
        }

        //! Construct a circle from 2 Points.
        Circle(const Vec2<Type> & p1, const Vec2<Type> & p2)
        {
            m_center = (p1 + p2) * 0.5f;
            m_radius = (p2 - p1).length();
        }

        //! Construct a circle from 3 Points. p1, p2, p3 should be co-planar.
        Circle(const Vec2<Type> & p1, const Vec2<Type> & p2, const Vec2<Type> & p3)
        {
            assert(setValue(p1,p2,p3)&&"Circle::Circle2(p1, p2, p3): points are colinear!");
        }

        //! Default destructor does nothing.
        virtual ~Circle(){}

        //! Change the center and radius
        void setValue(const Vec2<Type> & a_center, Type a_radius)
        {
            m_center = a_center;
            m_radius = a_radius;
        }

        //! Sets a circle from 3 Points. p1, p2, p3 should be co-planar.
        bool setValue(const Vec2<Type> & p1, const Vec2<Type> & p2, const Vec2<Type> & p3)
        {
            Type A2 = p1.sqrLength();
            Type B2 = p2.sqrLength();
            Type C2 = p3.sqrLength();

            // equation is (x - centre.x)^2 + (y - centre.y)^2 = radius^2
            Type det = 4* (p1.x()*p2.y() + p3.x()*p1.y() + p2.x()*p3.y() - p3.x()*p2.y() - p1.x()*p3.y() - p2.x()*p1.y());

            if(det != 0.0){
                m_center[0] = 2 * (A2*p2.y() + C2*p1.y() + B2*p3.y() - C2*p2.y() - A2*p3.y() - B2*p1.y() )/det;
                m_center[1] = 2 * (p1.x()*B2 + p3.x()*A2 + p2.x()*C2 - p3.x()*B2 - p1.x()*C2 - p2.x()*A2 )/det;

                m_radius = (p1 - m_center).length();
                return true;
            }
            m_center[0] = 0.0f;
            m_center[1] = 0.0f;
            m_radius = -1.0f;
            return false;
        }

        //! Set the center
        void setCenter(const Vec2<Type> & a_center)
        {
            m_center = a_center;
        }

        //! Set the radius
        void setRadius(Type a_radius)
        {
            m_radius = a_radius;
        }

        //! Return the center
        const Vec2<Type> & getCenter() const
        { 
            return m_center; 
        }

        //! Return the radius
        Type getRadius() const
        { 
            return m_radius; 
        }

        Type getSurface() const
        {
            return (Type)(M_PI * m_radius * m_radius);
        }

        Type getCircumference() const{
            return (Type)( 2.0 * M_PI * m_radius);
        }

        //! Check if \a a_point lies within the boundaries of this circle.
        bool intersect(const Vec2<Type> & a_point) const
        {
            return ((a_point - m_center).sqrLength() <= m_radius*m_radius);
        }

        //! Intersect with a circle, returning true if there is an intersection.
        bool intersect(const Circle<Type> & c, Vec2<Type> & p1, Vec2<Type> & p2) const
        {
            // Based on code from Paul Bourke
            // http://astronomy.swin.edu.au/~pbourke

            // dxy are the vertical and horizontal distances between the circle centers.
            Vec2<Type> dxy = c.m_center - m_center;

            // Determine the straight-line distance between the centers.
            Type d = dxy.length();

            // no solution. circles do not intersect.
            if (d > (m_radius + c.m_radius)) return false;

            // no solution. one circle is contained in the other
            if (d < std::abs(m_radius - c.m_radius)) return false;

            // 'point 2' is the point where the line through the circle
            // intersection points crosses the line between the circle
            // centers.  

            // Determine the distance from point 0 to point 2.
            Type a = ((m_radius*m_radius) - (c.m_radius*c.m_radius) + (d*d)) / (Type)(2.0 * d);

            // Determine the coordinates of point 2.
            Vec2<Type> xy2 = m_center + dxy * (a/d);

            // Determine the distance from point 2 to either of the intersection points.
            Type h = std::sqrt((m_radius*m_radius) - (a*a));

            // Now determine the offsets of the intersection points from point 2.
            Vec2<Type> rxy(-dxy[1] * (h/d), dxy[0] * (h/d));

            // Determine the absolute intersection points.
            p1 = xy2 + rxy;
            p2 = xy2 - rxy;

            return true;
        }

        friend std::ostream & operator<<(std::ostream & os, const Circle<Type> & c)
        { 
            return os << c.m_center.x() << " " << c.m_center.y() << " " << c.m_radius; 
        }

    private:
        Vec2<Type>  m_center;	//!< Circle center
        Type	    m_radius;	//!< Circle radius
    };

    typedef Circle<int>    Circlei; 
    typedef Circle<float>  Circlef; 
    typedef Circle<double> Circled;
} // namespace gtl

#endif
