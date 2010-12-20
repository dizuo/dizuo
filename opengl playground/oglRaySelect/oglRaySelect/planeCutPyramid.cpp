#include <vector>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

#include "../../gtl/vec3.hpp"
#include "../../gtl/ray.hpp"

using namespace std;

class LineSegment
{
public:
	LineSegment()
	{}

	LineSegment(const gtl::Vec3f& v_0, const gtl::Vec3f& v_1)
	{
		v0 = v_0;
		v1 = v_1;
	}
	gtl::Vec3f v0;
	gtl::Vec3f v1;
};

class Triangle
{
public:
	Triangle(const gtl::Vec3f& v0, const gtl::Vec3f& v1, const gtl::Vec3f& v2)
	{
		vert[0] = v0;
		vert[1] = v1;
		vert[2] = v2;

		ComputeNormal();
	}

	void ComputeNormal()
	{
		gtl::Vec3f edge1 = vert[1] - vert[0];
		gtl::Vec3f edge2 = vert[2] - vert[1];

		normal = edge1.cross( edge2 );
	}

	gtl::Vec3f normal;
	gtl::Vec3f vert[3];
};

//定义pyramid的顶点集合
class Pyramid
{
public:
	Pyramid(){}
	~Pyramid(){}

	void AddFaceVert(const gtl::Vec3f& vert) { baseFace.push_back(vert);}
	void SetTopVert(const gtl::Vec3f& vert) { topVert = vert; }

	//vector<Triangle> GetFacets() const 
	//{
	//	vector<Triangle> retTris;

	//	for(int i=0; i<baseFace.size(); i++)
	//	{
	//		int next = (i+1) % baseFace.size();
	//		retTris.push_back( Triangle( gtl::Vec3f() ) )
	//	}
	//}

	gtl::Vec3f topVert;
	vector<gtl::Vec3f> baseFace;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define SMALL_NUM  0.00000001 // anything that avoids division overflow

GLfloat	g_fAngle[3] = {0.0};
gtl::Vec3f vert0(0.0f, 2.0f, 0.0f);
gtl::Vec3f vert1(2.0f, 0.0f, 0.0f);
gtl::Vec3f vert2(0.0f, -2.0f, 0.0f);
gtl::Vec3f intersecVert;

//近平面 远平面上的点
gtl::Vec3f near_point, far_point;

//测试平面与三角形的交点
gtl::Vec3f	cutplaneVert0(0.0f, 0.0f, 0.0f);
gtl::Vec3f	cutplaneVert1(1.0f, 0.2f, 1.0f);
gtl::Vec3f	cutplaneVert2(0.0f, 0.0f, 1.0f);

LineSegment intersect_line;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*!	V0，V1，V2三角形三顶点
	I为交点
	返回值	-1：没有交点
			0：	交点在三角形外
			1：交点在三角形内部
*/
int intersect_RayTriangle( const gtl::Rayf& R, 
					   const gtl::Vec3f& V0, const gtl::Vec3f& V1, const gtl::Vec3f& V2,
					   gtl::Vec3f& I );

/*!	triVert0，triVert1，triVert2三角形三顶点
	planeVert0，planeVert1，planeVert2为平面上三点
*/
bool intersect_PlaneTriangle(const gtl::Vec3f& triVert0, 
							const gtl::Vec3f& triVert1, 
							const gtl::Vec3f& triVert2,
							const gtl::Vec3f& planeVert0,
							const gtl::Vec3f& planeVert1,
							const gtl::Vec3f& planeVert2,
							LineSegment& interLine);
//########################################################################
//	2/24/2010 RYF
//	根据鼠标位置求得选取三角形的射线
//########################################################################
gtl::Rayf GetSelectRay(int mouse_x, int mouse_y);

void DrawTris();

void init();
void display();
void motion(int x, int y);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();

	return 0;
}


