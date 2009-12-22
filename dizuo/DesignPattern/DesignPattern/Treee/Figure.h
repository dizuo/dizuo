//使用unix命名格式:
#ifndef FIGURE_H
#define FIGURE_H

#include <cmath>
#include "ScriptParser.hpp"
#include "XMLScriptParser.hpp"
#include "Joint.h"
#include <iostream>
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

using namespace std;
using namespace util;

//class Figure : public ScriptParser<Joint>
class Figure : public XMLScriptParser<Joint>
{
public:
	typedef Joint part;
	typedef void (*funcType)(part);
	typedef pre_order_iterator		preIter;
	typedef post_order_iterator		postIter;
	typedef sibling_iterator		levIter;

	Figure(std::string);
	~Figure();

	//manipulator
	void  resetJoints();
	void  creatFigure();
	void  renderBranch();

	void  preTraverse(funcType doSomething);
	void  postTraverse(funcType doSomething);
	void  levelTraverse(funcType doSomething);
protected:
	//void (*pDrawBone)(GLdouble);
	void buildFigure(sibling_iterator iRoot, unsigned int);
};

Figure::Figure(std::string name_)
	//: ScriptParser(name_)
	: XMLScriptParser(name_)
{}

Figure::~Figure()
{}

void Figure::creatFigure()
{
 	buildFigure(begin(), 0);
}

void Figure::renderBranch()
{
	//先序遍历绘制树枝
	for (preIter iRoot = begin(); iRoot != end(); ++iRoot)
	{
		sibling_iterator iChildren;
		
		glBegin(GL_LINES);
		for (iChildren = begin(iRoot); iChildren != end(iRoot); ++iChildren) 
		{
			glVertex3f( (*iRoot).offset[0], (*iRoot).offset[1], (*iRoot).offset[2] );
			glVertex3f( (*iChildren).offset[0], (*iChildren).offset[1], (*iChildren).offset[2] );
		}
		glEnd();
	}
	//绘制节点
}

void Figure::preTraverse(funcType doSomething)
{
	for (preIter bg = begin(); bg != end(); ++bg)
	{
		doSomething(*bg);
	}
}

void Figure::postTraverse(funcType doSomething)
{
	for (postIter bg = begin_post(); bg != end_post(); ++bg)
	{
		doSomething(*bg);
	}
}

void Figure::resetJoints()
{
	for( Figure::iterator it = begin(); it != end(); ++it )
		(*it).deactive();
}

inline void Figure::buildFigure(sibling_iterator iRoot, unsigned level)
{
	// 计算出孩子节点的offset[1]
	// iRoot指向iChild的父节点
	sibling_iterator iChildren;

	for (iChildren = begin(iRoot); iChildren != end(iRoot); ++iChildren) {
		(*iChildren).offset[1] = (*iRoot).offset[1] - OffsetY;
		//cout << (*iChildren).name << "\t";
	}
	// 计算出孩子节点的offset[0] offset[2]

	//计算孩子数目
	int num = number_of_children(iRoot);
	int siblingNum;
	if ( num > 1 ) 
	{
		for (iChildren = begin(iRoot), siblingNum = 0; iChildren != end(iRoot); ++iChildren, siblingNum++) {
			(*iChildren).offset[0] = (*iRoot).offset[0] + AreaRVec[level] * cosf( (2*M_PI*siblingNum) / num );
			(*iChildren).offset[2] = (*iRoot).offset[2] + AreaRVec[level] * sinf( (2*M_PI*siblingNum) / num );
		}
	}

	// 递归调用
	for (iChildren = begin(iRoot); iChildren != end(iRoot); ++iChildren) 
	{
		buildFigure(iChildren, level+1);
	}
}
//////////////////////////////////////////////////////////////////////////
//该函数计算出的节点会有多个点重合现象

//////////////////////////////////////////////////////////////////////////
//使用tree	不能再次使用递归，因为根节点head...
//			只能考虑使用迭代器遍历
//			强大的迭代器应该是可以完成功能的.

//4/30/2009	-	RYF
//	将Figure的脚本解析器改为XMLScriptParser

#endif 