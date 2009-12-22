#include "DragCamera.h"

#include <gl/glut.h>

DragCamera::DragCamera()
	: dragSpeed(4.0)
	, isDragging(false)
{}

DragCamera::~DragCamera()
{}

void DragCamera::dragCameraZ(DIR dir)
{
	if(dir == POSITIVE)
		position[2] += dragSpeed;
	else
		position[2] -= dragSpeed;

	restrictCamera();
}

void DragCamera::restrictCamera()
{
	if(position[2] >= 400)
		reset();

	if(position[2] <= -400.0)
		reset();		
}

void DragCamera::OnMouseHandle(int button, int state, int x, int y)
{
	if( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
		isDragging = true;

	if( button == GLUT_RIGHT_BUTTON && state == GLUT_UP )
		isDragging = false;
}

void DragCamera::OnMotionHandle(int x, int y)
{
	//Êó±êÔÚclientÇøÓò
	if( isDragging )
	{
		if(x > vRLastPos[0] || y > vRLastPos[1]) 
		{
			dragCameraZ(DragCamera::POSITIVE);
		}
		else
		{
			dragCameraZ(DragCamera::NEGATIVE);
		}
	}
	vRLastPos.setValue(x, y);
}