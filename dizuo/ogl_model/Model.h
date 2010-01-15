#ifndef MODEL_H_
#define MODEL_H_

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

class CModel
{
public:
	CModel()
		: pVertex(0)
		, pTriIndices(0)
		, indiceNum(0)
	{}
	~CModel()
	{
		if (pVertex)
		{
			delete [] pVertex;
			pVertex = 0;
		}
		if (pTriIndices)
		{
			delete [] pTriIndices;
			pTriIndices = 0;
		}
	}

	void RotateVectorY( const float srcX, const float srcY, const float srcZ,
						const float angle,
						float& destX, float& destY, float& destZ )
	{
		const float PI     = 3.14159265358979f;

		float sinAngle= sinf(PI*angle/180);
		float cosAngle= cosf(PI*angle/180);

		destX = srcX*cosAngle + srcZ*sinAngle;
		destY = srcY;
		destZ = -srcX*sinAngle + srcZ*cosAngle;
	}

	//-----------------------------------------------------------------------------
	// Name: renderSphere()
	// Desc: Create a sphere centered at cy, cx, cz with radius r, and 
	//       precision p. 
	//-----------------------------------------------------------------------------
	void SetupSphere( float cx, float cy, float cz, float r, int p )
	{
		verNum = (p/2 + 1) * (p+1);
		indiceNum = p * p * 3;	// (p/2) * p * 2
		pVertex = new CVertex[ verNum ];
		pTriIndices = new int[ indiceNum ];

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

		if( p < 4 || r <= 0 ) 
			return;

		for( int i = 0; i <= p/2; ++i )
		{
			theta1 = i * TWOPI / p - PIDIV2;

			for( int j = 0; j <= p; ++j )
			{
				theta3 = j * TWOPI / p;

				ex = cosf(theta1) * cosf(theta3);
				ey = sinf(theta1);
				ez = cosf(theta1) * sinf(theta3);
				px = cx + r * ex;
				py = cy + r * ey;
				pz = cz + r * ez;

 				pVertex[ i*(p+1) + j ] = CVertex( px, py, pz, 
 											   -(j/(float)p), 2*(i)/(float)p,
 										       ex, ey, ez );
			}
		}
		
		// setup sphere's indices...
		for (int outer=0; outer<p/2; outer++)
		{
			for (int inner=0; inner<p; inner++)
			{
				//triangle 1
				pTriIndices[ ((outer*p+inner)*2)*3 + 0 ] = outer*(p+1)+inner;
				pTriIndices[ ((outer*p+inner)*2)*3 + 1 ] = (outer+1)*(p+1)+inner;
				pTriIndices[ ((outer*p+inner)*2)*3 + 2 ] = outer*(p+1)+inner+1;

				//triangle 2
				pTriIndices[ ((outer*p+inner)*2+1)*3 + 0 ] = outer*(p+1)+inner+1;
				pTriIndices[ ((outer*p+inner)*2+1)*3 + 1 ] = (outer+1)*(p+1)+inner;
				pTriIndices[ ((outer*p+inner)*2+1)*3 + 2 ] = (outer+1)*(p+1)+inner+1;
			}
		}

		return;
	}

	//-----------------------------------------------------------------------------
	// Name: SetupTorus()
	// Desc: Create a torus centered at 0, 0, 0 with radius r, and 
	//       precision torusPrecision. 
	//-----------------------------------------------------------------------------
	void SetupTorus(float innerRadius, float outerRadius, int torusPrecision)
	{
		verNum = (torusPrecision+1)*(torusPrecision+1);
		indiceNum = 2*torusPrecision*torusPrecision*3;

		pVertex = new CVertex[verNum];
		pTriIndices = new int[indiceNum];

		// normals
		float ex = 0.0f;
		float ey = 0.0f;
		float ez = 0.0f;

		// positions
		float px = 0.0f;
		float py = 0.0f;
		float pz = 0.0f;

		//calculate the first ring - inner radius 4, outer radius 1.5
		for(int i=0; i<torusPrecision+1; i++)
		{
			const float PI     = 3.14159265358979f;
			float angle = i*360.0f/torusPrecision;
			float sinAngle=(float)sin( PI*angle/180);
			float cosAngle=(float)cos( PI*angle/180);
			
			px = outerRadius * cosAngle + innerRadius;
			py = outerRadius * sinAngle;
			pz = 0.0f;
			
			ex = cosAngle;
			ey = sinAngle;
			ez = 0.0f;

			pVertex[i] = CVertex( px, py, pz, 
								  0.0f, (float)i/torusPrecision,
								  ex, ey, ez );
		}

		//rotate this to get other rings
		//********the following rings are both computed by ring0********\\
		//********rotate the ring0 in some angle********		
		for(int ring=1; ring<torusPrecision+1; ring++)
		{
			for(int i=0; i<torusPrecision+1; i++)
			{
				float angle = ring*360.0f/torusPrecision;
				
				float src[3];
				src[0] = pVertex[i].v[0];
				src[1] = pVertex[i].v[1];
				src[2] = pVertex[i].v[2];
				RotateVectorY(src[0], src[1], src[2], angle, px, py, pz);

				src[0] = pVertex[i].n[0];
				src[1] = pVertex[i].n[1];
				src[2] = pVertex[i].n[2];
				RotateVectorY(src[0], src[1], src[2], angle, ex, ey, ez);

				pVertex[ring*(torusPrecision+1)+i] = CVertex(px, py, pz,
															 2.0f*ring/torusPrecision, pVertex[i].t[1],
															 ex, ey, ez);
			}
		}

		//calculate the indices
		for(int ring=0; ring<torusPrecision; ring++)
		{
			for(int i=0; i<torusPrecision; i++)
			{
				// triangle 0
				pTriIndices[ ((ring*torusPrecision+i)*2)*3+0 ]	= ring*(torusPrecision+1)+i;
				pTriIndices[ ((ring*torusPrecision+i)*2)*3+1 ]	= (ring+1)*(torusPrecision+1)+i;
				pTriIndices[ ((ring*torusPrecision+i)*2)*3+2 ]	= ring*(torusPrecision+1)+i+1;

				// triangle 1
				pTriIndices[ ((ring*torusPrecision+i)*2+1)*3+0 ]= ring*(torusPrecision+1)+i+1;
				pTriIndices[ ((ring*torusPrecision+i)*2+1)*3+1 ]= (ring+1)*(torusPrecision+1)+i;
				pTriIndices[ ((ring*torusPrecision+i)*2+1)*3+2 ]= (ring+1)*(torusPrecision+1)+i+1;
			}
		}

	}

