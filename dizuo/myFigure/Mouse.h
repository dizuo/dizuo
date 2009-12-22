/*************************************************************/
/**					Filename: Mouse.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  3/14/2009		RYF - Creation							**/	
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef MOUSE_H
#define MOUSE_H

#include "util/Singleton.hpp"
#include "util/gtl/vec2.hpp"
#include "util/gtl/vec3.hpp"

#define PickThreshold 10

//mouse 必须保持有一个camera
//mouse其实就是实时的修改camera
class MouseManger : public Singleton<MouseManger>
{
	friend class Singleton<MouseManger>;
protected:
	//NOT IMPLEMENT
	MouseManger();
	~MouseManger();
public:
	//MANIPULATORS
	void OnMouseHandle(int button, int state, int x, int y);
	void OnMotionHandle(int x, int y);
	void PickJointByMouse(int button, int state, int x, int y);
	void SelectJointByName(int button, int state, int x, int y);
	void MoveSelectedSkeletons(int x, int y);
	
	void ActiveDrag() {isDrag = true;};
	void ActiveTrack() {isTrack = true;};
	void ActiveEdit() {isEdit = true;};

	void DeActiveDrag() {isDrag = false;}
	void DeActiveTrack() {isTrack = false;}
	void DeActiveEdit() {isEdit = false;}
private:
	bool	isDrag;
	bool	isTrack;
	bool	isEdit;

	gtl::Vec2i	vMousePos;
	gtl::Vec3f	vGrabRot;
	//TrackBall
	//Camera
};
// [3/14/2009 RenYaFei]
//	单件类后面必须显示的freeInstance().
#endif