#ifndef CURVE_H_
#define CURVE_H_

#include "..//Define.h"

#include <algorithm>
#include <vector>

using namespace std;
using namespace gtl;

class CCurve
{
public:
	enum CURVE_TYPE
	{
		CURVE_2D,
		CURVE_3D
	};

	enum CURVE_STATE
	{
		CURVE_OPEN,
		CURVE_CLOSED
	};

	CCurve(){
	}

	CCurve(const RVec3Array& pnts)
	{
		SetPnts(pnts);
		ComputeNormal();
		ComputeGravCenter();
	}
	CCurve(const CCurve& curve)
	{
		SetPnts(curve.GetPnts());
		ComputeNormal();
		ComputeGravCenter();
	}
	~CCurve()
	{
		pathPnts.clear();
	};

	virtual void Render()
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_CURRENT_BIT);

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glLineWidth(3.0f);
		glPointSize(2.0f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		for (size_t i=0; i<pathPnts.size(); i++)
		{		
			glBegin(GL_LINES);
			int next = (i+1) % pathPnts.size();
			glVertex3fv( pathPnts[i].getValue() );
			glVertex3fv( pathPnts[next].getValue() );
			glEnd();
		}

		//以面形式绘制
		//glBegin(GL_TRIANGLE_FAN);
		//glVertex3fv( gravCenter.getValue() );
		//for (size_t i=0; i<=pathPnts.size(); i++)
		//	glVertex3fv( pathPnts[ i%pathPnts.size() ].getValue() );
		//glEnd();

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glLineWidth(1.0f);
		glPointSize(1.0f);
		glColor3f(1.0f, 1.0f, 1.0f);

		

		glPopAttrib();
	}
	//////////////////////////////////////////////////////////////////////////

	void ComputeNormal()
	{
		//	assert all points are in the one plane.
		//	assert curve has more than 2 points.
		RVec3 slice1 = GetSlice(0);
		RVec3 slice2 = GetSlice(1);
		pathNormal = slice1.cross(slice2);
		pathNormal.normalize();
		//gtl::Planef pathPlane(pathPnts);
		//pathNormal = pathPlane.getNormal();
	}

	void ComputeGravCenter()
	{
		//compute the shape's center of gravity
		for ( RVec3Array::iterator it = pathPnts.begin();
			it != pathPnts.end();
			it++ )
		{
			gravCenter += (*it);
		}
		gravCenter /= (Real)( pathPnts.size() );
	}

	//////////////////////////////////////////////////////////////////////////

	void SetPnts(const RVec3Array& path_)
	{
		if ( !Empty() )
		{
			pathPnts.clear();
		}
		pathPnts = path_;

		ComputeNormal();
		ComputeGravCenter();
	}

	void AddPnt(const RVec3& v)
	{
		pathPnts.push_back(v);
	}
	//////////////////////////////////////////////////////////////////////////

	const RVec3Array GetPnts() const { return pathPnts;}
	const RVec3	GetGravCenter() const { return gravCenter; }
	const RVec3	GetNormal() const {	return pathNormal; }

	// assert the index range
	const RVec3 GetPnt(int index) const { return pathPnts[index]; }
	RVec3& GetPnt(int index) { return pathPnts[index]; }

	// 越界判断
	RVec3& operator[](int index) { return pathPnts[index];}
	const RVec3& operator[](int index) const { return pathPnts[index];}

	const size_t GetSize() const { return pathPnts.size(); }
	bool	Empty() const { return pathPnts.size()==0; }

	//	assert the curve must be closed
	//	0 <= index <= path.size()-1
	const RVec3 GetSlice(int index) const 
	{
		int size = pathPnts.size();
		if (index < 0 || index >= size)
			return RVec3(0.0f, 0.0f, 0.0f);

		int next = (index+1) % size;
		return pathPnts[next] - pathPnts[index]; 
	}

	//////////////////////////////////////////////////////////////////////////

protected:
	RVec3Array	pathPnts;		//顶点数组
	RVec3		gravCenter;		//重心
	RVec3		pathNormal;		//法线
	bool		isClosed;		//

};

#endif