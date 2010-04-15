#ifndef MODEL_H_
#define MODEL_H_

// the include file order: glut must be behind with gtl/vec3
#include "CVertex.h"
#include "../SmartPtr.h"

#include <gl/glut.h>

class CModel
{
public:
	CModel();

	~CModel();

	CModel(const CModel& model);

	virtual bool Setup();

	virtual void LoadFile();
	virtual void SaveFile();

	virtual void LoadFromObjFile();
	virtual void SaveAsObjFile();
	virtual void Render();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	void RotateVectorY( const Real srcX, const Real srcY, const Real srcZ,
						const Real angle,
						Real& destX, Real& destY, Real& destZ );

	int GetVertNum() const { return verNum; }
	int GetIndiceNum() const { return indiceNum; }

	const CVertex& GetVertex(int idx) const { return pVertex[idx]; }
	const int&	GetIndice(int idx) const { return pTriIndices[idx]; }

	CVertex&	GetVertex(int idx) { return pVertex[idx]; }

	//dangerous
	CVertex*	GetVertexPointer() { return pVertex; }
	int*		GetIndicePointer() { return pTriIndices; }

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	void		SetMatrial(Real color[3]){ m_cMaterial[0]=color[0]; m_cMaterial[1]=color[1]; m_cMaterial[2]=color[2]; }

	void		ActiveBBox() { m_bIsShowBBox = true; }
	void		DeActiveBBox() { m_bIsShowBBox = false; }
	
	void		EnableTexture() { m_bIsUseTexture = true;}
	void		DisableTexture() { m_bIsUseTexture = false;};

	void		SetupBBox();

	void		Destroy();
	void		Init();

	//
protected:
	int*			pTriIndices;
	CVertex*		pVertex;

	int				indiceNum;		//indices	
	int				verNum;			//vertices

	bool			m_bIsShowBBox;
	RBBox			boundBox;

	bool			m_bIsUseTexture;
	Real			m_cMaterial[3];

	//4/6/2010 RYF
	void			_RenderCapFace(const CVertex* pV, int nV, DIRECTION dir=CCW);

	void			_RenderBBox();
};

//////////////////////////////////////////////////////////////////////////
//只是改变位置
template<class ModelType>
class ProduceModel
{
public:
	typedef CSmartPtr<ModelType> ModelPtrType;

	static ModelPtrType transformModel(const RMat4& transform, const ModelPtrType& pSrcModel)
	{
		ModelPtrType pRetModel = new ModelType( *pSrcModel );

		for (int i(0); i<pRetModel->GetVertNum(); i++)
		{
			RVec3 tmpV( pRetModel->GetVertex(i).v );
			transform.multVecMatrix(tmpV, tmpV);

			pRetModel->GetVertex(i).v[0] = tmpV[0];
			pRetModel->GetVertex(i).v[1] = tmpV[1];
			pRetModel->GetVertex(i).v[2] = tmpV[2];
		}

		return pRetModel;
	}
};

// RotateVector可以通过gtl中矩阵替换
#endif