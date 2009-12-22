#include "global.h"
#include "CButton.h"
#include "CTexture.h"

CButton::CButton(int x, int y, int Width, int Height)
{
	m_ix = x;;
	m_iy = y;
	m_iZOrder = 0.5f;         // used if you specifically want to set a window higher
	m_iWidth  = Width;
	m_iHeight = Height;
	m_bVisible = TRUE;     // start off visible
	m_fAlpha = 1.0f;       // defult for alpha blending

	m_bPressed = 0;

	//默认的设置图片
	//外面也可以显示地修改
	m_iTexID = TEXTURE->LoadTexture("data/menu_110_40.tga");
}

CButton::~CButton()
{
	//SAFEDELETE(m_pTex);
}


void CButton::Render()
{
	if (m_bVisible)
	{
		glBindTexture(GL_TEXTURE_2D, m_iTexID);
		glPushAttrib(GL_COLOR_BUFFER_BIT);
		glColor4f(1.0f, 1.0f, 1.0f, m_fAlpha);
		glPushMatrix();
		{
			//坐标系原点转化为左下角点
			//移动到屏幕上m_ix, m_iy位置			
			glTranslatef(m_ix+m_iWidth/2.0f, m_iy+m_iHeight/2.0f, m_iZOrder);	
			if( m_bPressed )
				glScalef(0.9f, 0.9f, 1.0f);
			
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			//						^Y
			//(-m_iWidth/2.0, m_iHei| ght/2.0)
			//				--------|--------(m_iWidth/2.0,  m_iHeight/2.0)
			//				|		|		|
			//-----------------------(0, 0)----------->X
			//				|		|		|
			//				--------|--------
			//				|		|		(m_iWidth/2.0,  -m_iHeight/2.0)
			//-m_iWidth/2.0, -m_iHeight/2.0

			glBegin(GL_QUADS);
			{
				// 此时：按钮的中心位于原点
				glTexCoord2f(1.0f, 1.0f);		glVertex2f(-m_iWidth/2.0f, -m_iHeight/2.0f);
				glTexCoord2f(0.0f, 1.0f);		glVertex2f(m_iWidth/2.0f,  -m_iHeight/2.0f);
				glTexCoord2f(0.0f, 0.0f);		glVertex2f(m_iWidth/2.0f,  m_iHeight/2.0f);
				glTexCoord2f(1.0f, 0.0f);		glVertex2f(-m_iWidth/2.0f, m_iHeight/2.0f);
			}
			glEnd();
		}
		glPopMatrix();
		glPopAttrib();
	}
}

bool CButton::OnMouseDown(LPARAM lParam)
{
	//得到的鼠标坐标系：原点位于左上角
	//					Y轴向下
	int MouseX = LOWORD(lParam);
	int MouseY = HIWORD(lParam);

	return OnMouseDown(MouseX, MouseY);
}

bool CButton::OnMouseDown(int MouseX, int MouseY)
{
	// Test to see if user clicked in caption bar
	if ((MouseX > m_ix) && (MouseX <m_ix + m_iWidth))
	{
		if ((MouseY > m_iy) && (MouseY < m_iy + m_iHeight))
		{
			m_bPressed = true;
			return true;
		}
	}
	return false;
}
void CButton::OnMouseUp(LPARAM lParam)
{
	m_bPressed = false;
}

void CButton::OnMouseUp()
{
	m_bPressed = false;
}

//glTexCoord2f(0.0, 0.0);		glVertex2f(-m_iWidth/2.0, -m_iHeight/2.0);
//glTexCoord2f(1.0, 0.0);		glVertex2f(m_iWidth/2.0,  -m_iHeight/2.0);
//glTexCoord2f(1.0, 1.0);		glVertex2f(m_iWidth/2.0,  m_iHeight/2.0);
//glTexCoord2f(0.0, 1.0);		glVertex2f(-m_iWidth/2.0, m_iHeight/2.0);