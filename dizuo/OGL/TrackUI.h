/*************************************************************/
/**					Filename: TRACKUI.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  05/06/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**					All Rights Reserved.					**/
/*************************************************************/
#include	"gtl/vec3.hpp"
#include	"TrackBall.h"

class TrackUI
{
public:
	TrackUI();
	~TrackUI();
	
	void OnMouseHandle(int,int,int,int);
	void OnMotionHandle(int,int);

	void Apply();
	void ComputerHome();
	
	void SetWnd(int x, int y){
		m_Ball->SetWnd(x, y);
	}
	Trackball* Get(){return m_Ball;}
private:
	bool isClicked;
	bool isDragging;

	Trackball* m_Ball;
};