void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glLoadIdentity ();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt (0.0, 0.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(g_fAngle[0], 0.0, 1.0, 0.0);
	glRotatef(g_fAngle[1], 1.0, .0, 0.0);
	glRotatef(g_fAngle[2], 0.0, .0, 1.0);
	
	// 显示鼠标点在三角形上面的点
	glPushMatrix();
	{
		glColor3f(1.0f, 0.0f, 0.0f);

		glBegin(GL_LINES);
		glVertex3fv( intersecVert.getValue() );
		glVertex3fv( far_point.getValue() );
		glEnd();

		cout << "----------------------------------" << endl;
		cout << "near_point" << "\t" << near_point << endl;
		cout << "intersectVert" << "\t" << intersecVert << endl;
		cout << "far_point" << "\t" << far_point << endl;
		cout << "----------------------------------" << endl;
	} glPopMatrix();

	//求平面和三角形的交线
	intersect_PlaneTriangle( vert0, vert1, vert2,
		cutplaneVert0, cutplaneVert1, cutplaneVert2,
		intersect_line);

	//显示三角形与平面相交的线段
	glPushMatrix();
	{
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex3fv( intersect_line.v0.getValue() );
		glVertex3fv( intersect_line.v1.getValue() );
		glEnd();
	} glPopMatrix();

	//显示切割平面
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3fv( cutplaneVert0.getValue() );
	glVertex3fv( cutplaneVert1.getValue() );
	glVertex3fv( cutplaneVert2.getValue() );
	glVertex3fv( intersect_line.v0.getValue() );
	glVertex3fv( intersect_line.v1.getValue() );
	glEnd();	

	//显示三角形
	DrawTris();

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	gluPerspective(60,1.0,1.5,20);
	//gluOrtho2D( 0, w, 0, h );
	glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			exit(0);
			break;
		case 'w':
			g_fAngle[0] += 5.0;
			break;
		case 's':
			g_fAngle[0] -= 5.0;
			break;
		case 'q':
			g_fAngle[1] += 5.0;
			break;
		case 'e':
			g_fAngle[1] -= 5.0;
			break;
		case 'x':
			g_fAngle[2] += 5.0;
			break;
		case 'd':
			g_fAngle[2] -= 5.0;
			break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button==GLUT_LEFT && state==GLUT_DOWN)
	{
		y = 500 - y;

		gtl::Rayf selectRay( GetSelectRay(x, y) );
		//int isIntertsec = intersect_RayTriangle( selectRay, vert0, vert1, vert2, intersecVert);
		int isIntertsec = selectRay.intersect(vert0, vert1, vert2, intersecVert);
	}
}

void motion(int x, int y)
{
}

