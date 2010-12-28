
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef VECTORSET_H
#define VECTORSET_H

#include <vector>

#include "Vector.h"

class VectorSet
{
public:
	VectorSet();
	VectorSet(int nMaxSize);
	virtual ~VectorSet();

	//int GetMaxSize();
	int GetSize();
	Vector GetVector(int i);

	void SetVector(int i, const Vector & vVector);
	void AddVector(const Vector & vVector);

	void RemoveVector(int i);

	Vector & operator[](int index);

	int length();

	typedef std::vector<Vector> VectorVector;

	VectorVector m_pVectors;

//private:
//
//	Vector * m_pVectors;
//	int m_nMaxSize;
//	int m_nSize;
};

#endif // VECTORSET_H

