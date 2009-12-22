#ifndef _TETRADT_H_
#define _TETRADT_H_

#include "Tetrad.h"

class CTetrad_T : public CTetrad
{
public:
	CTetrad_T(CBlocksMatrix* pParent);
	~CTetrad_T();

	bool Rotate();
	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();
	void FillMatrix();
	bool IsValid();

	void Draw();
	void DrawOnScreen(const TRectanglei& rect);
};

#endif  // _TETRADT_H_
