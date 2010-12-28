
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "Vector.h"

#include <math.h>

Vector Vector::Orthogonal2DA()
{
	return Vector(y, -x, 0);
}

Vector Vector::Orthogonal2DB()
{
	return Vector(-y, x, 0);
}

double		VectorCross2D(const Vector &a, const Vector &b)
{
	return a.x * b.y - a.y * b.x;
}

extern const Vector VectorX(1.0f, 0.0f, 0.0f);
extern const Vector VectorY(0.0f, 1.0f, 0.0f);
extern const Vector VectorZ(0.0f, 0.0f, 1.0f);

extern const Vector VectorZero(0.0f, 0.0f, 0.0f);

void		Vector::SetZero()					{ x = y = z = 0.0f; }
void		Vector::Set(double xv, double yv, double zv)	{ x = xv; y = yv; z = zv; }

double		Vector::Magnitude()	const				{ return sqrt(x * x + y * y + z * z); }
double		Vector::MagnitudeSquared()	const			{ return x * x + y * y + z * z; }

void		Vector::Normalise()
{
	double maxValue = 0.0f;

	if(fabs(x) > maxValue) maxValue = fabs(x);
	if(fabs(y) > maxValue) maxValue = fabs(y);
	if(fabs(z) > maxValue) maxValue = fabs(z);
	
	if(maxValue == 0.0f)
		return;
	
	x = x / maxValue;
	y = y / maxValue;
	z = z / maxValue;

	double mag = Magnitude();
	
	if(mag == 0.0f)
		return;

	double invMag = 1.0f / mag;
	x *= invMag;
	y *= invMag;
	z *= invMag;
}

Vector	Vector::Normalised() const
{
	Vector normalisedVector(x,y,z);
	
	double maxValue = 0.0f;
	
	if(fabs(normalisedVector.x) > maxValue) maxValue = fabs(normalisedVector.x);
	if(fabs(normalisedVector.y) > maxValue) maxValue = fabs(normalisedVector.y);
	if(fabs(normalisedVector.z) > maxValue) maxValue = fabs(normalisedVector.z);
	
	if(maxValue == 0.0f)
		return Vector(x,y,z);
		
	normalisedVector.x = normalisedVector.x / maxValue;
	normalisedVector.y = normalisedVector.y / maxValue;
	normalisedVector.z = normalisedVector.z / maxValue;
	
	double mag = normalisedVector.Magnitude();
	
	if(mag == 0.0f)
		return Vector(x,y,z);
	
	double invMag = 1.0f / mag;
	normalisedVector.x *= invMag;
	normalisedVector.y *= invMag;
	normalisedVector.z *= invMag;
	
	return normalisedVector;
}

Vector	Vector::operator +  (const Vector &v) const	{ return Vector(x + v.x, y + v.y, z + v.z); }
Vector	Vector::operator -  (const Vector &v) const	{ return Vector(x - v.x, y - v.y, z - v.z); }
void		Vector::operator += (const Vector &v)		{ x += v.x; y += v.y; z += v.z; }
void		Vector::operator -= (const Vector &v)		{ x -= v.x; y -= v.y; z -= v.z; }
void		Vector::operator *= (double s)			{ x *= s; y *= s; z *= s; }
Vector	Vector::operator -	() const			{ return Vector(-x, -y, -z);}

double		operator * (const Vector &a, const Vector &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

double VectorDot(const Vector &a, const Vector &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector	operator * (const Vector &v, double s)			{ return Vector(v.x * s, v.y * s, v.z * s); }
Vector	operator * (double s, const Vector &v)			{ return Vector(v.x * s, v.y * s, v.z * s); }

Vector	VectorCross(const Vector &a, const Vector &b)
{
	return Vector(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
}

Vector VectorScale(const Vector &a, const Vector &b)
{
	return Vector(
		a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

Vector VectorRotate(Vector p, const Vector &p1, const Vector &p2, double theta)
{
	Vector q,r;
	double costheta,sintheta;

	r.x = p2.x - p1.x;
	r.y = p2.y - p1.y;
	r.z = p2.z - p1.z;
	p.x -= p1.x;
	p.y -= p1.y;
	p.z -= p1.z;
	r.Normalise();

	costheta = cos(theta);
	sintheta = sin(theta);

	q.x += (costheta + (1 - costheta) * r.x * r.x) * p.x;
	q.x += ((1 - costheta) * r.x * r.y - r.z * sintheta) * p.y;
	q.x += ((1 - costheta) * r.x * r.z + r.y * sintheta) * p.z;

	q.y += ((1 - costheta) * r.x * r.y + r.z * sintheta) * p.x;
	q.y += (costheta + (1 - costheta) * r.y * r.y) * p.y;
	q.y += ((1 - costheta) * r.y * r.z - r.x * sintheta) * p.z;

	q.z += ((1 - costheta) * r.x * r.z - r.y * sintheta) * p.x;
	q.z += ((1 - costheta) * r.y * r.z + r.x * sintheta) * p.y;
	q.z += (costheta + (1 - costheta) * r.z * r.z) * p.z;

	q.x += p1.x;
	q.y += p1.y;
	q.z += p1.z;

	return q;
}

Vector Interpolate(double t, const Vector &a, const Vector &b)
{
	return a * (1.0f - t) + b * t;
}

double mlDistanceBetween(const Vector & a, const Vector & b)
{
	Vector difference = a - b;
	return difference.Magnitude();
}
