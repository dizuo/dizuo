#ifndef _TETRADJ_H_
#define _TETRADJ_H_

#include "Tetrad.h"

class CTetrad_J : public CTetrad
{
public:
	CTetrad_J(CBlocksMatrix* pParent);
	~CTetrad_J();

	bool Rotate();
	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();
	void FillMatrix();
	bool IsValid();

	void Draw();
	void DrawOnScreen(const TRectanglei& rect);
};

#endif  // _TETRADJ_H_