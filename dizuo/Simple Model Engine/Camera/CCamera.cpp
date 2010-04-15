#include "CCamera.h"

CCamera::CCamera()
{
	Reset();
}

CCamera::~CCamera()
{}

//更新照相机的位置
void CCamera::_UpdateCameraPosition()
{
	double x, y, z,r;
	r = BALL_RADIUS * cos(m_vRotVec.y() * PI / 180);

	x = fabs(r) * sin(m_vRotVec.x() * PI / 180);
	y = BALL_RADIUS * sin(m_vRotVec.y() * PI / 180);
	z = fabs(r) * cos(m_vRotVec.x() * PI / 180);

	m_vPosition.setValue( x, y, z );
}

void CCamera::PositionCamera()
{
	_UpdateCameraPosition();

	if(m_bPerspective)
	{
		if (m_vRotVec.y() < 90 || m_vRotVec.y() > 270)
		{
			gluLookAt( -m_vPosition.x()*ZOOM_FACTIOR*m_fZoomRate, 
						m_vPosition.y()*ZOOM_FACTIOR*m_fZoomRate,
						-m_vPosition.z()*ZOOM_FACTIOR*m_fZoomRate,	
				m_vView.x(),	 m_vView.y(),     m_vView.z(),	
				m_vUpVector.x(), -m_vUpVector.y(), m_vUpVector.z() );
		}
		else
		{
			gluLookAt( m_vPosition.x()*ZOOM_FACTIOR*m_fZoomRate, 
				m_vPosition.y()*ZOOM_FACTIOR*m_fZoomRate,
				m_vPosition.z()*ZOOM_FACTIOR*m_fZoomRate,	
				m_vView.x(),	 m_vView.y(),     m_vView.z(),	
				m_vUpVector.x(), m_vUpVector.y(), m_vUpVector.z() );
		}
	}
	else
	{
		if (m_vRotVec.y() <= 90 || m_vRotVec.y() >= 270)
		{
			gluLookAt(m_vPosition.x(), m_vPosition.y(), m_vPosition.z(),	
				m_vView.x(),	 m_vView.y(),     m_vView.z(),	
				m_vUpVector.x(), m_vUpVector.y(), m_vUpVector.z() );
		}
		else
		{
			gluLookAt( -m_vPosition.x(), m_vPosition.y(), -m_vPosition.z(),	
				m_vView.x(),	 m_vView.y(),     m_vView.z(),	
				m_vUpVector.x(), -m_vUpVector.y(), m_vUpVector.z() );
		}
	}

	// Save the Model view matrix.  This is used later for
	// conversion of mouse coordinates to world coordinates.
	glGetDoublev(GL_MODELVIEW_MATRIX, m_dModelViewMatrix);
}

