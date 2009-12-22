#ifndef _TETRADFACTORY_H_
#define _TETRADFACTORY_H_

#include "Tetrad.h"

class CBlocksMatrix;

// This class simply creates random tetrads.
class CTetradFactory
{
public:
	CTetradFactory();
	~CTetradFactory();

	CTetrad* CreateTetrad(CBlocksMatrix* pMatrix);
};

#endif  // _TETRADFACTORY_H_
