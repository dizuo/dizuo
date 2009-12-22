#include "Joint.h"

#include "util/global.h"

const GLfloat	green[] = { 20/255.0f, 254.0f/255.0f, 20/255.0f, 1.0f};
const GLfloat	yellow[] = { 253.0f/255.0f, 208.0f/255.0f, 0.0f/255.0f, 1.0f};

// ----------------------------------------------------------------
//	Name：			Joint::Joint
//	Description:	根据给定信息构造节点
//	Return Value:	...
// ----------------------------------------------------------------
Joint::Joint(std::string _name, float x, float y, float z )
{
	clear();
	name = _name;
	offset.setValue(x, y, z);
}
// ----------------------------------------------------------------
//	Name：			Joint::Joint
//	Description:	默认构造函数
//	Return Value:	...
// ----------------------------------------------------------------
Joint::Joint()
{
	clear();
}

// ----------------------------------------------------------------
//	Name：			Joint::~Joint
//	Description:	释放资源
//	Return Value:	...
// ----------------------------------------------------------------
Joint::~Joint()
{
	clear();
}

// ----------------------------------------------------------------
//	Name：			Joint::render
//	Description:	绘制节点，用球体,节点被选中时，放大一点.
//					获得每一帧，每个节点在窗口中的位置
//					随时捕获是否被鼠标选中
//	Return Value:	void
// ----------------------------------------------------------------
void Joint::render(GLenum mode)		
{
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	gluProject(0, 0, 0, modelMatrix, projMatrix, viewport,
		&screen_pos[0], &screen_pos[1], &screen_pos[2]);


	glColor4f(0.0, 1.0, 1.0, 1.0);
	if(is_select)
	{
		glColor4f(253.0f/255.0f, 208.0f/255.0f, 0.0f/255.0f, 1.0f);
		glutSolidSphere(joint_radius*2, 10, 6);
	}
	else
	{
		glutSolidSphere(joint_radius, 10, 6);
	}
}

// ----------------------------------------------------------------
//	Name：			Joint::active
//	Description:	节点被选中...
//	Return Value:	void
// ----------------------------------------------------------------
void Joint::active()
{
	is_select = true;
}

// ----------------------------------------------------------------
//	Name：			Joint::deactive
//	Description:	节点被释放...
//	Return Value:	void
// ----------------------------------------------------------------
void Joint::deactive()
{
	is_select = false;
}

// ----------------------------------------------------------------
//	Name：			Joint::clear
//	Description:	
//	Return Value:	void
// ----------------------------------------------------------------
void Joint::clear()
{
	is_select = false;
	parent = NULL;
	name = "";
	joint_radius = 0.9f;
	children.clear();
	offset.setValue(0.0, 0.0, 0.0);
	screen_pos.setValue(0.0, 0.0, 0.0);

	for (size_t i=0; i<channels.size(); i++ )
	{
		SAFE_DELETE( channels[i] );
	}
	channels.clear();
}

// [3/16/2009 RenYaFei]
//		改变Render的函数功能，增加选择...

// [3/16/2009 RenYaFei]
//	使用joint_sequence_number 失败，原因是顺序问题，先创建所有节点，
//		然后在递归渲染，如果创建一个，就立即渲染，就ok了..
//	增加变量joint_id.(1-节点总数)