void CCamera::ResetView(int w, int h)
{
	// Save the screen width and height
	if(w || h) {
		m_iScreenWidth  = (GLsizei)w;
		m_iScreenHeight = (GLsizei)h;
	}

	// calculate the aspect ratio of the screen
	if (m_iScreenHeight==0)
		m_fAspect = (GLdouble)m_iScreenWidth;
	else
		m_fAspect = (GLdouble)m_iScreenWidth/(GLdouble)m_iScreenHeight;

	// Reset the projection matrix (coordinate system)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(m_bPerspective) {
		// Perspective transformations.
		gluPerspective(m_fFovY, m_fAspect, 1.5*m_fZoomRate, 450*m_fZoomRate);
	}
	else {
		// Orthographic transformations.
		GLfloat XpY = m_fZoomRate*(GLfloat)m_iScreenWidth/(GLfloat)m_iScreenHeight;
		//newx1=-CX/2;newy1=-CY/2;newx2=CX/2;newy2=CY/2;
		//glOrtho( *XpY, newx2*XpY, newy1*XpY, newy2*XpY, -150, 150);
	}

	// Set Viewport to window dimensions
	glViewport(0, 0, m_iScreenWidth, m_iScreenHeight);
	// prepare for pan:
	glTranslatef( -m_vPanVec.x(), m_vPanVec.y(), m_vPanVec.z() );
	//glTranslatef( -10, 10, m_vPanVec.z() );


	// Save the Projection matrix.  This is used later for
	// conversion of mouse coordinates to world coordinates.
	glGetDoublev(GL_PROJECTION_MATRIX, m_dProjectionMatrix);

	// Save the Projection matrix.  This is used later for
	// conversion of mouse coordinates to world coordinates.
	glGetIntegerv(GL_VIEWPORT, m_iViewport);

	// Reset the ModelView matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CCamera::Reset()
{
	m_vPosition.setValue(0.0f, 0.0f, 20.0f);
	m_vView.setValue(0.0f, 0.0f, 0.0f);
	m_vUpVector.setValue(0.0f, 1.0f, 0.0f);

	m_fFovY = 45.0f;
	m_fNear = 1.0f;
	m_fFar  = 10000.0f;

	m_bPerspective	= TRUE;

	m_fZoomRate = 1.0f;
	m_vPanVec.setValue(0.0f, 0.0f, 0.0f);
	m_vRotVec.setValue(0.0f, 0.0f);

	// Clear our modelview and projection matrix
	memset(&m_dModelViewMatrix, 0, sizeof(GLdouble)*16);
	memset(&m_dProjectionMatrix, 0, sizeof(GLdouble)*16);
}

void CCamera::GetWorldCoord(int ix, int iy, double fz, RVec3& coord)
{
	GLdouble x, y, z, winX, winY, winZ;

	// Fix the yPos value.  MS Windows origin 0,0 is upper left
	// while OpenGL windows origin 0,0 is lower left...
	winX = (GLdouble)ix;
	winY = (GLdouble)m_iViewport[3]-iy;
	winZ = (GLdouble)1.0f;

	// Unproject the point
	gluUnProject(winX, winY, winZ,
		m_dModelViewMatrix,
		m_dProjectionMatrix,
		m_iViewport,
		&x, &y, &z);

	// Copmensate for rounding errors
	if(fabs(x) < SMALL_NUMBER)
		x = 0;
	if(fabs(y) < SMALL_NUMBER)
		y = 0;
	if(fabs(z) < SMALL_NUMBER)
		z = 0;

	coord.setValue(x, y, z);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// change the m_vPanVec
void CCamera::PanCamera(float deltax, float deltay)
{
	//float wx = ((float)(deltax)/3.415f)*m_fZoomRate*1.58f;
	//float wy = ((float)(deltay)/3.415f)*m_fZoomRate*1.58f;
	float wx = ((float)(deltax)/3.415f)*m_fZoomRate*0.1f;
	float wy = ((float)(deltay)/3.415f)*m_fZoomRate*0.1f;

	RVec3 deltaM(wx, wy, 0.0f);

	m_vPanVec += deltaM;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// change the m_fZoomRate
void CCamera::ZoomCamera(float zoomRate)
{
	m_fZoomRate *= 1.0f + zoomRate / 100.0f;
	if (m_fZoomRate <= 1E-15)
	{
		m_fZoomRate = 1E-15;
	}

	if (m_fZoomRate >= 100)
	{
		m_fZoomRate = 100;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// change the m_vRotVec
void CCamera::RotateCamera(float deltax, float deltay)
{
	m_vRotVec.y() += deltay;

	if(m_vRotVec.y() <=90 || m_vRotVec.y() >270)
		m_vRotVec.x() -= deltax;
	else
		m_vRotVec.x() += deltax;

	if (m_vRotVec.y() > 360)		
	{
		m_vRotVec.y() -= 360;
	}

	if (m_vRotVec.x() >360)
	{
		m_vRotVec.x() -= 360;
	}

	if (m_vRotVec.x() < 0)
	{
		m_vRotVec.x() += 360;
	}

	if (m_vRotVec.y() < 0)
	{
		m_vRotVec.y() += 360;
	}
}
