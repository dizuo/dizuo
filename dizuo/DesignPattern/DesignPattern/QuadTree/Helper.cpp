#include "Helper.h"

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
		glColor4f( color.r(), color.g(), color.b(), color.a() );
		glLineWidth(2.0);
		if( isEmpty ) 
		{
			glBegin(GL_LINE_STRIP);
			glVertex3f(left, bottom, 0.0);
			glVertex3f(right, bottom, 0.0);
			glVertex3f(right, top, 0.0);
			glVertex3f(left, top, 0.0);
			glVertex3f(left, bottom, 0.0);
			glEnd();
		}
		else
		{
			glBegin(GL_QUADS);
			glVertex3f(left, bottom, 0.0);
			glVertex3f(right, bottom, 0.0);
			glVertex3f(right, top, 0.0);
			glVertex3f(left, top, 0.0);
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

	const float DEG2RAD = 3.14159/180;

	glPushMatrix();
	{
		glColor4f( color.r(), color.g(), color.b(), color.a() );
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
			glVertex2f( 1.0 * xradius + xpos, 0.0 *yradius + ypos );
			glEnd();
		}
		glLineWidth(1.0);
	}glPopMatrix();
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