#ifndef CRenderer_h
#define CRenderer_h

class CVec3;
class CMatrix;

class CRenderer
{
public:
	CRenderer();
	int InitRenderWindow( HWND hWnd, int bStencil, int bDoubleBuffer );
	void Shutdown( );
	void SetInitialState( );
	int IsInitialized();
	void BufferSwap( );
	void ClearBuffer( );
	
	void SetSize( int nWidth, int nHeight );
	void SetProjection3D( );
	void SetProjection2D( );
	
	void SetColor( float r, float g, float b );
	void SetLineMode( bool bActive );
	void SetMatrix( const CMatrix &M );

	static void DrawLine( const CVec3 &P1, const CVec3 &P2 );
	void DrawMesh( CVec3 *pVertices, unsigned int *pTris, int nTris );
	void DrawTri( const CVec3 &P1, const CVec3 &P2, const CVec3 &P3 );
	static void HighlightVert( const CVec3 &Vert );
	void WireRect( float x1, float y1, float x2, float y2 );
	void DrawSelectionBox( int X1, int Y1, int X2, int Y2 );
	
private:
	int m_bInitialized;
	HDC m_hDC;
	HGLRC m_hGLRC;
	HWND m_hWnd;
	int m_nWidth, m_nHeight;
};

#endif