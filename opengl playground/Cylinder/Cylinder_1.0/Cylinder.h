#ifndef CYLINDER_H
#define CYLINDER_H

struct Vertex3f
{
	float x; 
	float y;
	float z;

	float nx;
	float ny;
	float nz;

	float u;
	float v;

	void setValue(float x_, float y_, float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	void setNormal(float x_, float y_, float z_)
	{
		nx = x_;
		ny = y_;
		nz = z_;
	}

	void setTexture(float u_, float v_)
	{
		u = u_;
		v = v_;
	}
};


enum REND_MODE
{
	SOLID = 3000,
	WIRE = 3001
};

/*
 *	Cylinder created by RYF. [11/25/2009]
 */
class Cylinder
{
public:
	Cylinder(float r, float h, int l, int v);
	~Cylinder();

	void Render(REND_MODE mode);

	void PrintMatrices();
protected:
	Cylinder(){};
	

	void RenderSlice( const Vertex3f& v1,
   					  const Vertex3f& v2,
					  const Vertex3f& v3,
					  const Vertex3f& v4,
					  int i, int j,			
					  REND_MODE mode );

	void RenderSliceNormal( const Vertex3f& v1, 
					  const Vertex3f& v2, 
					  const Vertex3f& v3, 
					  const Vertex3f& v4 );

	void _getMatrices();
	void _setupVertexTexcoord();
	void _getVertex(float alpha, float hgh, Vertex3f& vertex);

	unsigned int	texId;		//纹理ID
	bool			isHasTex;	//是否加载纹理

	float		radius;				//半径
	float		height;				//高度
	int			lSlice;				//水平分段数
	int			vSlice;				//垂直分段数
	Vertex3f*	pVertexBuffer;		//顶点数据
	float*		pTexcoord;			//最后一列的纹理坐标
};

//12/2/2009 RYF
//	将纹理坐标单独出来
//	改变RenderSlice的参数
#endif