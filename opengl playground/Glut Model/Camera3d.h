#pragma once
#include "camera.h"
#include "CVector.h"

class Camera3d :
	public Camera
{
public:
	Camera3d(void);
	
	~Camera3d(void);
	void setLentes(int angle, int ratio);
	void setLookAt(float ox,float oy,float oz,float dx,float dy,float dz,float ux,float uy,float uz);
private:
     CVector4 Olho;
	 CVector4 Direcao;
	 CVector4 Up;

};
