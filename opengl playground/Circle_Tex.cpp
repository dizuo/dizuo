#include <vector>
#include <GL/glut.h>
#include <cmath>
#include "gtl/quat.hpp"
#include <gl/glaux.h>

using namespace std;

#define M_PI 3.1415926
#define	checkImageWidth 256
#define	checkImageHeight 256
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;
GLfloat	g_fAngle[3] = {0.0};
GLUquadric	*g_pObj;	

void makeCheckImage();
void DrawCircleArea(float cx, float cy, float cz, float r, int num_segments);
void DrawCircle1(float cx, float cy, float r, int num_segments);
void DrawCircle2(float cx, float cy, float r, int num_segments);
void mySolidCylinder( GLUquadric*	quad, GLdouble base, GLdouble top,
					 GLdouble height, GLint slices, GLint stacks );
void mySolidPyramid (GLdouble base_height, GLdouble size, GLdouble top_height);

GLvoid draw_circle(const GLfloat radius,const GLuint num_vertex);

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

	gluDeleteQuadric(g_pObj);
	return 0;
}

int fakeSphere(int i, int j)
{
	float s = (float)i / checkImageWidth;
	float t = (float)j / checkImageHeight;

	float r = sqrt( (s-0.5)*(s-0.5) + (t-0.5)*(t-0.5) );
	if( r<0.5 )
		return (int)( (1-r/0.5)*255 );

	return (int)(0.2*255);
}

void makeCheckImage(void)
{
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			//c = ((((i&0x8)==0)^((j&0x8))==0))*255;
			c = fakeSphere(i, j);
			checkImage[i][j][0] = (GLubyte) c;
			checkImage[i][j][1] = (GLubyte) c;
			checkImage[i][j][2] = (GLubyte) c;
			checkImage[i][j][3] = (GLubyte) 255;
		}
	}
}

void init(void) 
{
	glClearColor (1.0, 1.0, 1.0, 0.0);
	glShadeModel (GL_FLAT);

	LoadGLTextures();
	//makeCheckImage();
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//#ifdef GL_VERSION_1_1
//	glGenTextures(1, &texName);
//	glBindTexture(GL_TEXTURE_2D, texName);
//#endif
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//#ifdef GL_VERSION_1_1
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 
//		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
//#else
//	glTexImage2D(GL_TEXTURE_2D, 0, 4, checkImageWidth, checkImageHeight, 
//		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
//#endif
	g_pObj = gluNewQuadric();
}

void DrawCircle1(float cx, float cy, float r, int num_segments) 
{ 
	float theta = 2 * M_PI / float(num_segments); 
	float tangetial_factor = tanf(theta);//calculate the tangential factor 
	float radial_factor = cosf(theta);//calculate the radial factor 
	float x = r;//we start at angle = 0 
	float y = 0; 

	glBegin(GL_LINE_LOOP); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 
		float tx = -y; 
		float ty = x; 
		//add the tangential vector 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
		//correct using the radial factor 
		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}

void DrawCircle2(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_STRIP);
	for (int i=0; i<=num_segments; i++)
	{
		glVertex2f( cx+r*cos( (2*M_PI*i)/num_segments ), 
					cy+r*sin( (2*M_PI*i)/num_segments ) );
	}
	glEnd();
}

/*
void gluCylinder( GLUquadric*	quad,
				 GLdouble base,
				 GLdouble top,
				 GLdouble height,
				 GLint slices,
				 GLint stacks )

*/

void mySolidCylinder( GLUquadric*	quad,
					 GLdouble base,
					 GLdouble top,
					 GLdouble height,
					 GLint slices,
					 GLint stacks )
{
	glColor3f(84.0/255, 0.0, 125.0/255.0);
	gluCylinder(quad, base, top, height, slices, stacks);
	//top
	DrawCircleArea(0.0, 0.0, height, top, slices);
	//base
	DrawCircleArea(0.0, 0.0, 0.0, base, slices);
}

