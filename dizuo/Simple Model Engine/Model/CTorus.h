#ifndef TORUS_H_
#define TORUS_H_

#include "Model.h"

class CTorus : public CModel
{
private:
	CTorus();

public:
	CTorus(Real iR, Real oR, int p)
		: innerRadius(iR)
		, outerRadius(oR)
		, torusPrecision(p)
	{
		verNum = (torusPrecision+1)*(torusPrecision+1);   
		indiceNum = 2*torusPrecision*torusPrecision*3;   

		pVertex = new CVertex[verNum];   
		pTriIndices = new int[indiceNum];   
	}

	~CTorus(){}

	//////////////////////////////////////////////////////////////////////////
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

		//calculate the first ring - inner radius 4, outer radius 1.5
		for(int i=0; i<torusPrecision+1; i++)
		{
			const Real PI     = 3.14159265358979f;
			Real angle = i*360.0f/torusPrecision;
			Real sinAngle=(Real)sin( PI*angle/180);
			Real cosAngle=(Real)cos( PI*angle/180);
			
			px = outerRadius * cosAngle + innerRadius;
			py = outerRadius * sinAngle;
			pz = 0.0f;
			
			ex = cosAngle;
			ey = sinAngle;
			ez = 0.0f;

			pVertex[i] = CVertex( px, py, pz, 
								  0.0f, (Real)i/torusPrecision,
								  ex, ey, ez );
		}

		//rotate this to get other rings
		//********the following rings are both computed by ring0********\\
		//********rotate the ring0 in some angle********		
		for(int ring=1; ring<torusPrecision+1; ring++)
		{
			for(int i=0; i<torusPrecision+1; i++)
			{
				Real angle = ring*360.0f/torusPrecision;
				
				Real src[3];
				src[0] = pVertex[i].v[0];
				src[1] = pVertex[i].v[1];
				src[2] = pVertex[i].v[2];
				RotateVectorY(src[0], src[1], src[2], angle, px, py, pz);

				src[0] = pVertex[i].n[0];
				src[1] = pVertex[i].n[1];
				src[2] = pVertex[i].n[2];
				RotateVectorY(src[0], src[1], src[2], angle, ex, ey, ez);

				pVertex[ring*(torusPrecision+1)+i] = CVertex(px, py, pz,
															 2.0f*ring/torusPrecision, pVertex[i].t[1],
															 ex, ey, ez);
			}
		}

		//4/2/2010 RYF
		//setup bbox;
		SetupBBox();

		//########################################################################
		//calculate the indices
		for(int ring=0; ring<torusPrecision; ring++)
		{
			for(int i=0; i<torusPrecision; i++)
			{
				// triangle 0
				pTriIndices[ ((ring*torusPrecision+i)*2)*3+0 ]	= ring*(torusPrecision+1)+i;
				pTriIndices[ ((ring*torusPrecision+i)*2)*3+1 ]	= (ring+1)*(torusPrecision+1)+i;
				pTriIndices[ ((ring*torusPrecision+i)*2)*3+2 ]	= ring*(torusPrecision+1)+i+1;

				// triangle 1
				pTriIndices[ ((ring*torusPrecision+i)*2+1)*3+0 ]= ring*(torusPrecision+1)+i+1;
				pTriIndices[ ((ring*torusPrecision+i)*2+1)*3+1 ]= (ring+1)*(torusPrecision+1)+i;
				pTriIndices[ ((ring*torusPrecision+i)*2+1)*3+2 ]= (ring+1)*(torusPrecision+1)+i+1;
			}
		}

		return true;
	}

private:
	Real	innerRadius;
	Real	outerRadius;
	int		torusPrecision;
};

// the code can be simplified by using the gtl's classes and functions
#endif