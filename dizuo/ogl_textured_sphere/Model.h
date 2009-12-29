#include <gl/glut.h>

class CVertex
{
public:
	CVertex()
	{}
	CVertex(float fv1, float fv2, float fv3, 
		float fu, float fv, 
		float fn1, float fn2, float fn3)
	{
		v[0] = fv1;
		v[1] = fv2;
		v[2] = fv3;

		t[0] = fu;
		t[1] = fv;

		n[0] = fn1;
		n[1] = fn2;
		n[2] = fn3;
	}
	float v[3];
	float t[2];
	float n[3];
};

class CTriangle
{
public:
	int v0;
	int v1;
	int v2;

	CTriangle()
	{}
	CTriangle(int i0, int i1, int i2)
		: v0(i0)
		, v1(i1)
		, v2(i2)
	{}
};

class CModel
{
public:
	CModel()
		: pVertex(0)
		, pTriangle(0)
		, triNum(0)
	{}
	~CModel()
	{
		if (pVertex)
		{
			delete pVertex;
			pVertex = 0;
		}
		if (pTriangle)
		{
			delete pTriangle;
			pTriangle = 0;
		}
	}

	//-----------------------------------------------------------------------------
	// Name: renderSphere()
	// Desc: Create a sphere centered at cy, cx, cz with radius r, and 
	//       precision p. Based on a function Written by Paul Bourke. 
	//       http://astronomy.swin.edu.au/~pbourke/opengl/sphere/
	//-----------------------------------------------------------------------------
	void SetupSphere( float cx, float cy, float cz, float r, int p )
	{
		int verNum = p * (p+1);
		int triNum = verNum - 2;
		pVertex = new CVertex[ verNum ];
		pTriangle = new CTriangle[ triNum ];

		const float PI     = 3.14159265358979f;
		const float TWOPI  = 6.28318530717958f;
		const float PIDIV2 = 1.57079632679489f;

		float theta1 = 0.0;
		float theta2 = 0.0;
		float theta3 = 0.0;

		float ex = 0.0f;
		float ey = 0.0f;
		float ez = 0.0f;

		float px = 0.0f;
		float py = 0.0f;
		float pz = 0.0f;

		// Disallow a negative number for radius.
		if( r < 0 )
			r = -r;

		// Disallow a negative number for precision.
		if( p < 0 )
			p = -p;

		// If the sphere is too small, just render a OpenGL point instead.
		if( p < 4 || r <= 0 ) 
		{
			glBegin( GL_POINTS );
			glVertex3f( cx, cy, cz );
			glEnd();
			return;
		}

		int vCount = 0;
		for( int i = 0; i < p/2; ++i )
		{
			theta1 = i * TWOPI / p - PIDIV2;
			theta2 = (i + 1) * TWOPI / p - PIDIV2;

			for( int j = 0; j <= p; ++j )
			{
				theta3 = j * TWOPI / p;

				ex = cosf(theta2) * cosf(theta3);
				ey = sinf(theta2);
				ez = cosf(theta2) * sinf(theta3);
				px = cx + r * ex;
				py = cy + r * ey;
				pz = cz + r * ez;

				pVertex[ vCount++ ] = CVertex( px, py, pz, 
											   -(j/(float)p), 2*(i+1)/(float)p,
											   ex, ey, ez );
				//glNormal3f( ex, ey, ez );
				//glTexCoord2f( -(j/(float)p) , 2*(i+1)/(float)p );
				//glVertex3f( px, py, pz );

				ex = cosf(theta1) * cosf(theta3);
				ey = sinf(theta1);
				ez = cosf(theta1) * sinf(theta3);
				px = cx + r * ex;
				py = cy + r * ey;
				pz = cz + r * ez;

				//glNormal3f( ex, ey, ez );
				//glTexCoord2f( -(j/(float)p), 2*i/(float)p );
				//glVertex3f( px, py, pz );
				pVertex[ vCount++ ] = CVertex( px, py, pz, 
											   -(j/(float)p), 2*(i)/(float)p,
										       ex, ey, ez );
			}
		}

		for (int loop=0, tCount=0; loop<vCount-2; loop++)
		{
			if ( (loop+1)/3)
			{
				pTriangle[ tCount++ ] = CTriangle(loop-2, loop-1, loop);
			}
		}
	}

	void RenderTriangle( const CTriangle& triangle ) const
	{
		glBegin(GL_TRIANGLES);
		{
			glNormal3fv( pVertex[triangle.v0].n );
			glVertex3fv( pVertex[triangle.v0].v );
			glTexCoord2fv( pVertex[triangle.v0].t );

			glNormal3fv( pVertex[triangle.v1].n );
			glVertex3fv( pVertex[triangle.v1].v );
			glTexCoord2fv( pVertex[triangle.v1].t );

			glNormal3fv( pVertex[triangle.v2].n );
			glVertex3fv( pVertex[triangle.v2].v );
			glTexCoord2fv( pVertex[triangle.v2].t );
		}glEnd();
	}

	void RenderModel()
	{
		for (int index = 0; index < triNum; index++)
		{
			RenderTriangle( pTriangle[index] );
		}
	}
protected:
	CTriangle*	pTriangle;
	CVertex*		pVertex;
	int		triNum;	
	int		verNum;

};