#ifndef _TETRADI_Z_
#define _TETRADI_Z_

#include "Tetrad.h"

class CTetrad_Z : public CTetrad
{
public:
	CTetrad_Z(CBlocksMatrix* pParent);
	~CTetrad_Z();

	bool Rotate();
	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();
	void FillMatrix();
	bool IsValid();

	void Draw();
	void DrawOnScreen(const TRectanglei& rect);
};

#endif  // _TETRADI_Z_
