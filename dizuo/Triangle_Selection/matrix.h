#ifndef CMatrix_h
#define CMatrix_h

const float PI = 3.14159265f;
const float TO_RAD = PI / 180.0f;

class CMatrix 
{
public:
// Data
	float mf[ 16 ];

// Functions
	CMatrix( const int bIdentity = true )
	{
	 if ( bIdentity ) Identity();
	}

	void Identity( )
	{
	mf[ 0] = 1.0f;    mf[ 1] = 0.0f;      mf[ 2] = 0.0f;    mf[ 3] = 0.0f;  
	mf[ 4] = 0.0f;    mf[ 5] = 1.0f;      mf[ 6] = 0.0f;    mf[ 7] = 0.0f;  
	mf[ 8] = 0.0f;    mf[ 9] = 0.0f;      mf[10] = 1.0f;    mf[11] = 0.0f;  
	mf[12] = 0.0f;    mf[13] = 0.0f;      mf[14] = 0.0f;    mf[15] = 1.0f;
	}

	// Concatenate 2 matrices with the * operator
	inline CMatrix operator* (const CMatrix &InM) const
	{
	CMatrix Result( 0 );
	for (int i=0;i<16;i+=4)
		{
		for (int j=0;j<4;j++)
			{
			Result.mf[i + j] = mf[ i + 0] * InM.mf[ 0 + j] + mf[ i + 1] * InM.mf[ 4 + j]
				+ mf[ i + 2] * InM.mf[ 8 + j] + mf[ i + 3] * InM.mf[ 12 + j];
			}
		}
	return Result;
	}

	// Use a matrix to transform a 3D point with the * operator
	inline CVec3 operator* (const CVec3 &Point ) const
	{
	 float x = Point.x*mf[0] + Point.y*mf[4] + Point.z*mf[8]  + mf[12];
	 float y = Point.x*mf[1] + Point.y*mf[5] + Point.z*mf[9]  + mf[13];
	 float z = Point.x*mf[2] + Point.y*mf[6] + Point.z*mf[10] + mf[14]; 
	 return CVec3( x, y, z );
	}

	// Rotate the *this matrix fDegrees counter-clockwise around a single axis( either x, y, or z )
	void Rotate( float fDegrees, int x, int y, int z )
	{
	 CMatrix Temp;
	 if (x == 1) Temp.RotX( -fDegrees );
	 if (y == 1) Temp.RotY( -fDegrees );
	 if (z == 1) Temp.RotZ( -fDegrees );
	 *this = Temp * (*this);
	}

	void Scale( float sx, float sy, float sz )
	{
	 int x;
	 for (x = 0; x <  4; x++) mf[x]*=sx;
	 for (x = 4; x <  8; x++) mf[x]*=sy;
	 for (x = 8; x < 12; x++) mf[x]*=sz;
	}

	void Translate( const CVec3 &Test )
	{
	for (int j=0;j<4;j++)
		{
		mf[12+j] += Test.x * mf[j] + Test.y * mf[4+j] + Test.z * mf[8+j]; 
		}	 
	}
	
	CVec3 GetTranslate( )
	{
		return CVec3( mf[12], mf[13], mf[14] );
	}
	
	// Zero out the translation part of the matrix
	CMatrix RotationOnly( )
	{
	 CMatrix Temp = *this;
	 Temp.mf[12] = 0;
	 Temp.mf[13] = 0;
	 Temp.mf[14] = 0;
	 return Temp;
	}

	// Create a rotation matrix for a counter-clockwise rotation of fDegrees around an arbitrary axis(x, y, z)
	void RotateMatrix( float fDegrees, float x, float y, float z)
	{
	Identity();
	float cosA = cosf(fDegrees*TO_RAD);
	float sinA = sinf(fDegrees*TO_RAD);
	float m = 1.0f - cosA;
	mf[0] = cosA + x*x*m;
	mf[5] = cosA + y*y*m;
	mf[10]= cosA + z*z*m;

	float tmp1 = x*y*m;
	float tmp2 = z*sinA;
	mf[4] = tmp1 + tmp2;
	mf[1] = tmp1 - tmp2;

	tmp1 = x*z*m;
	tmp2 = y*sinA;
	mf[8] = tmp1 - tmp2;
	mf[2] = tmp1 + tmp2;

	tmp1 = y*z*m;
	tmp2 = x*sinA;
	mf[9] = tmp1 + tmp2;
	mf[6] = tmp1 - tmp2;
	}

	// Simple but not robust matrix inversion. (Doesn't work properly if there is a scaling or skewing transformation.)
	inline CMatrix InvertSimple()
	{
	CMatrix R(0);
	R.mf[0]  = mf[0]; 		R.mf[1]  = mf[4];		R.mf[2]  = mf[8];	R.mf[3]  = 0.0f;
	R.mf[4]  = mf[1];		R.mf[5]  = mf[5];		R.mf[6]  = mf[9];	R.mf[7]  = 0.0f;
	R.mf[8]  = mf[2];		R.mf[9]  = mf[6];		R.mf[10] = mf[10];	R.mf[11] = 0.0f;
	R.mf[12] = -(mf[12]*mf[0]) - (mf[13]*mf[1]) - (mf[14]*mf[2]);
	R.mf[13] = -(mf[12]*mf[4]) - (mf[13]*mf[5]) - (mf[14]*mf[6]);
	R.mf[14] = -(mf[12]*mf[8]) - (mf[13]*mf[9]) - (mf[14]*mf[10]);
	R.mf[15] = 1.0f;
	return R;
	}
	
	// Invert for only a rotation, any translation is zeroed out
	CMatrix InvertRot( )
	{
	 CMatrix R( 0 );
	 R.mf[0]  = mf[0]; 		R.mf[1]  = mf[4];		R.mf[2]  = mf[8];	R.mf[3]  = 0.0f;
	 R.mf[4]  = mf[1];		R.mf[5]  = mf[5];		R.mf[6]  = mf[9];	R.mf[7]  = 0.0f;
	 R.mf[8]  = mf[2];		R.mf[9]  = mf[6];		R.mf[10] = mf[10];	R.mf[11] = 0.0f;
	 R.mf[12] = 0;			R.mf[13] = 0;			R.mf[14] = 0;		R.mf[15] = 1.0f;
	 return R;
	}	


private:
	// helpers for Rotate
	void RotX(float angle)
        {  
        mf[5]  = cosf(angle*TO_RAD);
        mf[6]  = sinf(angle*TO_RAD);
        mf[9]  = -sinf(angle*TO_RAD);
        mf[10] = cosf(angle*TO_RAD);
        }
	void RotY(float angle)
        {
        mf[0]  =  cosf(angle*TO_RAD);
        mf[2]  =  -sinf(angle*TO_RAD);
        mf[8]  =  sinf(angle*TO_RAD);
        mf[10] =  cosf(angle*TO_RAD);    
        }
	void RotZ(float angle)
        {
        mf[0] =  cosf(angle*TO_RAD);
        mf[1] =  sinf(angle*TO_RAD);
        mf[4] =  -sinf(angle*TO_RAD);
        mf[5] =  cosf(angle*TO_RAD);
        }
};

#endif