#include "Figure.h"
#include <cmath>

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

//#include "myGraphy.h"
#include "util/Helper.h"

using namespace std;

// ----------------------------------------------------------------
//	Name：			Figure::Figure()
//	Description:	默认构造函数
//	Return Value:	
// ----------------------------------------------------------------
Figure::Figure()
{
	init();
}

// ----------------------------------------------------------------
//	Name：			Figure::Figure()
//	Description:	重载构造函数, 参数为文件名
//	Return Value:	
// ----------------------------------------------------------------
Figure::Figure(std::string _name)
	: BvhEditer(_name)
{
	init();
}

Figure::Figure(const char * _name)
	: BvhEditer( std::string(_name) )
{
	init();
}
// ----------------------------------------------------------------
//	Name：			Figure::~Figure()
//	Description:	析构函数
//	Return Value:	
// ----------------------------------------------------------------
Figure::~Figure()
{
	cout << "delete the figure" << endl;
	init();
	SAFE_DELETE(font_container);
}

// ----------------------------------------------------------------
//	Name：			Figure::render_figure
//	Description:	绘制角色
//	Return Value:	void
// ----------------------------------------------------------------
void Figure::render_figure(float scale, GLenum mode)
{
	render(root, frame_control, scale, mode);
}

// ----------------------------------------------------------------
//	Name：			Figure::update
//	Description:	如果播放，则前进一帧
//	Return Value:	bool
// ----------------------------------------------------------------
bool Figure::update()
{
	if(is_play)
		forward();
	return true;
}

// ----------------------------------------------------------------
//	Name：			Figure::play
//	Description:	置is_play控制量为true
//	Return Value:	void
// ----------------------------------------------------------------
void Figure::play()
{
	is_play = true;
}

// ----------------------------------------------------------------
//	Name：			Figure::pause
//	Description:	置is_play为false
//	Return Value:	void
// ----------------------------------------------------------------
void Figure::pause()
{
	is_play = false;
}

// ----------------------------------------------------------------
//	Name：			Figure::forward
//	Description:	无条件，前进一帧
//	Return Value:	void
// ----------------------------------------------------------------
void Figure::forward()
{
	if( frame_control >= (frames_num-1) ) 
		frame_control = 0;
	frame_control++;
}

// ----------------------------------------------------------------
//	Name：			Figure::backward
//	Description:	无条件，后退一帧
//	Return Value:	void
// ----------------------------------------------------------------
void Figure::backward()
{
	if( frame_control <= 0 )
		frame_control = frames_num-1;
	frame_control--;
}

