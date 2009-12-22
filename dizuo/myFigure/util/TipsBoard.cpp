#include "Helper.h"
#include "TipsBoard.h"

char* TipsBoard::FontList[BUFFERSIZE] = {
	"The Bvh Backer", 
	"Esc, Q, q - Quit the demo",
	"LMouse Button - Track the figure",
	"LMouse Motion - Move the skeleton",
	"RMouse Button - Drag the figure is Z aixs direction",
	"M, m - Switch the display mode between fill and wide frame",
	"N, n - Tiggle the bone visibility",
	"C, c - Draw the bone in cylinder",
	"P, p - Draw the bone in pyramid",
	"L, l - Open the bvh file",
	"S, s - Save the new bvh file",
	"H, h - Toggle the helper board visibility",
	"Create Time - 4/22/2009",
	"Author - RenYaFei",
	"QQ - 546952720",
	"Email - dizuo@126.com"
};
void TipsBoard::init(int wid, int height)
{
	fontType = (int)GLUT_BITMAP_TIMES_ROMAN_24;
	offset.setValue( 0, 30 );
	_isShow = true;

	size.setValue( 800, offset.y()*BUFFERSIZE + 10);
	pos.setValue( ( wid - size.x() ) / 2.0f, ( height - size.y() ) / 2.0f );
}

void TipsBoard::showInfo()
{
	if( _isShow==false )
		return;

	glPushMatrix();
	{
		//display the panel
		glColor4f(1.0, 0.0, 0.0, 1.0);
		Helper::renderRectangle( TRectanglef( pos - offset, size ) );
		Helper::renderRectangle( TRectanglef( pos - offset, size ), Color4(0x556B2F), false );
		glColor4f(1.0, 1.0, 1.0, 1.0);
		for (size_t i(0); i<Helper::getBufferSize(FontList); i++)
		{
			Helper::renderBitmapString2D(pos.x(), pos.y() + offset.y() * i, (void *)fontType, FontList[i]); 
		}
	}glPopMatrix();
}