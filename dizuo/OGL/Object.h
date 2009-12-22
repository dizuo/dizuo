#ifndef __OBJECT_H
#define __OBJECT_H

#include	"gtl/vec3.hpp"

// class CCamera;
// class Trackball;

class Object
{
public:
	enum ObjectType{
		DEFAULT = 0,
		SKYBOX,
		GROUND,
		WALL,
		HOUSE,
		ROLE,
		WEAPON,
		FONT
	};

	Object()
		: m_bActive(false), m_Type(DEFAULT)
	{
		m_vPos.setValue(0.0, 0.0, 0.0);
	};
	virtual ~Object(){};

	virtual bool Init() = 0;
	virtual void Draw() = 0;

	void SetType(ObjectType type){m_Type = type;}
	void SetPos(float x, float y, float z){	m_vPos.setValue(x,y,z);	};
	void Active() {m_bActive = true;}
	void DeActive(){m_bActive = false;}

	gtl::Vec3f	GetPos() const {return m_vPos;}
	ObjectType  GetType() const{return m_Type;}
	
	bool		m_bActive;
protected:
	gtl::Vec3f	m_vPos;
	ObjectType	m_Type;
};

#endif