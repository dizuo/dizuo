#ifndef __ROLE_H
#define __ROLE_H

#include	"gtl/vec3.hpp"
#include	"Object.h"
#include	"MD2.h"
#include	"Texture.h"

class Role : public Object
{
public:
	Role();
	~Role(){};

	//void OnCollision(CObject *collisionObject);
	virtual bool Init();
	virtual void Draw();
	
	void SetTimSec(float dest){
		if(m_fLastTimSec == 0.0)
			m_fLastTimSec = dest;
		m_fTimSec = dest;
	}
	void Stop(){m_bStop = true;}
	void Move() {m_bStop = false;}
	float GetDir(){return direction;}
private:
	float		direction;         // angle the entity is facing (in radians)

	float		m_fTimSec;
	float		m_fLastTimSec;

	bool		m_bStop;

	gtl::Vec3f	m_vVelocity;
	gtl::Vec3f	m_vAcceleration;

	CMD2Model	m_Role;
	CMD2Model	m_Weapon;
};
#endif
//可以移动的，