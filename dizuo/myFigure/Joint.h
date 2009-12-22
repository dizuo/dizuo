#ifndef JOINT_H
#define JOINT_H

#include <string>
#include <vector>
#include <gl/glut.h>

#include "util/gtl/vec3.hpp"

class Htree;
class BvhEditer;
class Figure;

class Joint
{
public:
	friend class Htree;
	friend class Figure;
	friend class BvhEditer;

	Joint( std::string _name, float x, float y, float z );
	Joint();
	~Joint();

	void  render(GLenum mode = GL_RENDER);
	void  active();
	void  deactive();

	enum channelTypes {Xpos,Ypos,Zpos,Zrot,Xrot,Yrot};
	class Channel
	{
	public:
		Channel()
			: type(Xpos)
			, index(0)
		{}
		channelTypes type;
		size_t index;	//初始值设定
	};

	std::string name;	//parts's name
	gtl::Vec3d  screen_pos;	//记录节点在屏幕的位置和深度

	gtl::Vec3d	offset;		//transform matrix
	std::vector<Joint*>		children;
	std::vector<Channel*>	channels;
private:
	float	joint_radius;

	Joint	*parent;
	bool	is_select;
private:
	void clear();
};
// [3/10/2009 RenYaFei]
// 改变了channels的内容..
#endif