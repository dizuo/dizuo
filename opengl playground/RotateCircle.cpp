#include <vector>
#include <GL/glut.h>
#include <cmath>

#include "gtl/quat.hpp"
#include "gtl/vec3.hpp"
#include "gtl/plane.hpp"
#include "gtl/matrix3.hpp"
#include "gtl/matrix4.hpp"

#include <gl/glaux.h>

using namespace std;

#define M_PI 3.1415926
#define	checkImageWidth 256
#define	checkImageHeight 256
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;
GLfloat	g_fAngle[3] = {0.0};
//////////////////////////////////////////////////////////////////////////
// 效果预期：实现在circle的局部坐标系下将circle绕任意轴(axis)转任意角度(angle)

typedef float				Real;
typedef gtl::Vec3f			RVec3;
typedef vector<gtl::Vec3f>	RVec3Array;

class Circle
{
public:
	Circle(float cx, float cy, float cz, float r, float seg)
	{
		center.setValue(cx, cy, cz);
		radius = r;
		numSegment = seg;
	}
	
	void setup()
	{
		for (int i=0; i<=numSegment; i++)
		{
			float cx = center.x() + radius * cos( (2*M_PI*i)/numSegment );
			//float cy = center.y() + radius * sin( (2*M_PI*i)/numSegment );
			float cz = center.z() + radius * sin( (2*M_PI*i)/numSegment );
			profile.push_back( gtl::Vec3f(cx, center.y(), cz) );
		}
	}

	void render()
	{
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<profile.size(); i++)
		{
			glVertex3fv( profile[i].getValue() );
		}
		glEnd();
	}

	const RVec3Array& getProfile() const { return profile; }
private:
	gtl::Vec3f	center;
	float		radius;
	int			numSegment;
	RVec3Array	profile;
};


Circle g_circle(0.0f, 0.0f, 0.0f, 4.0, 30);

void makeCheckImage();

// 通过两个向量的坐标构建一个cylinder
void RenderBone( float x0, float y0, float z0, float x1, float y1, float z1 );
void init();
void display();
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;								// File Handle
	if (!Filename)									// Make Sure A Filename Was Given
	{
		return NULL;							// If Not Return NULL
	}
	File=fopen(Filename,"r");						// Check To See If The File Exists
	if (File)										// Does The File Exist?
	{
		fclose(File);								// Close The Handle
		return auxDIBImageLoadA(Filename);			// Load The Bitmap And Return A Pointer
	}
	return NULL;									// If Load Failed Return NULL
}

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
	int Status=FALSE;                               // Status Indicator
	AUX_RGBImageRec *TextureImage;               // Create Storage Space For The Textures


	if ( (TextureImage=LoadBMP("NeHe.bmp")) )
	{
		Status=TRUE;                            // Set The Status To TRUE
		glGenTextures(1, &texName);          // Create Five Textures

		glBindTexture(GL_TEXTURE_2D, texName);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
	}

	return Status;                                  // Return The Status
}

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
	glutMainLoop();

	return 0;
}

void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);

	g_circle.setup();

	LoadGLTextures();
}

//########################################################################
//使用mat变换srcShape的顶点结果存储在dstCurve
void _ProcessCurve(const RVec3Array &srcCurve, 
						  const gtl::Matrix4f& mat,
						  RVec3Array& dstCurve)
{
	for (int i=0; i<srcCurve.size(); i++)
	{
		RVec3 newVec;
		mat.multVecMatrix(srcCurve[i], newVec);
		dstCurve.push_back(newVec);
	}

	// normal
	// gravCenter
}

void _GetShapeMatrix(const RVec3& from, const RVec3& to, const RVec3& newPos,
							gtl::Matrix4f& finalMat)
{
	RVec3 rot_to = to;

	gtl::Quatf	rotQuat(from, rot_to);
	gtl::Matrix4f rotMat = rotQuat.getMatrix();

	gtl::Matrix4f tranMat;
	tranMat.setTranslate( newPos );

	finalMat = rotMat * tranMat;
}

void _ComputePlaneEquation(const RVec3 &leftPnt, const RVec3 &rightPnt, //input
								  const RVec3 &centerPnt,	//input
								  const RVec3 &pathNorm,	//input
								  RVec3 &planeNorm)			//output
{
	RVec3 ray1 = leftPnt - centerPnt;	ray1.normalize();
	RVec3 ray2 = rightPnt - centerPnt;	ray2.normalize();

	RVec3 ray = ray1 + ray2;			ray.normalize();

	planeNorm = ray.cross(pathNorm);	planeNorm.normalize();
}

//////////////////////////////////////////////////////////////////////////
// 计算射线和平面的交点
// rayDir & planeNorm 必须为单位向量
//////////////////////////////////////////////////////////////////////////
void _GetRayAndPlaneIntersetPnt(const RVec3& rayOrig, const RVec3& rayDir, 
									   const RVec3& planeNorm, const RVec3& planePnt, 
									   RVec3& point)
{
	Real v1 = rayOrig.dot(planeNorm);
	Real v2 = rayDir.dot(planeNorm);
	Real v3 = planePnt.dot(planeNorm);

	if (!v2)
		return;

	Real tFactor = (v3 - v1) / v2;
	point = rayOrig + tFactor*rayDir;
}


