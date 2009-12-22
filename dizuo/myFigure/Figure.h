//使用unix命名格式:
#ifndef FIGURE_H
#define FIGURE_H

#include "Joint.h"
//#include "util/Font.h"
#include <iostream>
#include <map>

#include "util/global.h"
#include "FontContainer.h"

#include "BvhEditer.h"

class Figure : public BvhEditer
{
public:
	typedef Joint part;
	typedef void (*pFunc) (GLdouble);

	Figure();
	Figure(std::string _name);
	Figure(const char * _name);
	~Figure();

	//animation control:
	bool	 update();
	void	 play();
	void	 pause();
	void	 forward();
	void	 backward();

	//render function
	void	 render_figure(float scale=1.0, GLenum mode=GL_RENDER);
	void	 render(part* some, size_t frame, float scale = 1.0, GLenum mode=GL_RENDER);
	void	 render_bone(double x0, double y0, double z0, double x1, double y1, double z1);
	
	//select function
	void	 reset_joints();	//重置所有的节点
	void	 init();

	void	 process_font(const tree_node* some, unsigned recurs_level);
public:
	void	 set_draw_mode(pFunc drawfunc){ pDrawBone = drawfunc; }
	void	 set_font_pos(int x, int y){font_pos_x = x;font_pos_y = y;}
	int		 get_current_framenum() const{return frame_control;}
	
	FontContainer	*font_container;
private:
	//void (*pDrawBone)(GLdouble);
	pFunc		pDrawBone;
	bool		is_play;
	int			frame_control;

	//结构树显示
	int			font_pos_offset_y;
	int			font_pos_x, font_pos_y;	//字体起始位置
	//显示字体，垂直方向是线性增加的，但是x方向确实螺旋式，
};
//////////////////////////////////////////////////////////////////////////
//提供多种setup方法
//		可以手动输入，以脚本形式，应用迭代器
//		可以以文件的新式
// [3/11/2009 RenYaFei]
//	增加了字体显示
// [3/16/2009 RenYaFei]
//		字体显示问题：font_pos_offset_y控制y方向偏移，线性的...
//				  recurs_level恰好可以控制x方向的，螺旋式的...
//		font_container提供右边的字体选取交互
// [3/17/2009 RenYaFei]
//		增加了reset_joints函数
// [4/1/2009 RenYaFei]
//		重构Figure类：
//		去掉map存储结构，使用线性vector，
//		调整reset实现
//		去掉proce_file()
//		Figure继承于htree基类
//		Figure继承于BvhEditer
//  [4/4/2009 RenYaFei]
//		增加函数指针实现多种模式绘制
//  [4/5/2009 RenYaFei]
//		有必要增加一个Bone类吗.
//  [4/23/2009 RenYaFei]
//		offset坐标是世界坐标系下的绝对坐标...
/*
+-----------------------------------------------------------+
|                                                           |
|                          o                                |
|                          | Head                           |
|        Right.shoulder    |   Left.shoulder                |
|                  o-------o-------o                        |
|  Right.upperarm  |       |Upper- | Left.upperarm          |
|                  |       |back   |                        |
|                  o       |       o                        |
|  Right.lowerarm  |       |       | Left.lowerarm          |
|                  |       oLower- |                        |
|                  o       |back   o                        |
|  Right.hand      |       |       | Left.hand              |
|                          |                                |
|               Right.butt | Left.butt                      |
|                       o--o--o                             |
|                       |     |                             |
|                       |     |                             |
|       Right.upperleg  |     | Left.upperleg               |
|                       |     |                             |
|                       o     o                             |
|                       |     |                             |
|                       |     |                             |
|       Right.lowerleg  |     | Left.lowerleg               |
|                       o     o                             |
|       Right.foot     /     /  Left.foot                   |
|                                                           |
|                                                           |
+-----------------------------------------------------------+
*/

#endif 