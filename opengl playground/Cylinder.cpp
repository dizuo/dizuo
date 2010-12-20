#include "Cylinder.h"
#include <gl/glut.h>
#include <iostream>
#include <iomanip>
#include <cmath>

// ----------------------------------------------------------------
//	Description:	重载构造函数
//	Para info:		r为上下圆的半径
//					h为高度
//					l为上下圆的分段数
//					v为高度上圆柱的段数v>=2
// ----------------------------------------------------------------

Cylinder::Cylinder(float r, float h, int l, int v)
	: radius(r)
	, height(h)
	, lSlice(l)
	, vSlice(v)
	, texId(0)
	, isHasTex(false)
	, pVertexBuffer(0)
	, pTexcoord(0)
{
	pVertexBuffer = new Vertex3f[vSlice*lSlice];
	pTexcoord = new float[vSlice];
	
	_setupVertexTexcoord();
	_getMatrices();
	PrintMatrices();
}

Cylinder::~Cylinder()
{
	if (pVertexBuffer)
	{
		delete pVertexBuffer;
		pVertexBuffer = 0;
	}
}

// ----------------------------------------------------------------
//	Description:	圆柱体绘制函数
//	绘制按照逆时钟：
//		v4		v3
//	
//		v1		v2
// ----------------------------------------------------------------
void Cylinder::Render(REND_MODE mode)
{
	int i(0);
	int j(0);

	// 绘制圆柱体
	for (i=0; i<vSlice-1; i++)
	{
		for (j=0; j<lSlice-1; j++)
		{
			RenderSlice( pVertexBuffer[(i+1)*lSlice+j],
						 pVertexBuffer[(i+1)*lSlice+j+1],			
						 pVertexBuffer[i*lSlice+j+1],
						 pVertexBuffer[i*lSlice+j],	
						 i, j,
						 mode );
		}

		RenderSlice( pVertexBuffer[(i+1)*lSlice+j],		//第二行末端
					 pVertexBuffer[(i+1)*lSlice],		//第二行始端	
		 			 pVertexBuffer[i*lSlice],			//第一行始端
					 pVertexBuffer[i*lSlice+j],			//第一行末端
					 i, j+1,
					 mode );

		//最后一段绘制为了让圆柱曲面封闭，但是此时根据四个顶点的纹理坐标绘制，
		//	结果是将整张图片压缩到了一个狭小的四边形区域中，导致曲面贴图不连续
		//	分段数越大，这个区域越小。
	}
}

// ----------------------------------------------------------------
//	Description:	绘制四边形函数
//	函数参数顺序：	1 2 3 4
//	GL_TRIANGLE_STRIP的绘制顺序：1 2 4 3
//		四个v参数提供：位置和法线数据
//		i，j提供纹理坐标数据
//		1, 4为要特殊处理的点
// ----------------------------------------------------------------
void Cylinder::RenderSlice( const Vertex3f& v1, 
						    const Vertex3f& v2, 
							const Vertex3f& v3, 
							const Vertex3f& v4,
							int i, int j, 
							REND_MODE mode )
{
	glDisable(GL_LIGHTING);

	switch(mode)
	{
	case SOLID:
		glBegin(GL_TRIANGLE_STRIP);
		//glBegin(GL_POLYGON);
		break;
	case WIRE:
		glBegin(GL_LINE_LOOP);
		break;
	}
		glTexCoord2f( v1.u, v1.v );		
		glVertex3f(v1.x, v1.y, v1.z);	/*glNormal3f(v1.nx, v1.ny, v1.nz);*/	

		//特殊处理v2的配置
		if (j==lSlice)
		{
			glTexCoord2f( 1.0f, pTexcoord[i+1] );		
			glVertex3f(v2.x, v2.y, v2.z);	/*glNormal3f(v2.nx, v2.ny, v2.nz);*/	
		}
		else
		{
			glTexCoord2f( v2.u, v2.v );		
			glVertex3f(v2.x, v2.y, v2.z);	/*glNormal3f(v2.nx, v2.ny, v2.nz);*/	
		}

		glTexCoord2f( v4.u, v4.v );		
		glVertex3f(v4.x, v4.y, v4.z);	/*glNormal3f(v4.nx, v4.ny, v4.nz);*/
		
		//特殊处理v3的配置
		if (j==lSlice)
		{
			glTexCoord2f( 1.0f, pTexcoord[i] );		
			glVertex3f(v3.x, v3.y, v3.z);	/*glNormal3f(v3.nx, v3.ny, v3.nz);*/	
		}
		else
		{
			glTexCoord2f( v3.u, v3.v );		
			glVertex3f(v3.x, v3.y, v3.z);	/*glNormal3f(v3.nx, v3.ny, v3.nz);*/	
		}

	glEnd();

	// 显示法线
	// RenderSliceNormal( v1, v2, v3, v4 );

	glEnable(GL_LIGHTING);
}
/*
//---------------------------------------------------------------------
row = 0.coll = 0
position(x, y, z) = 0.5 0 0
texture(u, v) = 0 0
//---------------------------------------------------------------------
//---------------------------------------------------------------------
row = 0.coll = 1
position(x, y, z) = -0.2499999 0 0.4330128
texture(u, v) = 0.3333333 0
//---------------------------------------------------------------------
//---------------------------------------------------------------------
row = 0.coll = 2
position(x, y, z) = -0.2500001 0 -0.4330126
texture(u, v) = 0.6666667 0
//---------------------------------------------------------------------
//---------------------------------------------------------------------
row = 1.coll = 0
position(x, y, z) = 0.5 1.3 0
texture(u, v) = 0 1
//---------------------------------------------------------------------
//---------------------------------------------------------------------
row = 1.coll = 1
position(x, y, z) = -0.2499999 1.3 0.4330128
texture(u, v) = 0.3333333 1
//---------------------------------------------------------------------
//---------------------------------------------------------------------
row = 1.coll = 2
position(x, y, z) = -0.2500001 1.3 -0.4330126
texture(u, v) = 0.6666667 1
//---------------------------------------------------------------------
*/

