//
// A Rendering class
// All rendering is done through this class. 
//
// There's a couple of reasons for this.
// 1. Resuse application specific rendering functions
// 2. To make it easier to replace OpenGL with another renderer, eg. Direct3D.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "vec3.h"
#include "matrix.h"
#include "renderinterface.h"

// Ctor 
CRenderer::CRenderer()
{
	m_hWnd = NULL;
	m_hDC = NULL;
	m_hGLRC = NULL;
	m_bInitialized = false;
}

int CRenderer::IsInitialized( )
{
	return m_bInitialized;
}

//
// Initialization
//
int CRenderer::InitRenderWindow( HWND hWnd, int bStencil, int bDoubleBuffer )
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	m_hWnd = hWnd;
	m_hDC = GetDC( m_hWnd );

	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof(  PIXELFORMATDESCRIPTOR ) );
	pfd.nSize = sizeof(  PIXELFORMATDESCRIPTOR );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	if ( bDoubleBuffer ) pfd.dwFlags |= PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	if ( bStencil ) pfd.cStencilBits = 8; 
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( m_hDC, &pfd );
    if ( !SetPixelFormat( m_hDC, format, &pfd ) )
		{
		return false;
		}

	// create the context
	m_hGLRC = wglCreateContext( m_hDC );
	
	if ( wglMakeCurrent( m_hDC, m_hGLRC ) ) 
	{
		m_bInitialized = true;
		SetInitialState();
	}
	
	return true;
}

//
//
//
void CRenderer::Shutdown( )
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( m_hGLRC );
	ReleaseDC( m_hWnd, m_hDC );
}

//
// 
//
void CRenderer::ClearBuffer( )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//
// 
//
void CRenderer::BufferSwap( )
{
	glFlush();
	SwapBuffers( m_hDC );
}

//
// Setting States
//
void CRenderer::SetInitialState( )
{
	glEnable( GL_CULL_FACE );
	//glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
}

void CRenderer::SetSize( int nWidth, int nHeight )
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	glViewport( 0, 0, nWidth, nHeight );
	SetProjection3D( );
}

void CRenderer::SetProjection3D( )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();	
	gluPerspective( 60, (GLdouble)m_nWidth/(GLdouble)m_nHeight, 5.0f, 30000.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();	
}

void CRenderer::SetProjection2D( )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();	
	gluOrtho2D( 0.0, (GLdouble)m_nWidth, 0.0, (GLdouble)m_nHeight );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();	
}

void CRenderer::SetMatrix(const CMatrix &M)
{
	glLoadMatrixf( M.mf );
}

void CRenderer::SetColor( float r, float g, float b )
{
	glColor3f( r, g, b );
}

void CRenderer::SetLineMode( bool bActive )
{
	if ( bActive ) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	} else {
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
}

//
// Drawing Primitives
//
void CRenderer::DrawLine( const CVec3 &P1, const CVec3 &P2 )
{
	glBegin( GL_LINES );
	glVertex3f( P1.x, P1.y, P1.z );
	glVertex3f( P2.x, P2.y, P2.z );
	glEnd();
}

void CRenderer::DrawMesh( CVec3 *pVertices, unsigned int *pTris, int nbTris )
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, sizeof(CVec3), pVertices );
	glDrawElements( GL_TRIANGLES, nbTris * 3, GL_UNSIGNED_INT, pTris );
	glDisableClientState (GL_VERTEX_ARRAY);	
}

void CRenderer::DrawTri( const CVec3 &P1, const CVec3 &P2, const CVec3 &P3 )
{
	glBegin( GL_TRIANGLES );
	glVertex3f( P1.x, P1.y, P1.z );
	glVertex3f( P2.x, P2.y, P2.z );
	glVertex3f( P3.x, P3.y, P3.z );
	glEnd();
}

//
// Render an X at location Vert, used to draw attention to a vert
//
void CRenderer::HighlightVert( const CVec3 &Vert )
{
	glPushMatrix( );
	glColor3f( 1.0f, 1.0f, 1.0f );
	DrawLine( Vert -CVec3(-2,2,2), Vert + CVec3(-2,2,2) );
	DrawLine( Vert -CVec3(2,-2,2), Vert + CVec3(2,-2,2) );
	DrawLine( Vert -CVec3(2,2,-2), Vert + CVec3(2,2,-2) );	
	glPopMatrix();
}

void CRenderer::WireRect( float x1, float y1, float x2, float y2 )
   {
    DrawLine( CVec3(x1,y1,0), CVec3(x2,y1,0) );
    DrawLine( CVec3(x2,y1,0), CVec3(x2,y2,0) );
    DrawLine( CVec3(x1,y2,0), CVec3(x2,y2,0) );
    DrawLine( CVec3(x1,y1,0), CVec3(x1,y2,0) );
   }
// 
// Draw the rectangle used for selection
// 
void CRenderer::DrawSelectionBox( int X1, int Y1, int X2, int Y2 )
	{
	Y1 = m_nHeight-Y1;
	Y2 = m_nHeight-Y2;
	
	SetProjection2D();
	
	// Draw stippled rectangle with OpenGL
	glLineStipple( 2, 43690 );
	glEnable( GL_LINE_STIPPLE);
	WireRect( (float)X1, (float)Y1, (float)X2, (float)Y2 );
	glLineStipple( 1, 65535);
	glDisable( GL_LINE_STIPPLE);
	
	SetProjection3D();
	}