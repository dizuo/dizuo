//使用unix命名格式:
#ifndef FIGURE_H
#define FIGURE_H

#include <cmath>
#include "ScriptParser.hpp"
#include <gl/glut.h>

#define AreaR 3
#define OffsetY 4
#define OffsetX 1
#define M_PI 3.1415926
static float AreaRVec[] = 
{
	//3.0, 2.5, 2.0, 1.5, 1.0, 0.5
	5.0, 4.0, 3.0, 2.0, 1.0
};
class Figure : public ScriptParser<string, Joint>
{
	typedef Joint part;
	typedef string Type;
public:
	typedef void (*pFunc) (GLdouble);

	Figure(std::string);
	~Figure();

	//manipulator
	void resetJoints();
public:
	//void (*pDrawBone)(GLdouble);
	void buildFigure(part* some, unsigned int);
	pFunc		pDrawBone;
};

Figure::Figure(std::string name_)
	: ScriptParser(name_)
{}

Figure::~Figure()
{}

void Figure::resetJoints()
{
	for( Figure::iterator it = begin(); it != end(); ++it )
		it.node->deactive();
}
inline void Figure::buildFigure(part* some, unsigned level)
{
	if( NULL==some )
		return;

	for(size_t i(0); i<some->getSize(); i++)
		some->getChild(i)->offset[1] = some->offset[1] - OffsetY;

	if( some->getSize()>1 )
	{
		int num = some->getSize();
		for (int i=0; i<num; i++)
		{
 			some->getChild(i)->offset[0] = some->offset[0] + AreaRVec[level] * cosf( (2*M_PI*i)/num );
 			some->getChild(i)->offset[2] = some->offset[2] + AreaRVec[level] * sinf( (2*M_PI*i)/num );
		}
	}

	for(size_t i(0); i<some->getSize(); i++)
		buildFigure(some->getChild(i), level+1);
}
//////////////////////////////////////////////////////////////////////////
//该函数计算出的节点会有多个点重合现象

#endif 