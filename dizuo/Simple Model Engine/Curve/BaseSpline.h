#ifndef _SPLINE_H
#define _SPLINE_H
#include <math.h>
#include <GL/glut.h>

namespace spline
{

#define DIV_FACTOR 0.004 //adjust this factor to adjust the curve smoothness

class CurveBacker
{
public:
	float  Ax,Ay,Az;
	float  Bx,By,Bz;
	float  Cx,Cy,Cz;
	int    Ndiv;

	CurveBacker(float ax,float ay,float az,float bx,float by,float bz,float cx,float cy,float cz,int ndiv) 
	{
		Ax = ax;
		Ay = ay;
		Az = az;
		Bx = bx;
		By = by;
		Bz = bz;
		Cx = cx;
		Cy = cy;
		Cz = cz;
		Ndiv = ndiv;
	}

	float max3i(float a,float b,float c)
	{
		float temp = a;
		if(b>temp)
			temp = b;
		if(c>temp)
			temp = c;

		return temp;
	}

	CurveBacker(float ax,float ay,float az,float bx,float by,float bz,float cx,float cy,float cz)
	{
		Ax = ax;
		Ay = ay;
		Az = az;
		Bx = bx;
		By = by;
		Bz = bz;
		Cx = cx;
		Cy = cy;
		Cz = cz;
		Ndiv = (int)(max3i(fabs(Ax),fabs(Ay),fabs(Az))/DIV_FACTOR);
	}
	CurveBacker() 
	{	
	};

	void PutCurve(float ax,float ay,float az,float bx,float by,float bz,float cx,float cy,float cz) 
	{
		Ax = ax;
		Ay = ay;
		Az = az;
		Bx = bx;
		By = by;
		Bz = bz;
		Cx = cx;
		Cy = cy;
		Cz = cz;
		Ndiv = (int)(max3i(fabs(Ax),fabs(Ay),fabs(Az))/DIV_FACTOR);
	}

	void draw(float x,float y,float z) 
	{
		float OrigX,OrigY,OrigZ,NewX,NewY,NewZ;
		float  t,f,g,h;
		if (Ndiv==0)
			Ndiv=1;

		OrigX = x; 
		OrigY = y;
		OrigZ = z;

		glBegin(GL_LINES);
			
			
		for(int i=1; i<=Ndiv ; i++)
		{
			t = 1.0f / (float)Ndiv * (float)i;
			f = t*t*(3.0f-2.0f*t);
			g = t*(t-1.0f)*(t-1.0f);
			h = t*t*(t-1.0f);
			NewX = (x + Ax*f + Bx*g + Cx*h);
			NewY = (y + Ay*f + By*g + Cy*h);
			NewZ = (z + Az*f + Bz*g + Cz*h);

			//MoveToEx(hdc, OrigX, OrigY, NULL);
			//LineTo(hdc, NewX, NewY);
			//OrigX = NewX;  
			//OrigY=NewY;
			glVertex3f(OrigX,OrigY,OrigZ);
			glVertex3f(NewX,NewY,NewZ);
			OrigX=NewX;
			OrigY=NewY;
			OrigZ=NewZ;
		}
		glEnd();
	}
	int GetCount()
	{
		if (Ndiv==0)
			Ndiv=1;
		int PointCount = 1;

		for(int i=1; i<=Ndiv ; i++)
		{
			PointCount++;
		}
		return PointCount;
	}
	void GetCurve(float x,float y,float z, RVec3Array& points)
	{
		float X,Y,Z;
		float  t,f,g,h;
		if (Ndiv==0)
			Ndiv=1;

		X = x; 
		Y = y;
		Z = z;

		//4/1/2010 RYF
		points.push_back( RVec3(X, Y, Z) );
		//points[PointCount].setValue(X, Y, Z);
		//points[PointCount].x = X;
		//points[PointCount].y = Y;
		//points[PointCount].z = Z;

		for(int i=1; i<=Ndiv ; i++)
		{
			t = 1.0f / (float)Ndiv * (float)i;
			f = t*t*(3.0f-2.0f*t);
			g = t*(t-1.0f)*(t-1.0f);
			h = t*t*(t-1.0f);
			X = (x + Ax*f + Bx*g + Cx*h);
			Y = (y + Ay*f + By*g + Cy*h);
			Z = (z + Az*f + Bz*g + Cz*h);

			//4/1/2010 RYF
			points.push_back( RVec3(X, Y, Z) );
			//points[PointCount].setValue(X, Y, Z);
			//points[PointCount].x = X;
			//points[PointCount].y = Y;
			//points[PointCount].z = Z;
		}
	}
};	// CurveBacker

class Spline 
{
public:
	float* Px;
	float* Py;
	float* Pz;
	float* Ax;
	float* Ay;
	float* Az;
	float* Bx;
	float* By;
	float* Bz;
	float* Cx;
	float* Cy;
	float* Cz;
	float*  k;
	float*  Mat[3];

