#include "CBspline.h"
#include <functional>
#include <algorithm>

CBspline::CBspline()
{}

CBspline::CBspline(const RVec3Array& points )
	: splinePtr(0)
{
	ctrlVertices.clear();
	std::copy(points.begin(), points.end(), ctrlVertices.begin());
}

CBspline::~CBspline()
{
	SAFE_DELETE_OBJECT(splinePtr);
	ctrlVertices.clear();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//创建spline对象
//计算曲线的顶点，存储到基类CCurve::pathPnts
bool CBspline::Setup()
{
	splinePtr = new spline::Spline(ctrlVertices);
	splinePtr->Generate();

	pathPnts.clear();
	splinePtr->GetCurve(pathPnts);


	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//增加曲线段
//AddKnot中没有隐式调用Setup函数，因为Setup函数代价比较大
//所以再调用AddKnot增加多段以后，必须显示调用Setup函数，重建曲线
void CBspline::AddKnot(const RVec3& point)
{
	ctrlVertices.push_back(point);
}