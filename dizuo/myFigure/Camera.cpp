#include "Camera.h"

#include <gl/glut.h>

Camera::Camera()
{
	//Load config file
	//get the config camera info...
	reset();
}

Camera::~Camera()
{}

void Camera::apply()
{
	gluLookAt( position.x(), position.y(), position.z(),
			   view.x(), view.y(), view.z(),
			   up.x(), up.y(), up.z() );
}

void Camera::computeHome()
{
	reset();
}

void Camera::reset()
{
	position.setValue(0.0, 0.0, 50);
	up.setValue(0.0, 1.0, 0.0);
	view.setValue(0.0, 0.0, 0.0);
}

