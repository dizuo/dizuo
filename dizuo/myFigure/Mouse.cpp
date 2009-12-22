#include "Share.h"
#include "Mouse.h"

#include "util/CButton.h"
#include "Joint.h"

#define ROTATE_SPEED		1.0		// SPEED OF ROTATION

MouseManger::MouseManger()
	: isDrag(false)
	, isTrack(false)
	, isEdit(false)
{}

MouseManger::~MouseManger()
{}


// ----------------------------------------------------------------
//	Name：			MouseManger::OnMouseHandle
//	Description:	1.如果鼠标在client区域
//					按下右键拖动，按下左键旋转
//					2.响应按钮按下事件
//					播放，暂停，前进一帧，后退一帧
//	Return Value:	void
// ----------------------------------------------------------------
void MouseManger::OnMouseHandle(int button, int state, int x, int y)
{
	//重置
	if(button==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		//computeHome();
		Share::getCamera()->computeHome();
		Share::getTrack()->ComputerHome();
	}
	
	//右键drag
	if( GUI->m_pInterface->GetClient().Intersect(x, y) && isDrag )
	{
		Share::getCamera()->OnMouseHandle(button, state, x, y);
	}
	//左键track
	if( GUI->m_pInterface->GetClient().Intersect(x, y) && isTrack )
	{
		Share::getTrack()->OnMouseHandle(button, state, x, y);
	}
	
	//button hit
	if(button==GLUT_LEFT_BUTTON)
	{
		switch(state)
		{
		case GLUT_DOWN:
			if( BUTTON->GetButton( std::string("play") )->OnMouseDown(x, y) )
				Share::getFigure()->play();

			if( BUTTON->GetButton( std::string("pause") )->OnMouseDown(x, y) )
				Share::getFigure()->pause();

			if( BUTTON->GetButton( std::string("forward") )->OnMouseDown(x, y) )
				Share::getFigure()->forward();

			if( BUTTON->GetButton( std::string("backward") )->OnMouseDown(x, y) )
				Share::getFigure()->backward();

			if( BUTTON->GetButton( std::string("edit") )->OnMouseDown(x, y) )
			{
				ActiveEdit();
				DeActiveDrag();
				DeActiveTrack();
			}

			if( BUTTON->GetButton( std::string("drag") )->OnMouseDown(x, y) )
			{
				ActiveDrag();
				DeActiveTrack();
				DeActiveEdit();
			}

			if( BUTTON->GetButton( std::string("track") )->OnMouseDown(x, y) )
			{
				ActiveTrack();
				DeActiveEdit();
				DeActiveDrag();
			}

			// 点击了旋转按钮 禁止掉isDrag和isTrack

			// 为了旋转骨架准备
			vMousePos.setValue(x, y);
			if( Share::getSelectJoint() )
			{
				Joint* pJoint = Share::getSelectJoint();
				for( size_t i(0); i<pJoint->channels.size(); i++ )
				{
					Joint::Channel *channel = pJoint->channels[i];
					float rot_data = Share::getFigure()->get_motion( Share::getFigure()->get_current_framenum(), channel->index);

					if ( channel->type == Joint::Xrot )
						vGrabRot.x() = rot_data;
					else if ( channel->type == Joint::Yrot )
						vGrabRot.y() = rot_data;
					else if ( channel->type == Joint::Zrot )
						vGrabRot.z() = rot_data;
				}
			}

			break;
		case GLUT_UP:
			( BUTTON->GetButton( std::string("play") ) )->OnMouseUp();
			( BUTTON->GetButton( std::string("pause") ) )->OnMouseUp();
			( BUTTON->GetButton( std::string("forward") ) )->OnMouseUp();
			( BUTTON->GetButton( std::string("backward") ) )->OnMouseUp();
			
			if( !isEdit )
				( BUTTON->GetButton( std::string("edit") ) )->OnMouseUp();
			if( !isTrack )
				( BUTTON->GetButton( std::string("track") ) )->OnMouseUp();
			if( !isDrag )
				( BUTTON->GetButton( std::string("drag") ) )->OnMouseUp();

			break;
		}
	}
}

