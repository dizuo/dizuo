/*************************************************************/
/**					Filename: SCENE.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  05/06/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef __SCENEGUI_H
#define __SCENEGUI_H

#include	"TrackUI.h"

#include	"CSound.h"

#include	"Font.h"
#include	"Role.h"

#include	"SkyBox.h"
#include	"Wall.h"
#include	"Ground.h"
#include	"OBJ_3DS.h"

#include	<vector>
#include	"Model_3DS.h"

#define SAFE_DELETE(p) {if(p){delete p; p = NULL;}}
#define FONT_NUM	27

class Scene
{
public:
	enum APPLY_MODE {
		SCENE,
		ROLE, 
		HOUSE, 
		TEAPOT, 
		GUN
	};

	Scene();
	~Scene();

	bool SceneInit();
	void SceneDisplay();

	void OnMouseHandle(int btn, int state, int x, int y);
	void OnMotionHandle(int x, int y);
	void KeyboardHandle();

	void Apply();
	void ComputerHome();

	void SetWindow(int x, int y);
	void SetApplyMode(int i){m_ApplyMode = i;}

public:
	static char* m_Control[FONT_NUM];

private:
	int			m_iWindowWid;
	int			m_iWindowHigh;

	int			m_ApplyMode;
	gtl::Vec3f	m_vCheckOffset;
	
	TrackUI		*m_Track;

	Font		*m_Font;

	Wall		*m_Wall;
	Ground		*m_Ground;
	CSkyBox		*m_Sky;
	CSound		*m_Sound;

	Role		*m_Role;
	Obj_3DS		*m_House;
	Obj_3DS		*m_Weapon;
};

#endif