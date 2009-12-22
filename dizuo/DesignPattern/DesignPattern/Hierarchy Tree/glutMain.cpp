#include "Figure.h"
#include <string>
using namespace std;

float	angle = 0;
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
	"l/L - in the level-order visit the tree node",
	"Esc - Quit the demo",
	"Create Time - 4/10/2009",
	"Author - RenYaFei",
	"QQ - 546952720",
	"Email - dizuo@126.com"
};
Figure	figure("Hierarchy Tree\\script.txt");
Figure::iterator visitIter;

void init();
void display();
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);

void setOrthographicProjection();	//正交投影绘制font
void resetPerspectiveProjection();	//绘制场景使用透视投影
void renderBitmapString2D(float x, float y, void *font, char *string);
void renderBitmapString3D(float x, float y, float z, void* font, std::string str);

void RenderJoint(Joint* some)
{
	glPushMatrix();
	{
		//Render the joint
		glPushMatrix();
		{
			glColor3f(1.0, 1.0, 1.0);
			renderBitmapString3D(some->offset[0], some->offset[1], some->offset[2], (void*)font, some->getData());
			glColor3f(1.0, 0.0, 0.0);
			glTranslatef(some->offset[0], some->offset[1], some->offset[2]);
			some->render();
		} glPopMatrix();

		glBegin(GL_LINES);
		for (size_t i(0); i<some->getSize(); i++)
		{
			glVertex3f( some->offset[0], some->offset[1], some->offset[2] );
			glVertex3f( some->getChild(i)->offset[0], some->getChild(i)->offset[1], some->getChild(i)->offset[2] );
		}
		glEnd();
	}glPopMatrix();
}

void VisitJoint(Joint* some)
{
	::Sleep(100);
	some->active();
}

void PrintJointPos(Joint* some)
{
	cout << some->getData() << "\t\t" << some->offset[0] << "\t" << some->offset[1] << "\t" << some->offset[2] << endl;
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

	figure.recursRead();
	figure.preRecursPrint( figure.root, 1);
	figure.buildFigure(figure.root, 0);
	figure.preTraverse(figure.root, PrintJointPos);

	visitIter = figure.begin();	//初始化迭代器
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
		figure.preTraverse( figure.root, RenderJoint );
	}glPopMatrix();

	setOrthographicProjection();
	glPushMatrix();
	{
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glLoadIdentity();
		for (size_t i(0); i<10; i++)
		{
			renderBitmapString2D(posX, posY+offset*i, (void *)font, FontList[i]); 
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
	switch (key) {
		case 27:
			exit(0);
			break;
		case 'r':
		case'R':
			angle += 2.0;
			break;

		case 'v':
		case 'V':
			//逐个节点激活.
			if( visitIter!= figure.end() )
			{
				//visitIter.get()->active();
				visitIter.node->active();
				++visitIter;
			}
			else
			{
				figure.resetJoints();
				visitIter = figure.begin();
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

void renderBitmapString2D(float x, float y, void *font, char *string)
{
	char *c;
	// set position to start drawing fonts
	glRasterPos2f(x, y);
	// loop all the characters in the string
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void renderBitmapString3D(float x, float y, float z, void* font, std::string str)
{
	// set position to start drawing fonts
	glRasterPos3f(x, y, z);

	for (size_t i(0); i<str.size(); i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}