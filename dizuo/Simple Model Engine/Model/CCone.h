#ifndef CCONE_H_
#define CCONE_H_

#include "Model.h"

// Without texture
class CCone : public CModel
{
public:
	CCone(Real h, Real botR, int cirP=20, bool b=true);
	~CCone();

	virtual bool Setup();
	virtual void Render();

private:
	Real	botRadius;
	Real	height;
	bool	isBottomCap;
	int		cirPrecision;
};

#endif