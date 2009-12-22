#ifndef BOX3_H
#define BOX3_H

#include "Vec3.hpp"

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

	Box3(const Box3<Type> & box)
	{
		setBounds( box.getMin(), box.getMax() );
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
		//从方面判断: 此时不相交，不相互包含
		if ((m_max[0] < a_box.m_min[0]) || (m_min[0] > a_box.m_max[0]) ||
			(m_max[1] < a_box.m_min[1]) || (m_min[1] > a_box.m_max[1]) ||
			(m_max[2] < a_box.m_min[2]) || (m_min[2] > a_box.m_max[2])){
				return false;
		}
		return true;
	}

	//! sometimes contain is also needed
	//  4/21/2009 RenYaFei]
	bool contain(const Box3<Type> & a_box) const
	{
		return ( a_box.m_min[0] > m_min[0] && a_box.m_min[1] > m_min[1] && a_box.m_min[2] > m_min[2] &&
				 a_box.m_max[0] < m_max[0] && a_box.m_max[1] < m_max[1] && a_box.m_max[2] < m_max[2] );
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

#endif