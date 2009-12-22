#pragma once

#include	<map>
#include	<string>
#include	"Singleton.hpp"
#include	"Camera.h"

using std::map;
using std::string;

typedef map< string, CCamera* >  CameraList;

class CameraMgr : public Singleton<CameraMgr>
{
	friend Singleton<CameraMgr>;
private:
	CameraList m_vCamera;
	CCamera *m_pActiveCamera;

private:
	CameraMgr()
		:m_pActiveCamera(NULL) 
	{}

	~CameraMgr()
	{
		CameraList::iterator iter;
		for( iter = m_vCamera.begin(); 
			iter != m_vCamera.end(); 
			iter++ )
		{
			delete iter->second;
			iter->second = 0;
		}
	}
public:

	inline CCamera* getActive() const;
	inline CCamera* get( const string &id );
	inline void apply() const;
	inline void update() const;

	template<class T>
	T* add( const string &id, bool bMakeActive = false );

	inline void activate( const string &id );
};

template<class T>
T* CameraMgr::add( const string &id, bool bMakeActive )
{
	T *newCam = new T;

	m_vCamera[ id ] = static_cast<CCamera*>(newCam);

	if( bMakeActive || !m_pActiveCamera ) 
		m_pActiveCamera = newCam;

	return newCam;
}

CCamera* CameraMgr::getActive() const { 
	return m_pActiveCamera; 
}

CCamera* CameraMgr::get( const string &id ) { 
	return m_vCamera[ id ]; 
}

void CameraMgr::apply() const { 
	m_pActiveCamera->Apply(); 
}

void CameraMgr::update() const{
	m_pActiveCamera->Update();
}

void CameraMgr::activate( const string &id ) {
	m_pActiveCamera = m_vCamera[ id ];
}