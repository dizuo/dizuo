#ifndef LATHE_H_
#define LATHE_H_

#include "Model.h"
#include "..//Curve/CCurve.h"

class CLathe : public CModel
{
public:
	CLathe()
		:lathePrecision(20)
	{
		rotAxis.setValue(0.0f, 1.0f, 0.0f);
	};
	~CLathe() {};

	//////////////////////////////////////////////////////////////////////////
	const CCurve GetProfile() const { return profile; }
	void SetProfile(RVec3Array pA) { 
		profile.SetPnts(pA); 
	}

	//////////////////////////////////////////////////////////////////////////
	//profile的重心到原点的距离作为lathe的半径
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
		
		int profSize = profile.GetSize();
		verNum = (lathePrecision+1)*(profSize+1);   
		indiceNum = 2*lathePrecision*(profSize)*3;  

		pVertex = new CVertex[verNum];   
		pTriIndices = new int[indiceNum];   
		
		latheRadius = profile.GetGravCenter().x();

		//calculate the first shape
		for(int i=0; i<profSize+1; i++)
		{
			int idx = i%profSize;
			RVec3 v = profile.GetSlice(idx);	//get the path's first slice
			RVec3 n = v.cross( profile.GetNormal() );	//compute the normal
			n.normalize();

			pVertex[i] = CVertex( profile.GetPnt(idx).x(), profile.GetPnt(idx).y(), profile.GetPnt(idx).z(), 
								  0.0f, (Real)i/(profSize),
								  n.x(), n.y(), n.z() );
		}

		//rotate the first shape to get other rings
		//********the following rings are both computed by ring0********\\
		//********rotate the ring0 in some angle********		
		for(int ring=1; ring<lathePrecision+1; ring++)
		{
			for(int i=0; i<profSize+1; i++)
			{
				Real angle = ring*360.0f/lathePrecision;
				
				Real src[3];
				// position
				src[0] = pVertex[i].v[0];
				src[1] = pVertex[i].v[1];
				src[2] = pVertex[i].v[2];
				RotateVectorY(src[0], src[1], src[2], angle, px, py, pz);

				// normal
				src[0] = pVertex[i].n[0];
				src[1] = pVertex[i].n[1];
				src[2] = pVertex[i].n[2];
				RotateVectorY(src[0], src[1], src[2], angle, ex, ey, ez);

				pVertex[ring*(profSize+1)+i] = CVertex(px, py, pz,
															 2.0f*ring/lathePrecision, pVertex[i].t[1],
															 ex, ey, ez);
			}
		}
		
		//4/2/2010 RYF
		//setup bbox;
		SetupBBox();

		//########################################################################
		//calculate the indices
		for(int ring=0; ring<lathePrecision; ring++)
		{
			int strip = profSize;
			for(int i=0; i<strip; i++)
			{
				// triangle 0
				pTriIndices[ ((ring*strip+i)*2)*3+0 ]	= ring*(strip+1)+i;
				pTriIndices[ ((ring*strip+i)*2)*3+1 ]	= (ring+1)*(strip+1)+i;
				pTriIndices[ ((ring*strip+i)*2)*3+2 ]	= ring*(strip+1)+i+1;

				// triangle 1
				pTriIndices[ ((ring*strip+i)*2+1)*3+0 ]= ring*(strip+1)+i+1;
				pTriIndices[ ((ring*strip+i)*2+1)*3+1 ]= (ring+1)*(strip+1)+i;
				pTriIndices[ ((ring*strip+i)*2+1)*3+2 ]= (ring+1)*(strip+1)+i+1;
			}
		}

		return true;
	}
private:
	CCurve	profile;	
	RVec3	rotAxis;
	Real	latheRadius;
	int		lathePrecision;
};

//////////////////////////////////////////////////////////////////////////
// profile must be not closed
// profile是否封闭：顶点数，索引数，顶点数组处理的跨度均不相同

#endif