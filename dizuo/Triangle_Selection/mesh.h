//
// Simple Mesh Class
//
// 
// Note: Each triangle is defined by the indices of 3 vertices

struct TexCoord {
	float x, y;
};

class CMesh
{
public:
	CMesh();
	~CMesh();
	void Allocate( int nVerts, int nTris );
	void Load( char *sFilename );
	void CalculateFaceNormals( );
	void Render( class CRenderer *pRender );
	
	void ClearSelection();
	void SetSelectionMode( int nMode );
	void SelectTriangle( int nTri );
	void MarkBackfacing( CVec3 VCam );
	
	int LineSelect( const CVec3 &LP1, const CVec3 &LP2 );
	int FrustumSelect( CVec3 Normals[4], CVec3 Points[8] );

	enum SelectionModes{ SELECT_ADD, SELECT_SUB };
	enum TriFlags{ TF_SELECTED = 1, TF_BACKFACING = (1<<1) };	
private:
	
	// Data		
	int m_nbVerts, m_nbTris;
	CVec3 *m_pVerts;
	TexCoord *m_pUVs;
	CVec3 *m_pTriNorms;
	unsigned int *m_pTris;
	int *m_pTriFlags;
	int m_nSelMode;
};