#include "Model.h"

CModel::CModel()
	: pVertex(0)
	, pTriIndices(0)
	, indiceNum(0)
	, verNum(0)
	, m_bIsShowBBox(true)
	, m_bIsUseTexture(true)
{
	m_cMaterial[0] = m_cMaterial[1] = m_cMaterial[2] = 1.0f;
}

CModel::~CModel()
{
	Destroy();
}

CModel::CModel(const CModel& model)
	: pVertex(0)
	, pTriIndices(0)
	, indiceNum(0)
	, verNum(0)
	, m_bIsShowBBox(true)
	, m_bIsUseTexture(true)
{
	m_cMaterial[0] = m_cMaterial[1] = m_cMaterial[2] = 1.0f;

	verNum = model.GetVertNum();
	indiceNum = model.GetIndiceNum();

	pVertex = new CVertex[verNum];
	pTriIndices = new int[indiceNum];

	for (int i=0; i<verNum; i++)
	{
		pVertex[i] = model.GetVertex(i);
	}

	for (int i=0; i<indiceNum; i++)
	{
		pTriIndices[i] = model.GetIndice(i);
	}
}

//////////////////////////////////////////////////////////////////////////
//virtual functions...
//////////////////////////////////////////////////////////////////////////

//派生类中Setup必须调用CModel::Setup生成BBox
bool CModel::Setup() { return true; }

void CModel::LoadFile() {}

void CModel::SaveFile() {}

void CModel::LoadFromObjFile() {}

void CModel::SaveAsObjFile() {}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CModel::Render()
{
	glPushAttrib(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_CURRENT_BIT);
	if ( m_bIsUseTexture )
		glEnable(GL_TEXTURE_2D);
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	//Set vertex arrays for torus
	glVertexPointer(3, GL_FLOAT, sizeof(CVertex), &pVertex[0].v);
	glEnableClientState(GL_VERTEX_ARRAY);

	glNormalPointer(GL_FLOAT, sizeof(CVertex), &pVertex[0].n);
	glEnableClientState(GL_NORMAL_ARRAY);

	if (m_bIsUseTexture)
	{
		glTexCoordPointer(2, GL_FLOAT, sizeof(CVertex), &pVertex[0].t);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	}

	//Draw
	glDrawElements(GL_TRIANGLES, indiceNum, GL_UNSIGNED_INT, pTriIndices);

	//disable vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	if (m_bIsUseTexture)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//
	if (m_bIsShowBBox)
		_RenderBBox();

	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
}

void CModel::RotateVectorY( const Real srcX, const Real srcY, const Real srcZ,
						   const Real angle,
						   Real& destX, Real& destY, Real& destZ )
{
	const Real PI     = 3.14159265358979f;

	Real sinAngle= sinf(PI*angle/180);
	Real cosAngle= cosf(PI*angle/180);

	destX = srcX*cosAngle + srcZ*sinAngle;
	destY = srcY;
	destZ = -srcX*sinAngle + srcZ*cosAngle;
}

void CModel::Destroy()
{
	if (pVertex)
	{
		delete pVertex;
		pVertex = 0;
	}
	if (pTriIndices)
	{
		delete pTriIndices;
		pTriIndices = 0;
	}
}

void CModel::Init()
{
	pVertex = 0;
	pTriIndices = 0;
	indiceNum = 0;
	verNum = 0;
	m_bIsShowBBox = true;
	m_bIsUseTexture = true;

	m_cMaterial[0] = m_cMaterial[1] = m_cMaterial[2] = 1.0f;
}

//生成BBox
void CModel::SetupBBox()
{
	if (!pVertex)
		return;

	for (int i=0; i<verNum; i++)
	{
		RVec3 tmpVert( GetVertex(i).v );
		boundBox.extendBy( tmpVert );
	}
}