	int  NP;

	// constructor
	Spline(RVec3 pt[], int np)
	{
		NP = np;
		Px = new float[NP];
		Py = new float[NP];
		Pz = new float[NP];
		Ax = new float[NP];
		Ay = new float[NP];
		Az = new float[NP];
		Bx = new float[NP];
		By = new float[NP];
		Bz = new float[NP];
		Cx = new float[NP];
		Cy = new float[NP];
		Cz = new float[NP];
		k = new float[NP];
		Mat[0] = new float[NP];
		Mat[1] = new float[NP];
		Mat[2] = new float[NP];

		for(int i=0;i<NP ;i++) 
		{
			Px[i] = pt[i].x();  
			Py[i] = pt[i].y();
			Pz[i] = pt[i].z();
		}
	}
	//4/1/2010 RYF
	Spline(const RVec3Array& pts)
	{
		NP = pts.size();
		Px = new float[NP];
		Py = new float[NP];
		Pz = new float[NP];
		Ax = new float[NP];
		Ay = new float[NP];
		Az = new float[NP];
		Bx = new float[NP];
		By = new float[NP];
		Bz = new float[NP];
		Cx = new float[NP];
		Cy = new float[NP];
		Cz = new float[NP];
		k = new float[NP];
		Mat[0] = new float[NP];
		Mat[1] = new float[NP];
		Mat[2] = new float[NP];

		for(int i=0;i<NP ;i++) 
		{
			Px[i] = pts[i].x();  
			Py[i] = pts[i].y();
			Pz[i] = pts[i].z();
		}
	}

	Spline(float px[] , float py[] , float pz[] ,int np) 
	{
		NP = np;
		Px = new float[NP];
		Py = new float[NP];
		Pz = new float[NP];
		Ax = new float[NP];
		Ay = new float[NP];
		Az = new float[NP];
		Bx = new float[NP];
		By = new float[NP];
		Bz = new float[NP];
		Cx = new float[NP];
		Cy = new float[NP];
		Cz = new float[NP];
		k = new float[NP];
		Mat[0] = new float[NP];
		Mat[1] = new float[NP];
		Mat[2] = new float[NP];

		for(int i=0;i<NP ;i++) 
		{
			Px[i] = px[i];  
			Py[i] = py[i];
			Pz[i] = pz[i];
		}

	}
	
	~Spline()
	{
		delete[] Px;
		delete[] Py;
		delete[] Pz;
		delete[] Ax;
		delete[] Ay;
		delete[] Az;
		delete[] Bx;
		delete[] By;
		delete[] Bz;
		delete[] Cx;
		delete[] Cy;
		delete[] Cz;
		delete[] k;
		delete[] Mat[0];
		delete[] Mat[1];
		delete[] Mat[2];
	}

