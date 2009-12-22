#include "TetradFactory.h"
#include "Tetrad_O.h"
#include "Tetrad_I.h"
#include "Tetrad_S.h"
#include "Tetrad_Z.h"
#include "Tetrad_T.h"
#include "Tetrad_J.h"
#include "Tetrad_L.h"
#include <time.h>

CTetradFactory::CTetradFactory()
{
	srand ( (unsigned int)time(NULL) );
}

CTetradFactory::~CTetradFactory()
{
}

CTetrad* CTetradFactory::CreateTetrad(CBlocksMatrix* pMatrix)
{
	CTetrad* pNewTetrad = NULL;

	int value = rand()%7;
	switch (value)
	{
	case 0:
		pNewTetrad = new CTetrad_I(pMatrix);
		break;
	case 1:
		pNewTetrad = new CTetrad_O(pMatrix);
		break;
	case 2:
		pNewTetrad = new CTetrad_S(pMatrix);
		break;
	case 3:
		pNewTetrad = new CTetrad_Z(pMatrix);
		break;
	case 4:
		pNewTetrad = new CTetrad_T(pMatrix);
		break;
	case 5:
		pNewTetrad = new CTetrad_J(pMatrix);
		break;
	case 6:
		pNewTetrad = new CTetrad_L(pMatrix);
		break;
	}

	return pNewTetrad;
}
