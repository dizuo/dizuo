/*************************************************************/
/**					Filename: CSOUND.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  18/05/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef CSOUND_H
#define CSOUND_H

#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class CSound
{
public:
	enum SoundType{
		MOVE,
		STOP,
		FLY		
	};
	CSound();
	~CSound();
	void Play(SoundType Type);
};

#endif