int intersect_RayTriangle( const gtl::Rayf& R, 
						  const gtl::Vec3f& V0, const gtl::Vec3f& V1, const gtl::Vec3f& V2,
						  gtl::Vec3f& I )
{
	gtl::Vec3f    u, v, n;             // triangle vectors
	gtl::Vec3f    dir, w0, w;          // ray vectors
	float     r, a, b;             // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = V1 - V0;
	v = V2 - V0;
	n = u.cross(v);

	if (n == gtl::Vec3f(0.0f, 0.0f, 0.0f))            // triangle is degenerate
		return -1;                 // do not deal with this case

	dir = R.getDirection();             // ray direction vector
	w0 = R.getOrigin() - V0;

	a = -( n.dot(w0) );
	// a = -dot(n,w0);
	b = n.dot(dir);
	//b = dot(n,dir);
	if (fabs(b) < SMALL_NUM) {     // ray is parallel to triangle plane
		if (a == 0)                // ray lies in triangle plane
			return 2;
		else return 0;             // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                   // ray goes away from triangle
		return 0;                  // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	I = R.getOrigin() + r * dir;           // intersect point of ray and plane

	//// is I inside T?
	float    uu, uv, vv, wu, wv, D;

	uu = u.dot(u);
	uv = u.dot(v);
	vv = v.dot(v);
	w = I - V0;
	wu = w.dot(u);
	wv = w.dot(v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)        // I is outside T
		return 0;
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	return 1;                      // I is in T
}

gtl::Rayf GetSelectRay(int mouse_x, int mouse_y)
{
	// 获取 Model-View、Projection 矩阵 & 获取Viewport视区
	GLdouble    modelview[16];
	GLdouble    projection[16];
	GLint       viewport[4];
	glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev (GL_PROJECTION_MATRIX, projection);
	glGetIntegerv (GL_VIEWPORT, viewport);

	GLdouble world_x, world_y, world_z;  

	// 获取近裁剪面上的交点
	gluUnProject( (GLdouble) mouse_x, (GLdouble) mouse_y, 0.0, 
		modelview, projection, viewport, 
		&world_x, &world_y, &world_z); 

	near_point.setValue(world_x, world_y, world_z);

	// 获取远裁剪面上的交点
	gluUnProject( (GLdouble) mouse_x, (GLdouble) mouse_y, 1.0, 
		modelview, projection, viewport, 
		&world_x, &world_y, &world_z); 

	far_point.setValue(world_x, world_y, world_z);

	gtl::Vec3f ray_dir = far_point - near_point;

	return gtl::Rayf(near_point, ray_dir);
}

bool intersect_PlaneTriangle(const gtl::Vec3f& triVert0, 
							const gtl::Vec3f& triVert1, 
							const gtl::Vec3f& triVert2,
							const gtl::Vec3f& planeVert0,
							const gtl::Vec3f& planeVert1,
							const gtl::Vec3f& planeVert2,
							LineSegment& interLine)
{
	gtl::Vec3f edge1(planeVert1 - planeVert0);
	gtl::Vec3f edge2(planeVert2 - planeVert1);
	gtl::Vec3f planeNorm( edge1.cross( edge2 ) );

	float dot0, dot1, dot2;
	dot0 = planeNorm.dot( triVert0 - planeVert0 );
	dot1 = planeNorm.dot( triVert1 - planeVert0 );
	dot2 = planeNorm.dot( triVert2 - planeVert0 );

	if ( fabs(dot0) <= SMALL_NUM )	//gtl.hpp
		dot0 = 0.0f;
	if ( fabs(dot1) <= SMALL_NUM )	//gtl.hpp
		dot1 = 0.0f;
	if ( fabs(dot2) <= SMALL_NUM )	//gtl.hpp
		dot2 = 0.0f;

	float d0d1 = dot0 * dot1;
	float d0d2 = dot0 * dot2;

	if (d0d1 > 0.0 && d0d2 > 0.0)
	{
		// all points above plane
		return false;
	}/*
	 else if ( d0d1 < 0.0 && d0d2 < 0.0)
		{
			// all points below plane
			return false;
		}*/
	
	if (fabs(dot0) + fabs(dot1) + fabs(dot2) == 0.0)
	{
		//coplanar case
		return true; 
	}

	//////////////////////////////////////////////////////////////////////////
	//common case
	//0. 1 同侧 且与2异侧
	if (dot0 > 0 && dot1 > 0 && dot2 < 0 || 
		dot0 < 0 && dot1 < 0 && dot2 > 0)
	{
		gtl::Rayf ray1( triVert0, triVert2 - triVert0);
		gtl::Rayf ray2( triVert1, triVert2 - triVert1);

		ray1.intersect( planeVert0, planeVert1, planeVert2, interLine.v0 );
		ray2.intersect( planeVert0, planeVert1, planeVert2, interLine.v1 );

		return true;
	}
	
	//0, 2同侧 且与1异侧
	if (dot0 > 0 && dot2 > 0 && dot1 < 0 || 
		dot0 < 0 && dot2 < 0 && dot1 > 0)
	{
		gtl::Rayf ray1( triVert0, triVert1 - triVert0);
		gtl::Rayf ray2( triVert2, triVert1 - triVert2);

		ray1.intersect( planeVert0, planeVert1, planeVert2, interLine.v0 );
		ray2.intersect( planeVert0, planeVert1, planeVert2, interLine.v1 );

		return true;
	}

	//1，2同侧 且与0异侧
	if (dot2 > 0 && dot1 > 0 && dot0 < 0 || 
		dot2 < 0 && dot1 < 0 && dot0 > 0)
	{
		gtl::Rayf ray1( triVert1, triVert0 - triVert1);
		gtl::Rayf ray2( triVert2, triVert0 - triVert2);

		ray1.intersect( planeVert0, planeVert1, planeVert2, interLine.v0 );
		ray2.intersect( planeVert0, planeVert1, planeVert2, interLine.v1 );

		return true;
	}

	return false;
}

void DrawTris()
{
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3fv( vert0.getValue() );
	glVertex3fv( vert1.getValue() );
	glVertex3fv( vert2.getValue() );
	glEnd();
}