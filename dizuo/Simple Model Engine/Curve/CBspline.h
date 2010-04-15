#ifndef CBSPLINE_H_
#define CBSPLINE_H_

#include "CCurve.h"
#include "BaseSpline.h"

class CBspline : public CCurve
{
public:
	CBspline();
	CBspline(const RVec3Array& points );
	~CBspline();
	
	bool Setup();
	void AddKnot(const RVec3& point);

protected:

	spline::Spline* splinePtr;
	RVec3Array		ctrlVertices;
};

#endif