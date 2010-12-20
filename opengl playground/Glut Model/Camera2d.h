
#include "camera.h"
#include "CVector.h"

class Camera2d :
	public Camera
{
public:
	Camera2d(void);
	~Camera2d(void);
	void setLentes(int left,int right, int bottom, int top);

//left  right  bottom top
};
