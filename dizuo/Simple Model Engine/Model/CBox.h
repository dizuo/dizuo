#ifndef BOX_H_
#define BOX_H_

#include "Model.h"

//in the origin position
class CBox : public CModel
{
public:
	CBox(const RVec3& size)
	{
		boxSize = size;
	}
	~CBox(){}

	bool Setup()
	{
		Real halfW = boxSize.y() / 2.0f;	//x
		Real halfH = boxSize.y() / 2.0f;	//y
		Real halfL = boxSize.y() / 2.0f;	//z

		
	}

private:
	RVec3 boxSize;
};

#endif