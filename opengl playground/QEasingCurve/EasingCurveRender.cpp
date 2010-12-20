#include <vector>
#include <GL/glut.h>
#include <cmath>
#include <gl/glaux.h>
#include "QEasingCurve.h"

using namespace std;

GLfloat	g_fAngle[3] = {0.0};
int		g_iAreaSizeX = 64;
int		g_iAreaSizeY = 64;

class Painter
{
public:
	Painter()
		: mPreX(0)
		, mPreY(0)
		, mBegin(0)
	{}
	void drawLine(qreal bgX, qreal bgY, qreal edX, qreal edY)
	{
		glPushMatrix();
		{
			glBegin(GL_LINES);
			glVertex3f(bgX, bgY, 0); glVertex3f(edX, edY, 0);
			glEnd();
		} glPopMatrix();
	}
	
	void moveTo(qreal x, qreal y)
	{
		mBegin = true;
		mPreX = x; mPreY = y;
	}

	void lineTo(qreal x, qreal y)
	{
		if (!mBegin)
			return;
		drawLine(mPreX, mPreY, x, y);
		mPreX = x; mPreY = y;
	}
private:
	bool  	mBegin;
	qreal	mPreX;
	qreal	mPreY;
};

struct CurveControl
{
	enum { 
		Range = 41
	};
	CurveControl()
	{
		mCurveType = 0;
		mCurve = new QEasingCurve( (QEasingCurve::Type)mCurveType );
	}

	void setRegion(int l, int b, int w, int h)
	{
		mLeft = l;
		mBottom = b;
		mWidth = w;
		mHeight = h;
	}

	void renderCurve()
	{
		glColor3f(0.5f, 0.5f, 0.5f);
		glRectd(mLeft, mBottom, mLeft+mWidth, mBottom+mHeight);

		glLineWidth(3.0f);
		glColor3f(1.0f, 1.0f, 0.0f);
		Painter painter;

		//qreal xAxis = mHeight/1.5;
		//qreal yAxis = mWidth/3;
		qreal xAxis = mBottom;
		qreal yAxis = mLeft;

		painter.drawLine(mLeft, xAxis, mLeft+mWidth,  xAxis);
		painter.drawLine(yAxis, mBottom, yAxis, mBottom+mHeight);

		qreal curveScale = mHeight*0.8;

		painter.moveTo(yAxis, xAxis - curveScale * mCurve->valueForProgress(0));
		for (qreal t = 0; t <= 1.0; t+=1.0/curveScale) {
			painter.lineTo(yAxis + curveScale * t, xAxis + curveScale * mCurve->valueForProgress(t));
		}
	}
	//MAX: QEasingCurve::NCurveTypes - 1
	//MIN: 0
	void decreTypeIdx()
	{
		mCurveType = (mCurveType-1 + Range) % Range;

		update();
	}
	void increTypeIdx()
	{
		mCurveType = (mCurveType+1) % Range;
		update();
	}

	void update()
	{
		mCurve->setType( (QEasingCurve::Type)mCurveType );
	}

	qreal	mLeft;
	qreal	mBottom;
	qreal	mWidth;
	qreal	mHeight;

	int		mCurveType;
	QEasingCurve* mCurve;
};

CurveControl g_CurveControl;

void init();
void display();
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
	glutMainLoop();

	return 0;
}

void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);

	g_CurveControl.setRegion(0, 0, g_iAreaSizeX, g_iAreaSizeY);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	glLoadIdentity ();             /* clear the matrix */
	/* viewing transformation  */
	gluLookAt (0.0, 0.0, 120.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(g_fAngle[0], 0.0, 1.0, 0.0);
	glRotatef(g_fAngle[1], 1.0, .0, 0.0);
	glRotatef(g_fAngle[2], 0.0, .0, 1.0);
	
	//Render Scene~
	//Painter painter;
	//painter.drawLine(0, 0, 64, 64);
	//glutSolidCube(1.0f);
	CurveControl leftTopRegion;
	leftTopRegion.setRegion(-g_iAreaSizeX, 0, g_iAreaSizeX, g_iAreaSizeY);
	leftTopRegion.increTypeIdx();
	leftTopRegion.renderCurve();

	CurveControl leftBottomRegion;
	leftBottomRegion.setRegion(-g_iAreaSizeX, -g_iAreaSizeY, g_iAreaSizeX, g_iAreaSizeY);
	leftBottomRegion.increTypeIdx();
	leftBottomRegion.increTypeIdx();
	leftBottomRegion.renderCurve();

	CurveControl rightBottomRegion;
	rightBottomRegion.setRegion(0, -g_iAreaSizeY, g_iAreaSizeX, g_iAreaSizeY);
	rightBottomRegion.increTypeIdx();
	rightBottomRegion.increTypeIdx();
	rightBottomRegion.increTypeIdx();
	rightBottomRegion.renderCurve();

	g_CurveControl.renderCurve();

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	gluPerspective(60,1.0,1.5,500);
	//gluOrtho2D( 0, w, 0, h );
	glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			exit(0);
			break;
		case 'i':
			g_CurveControl.increTypeIdx();
			printf("%d\n", g_CurveControl.mCurveType);
			break;
		case 'd':
			g_CurveControl.decreTypeIdx();
			printf("%d\n", g_CurveControl.mCurveType);			
			break;
	}
	glutPostRedisplay();
}
