/*************************************************************/
/**					Filename: SKYBOX.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  16/05/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**	  23/05/2008	-	change for another Texture version	**/	
/**	  26/05/2008	-	change Box Coordinates.				**/
/**	  31/05/2008	-	Derived from class Node.			**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef SKYBOX_H
#define SKYBOX_H

#include "gtl/vec3.hpp"
#include "Texture.h"
#include "Object.h"

class CSkyBox : public Object
{
	unsigned int m_TxtID[6];
	gtl::Vec3f	m_vHalfDim;
public:
	CSkyBox();
	~CSkyBox();

	//void setData(float x, float y, float z, float width, float height, float length);
	virtual bool Init();
	virtual void Draw();

 	float BoxHeight()const{return m_vHalfDim.x();}
 	float BoxWidth() const{return m_vHalfDim.y();}
 	float BoxLen()	 const{return m_vHalfDim.z();}
};

#endif // SKYBOX_H

//////////////////////////////////////////////////////////////////////////
//	法向量的设置
//	构造顶点数组.