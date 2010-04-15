#ifndef VERTEX_H_
#define VERTEX_H_

#include "../Define.h"
class CVertex
{
public:
	CVertex()
	{
		Init();
	}
	CVertex(Real fv1, Real fv2, Real fv3, 
			Real fu, Real fv, 
			Real fn1, Real fn2, Real fn3)
	{
		v[0] = fv1;
		v[1] = fv2;
		v[2] = fv3;

		t[0] = fu;
		t[1] = fv;

		n[0] = fn1;
		n[1] = fn2;
		n[2] = fn3;
	}

	CVertex(Real fv1, Real fv2, Real fv3, 
		Real fn1, Real fn2, Real fn3)
	{
		Init();

		v[0] = fv1;
		v[1] = fv2;
		v[2] = fv3;

		n[0] = fn1;
		n[1] = fn2;
		n[2] = fn3;
	}

	Real v[3];
	Real t[2];
	Real n[3];

	void Init()
	{
		v[0] = v[1] = v[2] = 0;
		n[0] = n[1] = n[2] = 0;

		t[0] = t[1] = 0;
	}
};

#endif