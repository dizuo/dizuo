#include "BlocksMatrix.h"
#include "Rectangle.h"

using namespace std;


CBlocksMatrix::CBlocksMatrix(CMatrixEventsListener* pListener, int xPos, int yPos) 
  : m_iXPos(xPos), m_iYPos(yPos), m_pTetrad(NULL),
    m_bRemovingLine(false), m_iLineBlinkCount(0), m_bLineBlinkOn(false), 
	m_vecLinesRemoved(), m_pListener(pListener), m_iTetradUpdate(1000),
	m_bGameOver(false)
{
	// Load the block images
	m_pBlockImg[0] = CImage::CreateImage("Block.PNG",TRectanglei(0,BLOCK_HEIGHT,0,BLOCK_WIDTH));
	m_pBlockImg[1] = CImage::CreateImage("Block.PNG",TRectanglei(0,BLOCK_HEIGHT,BLOCK_WIDTH,2*BLOCK_WIDTH));
	m_pBlockImg[2] = CImage::CreateImage("Block.PNG",TRectanglei(0,BLOCK_HEIGHT,2*BLOCK_WIDTH,3*BLOCK_WIDTH));
	m_pBlockImg[3] = CImage::CreateImage("Block.PNG",TRectanglei(0,BLOCK_HEIGHT,3*BLOCK_WIDTH,4*BLOCK_WIDTH));
	m_pBlockImg[4] = CImage::CreateImage("Block.PNG",TRectanglei(0,BLOCK_HEIGHT,4*BLOCK_WIDTH,5*BLOCK_WIDTH));
	m_pBlockImg[5] = CImage::CreateImage("Block.PNG",TRectanglei(BLOCK_HEIGHT,2*BLOCK_HEIGHT,0,BLOCK_WIDTH));
	m_pBlockImg[6] = CImage::CreateImage("Block.PNG",TRectanglei(BLOCK_HEIGHT,2*BLOCK_HEIGHT,BLOCK_WIDTH,2*BLOCK_WIDTH));

	// Initialize the matrix
	for (int i=0; i<MATRIX_WIDTH;i++)
	{
		for (int j=0; j<MATRIX_HEIGHT;j++)
		{
			m_pBlocksMatrix[i][j] = bcNone;
		}
	}

	// Create the current and next shapes
	m_pTetrad = m_TetradFactory.CreateTetrad(this);
	m_pNextShape = m_TetradFactory.CreateTetrad(this);
	m_dwLastShapeDown = GetCurrentTime();
}

CBlocksMatrix::~CBlocksMatrix()
{
}

void CBlocksMatrix::Reset()
{
	for (int i=0; i<MATRIX_WIDTH;i++)
	{
		for (int j=0; j<MATRIX_HEIGHT;j++)
		{
			m_pBlocksMatrix[i][j] = bcNone;
		}
	}

	delete m_pTetrad;
	delete m_pNextShape;
	m_pTetrad = m_TetradFactory.CreateTetrad(this);
	m_pNextShape = m_TetradFactory.CreateTetrad(this);
	m_dwLastShapeDown = GetCurrentTime();

    m_bRemovingLine = m_bLineBlinkOn = false; 
	m_iLineBlinkCount = 0;
	m_vecLinesRemoved.clear();
	m_iTetradUpdate = 1000;
	m_bGameOver = false;
}

void CBlocksMatrix::Draw()
{
	int iBlockX=0, iBlockY=0;
	// If some lines are currently being removed,
	// We shouldn't draw them all.
	if (m_bRemovingLine)
	{
		for (int j=0; j<MATRIX_HEIGHT;j++)
		{
			// Don't draw the line if it is being removed and blinking off
			if (IsLineRemoved(j) && !m_bLineBlinkOn)
				continue;

			// Else draw the line
			for (int i=0; i<MATRIX_WIDTH;i++)
			{
				if (m_pBlocksMatrix[i][j])
				{
					int color = m_pBlocksMatrix[i][j]-1;
					GetScreenPosFromCell(i, j, iBlockX, iBlockY);
					m_pBlockImg[color]->BlitImage(iBlockX, iBlockY);
				}			
			}
		}

		// Switch the blinking
		if (m_bLineBlinkOn)
			m_bLineBlinkOn = false;
		else
			m_bLineBlinkOn = true;
		m_iLineBlinkCount++;
		// If blink count equals 10, we stop blinking and remove
		// the lines.
		if (m_iLineBlinkCount == 10)
		{
			RemoveLines();
			m_bRemovingLine = false;
			m_bLineBlinkOn = false;
			m_iLineBlinkCount = 0;
			NewShape();
		}
	}
	else 
	{
		// Draw filled blocks
		for (int j=0; j<MATRIX_HEIGHT;j++)
		{
			for (int i=0; i<MATRIX_WIDTH;i++)
			{
				if (m_pBlocksMatrix[i][j])
				{
					int color = m_pBlocksMatrix[i][j]-1;
					GetScreenPosFromCell(i, j, iBlockX, iBlockY);
					m_pBlockImg[color]->BlitImage(iBlockX, iBlockY);
				}
			}
		}

		// Finally, draw the current shape
		if (!m_bGameOver)
			m_pTetrad->Draw();
	}

}

