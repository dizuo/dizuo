#ifndef _TETRADI_H_
#define _TETRADI_H_

#include "Tetrad.h"

class CTetrad_I : public CTetrad
{
public:
	CTetrad_I(CBlocksMatrix* pParent);
	~CTetrad_I();

	bool Rotate();
	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();
	void FillMatrix();
	bool IsValid();

	void Draw();
	void DrawOnScreen(const TRectanglei& rect);
};

#endif  // _TETRADI_H_