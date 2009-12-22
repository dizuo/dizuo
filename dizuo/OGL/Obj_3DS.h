#ifndef __OBJ_3DS_H
#define __OBJ_3DS_H

#include	"Object.h"
#include	"Model_3DS.h"

class Obj_3DS : public Object, public Model_3DS
{
public:
	Obj_3DS()
	{
		Active();

		m_Type = HOUSE;
		m_vRot.setValue(0., 0., 0.);
		m_vScale.setValue(1.0, 1.0, 1.0);
	}
	virtual bool Init(){
		return true;
	}
	virtual void Draw(){
		DrawModel();
	}

	const gtl::Vec3f GetRotv() const{return m_vRot;}
	const gtl::Vec3f GetScalev() const{return m_vScale;}

	gtl::Vec3f Rotv() {return m_vRot;}
	gtl::Vec3f Scalev(){return m_vScale;}
private:
	gtl::Vec3f	m_vRot;
	gtl::Vec3f	m_vScale;
};

#endif

// ø…“‘ºÃ≥–”ÎObject£¨Model_3DS