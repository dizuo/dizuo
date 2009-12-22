//
// Contains viewport and selection classes
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "vec3.h"
#include "matrix.h"
#include "viewport.h"
#include "mesh.h"
#include "renderinterface.h"

//
// VIEWPORT
//
CView::CView() 
{
	m_RotateX = 90.0f;
	m_RotateY = 0.0f;
}

CMatrix CView::GetMatrix() 
{
	CMatrix ModelView;
	ModelView.Translate( CVec3(0, 0, -800.0f) );
	ModelView.Translate( m_Translate );	
	ModelView.Rotate( -m_RotateY, 1, 0, 0 );
	ModelView.Rotate( -m_RotateX, 0, 1, 0 );
	return ModelView;
}

CVec3 CView::GetCamPos()
{
	return GetMatrix().InvertSimple().GetTranslate();
}

void CView::RotateByDeg( float fDegX, float fDegY )
{
	m_RotateX += fDegX;
	m_RotateY += fDegY;
	if ( m_RotateX > 360 ) m_RotateX -=360;
	if ( m_RotateX < 0 )   m_RotateX +=360;
	if ( m_RotateY > 360 ) m_RotateY -=360;
	if ( m_RotateY < 0 )   m_RotateY +=360;
}

void CView::DragView( CVec3 vAdd )
{
	m_Translate += vAdd;
}

//
// Helper Functions
//
void inline swapInt( int &x, int &y )
{
 int temp = x; 
 x = y;
 y = temp;
}

//
// SELECTION
//
void CSelection::ButtonDown( int nMouseX, int nMouseY, CMesh *pMesh )
{
	nStartX = nMouseX;
	nStartY = nMouseY;
	nEndX = nMouseX;
	nEndY = nMouseY;
    bButton = true;	
             
    // Create a Line from the selection
    GetLine( m_vLineP[0], m_vLineP[1], nMouseX, nMouseY );
        
    // If control or alt isn't held, clear the selection
    if ( GetKeyState( VK_CONTROL) >= 0 && GetKeyState( VK_MENU ) >= 0 ) pMesh->ClearSelection();
    // Alt unselects triangles
    if ( GetKeyState( VK_MENU ) < 0 ) pMesh->SetSelectionMode( CMesh::SELECT_SUB );
    else pMesh->SetSelectionMode( CMesh::SELECT_ADD );
        
    // Select Triangle Clicked on
    pMesh->LineSelect( m_vLineP[0], m_vLineP[1] );
}
	
void CSelection::MouseMove( int nMouseX, int nMouseY )
	{
	if (bButton ) {
		if ( nMouseX < 32000 ) nEndX = nMouseX; else nEndX = 0;
		if ( nMouseY < 32000 ) nEndY = nMouseY; else nEndY = 0;
		// (reason for if : when the mouse is beyond the window in a negative direction we end up with a high number )		
		}
	}

// Area Select on button up if drag area is large enough
void CSelection::ButtonUp( CMesh *pMesh )
	{
	bButton = false;
	if ( abs( nEndX - nStartX ) < 4 && abs( nEndY - nStartY ) < 4 ) return;
	if ( nEndX < nStartX ) swapInt( nEndX, nStartX );
	if ( nEndY < nStartY ) swapInt( nEndY, nStartY );
	
	CVec3 P[8];
	CVec3 Normals[6];
	GetFrustum( Normals, P );
	pMesh->FrustumSelect( Normals, P );
	}

// Render the Selection Box
void CSelection::Render( CRenderer *pRender )
{
	if ( bButton ) {
		pRender->SetColor( 1.0f, 1.0f, 1.0f );
		pRender->DrawSelectionBox( nStartX, nStartY, nEndX, nEndY );
		}	
}
//
// This function will find 2 points in world space that are on the line into the screen defined by windowspace point (x,y)
//
void CSelection::GetLine( CVec3 &L1, CVec3 &L2, int x, int y )
{
   double mvmatrix[16];
   double projmatrix[16];
   int viewport[4];
   double dX, dY, dZ, dClickY; // glUnProject uses doubles, but I'm using floats for these 3D vectors
	
   glGetIntegerv(GL_VIEWPORT, viewport);	
   glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
   glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
   dClickY = double (m_nWindowHeight - y); 
   // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

   gluUnProject ((double) x, dClickY, 0.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);
   L1 = CVec3( (float) dX, (float) dY, (float) dZ );
   gluUnProject ((double) x, dClickY, 1.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);
   L2 = CVec3( (float) dX, (float) dY, (float) dZ );
}

void CSelection::SetWindowHeight( int nHeight ) 
{
	m_nWindowHeight = nHeight;
}
//
// Create a 3d frustum from 2d screen selection rectangle
//
void CSelection::GetFrustum( CVec3 Normals[4], CVec3 P[8] )
{      
  GetLine( P[0], P[1], nStartX, nStartY );
  GetLine( P[2], P[3], nStartX, nEndY );
  GetLine( P[4], P[5], nEndX, nEndY );
  GetLine( P[6], P[7], nEndX, nStartY );
  Normals[0] = (P[0]-P[1]).CrossProduct( P[2]-P[3] );
  Normals[1] = (P[2]-P[3]).CrossProduct( P[4]-P[5] );
  Normals[2] = (P[4]-P[5]).CrossProduct( P[6]-P[7] );
  Normals[3] = (P[6]-P[7]).CrossProduct( P[0]-P[1] );
  for (int i = 0; i < 4; i++) Normals[i].Normalize();
}