	void Generate() 
	{
		int i=0;
		float AMag , AMagOld;
    	// vector A
		for(i= 0 ; i<=NP-2 ; i++ ) 
		{
			Ax[i] = Px[i+1] - Px[i];
			Ay[i] = Py[i+1] - Py[i];
			Az[i] = Pz[i+1] - Pz[i];
		}
		// k
		AMagOld = (float)sqrt(Ax[0]*Ax[0] + Ay[0]*Ay[0] + Az[0]*Az[0]);
		for(i=0 ; i<=NP-3 ; i++) 
		{
			AMag = (float)sqrt(Ax[i+1]*Ax[i+1] + Ay[i+1]*Ay[i+1] + Az[i+1]*Az[i+1]);
			k[i] = AMagOld / AMag;
			AMagOld = AMag;
		}
		k[NP-2] = 1.0f;

		// Matrix
		for(i=1; i<=NP-2;i++) 
		{
			Mat[0][i] = 1.0f;
			Mat[1][i] = 2.0f*k[i-1]*(1.0f + k[i-1]);
			Mat[2][i] = k[i-1]*k[i-1]*k[i];
		}
		Mat[1][0] = 2.0f;
		Mat[2][0] = k[0];
		Mat[0][NP-1] = 1.0f;
		Mat[1][NP-1] = 2.0f*k[NP-2];

		// 
		for(i=1; i<=NP-2;i++) 
		{
			Bx[i] = 3.0f*(Ax[i-1] + k[i-1]*k[i-1]*Ax[i]);
			By[i] = 3.0f*(Ay[i-1] + k[i-1]*k[i-1]*Ay[i]);
			Bz[i] = 3.0f*(Az[i-1] + k[i-1]*k[i-1]*Az[i]);
		}
		Bx[0] = 3.0f*Ax[0];
		By[0] = 3.0f*Ay[0];
		Bz[0] = 3.0f*Az[0];
		Bx[NP-1] = 3.0f*Ax[NP-2];
		By[NP-1] = 3.0f*Ay[NP-2];
		Bz[NP-1] = 3.0f*Az[NP-2];

		//
		MatrixSolve(Bx);
		MatrixSolve(By);
		MatrixSolve(Bz);

		for(i=0 ; i<=NP-2 ; i++ ) 
		{
			Cx[i] = k[i]*Bx[i+1];
			Cy[i] = k[i]*By[i+1];
			Cz[i] = k[i]*Bz[i+1];
		}
	}

	void MatrixSolve(float B[]) 
	{
		float* Work = new float[NP];
		float* WorkB = new float[NP];
		for(int i=0;i<=NP-1;i++) 
		{
			Work[i] = B[i] / Mat[1][i];
			WorkB[i] = Work[i];
		}

		for(int j=0 ; j<10 ; j++) 
		{ ///  need convergence judge
			Work[0] = (B[0] - Mat[2][0]*WorkB[1])/Mat[1][0];
			int i = 0;
			for(i=1; i<NP-1 ; i++ ) 
			{
				Work[i] = (B[i]-Mat[0][i]*WorkB[i-1]-Mat[2][i]*WorkB[i+1])
							/Mat[1][i];
			}
			Work[NP-1] = (B[NP-1] - Mat[0][NP-1]*WorkB[NP-2])/Mat[1][NP-1];

			for(i=0 ; i<=NP-1 ; i++ ) 
			{
				WorkB[i] = Work[i];
			}
		}
		for(int i=0 ; i<=NP-1 ; i++ ) 
		{
			B[i] = Work[i];
		}
		delete[] Work;
		delete[] WorkB;
	}

