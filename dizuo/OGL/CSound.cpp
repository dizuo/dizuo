#include "CSound.h"

CSound::CSound()
{}

CSound::~CSound()
{}

void CSound::Play(CSound::SoundType Type)
{
	char *soundPath;
	switch(Type)
	{
	case 0:
		soundPath = "Data\\Sound\\move.wav";
		break;
	case 1:
		soundPath = "Data\\Sound\\Soil -Redefine.wav";
		break;
	default:
		return;
	}
	PlaySound(soundPath, NULL, SND_ASYNC);
}