//显示BBox
void CModel::_RenderBBox()
{
	RVec3 vMin = boundBox.getMin();
	RVec3 vMax = boundBox.getMax();
	
	Real y3 = ( vMax.y() - vMin.y() ) / 3;
	Real x3 = ( vMax.x() - vMin.x() ) / 3;
	Real z3 = ( vMax.z() - vMin.z() ) / 3;

	glPushMatrix();
	glPushAttrib( GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_LINE_BIT);
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glLineWidth(2.0f);

	glBegin( GL_LINES ); //front

	glColor3f(1.0f, 1.0f, 1.0f);
	//min corner
	glVertex3f( vMin.x(), vMin.y(), vMin.z() );
	glVertex3f( vMin.x() + x3, vMin.y(), vMin.z() );

	glVertex3f( vMin.x(), vMin.y(), vMin.z() );
	glVertex3f( vMin.x(), vMin.y() + y3, vMin.z() );

	glVertex3f( vMin.x(), vMin.y(), vMin.z() );
	glVertex3f( vMin.x(), vMin.y(), vMin.z() + z3 );

	//vMax X,vMin all
	glVertex3f( vMax.x(), vMin.y(), vMin.z() );
	glVertex3f( vMax.x() - x3, vMin.y(), vMin.z() );

	glVertex3f( vMax.x(), vMin.y(), vMin.z() );
	glVertex3f( vMax.x(), vMin.y() + y3, vMin.z() );

	glVertex3f( vMax.x(), vMin.y(), vMin.z() );
	glVertex3f( vMax.x(), vMin.y(), vMin.z() + z3 );

	//vMax X,vMax Y vMin z
	glVertex3f( vMax.x(), vMax.y(), vMin.z() );
	glVertex3f( vMax.x() - x3, vMax.y(), vMin.z() );

	glVertex3f( vMax.x(), vMax.y(), vMin.z() );
	glVertex3f( vMax.x(), vMax.y() - y3, vMin.z() );

	glVertex3f( vMax.x(), vMax.y(), vMin.z() );
	glVertex3f( vMax.x(), vMax.y(), vMin.z() + z3 );

	//vMax X,vMax Y vMax z
	glVertex3f( vMax.x(), vMax.y(), vMax.z() );
	glVertex3f( vMax.x() - x3, vMax.y(), vMax.z() );

	glVertex3f( vMax.x(), vMax.y(), vMax.z() );
	glVertex3f( vMax.x(), vMax.y() - y3, vMax.z() );

	glVertex3f( vMax.x(), vMax.y(), vMax.z() );
	glVertex3f( vMax.x(), vMax.y(), vMax.z() - z3 );

	//vMin X vMax Y vMin Z
	glVertex3f( vMin.x(), vMax.y(), vMin.z() );
	glVertex3f( vMin.x() + x3, vMax.y(), vMin.z() );

	glVertex3f( vMin.x(), vMax.y(), vMin.z() );
	glVertex3f( vMin.x(), vMax.y() - y3, vMin.z() );

	glVertex3f( vMin.x(), vMax.y(), vMin.z() );
	glVertex3f( vMin.x(), vMax.y(), vMin.z() + z3 );

	//vMin X vMin Y vMax Z
	glVertex3f( vMin.x(), vMin.y(), vMax.z() );
	glVertex3f( vMin.x() + x3, vMin.y(), vMax.z() );

	glVertex3f( vMin.x(), vMin.y(), vMax.z() );
	glVertex3f( vMin.x(), vMin.y() + y3, vMax.z() );

	glVertex3f( vMin.x(), vMin.y(), vMax.z() );
	glVertex3f( vMin.x(), vMin.y(), vMax.z() - z3 );

	//vMax X vMin Y vMax Z
	glVertex3f( vMax.x(), vMin.y(), vMax.z() );
	glVertex3f( vMax.x() - x3, vMin.y(), vMax.z() );

	glVertex3f( vMax.x(), vMin.y(), vMax.z() );
	glVertex3f( vMax.x(), vMin.y() + y3, vMax.z() );

	glVertex3f( vMax.x(), vMin.y(), vMax.z() );
	glVertex3f( vMax.x(), vMin.y(), vMax.z() - z3 );

	//vMin X vMax Y vMax Z
	glVertex3f( vMin.x(), vMax.y(), vMax.z() );
	glVertex3f( vMin.x() + x3, vMax.y(), vMax.z() );

	glVertex3f( vMin.x(), vMax.y(), vMax.z() );
	glVertex3f( vMin.x(), vMax.y() - y3, vMax.z() );

	glVertex3f( vMin.x(), vMax.y(), vMax.z() );
	glVertex3f( vMin.x(), vMax.y(), vMax.z() - z3 );
	glEnd();
	glLineWidth(1.0f);

	glPopAttrib();
	glPopMatrix();
}

//pV指向顶点
//nV顶点的个数
//面得法向跟顶点的顺序有关：逆时针为正
void CModel::_RenderCapFace(const CVertex* pV, int nV, DIRECTION dir)
{
	if (nV < 3)
		return;

	glPushAttrib( GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_LINE_BIT);
	//glDisable( GL_LIGHTING );
	//glDisable( GL_TEXTURE_2D );

	//glColor3fv(faceColor);
	//求重心
	Real gravVert[3];
	gravVert[0] = 0; gravVert[1] = 0; gravVert[2] = 0;

	for (int i(0); i<nV; i++)
	{
		gravVert[0] += pV[i].v[0];
		gravVert[1] += pV[i].v[1];
		gravVert[2] += pV[i].v[2];
	}
	
	gravVert[0] /= static_cast<Real>(nV);
	gravVert[1] /= static_cast<Real>(nV);
	gravVert[2] /= static_cast<Real>(nV);

	//求面法线
	RVec3 vert0( pV[0].v );
	RVec3 vert1( pV[1].v );
	RVec3 vert2( pV[2].v );

	RVec3 edge1 = vert1 - vert0;
	RVec3 edge2 = vert2 - vert1;

	RVec3 faceNorm;

	if (dir==CCW)
		faceNorm = edge1.cross(edge2);
	else
		faceNorm = edge2.cross(edge1);

	faceNorm.normalize();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv( gravVert );
	glNormal3fv(faceNorm.getValue());

	if (dir==CCW)
		for (int i(0); i<nV; i++)
			glVertex3fv(pV[i].v);
	else
		for (int i(nV); i>=0; i--)
			glVertex3fv(pV[i].v);

	glEnd();

	//glEnable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);

	glPopAttrib();
}
