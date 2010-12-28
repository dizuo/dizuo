
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:

	double x;
	double y;
	double z;

	Vector	();
	Vector	(double xv, double yv, double zv);
	Vector  (const Vector & v);

	void		SetZero();
	void		Set(double xv, double yv, double zv);
	void		Normalise();

	Vector	Normalised() const;

	Vector Orthogonal2DA();
	Vector Orthogonal2DB();

	double		Magnitude()	const;
	double		MagnitudeSquared() const;

	Vector	operator -	() const;
	Vector	operator -  (const Vector &v) const;
	Vector	operator +  (const Vector &v) const;
	void		operator += (const Vector &v);
	void		operator -= (const Vector &v);
	void		operator *= (double s);

	bool		operator == (const Vector &other) const;
	bool		operator != (const Vector &other) const;

	double &		operator[](int index);
	const double &	operator[](int index) const;

};

inline Vector::Vector() : x(0.0f), y(0.0f), z(0.0f)
{
}

inline Vector::Vector(double xv, double yv, double zv) : x(xv), y(yv), z(zv)
{
}

inline Vector::Vector(const Vector & v) : x(v.x), y(v.y), z(v.z)
{
}

inline bool	Vector::operator==(const Vector &other) const
{
	return (x == other.x) && (y == other.y) && (z == other.z);
}

inline bool	Vector::operator!=(const Vector &other) const
{
	return (x != other.x) || (y != other.y) || (z != other.z);
}

inline double&	Vector::operator[](int index)
{
	return reinterpret_cast<double*>(this)[index];
}

inline const double&	Vector::operator[](int index) const
{
	return reinterpret_cast<const double*>(this)[index];
}

double		operator * (const Vector &a, const Vector &b);
Vector	operator * (const Vector &v, double s);
Vector	operator * (double s, const Vector &v);

Vector VectorScale(const Vector &a, const Vector &b);

double VectorDot(const Vector &a, const Vector &b);

Vector VectorCross(const Vector &a, const Vector &b);

Vector VectorRotate(Vector p, const Vector &p1, const Vector &p2, double theta);

Vector Interpolate(double t, const Vector &a, const Vector &b);

double mlDistanceBetween(const Vector & a, const Vector & b);

extern const Vector VectorX;
extern const Vector VectorY;
extern const Vector VectorZ;

extern const Vector VectorZero;

#endif // VECTOR_H
