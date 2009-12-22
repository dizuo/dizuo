#ifndef JOINT_H
#define JOINT_H

#include <string>
#include <iostream>

#include <gl/glut.h>

class Joint
{
public:
	Joint( std::string _name, float x=0., float y=0., float z=0. );
	Joint();
	~Joint();

	void  render();
	void  active();
	void  deactive();
public:
	std::string name;
	std::string type;	//"ROOT" "JOINT" "END"
	float	joint_radius;
	float	offset[3];		//transform matrix
	
	bool	is_select;
private:
	void clear();
};

// [3/10/2009 RenYaFei]
// 改变了channels的内容..
#endif