//########################################################################
void display(void)
{
	//////////////////////////////////////////////////////////////////////////	
	gtl::Matrix4f tranMat0;
	tranMat0.setTranslate( RVec3(-0.5f, -0.5f, 0.0f) );

	//////////////////////////////////////////////////////////////////////////
	RVec3Array shape;
	shape.push_back( RVec3(0.0f, 0.0f, 0.0f) );
	shape.push_back( RVec3(1.0f, 0.0f, 0.0f) );
	shape.push_back( RVec3(1.0f, 1.0f, 0.0f) );
	shape.push_back( RVec3(0.0f, 1.0f, 0.0f) );

	RVec3Array path = g_circle.getProfile();

	int pathSize = path.size();
	RVec3Array* pLoftShape = new RVec3Array[ pathSize+1 ];
	_ProcessCurve(shape, tranMat0, pLoftShape[0]);
	//////////////////////////////////////////////////////////////////////////
	RVec3 path1 = path[1] - path[0];
	RVec3 path2 = path[2] - path[1];
	RVec3 pathNorm = path1.cross(path2);
	pathNorm.normalize();

	RVec3 shape1 = shape[1] - shape[0];
	RVec3 shape2 = shape[2] - shape[1];
	RVec3 shapeNorm = shape1.cross(shape2);
	shapeNorm.normalize();
	//////////////////////////////////////////////////////////////////////////
	gtl::Matrix4f matrix;
 	for (int p=0; p<pathSize; p++)
 	{
 		// RVec3 are all row vectors...
 		RVec3 rot_from;
 		RVec3 rot_to;
 		gtl::Matrix4f matrix;
 		if (p==0)
 		{
 			_GetShapeMatrix(shapeNorm, path[1]-path[0], path[0], matrix);	
			_ProcessCurve(pLoftShape[0], matrix, pLoftShape[p+1]);
 		}
		else if (p<pathSize-1)
		{
			RVec3 cutPlaneNorm;
			_ComputePlaneEquation(path[p-1], path[p+1], path[p], pathNorm, cutPlaneNorm);
			//_GetShapeMatrix(shapeNorm, cutPlaneNorm, path[p], matrix);

			//_ProcessCurve(pLoftShape[0], matrix, pLoftShape[p+1]);

			//逐个求点
			RVec3 rayDir(path[p]-path[p-1]);
			rayDir.normalize();

			for (int i=0; i<shape.size(); i++)
			{
				RVec3 rayOrig(pLoftShape[p][i]);
				RVec3 temVec;
				_GetRayAndPlaneIntersetPnt(rayOrig, rayDir, cutPlaneNorm, path[p], temVec);
				
				pLoftShape[p+1].push_back(temVec);
			}
		}
		else	//last section.
		{
 			_GetShapeMatrix(shapeNorm, path[p]-path[p-1], path[p], matrix);	
 			_ProcessCurve(pLoftShape[0], matrix, pLoftShape[p+1]);
		}

		//_ProcessCurve(pLoftShape[0], matrix, pLoftShape[p+1]);
 	}

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//#########################################################################
	//_GetShapeMatrix(shapeNorm, path1, path[0], matrix);	
	//_ProcessCurve(pLoftShape[0], matrix, pLoftShape[1]);

	//RVec3 cutPlaneNorm;
	//_ComputePlaneEquation(path[0], path[2], path[1], pathNorm, cutPlaneNorm);
	//_GetShapeMatrix(shapeNorm, cutPlaneNorm, path[1], matrix);
	//_ProcessCurve(pLoftShape[0], matrix, pLoftShape[2]);

	//_GetShapeMatrix(shapeNorm, path2, path[2], matrix);	
	//_ProcessCurve(pLoftShape[0], matrix, pLoftShape[3]);
	//#########################################################################
	//////////////////////////////////////////////////////////////////////////
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glLoadIdentity ();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt (8.0, 2.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glRotatef(g_fAngle[1], g_fAngle[1], g_fAngle[2], 0.0f);
	
	{	//draw axis
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(50.0, 0.0, 0.0);
		glEnd();

		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 50.0, 0.0);
		glEnd();

		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 50.0);
		glEnd();
	}
	
	//pLoftShape[0]为原点处的shape
	//pLoftShape[1] - pLoftShape[3]为放样体path每个节点的shape
	for (int p=1; p<pathSize+1; p++)
	{
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<pLoftShape[ p ].size(); i++)
		{
			glVertex3fv(pLoftShape[ p ][i].getValue());
		} glVertex3fv(pLoftShape[ p ][0].getValue());
		glEnd();
	}

	// Draw path
	glBegin(GL_LINE_STRIP);
	for (int i=0; i<path.size(); i++)
	{
		glVertex3fv(path[i].getValue());
	} 
	glEnd();

	// Draw path normal
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3fv(pathNorm.getValue());
	glEnd();
	//g_circle.render();

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
