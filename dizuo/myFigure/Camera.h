#ifndef CAMERA_H
#define CAMERA_H

#include "util/gtl/vec3.hpp"

class Camera
{
protected:
	//DATA
	gtl::Vec3f	position;
	gtl::Vec3f	view;
	gtl::Vec3f	up;
public:
	//IMPLEMENT
	Camera();
	~Camera();
	
	//MANIPULATORS
	void apply();
	void computeHome();

	void setPos(float x, float y, float z)	{position.setValue(x, y, z);}
	void setView(float x, float y, float z)	{view.setValue(x, y, z);}
	void setUp(float x, float y, float z)	{up.setValue(x, y, z);}

	//ACCESSOR
	gtl::Vec3f getPos() const { return position;}
	gtl::Vec3f getView() const{ return view;}
	gtl::Vec3f getUp()	const { return up;}
	//up
	//position
	//view

protected:
	void reset();
};

//	maybe many cameras
//  [4/6/2009 RenYaFei]
#endif