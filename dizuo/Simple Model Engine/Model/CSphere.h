#ifndef SPHERE_H_
#define SPHERE_H_

#include "Model.h"

class CSphere : public CModel
{
private:
	CSphere();
public:
	CSphere(Real r, int p)
		: radius(r)
		, precision(p)
	{
		verNum = (p/2 + 1) * (p+1);
		indiceNum = p * p * 3;	// (p/2) * p * 2
		pVertex = new CVertex[ verNum ];
		pTriIndices = new int[ indiceNum ];
	}
	~CSphere(){}
	//////////////////////////////////////////////////////////////////////////
	// virtual functions:

	virtual bool Setup()
	{
		Real theta1 = 0.0;
		Real theta2 = 0.0;
		Real theta3 = 0.0;

		Real ex = 0.0f;
		Real ey = 0.0f;
		Real ez = 0.0f;

		Real px = 0.0f;
		Real py = 0.0f;
		Real pz = 0.0f;

		// Disallow a negative number for radius.
		if( radius < 0 )
			radius = -radius;

		// Disallow a negative number for precision.
		if( precision < 0 )
			precision = -precision;

		if( precision < 4 || radius <= 0 ) 
			return true;

		for( int i = 0; i <= precision/2; ++i )
		{
			theta1 = i * TWOPI / precision - PIDIV2;

			for( int j = 0; j <= precision; ++j )
			{
				theta3 = j * TWOPI / precision;

				ex = cosf(theta1) * cosf(theta3);
				ey = sinf(theta1);
				ez = cosf(theta1) * sinf(theta3);
				px = center.x() + radius * ex;
				py = center.y() + radius * ey;
				pz = center.z() + radius * ez;

				pVertex[ i*(precision+1) + j ] = CVertex( px, py, pz, 
														  -(j/(Real)precision), 2*(i)/(Real)precision,
														  ex, ey, ez );
			}
		}
		
		//4/2/2010 RYF
		//setup bbox;
		SetupBBox();

		//########################################################################
		// setup sphere's indices...
		for (int outer=0; outer<precision/2; outer++)
		{
			for (int inner=0; inner<precision; inner++)
			{
				int p = precision;
				//triangle 1
				pTriIndices[ ((outer*p+inner)*2)*3 + 0 ] = outer*(p+1)+inner;
				pTriIndices[ ((outer*p+inner)*2)*3 + 1 ] = (outer+1)*(p+1)+inner;
				pTriIndices[ ((outer*p+inner)*2)*3 + 2 ] = outer*(p+1)+inner+1;

				//triangle 2
				pTriIndices[ ((outer*p+inner)*2+1)*3 + 0 ] = outer*(p+1)+inner+1;
				pTriIndices[ ((outer*p+inner)*2+1)*3 + 1 ] = (outer+1)*(p+1)+inner;
				pTriIndices[ ((outer*p+inner)*2+1)*3 + 2 ] = (outer+1)*(p+1)+inner+1;
			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	const int GetPrecision() const { return precision; }
	const Real GetRadius()const { return radius; }
	const gtl::Vec3f	GetCenter() const { return center; }

	void SetCenter(const gtl::Vec3f& _center) { center = _center; }
	void SetRadius(const Real& r) { radius = r; }
	void SetPrecision(const Real& p){ precision = p; }
private:
	gtl::Vec3f	center;
	Real		radius;
	int			precision;
};

#endif