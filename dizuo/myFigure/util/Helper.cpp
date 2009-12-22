#include "Helper.h"

#include <string>
#include <fstream>
#include <iomanip>

//string to wstring
std::wstring Helper::s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs"); 
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");

	return result;
}

//wstring to string 
std::string Helper::ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);      
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

// ----------------------------------------------------------------
//	Name：			renderBitmapString2D
//	Description:	绘制二维的字体
//	Return Value:	void
// ----------------------------------------------------------------
void Helper::renderBitmapString2D( float x, float y, 
									void *font, char *string )
{
	char *c;
	// set position to start drawing fonts
	glRasterPos2f(x, y);
	// loop all the characters in the string
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

// ----------------------------------------------------------------
//	Name：			renderBitmapString3D
//	Description:	绘制三维的字体
//	Return Value:	void
// ----------------------------------------------------------------
void Helper::renderBitmapString3D( float x, float y, float z, 
									void* font, std::string str )
{
	// set position to start drawing fonts
	glRasterPos3f(x, y, z);

	for (size_t i(0); i<str.size(); i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// ----------------------------------------------------------------
//	Name：			renderRectangle
//	Description:	根据参数TRectanglef提供的参数绘制矩形
//	Return Value:	void
// ----------------------------------------------------------------
void Helper::renderRectangle( const TRectanglef& rect, 
								Color4 color, bool isEmpty )
{
	float left = rect.m_Left;
	float right = rect.m_Right;
	float bottom = rect.m_Bottom;
	float top = rect.m_Top;

	glPushMatrix();
	{	
		glColor4d( color.r(), color.g(), color.b(), color.a() );
		glLineWidth(2.0);
		if( isEmpty ) 
		{
			glBegin(GL_LINE_STRIP);
			glVertex3f(left, bottom, 0.0f);
			glVertex3f(right, bottom, 0.0f);
			glVertex3f(right, top, 0.0f);
			glVertex3f(left, top, 0.0f);
			glVertex3f(left, bottom, 0.0f);
			glEnd();
		}
		else
		{
			glBegin(GL_QUADS);
			glVertex3f(left, bottom, 0.0f);
			glVertex3f(right, bottom, 0.0f);
			glVertex3f(right, top, 0.0f);
			glVertex3f(left, top, 0.0f);
			glEnd();
		}
		glLineWidth(1.0);
	} glPopMatrix();
}

// ----------------------------------------------------------------
//	Name：			renderElipse
//	Description:	根据参数TRectanglef提供的参数绘制椭圆
//	Return Value:	void
// ----------------------------------------------------------------
void Helper::renderElipse( const TRectanglef& rect, 
							Color4 color, bool isEmpty )
{
	float xpos = rect.GetCenter().x();
	float ypos = rect.GetCenter().y();

	float xradius = rect.GetSize().x() / 2;
	float yradius = rect.GetSize().y() / 2;

	const float DEG2RAD = 3.14159f/180;

	glPushMatrix();
	{
		glColor4d( color.r(), color.g(), color.b(), color.a() );
		glLineWidth(5.0);
		if( isEmpty )
		{
			glBegin(GL_LINE_LOOP);   
			for (int i=0; i<360; i++)   
			{   
				//convert   degrees   into   radians   
				float degInRad = i*DEG2RAD;   
				glVertex2f( xpos + cos(degInRad) * xradius, ypos + sin(degInRad) * yradius );   
			}   
			glEnd();   
		}
		else
		{
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(xpos, ypos);
			//draw the vertex on the contour of the circle
			for(int i = 0; i < 360; i++)
			{	
				float degInRad = i*DEG2RAD;   
				glVertex2f( xpos + cos(degInRad) * xradius, ypos + sin(degInRad) * yradius );
			}
			glVertex2f( 1.0f * xradius + xpos, 0.0f *yradius + ypos );
			glEnd();
		}
		glLineWidth(1.0);
	}glPopMatrix();
}

// ----------------------------------------------------------------
//	Name：			DrawNet()
//	Description:	绘制网
//					size为长度
//					LineX为X方向上的网格数
//					LineZ为Z方向上的网格数
//	Return Value:	static void
// ----------------------------------------------------------------	
void Helper::DrawNet(GLfloat size, GLint LinesX, GLint LinesZ)
{
	glPushMatrix();
	glBegin(GL_LINES);
	for (int xc = 0; xc < LinesX; xc++)
	{
		glVertex3f(	-size / 2.0f + xc / (GLfloat)(LinesX-1)*size,
			0.0f,
			size / 2.0f);
		glVertex3f(	-size / 2.0f + xc / (GLfloat)(LinesX-1)*size,
			0.0f,
			size / -2.0f);
	}
	for (int zc = 0; zc < LinesX; zc++)
	{
		glVertex3f(	size / 2.0f,
			0.0f,
			-size / 2.0f + zc / (GLfloat)(LinesZ-1)*size);
		glVertex3f(	size / -2.0f,
			0.0f,
			-size / 2.0f + zc / (GLfloat)(LinesZ-1)*size);
	}
	glEnd();
	glPopMatrix();
}


// ----------------------------------------------------------------
//	Name：			DrawAxis()
//	Description:	绘制坐标系
//	Return Value:	static void
// ----------------------------------------------------------------	
void Helper::DrawAxis()
{
	glLineWidth(2.0);
	glBegin(GL_LINES);
	{
		glVertex3f(50, 0, 0); glVertex3f(-50, 0, 0);
		glVertex3f(0, 50, 0); glVertex3f(0, -50, 0);
		glVertex3f(0, 0, 50); glVertex3f(0, 0, -50);
	}
	glEnd();
}

// ----------------------------------------------------------------
//	Name：			Float2Str()
//	Description:	将float转化为string
//	Return Value:	static std::string
// ----------------------------------------------------------------	
std::string Helper::Float2Str( float x )
{
	std::ostringstream o;
	std::setprecision(4);
	o << x;
	return o.str();
}

// ----------------------------------------------------------------
//	Name：			mySolidCylinder()
//	Description:	绘制四棱锥
//					参数为长度
//					底面圆半径 0.3
//	Return Value:	static void
// ----------------------------------------------------------------	
void Helper::mySolidCylinder(GLdouble bone_length)
{	
	GLdouble bone_radius= 0.3; 
	GLuint slices = 8; 
	GLuint stack = 3; 

	static GLUquadricObj *  quad_obj = NULL;
	if ( quad_obj == NULL )
		quad_obj = gluNewQuadric();
	gluQuadricDrawStyle( quad_obj, GLU_FILL );
	gluQuadricNormals( quad_obj, GLU_SMOOTH );

	gluCylinder( quad_obj, bone_radius, bone_radius, 
		bone_length, slices, stack ); 
}

// ----------------------------------------------------------------
//	Name：			mySolidPyramid()
//	Description:	绘制四棱锥
//					上面高度7/10 顶点位于原点
//					下面高度3/10 底点位于(0, 0, bone_length)位置
//					中间边长0.6  位于z = bone_length*0.3 平面上
//	Return Value:	static void
// ----------------------------------------------------------------	
void Helper::mySolidPyramid(GLdouble bone_length)
{
	GLfloat size = 0.6f;
	GLfloat base_length = static_cast<GLfloat>(bone_length) * 0.1f;
	GLfloat top_length = static_cast<GLfloat>(bone_length) * 0.9f;

	glPushMatrix();
	{
		glLineWidth(1.0f);
		//top
		glBegin(GL_TRIANGLE_FAN);     	// draw triangle		
		glVertex3f( 0.0f, 0.0f, 0.0f);
		glVertex3f(-size, 0.0f, base_length);
		glVertex3f( 0.0f, size, base_length);
		glVertex3f( size, 0.0f, base_length);
		glVertex3f( 0.0f, -size, base_length);
		glVertex3f(-size, 0.0f, base_length);
		glEnd();
		glBegin(GL_QUADS);               // draw square
		glVertex3f(-size, 0.0f, base_length);
		glVertex3f( 0.0f, size, base_length);
		glVertex3f( size, 0.0f, base_length);
		glVertex3f( 0.0f, -size, base_length);
		glEnd();
		// base
		glBegin(GL_TRIANGLE_FAN);     	// draw triangle
		glVertex3f(0.0f, 0.0f, static_cast<GLfloat>(bone_length) );
		glVertex3f(-size, 0.0f, base_length);
		glVertex3f( 0.0f, size, base_length);
		glVertex3f( size, 0.0f, base_length);
		glVertex3f( 0.0f, -size, base_length);
		glVertex3f(-size, 0.0f, base_length);
		glEnd();
	} glPopMatrix();
}

// ----------------------------------------------------------------
//	Name：			myWirePyramid()
//	Description:	绘制线框四棱锥
//	Return Value:	static void
// ----------------------------------------------------------------	
void Helper::myWirePyramid(GLdouble bone_length)
{
	GLfloat size = 0.6f;
	GLfloat base_length = size * 1.5f;
	GLfloat top_length = size - static_cast<GLfloat>(bone_length);	//negtive

	glPushMatrix();
	{
		glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
		glTranslatef( 0.0f, -top_length, 0.0f );
		glBegin(GL_LINE_STRIP);
		{
			glVertex3f( 0.0f,  base_length, 0.0f);		// 0
			glVertex3f(-size,  0.0f,-size);		// 1
			glVertex3f( size,  0.0f,-size);		// 2
			glVertex3f( 0.0f,  top_length, 0.0f);		// Base
			glVertex3f(-size,  0.0f,-size);		// 1
			glVertex3f(-size,  0.0f, size);		// 4
			glVertex3f( 0.0f,  base_length, 0.0f);		// 0
			glVertex3f( size,  0.0f,-size);		// 2
			glVertex3f( size,  0.0f, size);		// 3
			glVertex3f( 0.0f,  base_length, 0.0f);		// 0
			glVertex3f(-size,  0.0f, size);		// 4
			glVertex3f( 0.0f,  top_length, 0.0f);		// Base
			glVertex3f( size,  0.0f, size);		// 3
			glVertex3f(-size,  0.0f, size);		// 4
		} glEnd();
	} glPopMatrix();
}

// ----------------------------------------------------------------
//	Name：			myDrawNone()
//	Description:	do nothing...
//	Return Value:	static void
// ----------------------------------------------------------------	
void Helper::myDrawNone(GLdouble bone_length)
{
	//do nothing...
}


GLuint Helper::ColorStable[] = 
{
	(0xFF0000),	//red 
	(0xDC143C),	//crimson
	(0x9400D3),	//violet
	(0xFFA000),	//orange
	(0xFFFF00),	//yellow
	(0xFFD700),	//glod
	(0x00FF00),	//green
	(0x90FF90),	//lightgreen
	(0x006400),	//darkgreen
	(0x556B2F),	//olivegreen		//very good color绿橄榄
	(0x0000FF),	//blue
	(0x00008B),	//darkblue
	(0x4169E1),	//royalblue
	(0x5555FF),	//skyblue
	(0x191970),	//midnightbue
	(0xFF00FF),	//fuchsia
	(0x00FFFF),	//aqua
	(0xffb6c1),	//pink
	(0xFF91A4),	//salmonpink
	(0x30D5C8),	//turquoise
	(0x008080),	//darkturpuoise
	(0xA9A9A9),	//gray
	(0xD3D3D3),	//lightgray
	(0x808080)		//darkgray
};
// 		 (0x000000),	//black
// 		 (0xFFFFFF),	//white