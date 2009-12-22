//
// Functions for mathematical intersection tests
//
// by Jonathan Kreuzer
// http://www.3dkingdoms.com/weekly
//
#include <math.h>
#include "vec3.h"

//
// CheckLineBox Helper Functions
//
bool inline GetIntersection( float fDst1, float fDst2, CVec3 P1, CVec3 P2, CVec3 &Hit) {
if ( (fDst1 * fDst2) >= 0.0f) return false;
if ( fDst1 == fDst2) return false;
Hit = P1 + (P2-P1) * ( -fDst1/(fDst2-fDst1) );
return true;
}

bool inline InBox( CVec3 Hit, CVec3 B1, CVec3 B2, const int Axis ) {
if ( Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
if ( Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
if ( Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
return false;
}

//  
//  returns true if line (L1, L2) intersects with axis-aligned box (B1, B2) 
//  The point of intersection is returned in HitP
//
bool CheckLineBox( CVec3 L1, CVec3 L2, CVec3 B1, CVec3 B2, CVec3 &HitP )
{
// Check for a quick exit if the line is completely to one side of the box
if (L2.x < B1.x && L1.x < B1.x) return false;
if (L2.x > B2.x && L1.x > B2.x) return false;
if (L2.y < B1.y && L1.y < B1.y) return false;
if (L2.y > B2.y && L1.y > B2.y) return false;
if (L2.z < B1.z && L1.z < B1.z) return false;
if (L2.z > B2.z && L1.z > B2.z) return false;

// Check if the line originates in the box
if (L1.x > B1.x && L1.x < B2.x && L1.y > B1.y && L1.y < B2.y && L1.z > B1.z && L1.z < B2.z ) {
	HitP = L1; 
	return true;
	}
	
// Check for a line intersection with each side of the box
if ( GetIntersection( L1.x-B1.x, L2.x-B1.x, L1, L2, HitP) && InBox( HitP, B1, B2, 1 ) ) return true;
if ( GetIntersection( L1.y-B1.y, L2.y-B1.y, L1, L2, HitP) && InBox( HitP, B1, B2, 2 ) ) return true;
if ( GetIntersection( L1.z-B1.z, L2.z-B1.z, L1, L2, HitP) && InBox( HitP, B1, B2, 3 ) ) return true;
if ( GetIntersection( L1.x-B2.x, L2.x-B2.x, L1, L2, HitP) && InBox( HitP, B1, B2, 1 ) ) return true;
if ( GetIntersection( L1.y-B2.y, L2.y-B2.y, L1, L2, HitP) && InBox( HitP, B1, B2, 2 ) ) return true;
if ( GetIntersection( L1.z-B2.z, L2.z-B2.z, L1, L2, HitP) && InBox( HitP, B1, B2, 3 ) ) return true;

return false;
}

//
//  returns true if line (L1, L2) intersects with triangle( PV1, PV2, PV3 )
//  The point of intersection is returned in HitP
//
bool CheckLineTri( const CVec3 &L1, const CVec3 &L2, const CVec3 &PV1, const CVec3 &PV2, const CVec3 &PV3, CVec3 &HitP )
{
CVec3 VIntersect;

// Find Triangle Normal, would be quicker to have these computed already
CVec3 VNorm;
VNorm = ( PV2 - PV1 ).CrossProduct( PV3 - PV1 );
VNorm.Normalize();

// Find distance from L1 and L2 to the plane defined by the triangle
float fDst1 = (L1-PV1).Dot( VNorm );
float fDst2 = (L2-PV1).Dot( VNorm );

if ( (fDst1 * fDst2) >= 0.0f) return false;  // line doesn't cross the triangle.
if ( fDst1 == fDst2) {return false;} // line and plane are parallel

// Find point on the line that intersects with the plane
VIntersect = L1 + (L2-L1) * ( -fDst1/(fDst2-fDst1) );

// Find if the interesection point lies inside the triangle by testing it against all edges
CVec3 VTest;
VTest = VNorm.CrossProduct( PV2-PV1 );
if ( VTest.Dot( VIntersect-PV1 ) < 0.0f ) return false;
VTest = VNorm.CrossProduct( PV3-PV2 );
if ( VTest.Dot( VIntersect-PV2 ) < 0.0f ) return false;
VTest = VNorm.CrossProduct( PV1-PV3 );
if ( VTest.Dot( VIntersect-PV1 ) < 0.0f ) return false;

HitP = VIntersect;

return true;
}

//
// returns true if point P is in the 4-plane frustum
//
bool PointInFrustum( const CVec3 &P, CVec3 Normals[4], CVec3 Points[8] )
{
	for ( int x = 0; x < 4; x++ )
	{
		if ( Normals[x].Dot( P - Points[x*2] ) > 0 ) return false;
	}
    return true;
}

//
// LineInFrustum is much slower than it could be. I don't think it will be called very often, if it ever is I'll rewrite it.
// It just constructs a 4-plane frustum box out of triangles, and uses the Line-Triangle test.
//
bool LineInFrustum( const CVec3 &LP1, const CVec3 &LP2, CVec3 Points[8] )
{
	CVec3 HitP;
	int List[24] = { 0, 1, 2, 1, 2, 3, 0, 1, 6, 1, 6, 7, 6, 7, 5, 6, 5, 4, 2, 3, 5, 2, 5, 4 };
	
	for ( int x = 0; x < 8; x++ )
		if ( CheckLineTri( LP1, LP2, Points[ List[x*3] ], Points[ List[x*3+1] ], Points[ List[x*3+2] ], HitP ) )
			return true;
			
	return false;
}

//
// returns false if the triangle is not within the frustum
//
bool TriInFrustum( CVec3 vTri[3], CVec3 Normals[4], CVec3 Points[8] )
{
 int i;
 // If all 3 points are to one side any frustum plane return false
 for( int x = 0; x < 4; x++ )
	{
	for ( i = 0; i < 3; i++ )
		{
		if ( Normals[x].Dot( vTri[i] - Points[x*2] ) < 0 ) break;
		}
	if ( i >= 3 ) return false;
	}
  // If any point is in the frustum, return true
  for ( i = 0; i < 3; i++ )
	{
	if ( PointInFrustum( vTri[i], Normals, Points ) ) return true;
	}
  // If we didn't get quick result, do a slower but accurate test.
  // If any of the lines of the triangle are in the frustum, the triangle is in the frustum
  if ( LineInFrustum( vTri[0], vTri[1], Points ) ) return true;
  if ( LineInFrustum( vTri[1], vTri[2], Points ) ) return true;
  if ( LineInFrustum( vTri[2], vTri[0], Points ) ) return true;
  
  // If the frustum is completely inside the triangle, any frustum line into the screen will intersect the triangle
  CVec3 HitP;
  if ( CheckLineTri( Points[0], Points[1], vTri[0], vTri[1], vTri[2], HitP ) ) return true;
  
return false;
}