void Cylinder::RenderSliceNormal( const Vertex3f& v1, const Vertex3f& v2, const Vertex3f& v3, const Vertex3f& v4 )
{
	glBegin(GL_LINES);
	glVertex3f(v1.x, v1.y, v1.z);	
	glVertex3f(v1.nx + v1.x, v1.ny+v1.y, v1.nz+v1.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(v2.x, v2.y, v2.z);	
	glVertex3f(v2.nx + v2.x, v2.ny+v2.y, v2.nz+v2.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(v3.x, v3.y, v3.z);	
	glVertex3f(v3.nx + v3.x, v3.ny+v3.y, v3.nz+v3.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(v4.x, v4.y, v4.z);	
	glVertex3f(v4.nx + v4.x, v4.ny+v4.y, v4.nz+v4.z);
	glEnd();
}

// ----------------------------------------------------------------
//	Description:	根据vSlice和lSlice生成纹理坐标
//					
// ----------------------------------------------------------------
void Cylinder::_setupVertexTexcoord()
{
	if (!pVertexBuffer || !pTexcoord)
		return;
	
	float ds = 1.0f / lSlice;			//x,u 方向
	float dt = 1.0f / (vSlice-1);		//y,v 方向

	for (int i=0; i<vSlice; i++)
	{
		for (int j=0; j<lSlice; j++)
		{
			pVertexBuffer[j + i*lSlice].u = j*ds;
			pVertexBuffer[j + i*lSlice].v = i*dt;
		}

		// 最后一列的纹理横坐标全部为：1.0
		// pTexcoord中只是纵坐标
		pTexcoord[i] = i * dt;
	}
}


// ----------------------------------------------------------------
//	Description:	根据角度和高度求取顶点的坐标
//	Para info:		theta为弧度数，圆中的角度
//					hgh为高度	
// ----------------------------------------------------------------
void Cylinder::_getVertex(float theta, float hgh, Vertex3f& vertex)
{
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);

	// setup position coordinate
	vertex.x = radius * cosTheta;
	vertex.y = hgh;
	vertex.z = radius * sinTheta;

	// setup normal coordinate
	vertex.nx = cosTheta;
	vertex.ny = 0;
	vertex.nz = sinTheta;
}

// ----------------------------------------------------------------
//	Description:	求取圆柱的顶点阵列
// ----------------------------------------------------------------
void Cylinder::_getMatrices()
{
	const float pi = 3.1415926;

	float angle = 2.0f * pi / static_cast<float>(lSlice);
	// 注意分母为高度段数减1
	float span = height / static_cast<float>(vSlice-1);

	for (int v=0; v<vSlice; v++)
	{
		float y = span * v;
		for (int l=0; l<lSlice; l++)
		{
			float x = angle * static_cast<float>(l);
			_getVertex(x, y, pVertexBuffer[l + v*lSlice]);
		}
	}
}

// ----------------------------------------------------------------
//	Description:	打印matrices中的元素
// ----------------------------------------------------------------
void Cylinder::PrintMatrices()
{
	using namespace std;
	for (int i=0; i<vSlice*lSlice; i++)
	{
		cout << "//---------------------------------------------------------------------" << endl;
		cout << "row = " << i / lSlice << ".";
		cout << "coll = " << i % lSlice << endl;

		cout << "position(x, y, z) = " << setprecision (7) << pVertexBuffer[i].x << " "
									   << setprecision (7) << pVertexBuffer[i].y << " "
									   << setprecision (7) << pVertexBuffer[i].z << endl;

		cout << "texture(u, v) = " << setprecision (7) << pVertexBuffer[i].u << " "
								   << setprecision (7) << pVertexBuffer[i].v << endl;
		cout << "//---------------------------------------------------------------------" << endl;
	}
}


//使用GL_POLYGON绘制有点问题：四边形其实也是两个三角形绘制成的。直接使用三角形GL_TRIANGLE_STRIP
//	Cylinder中的加载纹理有问题。。。

//研究了一下3dmax中圆柱体的纹理坐标：通过降低圆柱体的水平分段数，保存为obj文件，
//	发现纹理坐标比顶点，法线坐标多了vSlice个。
