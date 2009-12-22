#include "Figure.h"
#include "Helper.h"
#include <string>
using namespace std;

#pragma comment(lib, "tinyxml.lib")

float	angle = 90.0f;
float	windowWidth = 1024;
float	windowHeight = 768;
int		font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
int		posX = 20;
int		posY = 50;
int		offset = 30;

static char *FontList [] =
{
	"The Multiway  Hierarchy Tree Tutorial", 
	"r/R - rotate the tree",
	"v/V - in the pre-order visit the tree node",
	"p/P - in the post-order visit the tree node",
	"a/A - automatically visit the tree int preorder",
	"Esc q/Q - Quit the demo",
	"Create Time - 4/10/2009",
	"Author - RenYaFei",
	"QQ - 546952720",
	"Email - dizuo@126.com"
};

//Figure	figure("script.txt");
Figure	figure("tree.xml");
Figure::preIter preVisitIter;
Figure::postIter postVisitIter;

void init();
void display();
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);

void setOrthographicProjection();	//正交投影绘制font
void resetPerspectiveProjection();	//绘制场景使用透视投影

//void RenderJoint(Figure::preIter& node)
void RenderJoint(Joint some)
{
	glPushMatrix();
	{
		//Render the joint
		glPushMatrix();
		{
			glColor3f(1.0, 1.0, 1.0);
			Helper::renderBitmapString3D(some.offset[0], some.offset[1], some.offset[2], (void*)font, some.name);
			glColor3f(1.0, 0.0, 0.0);
			glTranslatef(some.offset[0], some.offset[1], some.offset[2]);
			some.render();
		} glPopMatrix();
	}glPopMatrix();
}

void VisitJoint(Joint some)
{
	::Sleep(100);
	some.active();
}

void PrintJointPos(Joint some)
{
	cout << some.name << "\t\t" << some.offset[0] << "\t" << some.offset[1] << "\t" << some.offset[2] << endl;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (windowWidth, windowHeight); 
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
	glClearColor(84/255.0, 25.0/255.0, 125.0/255.0, 0.0);
	glShadeModel (GL_FLAT);

	//figure.recursBuildByIterator();
	figure.loadXMLScript();
	figure.creatFigure();
	figure.preTraverse(PrintJointPos);

	preVisitIter = figure.begin();	//初始化迭代器
	postVisitIter = figure.begin_post();
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glLoadIdentity ();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt (20.0, 2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	{
		glRotatef(angle, 0.0, 1.0, 0.0);
		figure.preTraverse( RenderJoint );	//绘制树节点
		figure.renderBranch();				//绘制树枝

	}glPopMatrix();

	setOrthographicProjection();
	glPushMatrix();
	{
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glLoadIdentity();
		for (size_t i(0); i<10; i++)
		{
			Helper::renderBitmapString2D(posX, posY+offset*i, (void *)font, FontList[i]); 
		}
	}glPopMatrix();
	resetPerspectiveProjection();

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	//glOrtho(-windowWidth, windowWidth, -windowHeight, windowHeight, -50.0, 50.0);
	gluPerspective(80, (float)(windowWidth)/(float)(windowHeight), 1.5, 500);
	glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	Figure::preIter preIt;

	switch (key) {
		case 27:
		case 'q':
		case 'Q':
			exit(0);
			break;
		case 'r':
		case'R':
			angle += 2.0;
			break;

		case 'a':
		case 'A':
			for ( preIt = figure.begin(); 
				preIt != figure.end(); 
				++preIt )
			{
				(*preIt).active();
				::Sleep(500);
				//glutPostRedisplay();
				display();
			}
			figure.resetJoints();
			break;

		case 'v':
		case 'V':
			//逐个节点激活.
			if( preVisitIter!= figure.end() )
			{
				//visitIter.get()->active();
				(*preVisitIter).active();
				++preVisitIter;
			}
			else
			{
				figure.resetJoints();
				preVisitIter = figure.begin();
			}
			break;
		case 'p':
		case 'P':
			if( postVisitIter!= figure.end_post() )
			{
				//visitIter.get()->active();
				(*postVisitIter).active();
				++postVisitIter;
			}
			else
			{
				figure.resetJoints();
				postVisitIter = figure.begin_post();
			}
			break;
	}
	glutPostRedisplay();
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, windowWidth, 0, windowHeight);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -windowHeight, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}
