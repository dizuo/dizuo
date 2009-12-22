#ifndef _TETRADI_S_
#define _TETRADI_S_

#include "Tetrad.h"

class CTetrad_S : public CTetrad
{
public:
	CTetrad_S(CBlocksMatrix* pParent);
	~CTetrad_S();

	bool Rotate();
	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();
	void FillMatrix();
	bool IsValid();

	void Draw();
	void DrawOnScreen(const TRectanglei& rect);
};

#endif  // _TETRADS_H_
