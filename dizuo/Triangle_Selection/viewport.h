#ifndef CView_h
#define CView_h

class CRenderer;
class CMesh;

class CView {
public:
	CView();
	CMatrix GetMatrix();
	CVec3 GetCamPos();
	void RotateByDeg( float fDegX, float fDegY );
	void DragView( CVec3 vAdd );

private:
	float m_RotateX;
	float m_RotateY;
	CVec3 m_Translate;
};

class CSelection {
public:
	void ButtonDown( int nMouseX, int nMouseY, CMesh *pMesh );
	void MouseMove( int nMouseX, int nMouseY );
	void ButtonUp( CMesh *pMesh );
	void GetFrustum( CVec3 Normals[4], CVec3 Points[8] );
	void GetLine( CVec3 &P1, CVec3 &P2, int nMouseX, int nMouseY );
	void SetWindowHeight( int nHeight );
	void Render( CRenderer *pRender );
	
private:
	int nStartX, nStartY, nEndX, nEndY;
	bool bButton;
	int m_nWindowHeight;
	CVec3 m_vLineP[2];
};

#endif