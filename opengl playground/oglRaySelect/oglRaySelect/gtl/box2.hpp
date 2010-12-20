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
#ifndef BOX2_H
#define BOX2_H

#include <gtl/gtl.hpp>
#include <gtl/vec2.hpp>

namespace gtl
{
    /*!
    \class Box2 Box2.hpp geometry/Box2.hpp
    \brief Axis-Aligned 2D Bounding Box Class..
    \ingroup base

    This box class is used by many other classes.

    \sa Box3
    */
    template<typename Type>
    class Box2
    {
    public:
        //! The default constructor makes an empty box.
        Box2()
        {
            makeEmpty();
        }

        //!	Constructs a box with the given corners.
        Box2(const Vec2<Type> & a_min, const Vec2<Type> & a_max)
        {
            setBounds(a_min, a_max);
        }

        //! Default destructor does nothing.
        virtual ~Box2(){}

        //! Reset the boundaries of the box with the given corners.
        void setBounds(const Vec2<Type> & a_min, const Vec2<Type> & a_max)
        {
            m_min = a_min;
            m_max = a_max;
        }

        //! Check if this has been marked as an empty box. \sa makeEmpty().
        bool isEmpty() const
        {
            return (m_max[0] < m_min[0] || m_max[1] < m_min[1]);
        }

        //! Marks this as an empty box.	\sa isEmpty().
        void makeEmpty()
        {
            m_min =  Vec2<Type>::max();
            m_max = -Vec2<Type>::max();
        }

        //! Returns the lower left corner of the box. \sa getCenter(), getMax().
        const Vec2<Type> & getMin() const
        { 
            return m_min; 
        }

        //! Returns the upper right corner of the box. \sa getMin().
        const Vec2<Type> & getMax() const
        { 
            return m_max; 
        }

        //! Returns width and height of box.
        Vec2<Type> getSize() const
        {
            return m_max - m_min;
        }

        //! Returns the center point of the box.
        Vec2<Type> getCenter() const
        {
            return Vec2<Type>((m_max[0] + m_min[0]) * 0.5f,
                              (m_max[1] + m_min[1]) * 0.5f);
        }

        //! Extend the boundaries of the box by the given point.
        void extendBy(const Vec2<Type> & a_point)
        {
            if(isEmpty()){
                setBounds(a_point, a_point);
            }else{
                if(a_point[0] < m_min[0]) m_min[0] = a_point[0];
                if(a_point[1] < m_min[1]) m_min[1] = a_point[1];

                if(a_point[0] > m_max[0]) m_max[0] = a_point[0];
                if(a_point[1] > m_max[1]) m_max[1] = a_point[1];
            }
        }

        //! Extend the boundaries of the box by the given \a a_box parameter.
        void extendBy(const Box2<Type> & a_box)
        {
            if(isEmpty()){
                *this = a_box;
            }else{
                extendBy(a_box.getMin());
                extendBy(a_box.getMax());
            }
        }

        //! Give the surface of the box
        Type getSurface() const
        {
            Vec2<Type> size = m_max - m_min;
            return size[0] * size[1];
        }

        //! Check if \a a_point lies within the boundaries of this box.
        bool intersect(const Vec2<Type> & a_point) const
        {
            return !(a_point[0] < m_min[0] || a_point[0] > m_max[0] ||
                     a_point[1] < m_min[1] || a_point[1] > m_max[1]);
        }

        //! Check if the given box lies wholly or partly within the boundaries of this box.
        bool intersect(const Box2<Type> & a_box) const
        {
            if ((m_max[0] < a_box.m_min[0]) || (m_min[0] > a_box.m_max[0]) ||
                (m_max[1] < a_box.m_min[1]) || (m_min[1] > a_box.m_max[1])){
                    return false;
            }
            return true;
        }

        //! Check \a b1 and \a b2 for equality.
        friend bool operator ==(const Box2<Type> & b1, const Box2<Type> & b2)
        { 
            return b1.getMin() == b2.getMin() && b1.getMax() == b2.getMax(); 
        }

        //! Check \a b1 and \a b2 for inequality.
        friend bool operator !=(const Box2<Type> & b1, const Box2<Type> & b2)
        { 
            return !(b1 == b2); 
        }

    private:
        Vec2<Type> m_min;
        Vec2<Type> m_max;
    };

    typedef Box2<int>    Box2i;
    typedef Box2<float>  Box2f;
    typedef Box2<double> Box2d;
} // namespace gtl

#endif