void CBlocksMatrix::Update(DWORD dwCurrentTime)
{
	if (!m_bGameOver)
	{
		// Check if the current shape should be moved down
		if (dwCurrentTime > m_dwLastShapeDown+m_iTetradUpdate)
			ShapeDown();
	}
}

void CBlocksMatrix::ShapeLeft()
{
	if (m_pTetrad)
		m_pTetrad->MoveLeft();
}

void CBlocksMatrix::ShapeRight()
{
	if (m_pTetrad)
		m_pTetrad->MoveRight();
}

void CBlocksMatrix::ShapeDown()
{
	if (m_pTetrad && !m_pTetrad->MoveDown())
	{
		// If the current shape can't move down,
		// we ask it to fill the matrix.
		m_pTetrad->FillMatrix();
		// Then delete the current shape
		delete m_pTetrad;	
		m_pTetrad = NULL;
		
		// We then check if no lines have been completed
		// and create the next shape. The m_bGameOver flag
		// can be set in this NewShape function.
		if (!CheckMatrix())
			NewShape();
	}
	// Set the last update (down) of the shape to 
	// the current time.
	m_dwLastShapeDown = GetCurrentTime();
}

void CBlocksMatrix::NewShape()
{
	m_pTetrad = m_pNextShape;
	m_pNextShape = NULL;
	if (!m_pTetrad->IsValid())
	{
		// If the new shape occupies filled cells,
		// the game is over
		m_bGameOver = true;
		// Inform the listener about it.
		m_pListener->OnMatrixFull();
	}
	else
		m_pNextShape = m_TetradFactory.CreateTetrad(this);
}

void CBlocksMatrix::ShapeRotate()
{
	if (m_pTetrad)
		m_pTetrad->Rotate();
}

bool CBlocksMatrix::IsCellFree(int XPos, int YPos)
{
	if ( (XPos<0) || (XPos>=MATRIX_WIDTH) )
		return false;
	if ( (YPos<0) || (YPos>=MATRIX_HEIGHT) )
		return false;

	if (m_pBlocksMatrix[XPos][YPos] == bcNone)
		return true;
	return false;
}

void CBlocksMatrix::FillCell(int XPos, int YPos, EBlockColor BlockColor)
{
	if ( (XPos<0) || (XPos>=MATRIX_WIDTH) )
		return;
	if ( (YPos<0) || (YPos>=MATRIX_HEIGHT) )
		return;

	m_pBlocksMatrix[XPos][YPos] = BlockColor;
}

void CBlocksMatrix::GetScreenPosFromCell(int cellXPos, 
										 int cellYPos, 
										 int& screenXPos, 
										 int& screenYPos)
{
	screenXPos = m_iXPos + BLOCK_WIDTH * cellXPos;	
	screenYPos = m_iYPos + BLOCK_HEIGHT * cellYPos;	
}

bool CBlocksMatrix::CheckMatrix()
{
	bool bOneLineComplete = false;
	bool bComplete = true;
	// Scan all the lines to see if they are complete
	for (int iRow=0; iRow<MATRIX_HEIGHT;iRow++)
	{
		bComplete = true;
		for (int j=0; j<MATRIX_WIDTH;j++)
		{
			if (m_pBlocksMatrix[j][iRow] == bcNone)
			{
				bComplete = false;
				break;
			}
		}

		// If the current line is complete
		if (bComplete)
		{
			// Push the line index in the vector.
			m_vecLinesRemoved.push_back(iRow);
			// Specify that we switch to the removing line state.
			m_bRemovingLine = true;
			bOneLineComplete = true;
			m_pListener->OnStartRemoveLines();
		}
	}

	return bOneLineComplete;
}

bool CBlocksMatrix::IsLineRemoved(int iRow)
{
	bool bFound = false;
	vector<int>::iterator iter = m_vecLinesRemoved.begin();
	for (iter; iter!=m_vecLinesRemoved.end(); iter++)
	{
		if (iRow == (*iter) )
		{
			bFound = true;
			break;
		}
	}

	return bFound;
}

void CBlocksMatrix::RemoveLines()
{
	vector<int>::iterator iter = m_vecLinesRemoved.begin();
	for (iter; iter!=m_vecLinesRemoved.end();iter++)
	{
		for (int j=0; j<MATRIX_WIDTH;j++)
		{
			// set the current line to empty
			m_pBlocksMatrix[j][(*iter)] = bcNone;

			// Move all the upper blocks one cell down
			for (int iNewRow=(*iter);iNewRow>1;iNewRow--)
			{
				m_pBlocksMatrix[j][iNewRow] = m_pBlocksMatrix[j][iNewRow-1];
			}
		}
	}

	if (m_pListener)
		m_pListener->OnLinesRemoved((int)m_vecLinesRemoved.size());

	m_vecLinesRemoved.clear();
}