#ifndef _TETRADL_H_
#define _TETRADL_H_

#include "Tetrad.h"

class CTetrad_L : public CTetrad
{
public:
	CTetrad_L(CBlocksMatrix* pParent);
	~CTetrad_L();

	bool Rotate();
	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();
	void FillMatrix();
	bool IsValid();

	void Draw();
	void DrawOnScreen(const TRectanglei& rect);
};

#endif  // _TETRADL_H_