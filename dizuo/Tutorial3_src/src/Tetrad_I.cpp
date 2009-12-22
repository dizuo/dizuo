#include "Tetrad_I.h"
#include "BlocksMatrix.h"

CTetrad_I::CTetrad_I(CBlocksMatrix* pParent) : CTetrad(pParent,bcCyan)
{
}

CTetrad_I::~CTetrad_I()
{
}

bool CTetrad_I::Rotate()
{
	bool bSuccess = false;
	switch (m_Orientation)
	{
	case Rotation0:
	case Rotation180:
		if (m_pParentMatrix->IsCellFree(m_iXPos,m_iYPos-1) &&
			m_pParentMatrix->IsCellFree(m_iXPos,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos,m_iYPos+2) )
		{
			m_Orientation = Rotation90;
			bSuccess = true;
		}
		break;

	case Rotation90:
	case Rotation270:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos) &&
			m_pParentMatrix->IsCellFree(m_iXPos+2,m_iYPos) )
		{
			m_Orientation = Rotation0;
			bSuccess = true;
		}
		break;
	}
	return bSuccess;
}

bool CTetrad_I::MoveLeft()
{
	bool bSuccess = false;
	switch (m_Orientation)
	{
	case Rotation0:
	case Rotation180:
		if (m_pParentMatrix->IsCellFree(m_iXPos-2,m_iYPos) )
		{
			m_iXPos--;
			bSuccess = true;
		}
		break;

	case Rotation90:
	case Rotation270:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos-1) &&
			m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos  ) &&
			m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos+2) )
		{
			m_iXPos--;
			bSuccess = true;
		}
		break;
	}
	return bSuccess;
}

bool CTetrad_I::MoveRight()
{
	bool bSuccess = false;
	switch (m_Orientation)
	{
	case Rotation0:
	case Rotation180:
		if (m_pParentMatrix->IsCellFree(m_iXPos+3,m_iYPos) )
		{
			m_iXPos++;
			bSuccess = true;
		}
		break;

	case Rotation90:
	case Rotation270:
		if (m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos-1) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos  ) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+2) )
		{
			m_iXPos++;
			bSuccess = true;
		}
		break;
	}
	return bSuccess;
}

bool CTetrad_I::MoveDown()
{
	bool bSuccess = false;
	switch (m_Orientation)
	{
	case Rotation0:
	case Rotation180:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos+2,m_iYPos+1) )
		{
			m_iYPos++;
			bSuccess = true;
		}
		break;

	case Rotation90:
	case Rotation270:
		if (m_pParentMatrix->IsCellFree(m_iXPos,m_iYPos+3) )
		{
			m_iYPos++;
			bSuccess = true;
		}
		break;
	}
	return bSuccess;
}

void CTetrad_I::Draw()
{
	int screenX=0, screenY=0; 

	switch (m_Orientation)
	{
	case Rotation0:
	case Rotation180:
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos-1,m_iYPos,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos  ,m_iYPos,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos+1,m_iYPos,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos+2,m_iYPos,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		break;

	case Rotation90:
	case Rotation270:
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos,m_iYPos-1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos,m_iYPos  ,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos,m_iYPos+1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos,m_iYPos+2,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		break;
	}
}

void CTetrad_I::DrawOnScreen(const TRectanglei& rect)
{
	int XPos = (rect.GetWidth() - 4*BLOCK_WIDTH)/2 + rect.m_Left;
	int YPos = (rect.GetHeight() - 1*BLOCK_HEIGHT)/2 + rect.m_Top;
	m_pBlockImg->BlitImage(XPos,YPos);
	m_pBlockImg->BlitImage(XPos+BLOCK_WIDTH,YPos);
	m_pBlockImg->BlitImage(XPos+2*BLOCK_WIDTH,YPos);
	m_pBlockImg->BlitImage(XPos+3*BLOCK_WIDTH,YPos);
}

void CTetrad_I::FillMatrix()
{
	switch (m_Orientation)
	{
	case Rotation0:
	case Rotation180:
		m_pParentMatrix->FillCell(m_iXPos-1,m_iYPos,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos  ,m_iYPos,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos+1,m_iYPos,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos+2,m_iYPos,m_BlockColor);
		break;

	case Rotation90:
	case Rotation270:
		m_pParentMatrix->FillCell(m_iXPos,m_iYPos-1,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos,m_iYPos  ,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos,m_iYPos+1,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos,m_iYPos+2,m_BlockColor);
		break;
	}
}

bool CTetrad_I::IsValid()
{
	bool bValid = false;
	if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos+2,m_iYPos) )
	{
		bValid = true;
	}

	return bValid;
}
