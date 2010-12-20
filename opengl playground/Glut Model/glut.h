
#include <gl/glut.h>
class glut
{
public:
	glut(void);
	void registraDraw(void);
	void registraKeyboard(void);
	void registraReshape(void);
	void init(void);
	void loop();
	~glut(void);
	void setViewPort(int a, int b, int c, int d);
	void setCamera();
};
