/*************************************************************/
/**					Filename: Keyboard.H		            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  3/14/2009		RYF - Creation							**/	
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "util/Singleton.hpp"
#include <string>

class KeyboardManger : public Singleton<KeyboardManger>
{
	friend class Singleton<KeyboardManger>;
protected:
	KeyboardManger(){};
	~KeyboardManger(){};

	std::string fileName;
public:
	void OnKeyHandle(unsigned char key, int x, int y);
	void OnSpecialHandle(int key, int x, int y);
};

// [3/14/2009 RenYaFei]
//	单件类后面必须显示的freeInstance().
#endif