// [4/4/2009 RenYaFei]
// base_height为底部的高度
// top_height为顶部的高度
// size为中部正方形的边长
void mySolidPyramid(GLdouble base_height, GLdouble size, GLdouble top_height)
{
	glPushMatrix();
	{
		//top
		glBegin(GL_TRIANGLE_FAN);     	// draw triangle
			glColor3f(1.0f,0.0f,0.0f);                	// set color to red
			glVertex3f(  0.0f,  top_height, 0.0f);
			glColor3f(0.0f,1.0f,0.0f);                	// set color to green
			glVertex3f(-size, 0.0f, size);
			glColor3f(1.0f,1.0f,0.0f);               	// set color to yellow
			glVertex3f( size, 0.0f, size);
			glColor3f(0.0f,0.0f,1.0f);                	// set color to blue
			glVertex3f( size, 0.0f, -size);
			glColor3f(1.0f,1.0f,1.0f);                	// set color to white
			glVertex3f( -size, 0.0f, -size);
			glColor3f(0.0f,1.0f,0.0f);                	// set color to green
			glVertex3f(-size, 0.0f, size);
		glEnd();
		glBegin(GL_QUADS);                           	// draw square
			glColor3f(0.0f,1.0f,0.0f);                	// set color to green
			glVertex3f(-size, 0.0f, size);
			glColor3f(1.0f,1.0f,1.0f);                	// set color to white
			glVertex3f( -size, 0.0f, -size);
			glColor3f(0.0f,0.0f,1.0f);                	// set color to blue
			glVertex3f( size, 0.0f, -size);
			glColor3f(1.0f,1.0f,0.0f);                	// set color to yellow
			glVertex3f( size, 0.0f, size);
		glEnd();
		// base
		glBegin(GL_TRIANGLE_FAN);     	// draw triangle
		glColor3f(1.0f,0.0f,0.0f);                	// set color to red
		glVertex3f(  0.0f,  -base_height, 0.0f);
		glColor3f(0.0f,1.0f,0.0f);                	// set color to green
		glVertex3f(-size, 0.0f, size);
		glColor3f(1.0f,1.0f,0.0f);               	// set color to yellow
		glVertex3f( size, 0.0f, size);
		glColor3f(0.0f,0.0f,1.0f);                	// set color to blue
		glVertex3f( size, 0.0f, -size);
		glColor3f(1.0f,1.0f,1.0f);                	// set color to white
		glVertex3f( -size, 0.0f, -size);
		glColor3f(0.0f,1.0f,0.0f);                	// set color to green
		glVertex3f(-size, 0.0f, size);
		glEnd();
	} glPopMatrix();
}

GLvoid DrawCircleArea(float cx, float cy, float cz, float r, int num_segments)
{
	GLfloat vertex[4]; 

	const GLfloat delta_angle = 2.0*M_PI/num_segments;
	glBegin(GL_TRIANGLE_FAN);

	vertex[0] = cx;
	vertex[1] = cy;
	vertex[2] = cz;
	vertex[3] = 1.0;        
	glVertex4fv(vertex);

	//draw the vertex on the contour of the circle
	for(int i = 0; i < num_segments ; i++)
	{
		vertex[0] = std::cos(delta_angle*i) * r + cx;
		vertex[1] = std::sin(delta_angle*i) * r + cy;
		vertex[2] = cz;
		vertex[3] = 1.0;
		glVertex4fv(vertex);
	}

	vertex[0] = 1.0 * r + cx;
	vertex[1] = 0.0 * r + cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);
	glEnd();
}