	//-----------------------------------------------------------------------------
	// Name: SetupCylinder()
	// Desc: Create a cylinder centered at 0, 0, 0 
	//		with	top circle radius, 
	//				bottom circle radius
	//				height
	//				circle's precision
	//				and height segment.(>=1)
	//-----------------------------------------------------------------------------
	void SetupCylinder(float topRadius, float botRadius, int cirPrecision, float height, int heighSegment)
	{
		const float TWOPI  = 6.28318530717958f;
		verNum = (cirPrecision+1)*(heighSegment+1);
		indiceNum = 2*cirPrecision*heighSegment*3;

		pVertex = new CVertex[verNum];
		pTriIndices = new int[indiceNum];

		// normals
		float ex = 0.0f;
		float ey = 0.0f;
		float ez = 0.0f;

		// positions
		float px = 0.0f;
		float py = 0.0f;
		float pz = 0.0f;

		for (int i=0; i<heighSegment+1; i++)
		{
			float heighFactor = (float)(i)/(float)(heighSegment);
			//0.0 <= delta <= 1.0
			py = heighFactor*height; 
			ey = 0.0f;
			
			float radiusDelta = botRadius - topRadius;
			float radius = topRadius + heighFactor * radiusDelta;

			for (int j=0; j<=cirPrecision; j++)
			{
				float angle = (float)(j)*TWOPI/cirPrecision;
				ex = cosf(angle);
				ez = sinf(angle);

				px = radius*ex;
				pz = radius*ez;

				//strip
				pVertex[ i*(cirPrecision+1)+j ] = 
					CVertex(px, py, pz, (float)(j)/cirPrecision, heighFactor, ex, ey, ez);
			}
		}

		//calculate the indices
		for(int hgh=0; hgh<heighSegment; hgh++)
		{
			for(int i=0; i<cirPrecision; i++)
			{
				// triangle 0
				pTriIndices[ ((hgh*cirPrecision+i)*2)*3+0 ]	= hgh*(cirPrecision+1)+i;
				pTriIndices[ ((hgh*cirPrecision+i)*2)*3+1 ]	= (hgh+1)*(cirPrecision+1)+i;
				pTriIndices[ ((hgh*cirPrecision+i)*2)*3+2 ]	= hgh*(cirPrecision+1)+i+1;

				// triangle 1
				pTriIndices[ ((hgh*cirPrecision+i)*2+1)*3+0 ]= hgh*(cirPrecision+1)+i+1;
				pTriIndices[ ((hgh*cirPrecision+i)*2+1)*3+1 ]= (hgh+1)*(cirPrecision+1)+i;
				pTriIndices[ ((hgh*cirPrecision+i)*2+1)*3+2 ]= (hgh+1)*(cirPrecision+1)+i+1;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void RenderModel()
	{
		//Set vertex arrays for torus
		glVertexPointer(3, GL_FLOAT, sizeof(CVertex), &pVertex[0].v);
		glEnableClientState(GL_VERTEX_ARRAY);

		glNormalPointer(GL_FLOAT, sizeof(CVertex), &pVertex[0].n);
		glEnableClientState(GL_NORMAL_ARRAY);

		glTexCoordPointer(2, GL_FLOAT, sizeof(CVertex), &pVertex[0].t);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

		//Draw
		glDrawElements(GL_TRIANGLES, indiceNum, GL_UNSIGNED_INT, pTriIndices);

		//disable vertex arrays
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
protected:
	int*	pTriIndices;
	CVertex*		pVertex;
	int		indiceNum;	//indices	
	int		verNum;	//vertices
};

#endif