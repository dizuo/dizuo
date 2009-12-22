#include "Tetrad_O.h"
#include "Constants.h"
#include "BlocksMatrix.h"

CTetrad_O::CTetrad_O(CBlocksMatrix* pParent) : CTetrad(pParent,bcYellow)
{
}

CTetrad_O::~CTetrad_O()
{
}

bool CTetrad_O::Rotate()
{
	// For a O tetrad, the rotation has no effect
	return true;
}

bool CTetrad_O::MoveLeft()
{
	if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos+1))
	{
		m_iXPos--;
		return true;
	}

	return false;
}

bool CTetrad_O::MoveRight()
{
	if (m_pParentMatrix->IsCellFree(m_iXPos+2,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos+2,m_iYPos+1))
	{
		m_iXPos++;
		return true;
	}

	return false;
}

bool CTetrad_O::MoveDown()
{
	if (m_pParentMatrix->IsCellFree(m_iXPos,m_iYPos+2) &&
		m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+2))
	{
		m_iYPos++;
		return true;
	}
	return false;
}

void CTetrad_O::Draw()
{
	int screenX=0, screenY=0; 
	m_pParentMatrix->GetScreenPosFromCell(m_iXPos,m_iYPos,screenX,screenY);
	m_pBlockImg->BlitImage(screenX,screenY);
	m_pParentMatrix->GetScreenPosFromCell(m_iXPos+1,m_iYPos,screenX,screenY);
	m_pBlockImg->BlitImage(screenX,screenY);
	m_pParentMatrix->GetScreenPosFromCell(m_iXPos,m_iYPos+1,screenX,screenY);
	m_pBlockImg->BlitImage(screenX,screenY);
	m_pParentMatrix->GetScreenPosFromCell(m_iXPos+1,m_iYPos+1,screenX,screenY);
	m_pBlockImg->BlitImage(screenX,screenY);
}

void CTetrad_O::DrawOnScreen(const TRectanglei& rect)
{
	int XPos = (rect.GetWidth() - 2*BLOCK_WIDTH)/2 + rect.m_Left;
	int YPos = (rect.GetHeight() - 2*BLOCK_HEIGHT)/2 + rect.m_Top;
	m_pBlockImg->BlitImage(XPos,YPos);
	m_pBlockImg->BlitImage(XPos+BLOCK_WIDTH,YPos);
	m_pBlockImg->BlitImage(XPos,YPos+BLOCK_HEIGHT);
	m_pBlockImg->BlitImage(XPos+BLOCK_WIDTH,YPos+BLOCK_HEIGHT);
}

void CTetrad_O::FillMatrix()
{
	m_pParentMatrix->FillCell(m_iXPos,m_iYPos,m_BlockColor);
	m_pParentMatrix->FillCell(m_iXPos+1,m_iYPos,m_BlockColor);
	m_pParentMatrix->FillCell(m_iXPos,m_iYPos+1,m_BlockColor);
	m_pParentMatrix->FillCell(m_iXPos+1,m_iYPos+1,m_BlockColor);
}

bool CTetrad_O::IsValid()
{
	bool bValid = false;
	if (m_pParentMatrix->IsCellFree(m_iXPos,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos,m_iYPos+1) &&
		m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+1) )
	{
		bValid = true;
	}

	return bValid;
}