GLvoid draw_circle(const GLfloat radius,const GLuint num_vertex)
{
	GLfloat vertex[4]; 
	GLfloat texcoord[2];

	const GLfloat delta_angle = 2.0*M_PI/num_vertex;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texName);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glBegin(GL_TRIANGLE_FAN);

	//draw the vertex at the center of the circle
	texcoord[0] = 0.5;
	texcoord[1] = 0.5;
	glTexCoord2fv(texcoord);
	vertex[0] = vertex[1] = vertex[2] = 0.0;
	vertex[3] = 1.0;        
	glVertex4fv(vertex);

	//draw the vertex on the contour of the circle
	for(int i = 0; i < num_vertex ; i++)
	{
		//纹理圆的纹理坐标空间为：直径为1的圆
		texcoord[0] = (std::cos(delta_angle*i) + 1.0)*0.5;
		texcoord[1] = (std::sin(delta_angle*i) + 1.0)*0.5;
		glTexCoord2fv(texcoord);

		vertex[0] = std::cos(delta_angle*i) * radius;
		vertex[1] = std::sin(delta_angle*i) * radius;
		vertex[2] = 0.0;
		vertex[3] = 1.0;
		glVertex4fv(vertex);
	}

	texcoord[0] = (1.0 + 1.0)*0.5;
	texcoord[1] = (0.0 + 1.0)*0.5;
	glTexCoord2fv(texcoord);

	vertex[0] = 1.0 * radius;
	vertex[1] = 0.0 * radius;
	vertex[2] = 0.0;
	vertex[3] = 1.0;
	glVertex4fv(vertex);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void RenderBone(float x0, float y0, float z0, float x1, float y1, float z1 )
{
	GLdouble  dir_x = x1 - x0;
	GLdouble  dir_y = y1 - y0;
	GLdouble  dir_z = z1 - z0;
	GLdouble  bone_length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );

	// 昤夋僷儔儊僞偺愝掕
	static GLUquadricObj *  quad_obj = NULL;
	if ( quad_obj == NULL )
		quad_obj = gluNewQuadric();
	gluQuadricDrawStyle( quad_obj, GLU_FILL );
	gluQuadricNormals( quad_obj, GLU_SMOOTH );

	glPushMatrix();

	// 暯峴堏摦傪愝掕
	glTranslated( x0, y0, z0 );

	// 埲壓丄墌拰偺夞揮傪昞偡峴楍傪寁嶼

	// 倸幉傪扨埵儀僋僩儖偵惓婯壔
	double  length;
	length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
	if ( length < 0.0001 ) { 
		dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
	}
	dir_x /= length;  dir_y /= length;  dir_z /= length;

	// 婎弨偲偡傞倷幉偺岦偒傪愝掕
	GLdouble  up_x, up_y, up_z;
	up_x = 0.0;
	up_y = 1.0;
	up_z = 0.0;

	// 倸幉偲倷幉偺奜愊偐傜倶幉偺岦偒傪寁嶼
	double  side_x, side_y, side_z;
	side_x = up_y * dir_z - up_z * dir_y;
	side_y = up_z * dir_x - up_x * dir_z;
	side_z = up_x * dir_y - up_y * dir_x;

	// 倶幉傪扨埵儀僋僩儖偵惓婯壔
	length = sqrt( side_x*side_x + side_y*side_y + side_z*side_z );
	if ( length < 0.0001 ) {
		side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
	}
	side_x /= length;  side_y /= length;  side_z /= length;

	// 倸幉偲倶幉偺奜愊偐傜倷幉偺岦偒傪寁嶼
	up_x = dir_y * side_z - dir_z * side_y;
	up_y = dir_z * side_x - dir_x * side_z;
	up_z = dir_x * side_y - dir_y * side_x;

	// 夞揮峴楍傪愝掕
	GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
		up_x,   up_y,   up_z,   0.0,
		dir_x,  dir_y,  dir_z,  0.0,
		0.0,    0.0,    0.0,    1.0 };
	glMultMatrixd( m );

	// 墌拰偺愝掕
	GLdouble radius= 0.2; // 墌拰偺懢偝
	GLdouble slices = 8.0; // 墌拰偺曻幩忬偺嵶暘悢乮僨僼僅儖僩12乯
	GLdouble stack = 3.0;  // 墌拰偺椫愗傝偺嵶暘悢乮僨僼僅儖僩侾乯

	// 墌拰傪昤夋
	gluCylinder( quad_obj, radius, radius, bone_length, slices, stack ); 

	glPopMatrix();
}


void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glLoadIdentity ();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt (0.0, 0.0, 80.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(g_fAngle[0], 0.0, 1.0, 0.0);
	glRotatef(g_fAngle[1], 1.0, .0, 0.0);
	glRotatef(g_fAngle[2], 0.0, .0, 1.0);
	
	//mySolidPyramid(1.0, 0.5, 4.0);
	//RenderBone(0, 0, 0, 1.0, 2.0, 3.0);
	//DrawCircle2(0.0, 0.0, 3.0, 2);
	//DrawCircle2(0.0, 0.0, 3.0, 3);
	//DrawCircle2(0.0, 0.0, 3.0, 4);
	//DrawCircle2(0.0, 0.0, 3.0, 5);
	draw_circle(2.0, 20);

	//DrawCircle1(0.0, 0.0, 3.0, 2);
	//DrawCircle1(0.0, 0.0, 3.0, 3);
	//DrawCircle1(0.0, 0.0, 3.0, 4);
	//DrawCircle1(0.0, 0.0, 3.0, 5);

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	//gluPerspective(60,1.0,1.5,20);
	//gluOrtho2D( 0, w, 0, h );
			
	glOrtho(-100.0, 100.0, -100.0, 100.0, -10000.0, 100);
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
