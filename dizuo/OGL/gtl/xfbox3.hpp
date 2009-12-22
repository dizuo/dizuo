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
#ifndef XFBOX3_H
#define XFBOX3_H

// #include <gtl/gtl.hpp>
// #include <gtl/vec3.hpp>
// #include <gtl/box3.hpp>
// #include <gtl/matrix4.hpp>
#include "gtl.hpp"
#include "vec3.hpp"
#include "box3.hpp"
#include "matrix4.hpp"

namespace gtl
{
    /*!
    \class XfBox3 XfBox3.hpp geometry/XfBox3.hpp
    \brief The XfBox3 class is an object oriented box.
    \ingroup base

    This box class is used by many other classes.

    \sa Box3
    */
    template<typename Type>
    class XfBox3 : public Box3<Type>
    {
    public:
        //! The default constructor makes an empty box.
        XfBox3() : Box3<Type>()
        {
            m_matrix.makeIdentity();
            m_invertedMatrix.makeIdentity();
        }

        //!	Constructs a box with the given corners.
        XfBox3(const Vec3<Type> & a_min, const Vec3<Type> & a_max) : Box3<Type>(a_min, a_max)
        {
            m_matrix.makeIdentity();
            m_invertedMatrix.makeIdentity();
        }

        XfBox3(const Box3<Type> & a_box) : Box3<Type>(a_box.m_min, a_box.m_max)
        {
            m_matrix.makeIdentity();
            m_invertedMatrix.makeIdentity();
        }

        void transform(const Matrix4<Type> & m)
        {
            setTransform(m_matrix * m);
        }

        void setTransform(const Matrix4<Type> & m)
        {
            m_matrix = m;
            m_invertedMatrix = m.inverse();
        }

        const Matrix4<Type> & getTransform() const
        {
            return m_matrix;
        }

        const Matrix4<Type> & getInverse() const
        {
            return m_invertedMatrix;
        }

        Vec3<Type> getCenter() const
        {
            Vec3<Type> transcenter;
            m_matrix.multVecMatrix(Box3<Type>::getCenter(),transcenter);
            return transcenter;
        }

        void extendBy(const Vec3<Type> &pt)
        {
            Vec3<Type> trans;
            m_invertedMatrix.multVecMatrix(pt, trans);
            Box3<Type>::extendBy(trans);
        }

        void extendBy(const Box3<Type> &bb) 
        {
            if (bb.isEmpty()) return;

            if (Box3<Type>::isEmpty()) {
                *this = bb;
                m_matrix.makeIdentity();
                m_invertedMatrix.makeIdentity();
                return;
            }

        }
        /*
        void extendBy(const XfBox3<Type> &bb)
        {

        }

        bool intersect(const Vec3<Type> &pt) const
        {
        Vec3<Type> p;
        m_invertedMatrix.multVecMatrix(pt, p);
        return Box3<Type>::intersect(p);
        }

        bool intersect(const Box3<Type> &bb) const
        {
        return false;
        }

        bool intersect (const XfBox3<Type> &bb) const
        {
        return false;	
        }
        */
        Box3<Type> project () const
        {
            Box3<Type> box =  (Box3<Type>)*this;

            if (!box.isEmpty()) box.transform(m_matrix);

            return box;
        }

        Type getVolume () const
        {
            if(Box3<Type>::isEmpty()) return 0.0;

            return std::abs( Box3<Type>::getVolume() * m_matrix.det3() );
        }

        //! Check \a b1 and \a b2 for equality.
        friend bool operator ==(const XfBox3<Type> & b1, const XfBox3<Type> & b2)
        { 
            return  (b1.getMin() == b2.getMin() && 
                     b1.getMax() == b2.getMax() &&
                     b1.m_matrix == b2.m_matrix); 
        }

        //! Check \a b1 and \a b2 for inequality.
        friend bool operator !=(const XfBox3<Type> & b1, const XfBox3<Type> & b2)
        { 
            return !(b1 == b2); 
        }
    private:
        Matrix4<Type> m_matrix;
        Matrix4<Type> m_invertedMatrix;
    };

    typedef XfBox3<int>    XfBox3i;
    typedef XfBox3<float>  XfBox3f;
    typedef XfBox3<double> XfBox3d;
} // namespace gtl

#endif
