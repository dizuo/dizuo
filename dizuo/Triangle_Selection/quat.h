//
// Quaternion Class
//
#ifndef CQuat_h
#define CQuat_h

const float TO_HALF_RAD = 3.14159265f / 360.0f;

class CQuat
{
public:
	float x,y,z,w;

	CQuat( ) 
	{
		Reset();
	}

	// The last parameter is to differentiate this constructor from the axis angle one
	CQuat( float fx, float fy, float fz, float fw, int bSetXYZW ) 
	{
		if (bSetXYZW) {
			x = fx;
			y = fy;
			z = fz;
			w = fw;
		}
	}

	CQuat( float fAngle, float fX, float fY, float fZ )
	{
		SetAxis( fAngle, fX, fY, fZ);
	}

	// No rotation
	void Reset( )
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}

	void SetAxis( float angle, float fX, float fY, float fZ )
	{
		float fHAngle = angle * TO_HALF_RAD; // Get half angle in radians from angle in degrees
		float sinA = (float) sin( fHAngle ) ;
		w = (float) cos( fHAngle );
		x = fX * sinA;
		y = fY * sinA;
		z = fZ * sinA;
	}

	CQuat Invert( )
	{
		return CQuat( -x, -y, -z, w, 1);
	}

	int IsIdentity( )
	{
		if (x == 0.0f && y == 0.0f && z == 0.0f && w==1.0f)	return true;
		return false;
	}

	int Normalize( )
	{
		float factor = x * x + y * y + z * z + w * w;

		if (factor == 0.0 ) return -1;
		if (factor != 1.0 )
			{
			float scale = ( 1.0f / sqrtf( factor ) );
			x *= scale;
			y *= scale;
			z *= scale;
			w *= scale;
			return 1;
			}
		return 0;
	}

	inline CQuat operator* (const CQuat &b) const
	{
		CQuat r;

		r.w = w*b.w - x*b.x  -  y*b.y  -  z*b.z;
		r.x = w*b.x + x*b.w  +  y*b.z  -  z*b.y;
		r.y = w*b.y + y*b.w  +  z*b.x  -  x*b.z;
		r.z = w*b.z + z*b.w  +  x*b.y  -  y*b.x;

		return r;
	}
	
	inline int operator== ( const CQuat &b ) const
	{
		if (x == b.x && y == b.y && z == b.z && w == b.w) return 1;
			else return 0;
	}

	void FromEuler( float rx, float ry, float rz )
	{
		CQuat qx(-rx, 1, 0, 0 );
		CQuat qy(-ry, 0, 1, 0 );
		CQuat qz(-rz, 0, 0, 1 );
		qz = qy * qz;
		*this = qx * qz;
	}

// ------------------------------------
// Creates a value for this Quaternion from spherical linear interpolation
// t is the interpolation value from 0 to 1
// ------------------------------------
	void Slerp(const CQuat a, const CQuat b, float t)
	{
	  float w1, w2;

	  float cosTheta = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	  float theta    = (float)acos(cosTheta);
	  float sinTheta = (float)sin(theta);

	  if( sinTheta > 0.0f )
	  {
		w1 = float( sin( (1.0f-t)*theta ) / sinTheta);
		w2 = float( sin( t*theta) / sinTheta);
	  } else  {
		// CQuat a == CQuat b
		w1 = 1.0f;
		w2 = 0.0f;
	  }

	  x = w1*a.x + w2*b.x;
	  y = w1*a.y + w2*b.y;
	  z = w1*a.z + w2*b.z;
	  w = w1*a.w + w2*b.w;
	}

	//
	// Set a 4x4 matrix with the rotation of this Quaternion
	//
	void inline ToMatrix( float mf[16] )
	{
		float xy = x * y, xz = x * z;
		float yy = y * y, yw = y * w;
		float zw = z * w, zz = z * z;

		mf[ 0] = 1.0f - 2.0f * ( yy + zz );
		mf[ 1] = 2.0f * ( xy - zw );  
		mf[ 2] = 2.0f * ( xz + yw );  
		mf[ 3] = 0.0f;  

		float xx = x * x, xw = x * w, yz = y * z;

		mf[ 4] = 2.0f * ( xy +  zw );  
		mf[ 5] = 1.0f - 2.0f * ( xx + zz );  
		mf[ 6] = 2.0f * ( yz - xw );  
		mf[ 7] = 0.0f;  

		mf[ 8] = 2.0f * ( xz - yw );  
		mf[ 9] = 2.0f * ( yz + xw );  
		mf[10] = 1.0f - 2.0f * ( xx + yy );  
		mf[11] = 0.0f;  

		mf[12] = 0.0f;  
		mf[13] = 0.0f;   
		mf[14] = 0.0f;   
		mf[15] = 1.0f;
	}
};

#endif