// ----------------------------------------------------------------
//	Name：			Figure::render
//	Description:	递归地绘制Figure的树结构
//					First:	Translate()
//					Second: Rotate()
//					Third:	Render()
//					Last:	Recurs_render()	
//					frame:从0开始计
//	Return Value:	void
// ----------------------------------------------------------------
void Figure::render(part *some, size_t frame, float scale, GLenum mode)
{	
	glPushMatrix();
	{
		//////////////////////////////////////////////////////////////////////////
		//Translate()
		glTranslated( some->offset[0] * scale, 
					  some->offset[1] * scale, 
					  some->offset[2] * scale );
		some->render(mode);	//显示关节点
	
		//////////////////////////////////////////////////////////////////////////
		//Rotate()
		for ( size_t i=0; i<some->channels.size(); i++ )
		{
 			Joint::Channel *channel = some->channels[i];
 			float rot_data = get_motion(frame, channel->index);
 
 			//motion_data的索引，应该是frame_count和motion_data联合使用
 			if ( channel->type == part::Xrot )
 				glRotatef( rot_data, 1.0f, 0.0f, 0.0f );
 			else if ( channel->type == part::Yrot )
 				glRotatef( rot_data, 0.0f, 1.0f, 0.0f );
 			else if ( channel->type == part::Zrot )
 				glRotatef( rot_data, 0.0f, 0.0f, 1.0f );
		}
		//////////////////////////////////////////////////////////////////////////
		//Render()
		// 一个孩子情况:
		if ( some->children.size() == 1 )
		{
			Joint *  child = some->children[0];
			render_bone( 0.0f, 0.0f, 0.0f, 
						 child->offset[0] * scale, 
						 child->offset[1] * scale, 
						 child->offset[2] * scale );
		}
		// 多个孩子情况：加权平均，求出中心center
		if ( some->children.size() > 1 )
		{
			double center[3] = { 0.0f, 0.0f, 0.0f };
			for (size_t i=0; i<some->children.size(); i++ )
			{
				part *child = some->children[i];
				center[0] += child->offset[0];
				center[1] += child->offset[1];
				center[2] += child->offset[2];
			}
			//加权平均
			center[0] /= some->children.size() + 1;
			center[1] /= some->children.size() + 1;
			center[2] /= some->children.size() + 1;

			render_bone( 0.0f, 0.0f, 0.0f, 
						 center[0] * scale, 
						 center[1] * scale, 
						 center[2] * scale );

			for (size_t i=0; i<some->children.size(); i++ )
			{
				part *child = some->children[i];
				render_bone( center[0] * scale, center[1] * scale, 
							 center[2] * scale, child->offset[0] * scale, 
							 child->offset[1] * scale, child->offset[2] * scale );
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//Recurs_render()
		//当some为叶子节点时候，不再深入递归
		for (size_t i=0; i<some->children.size(); i++)
		{
			render(some->children[i], frame, scale, mode);
		}
	}
	glPopMatrix();
}

// ----------------------------------------------------------------
//  Name:           Figure::render_bone
//  Description:    根据六个参数，确实绘制出bone所要进行的变换
//  Return Value:   void
// ----------------------------------------------------------------
// void  Figure::render_bone( float x0, float y0, float z0, 
// 						   float x1, float y1, float z1 )
void Figure::render_bone( double x0, double y0, double z0, 
						  double x1, double y1, double z1 )
{
	glColor4f(1.0, 0.0, 0.0, 4.0);
	//计算出bone的长度
	GLdouble  dir_x = x1 - x0;
	GLdouble  dir_y = y1 - y0;
	GLdouble  dir_z = z1 - z0;
	GLdouble  bone_length = 
		sqrt( dir_x*dir_x + dir_y*dir_y+ dir_z*dir_z );

	glPushMatrix();
	//平移
	glTranslated( x0, y0, z0 );

	double  length;
	length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
	if ( length < 0.0001 ) { 
		dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
	}
	//单位话dir向量
	dir_x /= length;  dir_y /= length;  dir_z /= length;

	// Up 向量
	GLdouble  up_x, up_y, up_z;
	up_x = 0.0;
	up_y = 1.0;
	up_z = 0.0;

	// (up_x, up_y, up_z)×(dir_x, dir_y, dir_z)
	//------------------------------------------
	//	i		 j		 k
	// up_x		up_y	up_z
	// dir_x	dir_y	dir_z
	//------------------------------------------
	// Vector(side) = Vector(up) × Vector(dir)
	double  side_x, side_y, side_z;
	side_x = up_y * dir_z - up_z * dir_y;
	side_y = up_z * dir_x - up_x * dir_z;
	side_z = up_x * dir_y - up_y * dir_x;

	length = sqrt( side_x*side_x + side_y*side_y + side_z*side_z );
	if ( length < 0.0001 ) {
		side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
	}
	//单位化side向量
	side_x /= length;  side_y /= length;  side_z /= length;

	// 再次叉乘计算出up向量
	up_x = dir_y * side_z - dir_z * side_y;
	up_y = dir_z * side_x - dir_x * side_z;
	up_z = dir_x * side_y - dir_y * side_x;

	// 构造变换矩阵:OpenGL风格列优先
	GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
		up_x,   up_y,   up_z,   0.0,
		dir_x,  dir_y,  dir_z,  0.0,
		0.0,    0.0,    0.0,    1.0 };
	glMultMatrixd( m );
	
	pDrawBone(bone_length);

	glPopMatrix();
}
// vec3类还是有问题，
// 使用vec3有几个cylinder的半径会变化...???



// ----------------------------------------------------------------
//	Name：			Figure::process_font()
//	Description:	加载层次树.
//	Return Value:	void
// ----------------------------------------------------------------
void Figure::process_font( const tree_node* some, 
						   unsigned recurs_level )
{
	//框架中的right区域显示树图
	font_container->AddFontElem( font_pos_x + recurs_level*10, 
								 font_pos_y + font_pos_offset_y*20, -0.05, 
								 std::string("9 by 15"), some->name.c_str() );

	font_pos_offset_y++;

	for (unsigned i = 0; i < some->children.size(); i++)
		process_font(some->children[i], recurs_level+1);
}

// ----------------------------------------------------------------
//	Name：			Figure::reset_joints()
//	Description:	deactive整个Figure的所有Joint
//					用于控制每次只能选取一个节点
//					add -- 3/17/2009 RenYaFei
//	Return Value:	void
// ----------------------------------------------------------------
void Figure::reset_joints()
{
	for( vec_iter it = linear_joint_vec.begin(); 
		 it != linear_joint_vec.end();
		 it++ )
	{
		(*it)->deactive();
	}
}

// ----------------------------------------------------------------
//	Name：			Figure::init()
//	Description:	初始化
//	Return Value:	void
// ----------------------------------------------------------------
void Figure::init()
{
	frame_control = 0;
	is_play = false;
	font_container = new FontContainer;
	font_pos_offset_y = 1;
	font_pos_x = 0;
	font_pos_y = 0;

	//pDrawBone = &(Helper::myWirePyramid);
	pDrawBone = &(Helper::mySolidCylinder);
	//pDrawBone = &(Helper::mySolidPyramid);
	//pDrawBone = &(Helper::myDrawNone);
}
