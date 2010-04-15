#ifndef EXTRUDE_H_
#define EXTRUDE_H_

#include "Model.h"
#include "..//Curve/CCurve.h"

class CExtrude : public CModel
{
public:
	CExtrude(int seg=1)
		: heightSegment(seg)
	{
		Init();
	};

	CExtrude(const RVec3Array& curve)
	{
		SetProfile(curve);
		Init();
	}
	~CExtrude() {};

	virtual bool Setup() 
	{
		int profSize = profile.GetSize();

		verNum = (profSize+1) * (heightSegment+1);   
		indiceNum = 2*profSize*heightSegment*3;  

		pVertex = new CVertex[verNum];   
		pTriIndices = new int[indiceNum]; 

		//calculate the first shape
		for(int i=0; i<profSize+1; i++)
		{
			int idx = i%profSize;
			RVec3 v = profile.GetSlice(idx);	//get the path's first slice
			RVec3 n = v.cross( profile.GetNormal() );	//compute the normal
			n.normalize();

			pVertex[i] = CVertex( profile.GetPnt(idx).x(), profile.GetPnt(idx).y(), profile.GetPnt(idx).z(), 
				(Real)(i)/(profSize), 0.0f,
				n.x(), n.y(), n.z() );
		}

		//calculate the other shapes
		for (int hgh=1; hgh<=heightSegment; hgh++)
		{
			Real heighFactor = (Real)(hgh)/(Real)(heightSegment);
			Real offset = heighFactor*height; 
			
			for (int i=0; i<=profSize; i++)
			{
				Real px = pVertex[i].v[0];
				Real py = pVertex[i].v[1] + offset;
				Real pz = pVertex[i].v[2];

				Real nx = pVertex[i].n[0];
				Real ny = pVertex[i].n[1];
				Real nz = pVertex[i].n[2];

				pVertex[hgh*(profSize+1)+i] = CVertex(px, py, pz,
					(Real)(i)/profSize, heighFactor,
					nx, ny, nz);
			}
		}

		//4/2/2010 RYF
		//setup bbox;
		SetupBBox();

		//########################################################################
		//calculate the indices
		for(int hgh=0; hgh<heightSegment; hgh++)
		{
			for(int i=0; i<profSize; i++)
			{
				// triangle 0
				pTriIndices[ ((hgh*profSize+i)*2)*3+0 ]	= hgh*(profSize+1)+i;
				pTriIndices[ ((hgh*profSize+i)*2)*3+1 ]	= (hgh+1)*(profSize+1)+i;
				pTriIndices[ ((hgh*profSize+i)*2)*3+2 ]	= hgh*(profSize+1)+i+1;

				// triangle 1
				pTriIndices[ ((hgh*profSize+i)*2+1)*3+0 ]= hgh*(profSize+1)+i+1;
				pTriIndices[ ((hgh*profSize+i)*2+1)*3+1 ]= (hgh+1)*(profSize+1)+i;
				pTriIndices[ ((hgh*profSize+i)*2+1)*3+2 ]= (hgh+1)*(profSize+1)+i+1;
			}
		}

		return true;
	}
	
	const RVec3& GetDir() const { return extrudeDir; }
	const CCurve& GetProfile() const { return profile; }
	const Real& GetHeight() const { return height; }
	
	void SetDir(const RVec3& dir) { extrudeDir = dir; }
	void SetProfile(const RVec3Array& curve) { profile.SetPnts(curve); }
	void SetHeight(const Real& h) { height = h;}
private:
	void Init()
	{
		extrudeDir.setValue(0.0f, 1.0f, 0.0f);
		height = 1.0f;
	}
	CCurve	profile;	
	Real	height;	
	int		heightSegment;
	RVec3	extrudeDir;
};

//////////////////////////////////////////////////////////////////////////
// 只支持y方向上offset
// 封闭曲线的挤出
// 挤出体没有caps
//////////////////////////////////////////////////////////////////////////
#endif