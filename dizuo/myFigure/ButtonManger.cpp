#include "ButtonManger.h"
#include "util/CButton.h"

ButtonManger::ButtonManger()
{
	m_vButton.clear();
}

ButtonManger::~ButtonManger()
{}

void ButtonManger::AddButton(const std::string& id, ButtonPtr& pButton)
{
	m_vButton[ id ] = pButton;
}

void ButtonManger::Render()
{
	for ( ButtonIter it = m_vButton.begin();
		it != m_vButton.end();
		it++ )
	{
		(it->second)->Render();
	}
}

ButtonManger::ButtonPtr ButtonManger::GetButton(std::string& id)
{
	return m_vButton[ id ];
}