#ifndef BUTTONMANGER_H_
#define BUTTONMANGER_H_

#include "util/Singleton.hpp"
#include "util/SmartPtr.h"
#include <map>
#include <string>

class CButtonMovable;

class ButtonManger : public Singleton<ButtonManger>
{
	friend class Singleton<ButtonManger>;
public:
	typedef SmartPtr<CButtonMovable> ButtonPtr;
	typedef std::map<std::string, ButtonPtr> ButtonContainer;
	typedef ButtonContainer::iterator ButtonIter;

	ButtonContainer m_vButton;
protected:
	ButtonManger();
	~ButtonManger();

public:
	ButtonPtr GetButton(std::string& id);
	void AddButton(const std::string& id, ButtonPtr& button);
	void Render();
};
#endif

// 通过name获得

//////////////////////////////////////////////////////////////////////////
//  [5/15/2009 RYF]
//		多使用typedef 