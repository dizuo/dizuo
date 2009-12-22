#ifndef _TETRAD_H_
#define _TETRAD_H_

#include "Image.h"
#include "Constants.h"

class CBlocksMatrix;

// Base class for all shapes (tetrad)
class CTetrad
{
public:
	// Construct a new tetrad. The image of the block used to draw
	// the tetrad is loaded depending of the tetrad color.
	CTetrad(CBlocksMatrix* pParent, EBlockColor blockColor) 
		: m_pParentMatrix(pParent), m_iXPos(4), m_iYPos(0), 
		  m_Orientation(Rotation0), m_pBlockImg(NULL), m_BlockColor(blockColor)  
	{ 
		switch (blockColor)
		{
		case bcCyan:
			m_pBlockImg = CImage::CreateImage("Block.PNG",
					TRectanglei(0,BLOCK_HEIGHT,0,BLOCK_WIDTH));
			break;
		case bcBlue:
			m_pBlockImg = CImage::CreateImage("Block.PNG",
					TRectanglei(0,BLOCK_HEIGHT,BLOCK_WIDTH,2*BLOCK_WIDTH));
			break;
		case bcOrange:
			m_pBlockImg = CImage::CreateImage("Block.PNG",
					TRectanglei(0,BLOCK_HEIGHT,2*BLOCK_WIDTH,3*BLOCK_WIDTH));
			break;
		case bcYellow:
			m_pBlockImg = CImage::CreateImage("Block.PNG",
					TRectanglei(0,BLOCK_HEIGHT,3*BLOCK_WIDTH,4*BLOCK_WIDTH));
			break;
		case bcGreen:
			m_pBlockImg = CImage::CreateImage("Block.PNG",
					TRectanglei(0,BLOCK_HEIGHT,4*BLOCK_WIDTH,5*BLOCK_WIDTH));
			break;
		case bcPurple:
			m_pBlockImg = CImage::CreateImage("Block.PNG",
					TRectanglei(BLOCK_HEIGHT,2*BLOCK_HEIGHT,0,BLOCK_WIDTH));
			break;
		case bcRed:
			m_pBlockImg = CImage::CreateImage("Block.PNG",
					TRectanglei(BLOCK_HEIGHT,2*BLOCK_HEIGHT,BLOCK_WIDTH,2*BLOCK_WIDTH));
			break;
		}
	}
	virtual ~CTetrad() { }

	// Tries to rotate the tetrad. If it can't be rotated, 
	// the function returns false.
	virtual bool Rotate() = 0;
	// Tries to move the tetrad to the left. If it can't be 
	// moved, the function returns false.
	virtual bool MoveLeft() = 0;
	// Tries to move the tetrad to the right. If it can't be 
	// moved, the function returns false.
	virtual bool MoveRight() = 0;
	// Tries to move the tetrad down. If it can't be 
	// moved, the function returns false.
	virtual bool MoveDown() = 0;

	// Ask the tetrad to fill the cells in the matrix.
	// This function is called when the tetrad is positioned.
	virtual void FillMatrix() = 0;
	// Checks if the tetrad is at a valid position (do not 
	// overlap with a filled cell in the matrix). This is 
	// called when the tetrad is created to check for game over.
	virtual bool IsValid() = 0;

	// Draw the tetrad at its position in the matrix.
	virtual void Draw() = 0;
	// Draw the tetrad somewhere on the screen (used to
	// display the next shape). The tetrad is centered
	// in the rectangle.
	virtual void DrawOnScreen(const TRectanglei& rect) = 0;

protected:
	// The play area in which the tetrad is used
	CBlocksMatrix* m_pParentMatrix;

	// The position in the play area (in
	// blocks).
	int m_iXPos;
	int m_iYPos;

	enum EOrientation
	{
		Rotation0,
		Rotation90,
		Rotation180,
		Rotation270,
	};
	// Orientation of the tetrad
	EOrientation m_Orientation;

	// The block image use to draw the tetrad.
	TImagePtr m_pBlockImg;
	// The block color.
	EBlockColor m_BlockColor;
};

#endif  // _TETRAD_H_