// ----------------------------------------------------------------
//	Name：			MouseManger::OnMotionHandle
//	Description:	右键拖动，左键旋转，移动按钮
//	Return Value:	void
// ----------------------------------------------------------------
void MouseManger::OnMotionHandle(int x, int y)
{
	// 关闭Drag和track模式，测试旋转骨骼
//  	isDrag = false;
//  	isTrack = false;
	
	// 旋转选中的节点 以下部位的skeleton
	if( isEdit )
		this->MoveSelectedSkeletons(x, y);
	
	//Right button Drag
	if( isDrag )
		Share::getCamera()->OnMotionHandle(x, y);

	//Left button TrackBall
	if( isTrack )
		Share::getTrack()->OnMotionHandle(x, y);
	
	//Move the button
	//....

	glutPostRedisplay();
}

// ----------------------------------------------------------------
//	Name：			MouseManger::MoveSelectedSkeleton
//	Description:	旋转选中节点以下的所有skeleton
//	Return Value:	void
// ----------------------------------------------------------------
void MouseManger::MoveSelectedSkeletons(int x, int y)
{
	int mousey =  Share::getWindowHeight() - y;

	Joint* pJoint = Share::getSelectJoint();
	//if ( !( ( Share::getSelectJoint() ).isNull() ) )
	if (pJoint==NULL)
		return;

	float frame = Share::getFigure()->get_current_framenum();
	for( size_t i(0); i<pJoint->channels.size(); i++ )
	{
		Joint::Channel *channel = pJoint->channels[i];
		//改变motion data
		if ( channel->type == Joint::Xrot && (x - vMousePos.x()) != 0 )
		{
			float value = vGrabRot.x() + ( (float)ROTATE_SPEED * (x - vMousePos.x()) );
			Share::getFigure()->set_motion( frame, channel->index, value );
		}
		else if ( channel->type == Joint::Yrot && (mousey - vMousePos.y()) != 0 )
		{
			float value = vGrabRot.y() + ( (float)ROTATE_SPEED * (mousey - vMousePos.y()) );
			Share::getFigure()->set_motion( frame, channel->index, value );
		}
		else if ( channel->type == Joint::Zrot && (mousey - vMousePos.y()) != 0 && (x - vMousePos.x()) != 0 )
		{
			float delta = ( (float)ROTATE_SPEED * (mousey - vMousePos.y()) ) + ( (float)ROTATE_SPEED * (x - vMousePos.x()) );
			float value = vGrabRot.z() + delta / 2.0f;
			Share::getFigure()->set_motion( frame, channel->index, value );
		}
	}
}


// ----------------------------------------------------------------
//	Name：			MouseManger::PickJointByMouse
//	Description:	通过鼠标直接点击选取节点
//	Return Value:	void
// ----------------------------------------------------------------
void MouseManger::PickJointByMouse(int button, int state, int x, int y)
{
	if( button != GLUT_LEFT || state != GLUT_DOWN)
		return;

	#define MyVector Share::getFigure()->get_joints_vector()

	Share::getFigure()->reset_joints();
	for (size_t i(0); i<MyVector.size(); i++)
	{
		//选中节点了:
		if( x > MyVector[i]->screen_pos[0] - PickThreshold &&
			x < MyVector[i]->screen_pos[0] + PickThreshold &&
			y > MyVector[i]->screen_pos[1] - PickThreshold &&
			y < MyVector[i]->screen_pos[1] + PickThreshold )
		{
			MyVector[i]->active();
			Share::setSelectJoint( MyVector[i] );
		}
	}
}

// ----------------------------------------------------------------
//	Name：			MouseManger::SelectJointByName
//	Description:	通过左边的节点name选取节点
//	Return Value:	void
// ----------------------------------------------------------------
void MouseManger::SelectJointByName(int button, int state, int x, int y)
{
	//右边结构树选取
	if(button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	if( Share::getFigure()->font_container->OnMouseDown(x, y) )
	{
		//在激活一个之前对所有的Joint执行reset
		Share::getFigure()->reset_joints();
		(Share::getFigure()->get_joint( Share::getFigure()->font_container->GetSelectName() ))->active();

		Share::setSelectJoint( Share::getFigure()->get_joint( Share::getFigure()->font_container->GetSelectName() ) );
	}
	else
		Share::getFigure()->reset_joints();
}