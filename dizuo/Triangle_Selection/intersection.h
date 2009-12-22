
bool CheckLineBox( CVec3 L1, CVec3 L2, CVec3 B1, CVec3 B2, CVec3 &HitP );
bool CheckLineTri( const CVec3 &L1, const CVec3 &L2, const CVec3 &PV1, const CVec3 &PV2, const CVec3 &PV3, CVec3 &HitP );
bool PointInFrustum( const CVec3 &P, CVec3 Normals[4], CVec3 Points[8] );
bool TriInFrustum( CVec3 vTri[3], CVec3 Normals[4], CVec3 Points[8] );
bool LineInFrustum( const CVec3 &LP1, const CVec3 &LP2, CVec3 Normals[4], CVec3 Points[8] );