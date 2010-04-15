#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "Model.h"

class CCylinder : public CModel
{
private:
	CCylinder();

public:
	CCylinder(Real r, Real h, int cirP, int hghS, bool b=true, bool t=true)
		: topRadius(r)
		, botRadius(r)
		, height(h)
		, cirPrecision(cirP)
		, heighSegment(hghS)
		, isTopCap(t)
		, isBottomCap(b)
	{
		verNum = (cirPrecision+1)*(heighSegment+1);
		indiceNum = 2*cirPrecision*heighSegment*3;

		pVertex = new CVertex[verNum];
		pTriIndices = new int[indiceNum];
	}

	CCylinder(const CCylinder& model)
		: CModel(model)
		, topRadius(model.topRadius)
		, botRadius(model.botRadius)
		, height(model.height)
		, cirPrecision(model.cirPrecision)
		, heighSegment(model.heighSegment)
		, isTopCap(model.isTopCap)
		, isBottomCap(model.isBottomCap)
	{
	}

	~CCylinder(){}

	//////////////////////////////////////////////////////////////////////////
	// virtual functions:
	virtual bool Setup()
	{
		// normals
		Real ex = 0.0f;
		Real ey = 0.0f;
		Real ez = 0.0f;

		// positions
		Real px = 0.0f;
		Real py = 0.0f;
		Real pz = 0.0f;

		for (int i=0; i<heighSegment+1; i++)
		{
			Real heighFactor = (Real)(i)/(Real)(heighSegment);
			//0.0 <= delta <= 1.0
			//坐标原点为圆柱中心
			py = heighFactor*height - height/2.0f; 
			ey = 0.0f;

			Real radiusDelta = botRadius - topRadius;
			Real radius = topRadius + heighFactor * radiusDelta;

			for (int j=0; j<=cirPrecision; j++)
			{
				Real angle = (Real)(j)*TWOPI/cirPrecision;
				ex = cosf(angle);
				ez = sinf(angle);

				px = radius*ex;
				pz = radius*ez;

				//strip
				pVertex[ i*(cirPrecision+1)+j ] = 
					CVertex(px, py, pz, (Real)(j)/cirPrecision, heighFactor, ex, ey, ez);
			}
		}

		//4/2/2010 RYF
		//setup bbox;
		SetupBBox();
		//########################################################################
		//calculate the indices
		for(int hgh=0; hgh<heighSegment; hgh++)
		{
			for(int i=0; i<cirPrecision; i++)
			{
				// triangle 0
				pTriIndices[ ((hgh*cirPrecision+i)*2)*3+0 ]	= hgh*(cirPrecision+1)+i;
				pTriIndices[ ((hgh*cirPrecision+i)*2)*3+1 ]	= (hgh+1)*(cirPrecision+1)+i;
				pTriIndices[ ((hgh*cirPrecision+i)*2)*3+2 ]	= hgh*(cirPrecision+1)+i+1;

				// triangle 1
				pTriIndices[ ((hgh*cirPrecision+i)*2+1)*3+0 ]= hgh*(cirPrecision+1)+i+1;
				pTriIndices[ ((hgh*cirPrecision+i)*2+1)*3+1 ]= (hgh+1)*(cirPrecision+1)+i;
				pTriIndices[ ((hgh*cirPrecision+i)*2+1)*3+2 ]= (hgh+1)*(cirPrecision+1)+i+1;
			}
		}

		return true;
	}

	virtual void Render()
	{
		//top: [ 0 - cirPrecision ] 
		//bottom: [ heighSegment*(cicPrecision+1) ,
		//			heighSegment*(cicPrecision+1) + cirPrecision) ]
		Real faceColor[3] = {1.0f, 1.0f, 0.0f};

		if (isTopCap)
		{
			CVertex* pTop = pVertex;
			Real faceN[3] = {0.0f, 1.0f, 0.0f};

			_RenderCapFace(pTop, cirPrecision+1);
		}

		if (isBottomCap)
		{
			CVertex* pBottom = pVertex + heighSegment*(cirPrecision+1);
			Real faceN[3] = {0.0f, -1.0f, 0.0f};

			_RenderCapFace(pBottom, cirPrecision+1, CW);
		}
		//绘制侧面
		CModel::Render();
	}
protected:
	Real	topRadius;
	Real	botRadius;
	Real	height;

	bool	isTopCap;
	bool	isBottomCap;

	int		cirPrecision;
	int		heighSegment;
};

#endif