//
// Mesh Class
// 
// Right now just loads simple meshes with only verts and triangles
//
#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "vec3.h"
#include "matrix.h"
#include "mesh.h"
#include "renderInterface.h"
#include "intersection.h"

CMesh::CMesh()
{
	m_nbVerts = 0;
	m_nbTris = 0;
	m_pVerts = NULL;
	m_pUVs = NULL;
	m_pTris = NULL;
	m_pTriFlags = NULL;
	m_pTriNorms = NULL;
	m_nSelMode = SELECT_ADD;
}

CMesh::~CMesh()
{
	delete[] m_pVerts;
	delete[] m_pUVs;
	delete[] m_pTris;
	delete[] m_pTriFlags;
	delete[] m_pTriNorms;
}

void CMesh::Allocate( int nbVerts, int nbTris )
{
	m_pVerts = new CVec3[ nbVerts + 1 ];
	m_pUVs = new TexCoord[ nbVerts + 1 ];
	m_pTris = new unsigned int[ nbTris*3 + 1 ];
	m_pTriFlags = new int[ nbTris + 1 ];
	m_pTriNorms = new CVec3[ nbTris + 1 ];
}

void CMesh::Load( char *sFilename )
{
	FILE *pFile = fopen( sFilename, "rb" );
	if ( pFile ) 
		{
		fread( &m_nbVerts, 4, 1, pFile );
		fread( &m_nbTris , 4, 1, pFile );
		
		Allocate( m_nbVerts, m_nbTris );
		
		fread( m_pVerts, sizeof(CVec3), m_nbVerts, pFile );
		fread( m_pTris, 4 * 3, m_nbTris, pFile );
		memset( m_pTriFlags, 0, m_nbTris * sizeof(int) );
		CalculateFaceNormals();
		
		fclose( pFile );
		}
}

void CMesh::CalculateFaceNormals( )
{
	for ( int nTri = 0; nTri < m_nbTris; nTri++ )
		{
		int nV = nTri*3;
		CVec3 P0 = m_pVerts[ m_pTris[ nV ] ];
		CVec3 P1 = m_pVerts[ m_pTris[ nV+1 ] ];
		CVec3 P2 = m_pVerts[ m_pTris[ nV+2 ] ];		
		m_pTriNorms[ nTri ] = (P1-P0).CrossProduct(P2-P0);
		m_pTriNorms[ nTri ].Normalize();
		}
}

void CMesh::Render( CRenderer *pRender )
{
	// Highlight Selected Triangles
	pRender->SetLineMode( false );
	pRender->SetColor( .75f, .8f, 1.0f );
	for (int nTri = 0; nTri < m_nbTris; nTri++ )
		{
		if ( (m_pTriFlags[ nTri ] & TF_SELECTED) ) {
			pRender->DrawTri( m_pVerts[ m_pTris[nTri*3] ], m_pVerts[ m_pTris[nTri*3+1] ], m_pVerts[ m_pTris[nTri*3+2 ]] );
			}
		}
	
	// Draw the Mesh Wireframe
	pRender->SetColor( 0.0f, .4f, .8f );
	pRender->SetLineMode( true );
	pRender->DrawMesh( m_pVerts, m_pTris, m_nbTris );		
}

//
// MESH SELECTION
//
void CMesh::ClearSelection( )
{
	for (int nTri = 0; nTri < m_nbTris; nTri++ )
	{
		m_pTriFlags[ nTri ] &= ~TF_SELECTED;
	}
}

void CMesh::SetSelectionMode( int nMode )
{
	m_nSelMode = nMode; 
}

void CMesh::SelectTriangle( int nTri ) 
{
	if ( nTri < 0 || nTri >= m_nbTris ) return;
	if ( m_nSelMode == SELECT_ADD ) m_pTriFlags[ nTri ] |= TF_SELECTED;
	if ( m_nSelMode == SELECT_SUB ) m_pTriFlags[ nTri ] &= ~TF_SELECTED;
}

// Sets the TF_BACKFACING flag for triangles that are backfacing from vCam's point of view
void CMesh::MarkBackfacing( CVec3 vCam )
{
	for (int nTri = 0; nTri < m_nbTris; nTri++ )
	{
		if ( m_pTriNorms[ nTri ].Dot( vCam - m_pVerts[ m_pTris[ nTri*3 ] ] ) < 0 ) 
			m_pTriFlags[ nTri ] |= TF_BACKFACING;
		else
			m_pTriFlags[ nTri ] &= ~TF_BACKFACING;
	}
}

//
// Test a line against a mesh
// Selects the closest front-facing triangle
//
int CMesh::LineSelect( const CVec3 &LP1, const CVec3 &LP2 )
{
	CVec3 HitP;
	int nbHits = 0;
	int nSelTri = -1;
	float fDistance = 1000000000.0f;
	
	for (int nTri = 0; nTri < m_nbTris; nTri++ )
		{
		if ( m_pTriFlags[ nTri ] & TF_BACKFACING ) continue; // Check only front facing triangles
		
		int nV = nTri*3;	

		bool bHit = CheckLineTri( LP2, LP1, m_pVerts[ m_pTris[nV] ], m_pVerts[ m_pTris[nV+1] ], m_pVerts[ m_pTris[nV+2] ], HitP );
		if ( bHit ) {
			if ( HitP.Distance( LP1 ) < fDistance ) {
				fDistance = HitP.Distance( LP1 );
				nSelTri = nTri;
				}
			nbHits++;
			}
		}
		
	SelectTriangle( nSelTri );
	
	return nbHits;
}

//
// Select mesh triangles that are in a frustum defined by 8 Points( and 4 face normals )
// I'm currently ignoring near and far planes, but they could be easily added
//
int CMesh::FrustumSelect( CVec3 Normals[4], CVec3 Points[8] )
{
	int nbHits = 0;
	CVec3 Tri[3];
	
	for (int nTri = 0; nTri < m_nbTris; nTri++ )
		{	
		int nV = nTri*3;
		Tri[0] = m_pVerts[ m_pTris[ nV ] ];
		Tri[1] = m_pVerts[ m_pTris[ nV+1 ] ];
		Tri[2] = m_pVerts[ m_pTris[ nV+2 ] ];

		if ( TriInFrustum( Tri, Normals, Points ) )
			{
			SelectTriangle( nTri );			
			nbHits++;			
			}
		}
	
	return nbHits;
}