#ifndef _BLOCKSMATRIX_H_
#define _BLOCKSMATRIX_H_

#include "Image.h"
#include "Constants.h"
#include "TetradFactory.h"
#include <vector>

class CTetrad;

// Listener for the blocks matrix
class CMatrixEventsListener
{
public:
	CMatrixEventsListener()  { }
	virtual ~CMatrixEventsListener()  { }

	// Virtual function called when filled lines are
	// detected and will start blinking
	virtual void OnStartRemoveLines() = 0;
	// Virtual function called when the filled lines have
	// been removed
	virtual void OnLinesRemoved(int iLinesCount) = 0;
	// Virtual function called when the game is over.
	virtual void OnMatrixFull() = 0;
};

// Class managing the playing area (where the shapes are
// falling). It handles all the logic related to lines.
class CBlocksMatrix
{
public:
	// Constructor and destructor
	CBlocksMatrix(CMatrixEventsListener* pListener, int xPos, int yPos);
	~CBlocksMatrix();

	// Draw and update the matrix
	void Draw();
	void Update(DWORD dwCurrentTime);
	// Reset the matrix to its initial state
	void Reset();

	// Move the current shape
	void ShapeLeft();
	void ShapeRight();
	void ShapeDown();
	void ShapeRotate();

	// Check if the specified cell is free or not.
	bool IsCellFree(int XPos, int YPos);
	// Fill the specified cell with a specific block color
	void FillCell(int XPos, int YPos, EBlockColor BlockColor);
	// Transform a cell coordinates into screen coordinates.
	void GetScreenPosFromCell(int cellXPos, int cellYPos, 
		int& screenXPos, int& screenYPos);

	// Returns the next shape
	CTetrad* GetNextShape() const  { return m_pNextShape; }

	// Sets/Gets the time between two update of the current
	// shape (determines the speed at which it falls).
	void SetTetradUpdate(int iNewUpdate)  { m_iTetradUpdate = iNewUpdate; }
	int  GetTetradUpdate() const  { return m_iTetradUpdate; }

private:
	// Check if there are lines completed in the
	// matrix. This returns true if at least one
	// line is complete
	bool CheckMatrix();
	// Check if the specified line is currently being
	// removed
	bool IsLineRemoved(int iRow);
	// Remove the lines that are complete from the
	// matrix and adjust the remaining blocks.
	void RemoveLines();
	// Tries to create a new shape. If this is not
	// possible (e.g. matrix full), m_bGameOver is
	// set to true.
	void NewShape();

	// The screen coordinates of the top-left 
	// corner.
	int m_iXPos;
	int m_iYPos;

	// The matrix of blocks which are already filled
	int m_pBlocksMatrix[MATRIX_WIDTH][MATRIX_HEIGHT];

	// The images of the 7 different blocks
	TImagePtr m_pBlockImg[7];

	// The tetrad factory
	CTetradFactory m_TetradFactory;
	// Current shape that the player manipulates
	CTetrad* m_pTetrad;
	// Next shape
	CTetrad* m_pNextShape;
	// The last move down of the current shape
	DWORD m_dwLastShapeDown;

	// Flag indicating that one or more
	// lines are being removed (blinking)
	bool m_bRemovingLine;
	// The number of times the line being removed
	// has already blinked.
	int	 m_iLineBlinkCount;
	// Specify if the the line being removed is currently
	// visible or not (for blinking)
	bool m_bLineBlinkOn;
	// Vector containing the line numbers of the
	// lines being removed.
	std::vector<int> m_vecLinesRemoved;

	// The event listener
	CMatrixEventsListener* m_pListener;
	// Time (in msec) before the tetrad is moved down
	// one step.
	int m_iTetradUpdate;

	// Flag indicating a game over.
	bool m_bGameOver;
};

#endif  // _BLOCKSMATRIX_H_