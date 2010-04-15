#include "CCone.h"

CCone::CCone(Real h, Real botR, int cirP/* =20 */, bool b/* =true */)
	: height(h)
	, cirPrecision(cirP)
	, botRadius(botR)
	, isBottomCap(b)
{
	verNum = (cirPrecision+1) + 1;
	indiceNum = (cirPrecision)*3;

	pVertex = new CVertex[verNum];
	pTriIndices = new int[indiceNum];
}

CCone::~CCone()
{}

bool CCone::Setup()
{
	pVertex[0] = CVertex(0.0f, height, 0.0f, 
					0.0f, 0.0f, 
					0.0f, 1.0f, 0.0f);

	for (int j=0; j<=cirPrecision; j++)
	{
		Real ex(0), ez(0);
		Real px(0), pz(0);

		Real angle = (Real)(j)*TWOPI/cirPrecision;
		ex = cosf(angle);
		ez = sinf(angle);

		px = botRadius*ex;
		pz = botRadius*ez;

		//strip
		pVertex[j+1] = 
			CVertex(px, 0.0f, pz, 
					0.0f, 1.0f, 
					ex, 0.0f, ez);
	}

	//setup bbox;
	SetupBBox();

	//########################################################################
	//calculate the indices
	for(int i=0; i<cirPrecision; i++)
	{
		pTriIndices[ 3*i + 0] = i+2;
		pTriIndices[ 3*i + 1] = i+1;
		pTriIndices[ 3*i + 2] = 0;
	}

	return true;
}

void CCone::Render()
{
	CVertex* pBotCap = pVertex + 1;
	int nV = cirPrecision+1;		

	_RenderCapFace(pBotCap, nV, CW);

	//»æÖÆ²àÃæ
	DisableTexture();

	CModel::Render();
}