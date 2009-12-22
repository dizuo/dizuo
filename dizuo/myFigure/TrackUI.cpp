#include	"TrackUI.h"
#include	<GL/glut.h>

TrackUI::TrackUI()
{
	m_Ball = new Trackball;
	m_Ball->Reset();
	isClicked = false;
	isDragging = false;
}

TrackUI::~TrackUI()
{
	if(m_Ball)
		delete m_Ball;
}

void TrackUI::OnMouseHandle(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON)
		switch(state)
	{
		case GLUT_DOWN:
			m_Ball->BeginTracking(x, y);
			isClicked = true;
			break;
		case GLUT_UP:
			isClicked = false;
			isDragging = false;
			break;
	}

	if(btn==GLUT_RIGHT_BUTTON)
		switch(state)
		{
		case GLUT_DOWN:
			ComputerHome();
			break;
		}
}

void TrackUI::OnMotionHandle(int x, int y)
{
	if (isClicked)
	{
		m_Ball->Tracking(x, y);
	}
}

void TrackUI::Apply()
{
	m_Ball->toOpenGL();
}

void TrackUI::ComputerHome()
{
	m_Ball->Reset();
	isClicked = false;
	isDragging = false;
}

