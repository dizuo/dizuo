#ifndef DRAGCAMERA_H_
#define DRAGCAMERA_H_

#include "Camera.h"
#include "util/gtl/vec2.hpp"

class DragCamera : public Camera
{
	//DATA
	float		dragSpeed;
	gtl::Vec2i	vRLastPos;
	bool		isDragging;
public:
	//ENUM
	enum DIR{ POSITIVE, NEGATIVE };

	//IMPLEMENT
	DragCamera();
	~DragCamera();

	//MANIPULATORS
	void dragCameraZ(DIR dir);
	void setSpeed(float speed){ dragSpeed = speed; }
	void OnMouseHandle(int button, int state, int x, int y);
	void OnMotionHandle(int x, int y);

	//ACCESSOR
	float getSpeed() const {return dragSpeed;}

private:
	void  restrictCamera();
};

#endif