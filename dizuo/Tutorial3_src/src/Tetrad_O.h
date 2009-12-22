#ifndef _TETRADO_H_
#define _TETRADO_H_

#include "Tetrad.h"

class CTetrad_O : public CTetrad
{
public:
	CTetrad_O(CBlocksMatrix* pParent);
	~CTetrad_O();

	bool Rotate();
	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();
	void FillMatrix();
	bool IsValid();

	void Draw();
	void DrawOnScreen(const TRectanglei& rect);
};

#endif  // _TETRADO_H_