	void draw() 
	{
		CurveBacker c;
		for(int i=0; i<NP-1 ; i++) 
		{
			c.PutCurve(Ax[i],Ay[i],Az[i],Bx[i],By[i],Bz[i],Cx[i],Cy[i],Cz[i]);
			c.draw(Px[i],Py[i],Pz[i]);
		}
		
	}
	int GetCurveCount()
	{
		CurveBacker c;
		int count = 0;
		for(int i=0; i<NP-1 ; i++) 
		{
			c.PutCurve(Ax[i],Ay[i],Az[i],Bx[i],By[i],Bz[i],Cx[i],Cy[i],Cz[i]);
			count += c.GetCount();
		}
		return count;
	}
	void GetCurve(RVec3Array& points)
	{
		CurveBacker c;
		for(int i=0; i<NP-1 ; i++) 
		{
			c.PutCurve(Ax[i],Ay[i],Az[i],Bx[i],By[i],Bz[i],Cx[i],Cy[i],Cz[i]);
			c.GetCurve(Px[i],Py[i],Pz[i], points);
		}
	}
  //////////// closed cubic spline ////////////////////
	void GenClosed() 
	{
		float AMag , AMagOld , AMag0;
        // vector A
		for(int i= 0 ; i<=NP-2 ; i++ ) 
		{
			Ax[i] = Px[i+1] - Px[i];
			Ay[i] = Py[i+1] - Py[i];
			Az[i] = Pz[i+1] - Pz[i];
		}
		Ax[NP-1] = Px[0] - Px[NP-1];
		Ay[NP-1] = Py[0] - Py[NP-1];
		Az[NP-1] = Pz[0] - Pz[NP-1];

		// k
		AMag0 = AMagOld = (float)sqrt(Ax[0]*Ax[0] + Ay[0]*Ay[0] + Az[0]*Az[0]);
		for(int i=0 ; i<=NP-2 ; i++) 
		{
			AMag = (float)sqrt(Ax[i+1]*Ax[i+1] + Ay[i+1]*Ay[i+1] + Az[i+1]*Az[i+1]);
			k[i] = AMagOld / AMag;
			AMagOld = AMag;
		}
		k[NP-1]=AMagOld/AMag0; 

		// Matrix
		for(int i=1; i<=NP-1;i++) 
		{
			Mat[0][i] = 1.0f;
			Mat[1][1] = 2.0f*k[i-1]*(1.0f + k[i-1]);
			Mat[2][i] = k[i-1]*k[i-1]*k[i];
		}
		Mat[0][0] = 1.0f;
		Mat[1][0] = 2.0f*k[NP-1]*(1.0f + k[NP-1]);
		Mat[2][0] = k[NP-1]*k[NP-1]*k[0];

		// 
		for(int i=1; i<=NP-1;i++) 
		{
			Bx[i] = 3.0f*(Ax[i-1] + k[i-1]*k[i-1]*Ax[i]);
			By[i] = 3.0f*(Ay[i-1] + k[i-1]*k[i-1]*Ay[i]);
			Bz[i] = 3.0f*(Az[i-1] + k[i-1]*k[i-1]*Az[i]);
		}
		Bx[0] = 3.0f*(Ax[NP-1] + k[NP-1]*k[NP-1]*Ax[0]);
		By[0] = 3.0f*(Ay[NP-1] + k[NP-1]*k[NP-1]*Ay[0]);
		Bz[0] = 3.0f*(Az[NP-1] + k[NP-1]*k[NP-1]*Az[0]);

		//
		MatrixSolveEX(Bx);
		MatrixSolveEX(By);
		MatrixSolveEX(Bz);

		for(int i=0 ; i<=NP-2 ; i++ ) 
		{
			Cx[i] = k[i]*Bx[i+1];
			Cy[i] = k[i]*By[i+1];
			Cz[i] = k[i]*Bz[i+1];
		}
		Cx[NP-1] = k[NP-1]*Bx[0];
		Cy[NP-1] = k[NP-1]*By[0];
		Cz[NP-1] = k[NP-1]*Bz[0];
	}

  ///// tridiagonal matrix + elements of [0][0], [N-1][N-1] //// 
	void MatrixSolveEX(float B[]) 
	{
		float* Work = new float[NP];
		float* WorkB = new float[NP];

		for(int i=0;i<=NP-1;i++) 
		{
			Work[i] = B[i] / Mat[1][i];
			WorkB[i] = Work[i];
		}

		for(int j=0 ; j<10 ; j++) 
		{  // need judge of convergence
			Work[0] = (B[0]-Mat[0][0]*WorkB[NP-1]-Mat[2][0]*WorkB[1])
					/Mat[1][0];
			for(int i=1; i<NP-1 ; i++ ) 
			{
				Work[i] = (B[i]-Mat[0][i]*WorkB[i-1]-Mat[2][i]*WorkB[i+1])
							/Mat[1][i];
			}
			Work[NP-1] = (B[NP-1]-Mat[0][NP-1]*WorkB[NP-2]-Mat[2][NP-1]*WorkB[0])
							/Mat[1][NP-1];

			for(int i=0 ; i<=NP-1 ; i++ ) 
			{
				WorkB[i] = Work[i];
			}
		}

		for(int i=0 ; i<=NP-1 ; i++ ) 
		{
			B[i] = Work[i];
		}
	}

	void drawClosed() 
	{
		CurveBacker c;
		for(int i=0; i<NP ; i++) 
		{
			c.PutCurve(Ax[i],Ay[i],Az[i],Bx[i],By[i],Bz[i],Cx[i],Cy[i],Cz[i]);
			c.draw(Px[i],Py[i],Pz[i]);
		}
	}
};	//class Spline

} //namespace spline
#endif
