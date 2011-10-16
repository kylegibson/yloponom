/////////////////////////////////////////////////////////////////////////////////////////////////
//
// YLOPONOM
//
// CS3141 - Spring 2006
// Group 7
// Michael Hyde, Nick Quaderer, Will Pratt, Kyle Gibson
//
// GUI - This file contains the code for structuring the GUI for Yloponom
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "DiceView.h"
#include <qpainter.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
// Diceview class
/////////////////////////////////////////////////////////////////////////////////////////////////
DiceView::DiceView(QWidget *parent, const char *name) 
: QWidget( parent, name), _dice_num1(0), _dice_num2(0)
{
		this->setFixedWidth(134);
		this->setFixedHeight(67);
		this->setBackgroundColor(green);
}

void DiceView::change_dice(int d1, int d2)
{
	_dice_num1 = d1;
	_dice_num2 = d2;
	repaint();
}

void DiceView::paintEvent(QPaintEvent *)
{
		QPainter p(this);

//-----------------------------------------------------------------------------------------------
// Draw dice
//-----------------------------------------------------------------------------------------------
		if(_dice_num1 !=0){
			p.setBrush(white);
			p.drawRoundRect(2, 2, 63, 63, 20, 20);
			p.drawRoundRect(69, 2, 63, 63, 20, 20);
		}
		else{
			return;
		}

	// Draw left die according to number generated
		p.setBrush(black);
		if (_dice_num1 == 1) {
				p.drawEllipse(29, 29, 10, 10);
		}
		else if (_dice_num1 == 2) {
				p.drawEllipse(8, 8, 10, 10);
				p.drawEllipse(49, 50, 10, 10);
		}
		else if (_dice_num1 == 3) {
		  	p.drawEllipse(8, 8, 10, 10);
				p.drawEllipse(29, 29, 10, 10);
				p.drawEllipse(49, 50, 10, 10);
		}
		else if (_dice_num1 == 4) {
				p.drawEllipse(8, 8, 10, 10);
				p.drawEllipse(8, 50, 10, 10);
				p.drawEllipse(49, 8, 10, 10);
				p.drawEllipse(49, 50, 10, 10);
		}
		else if (_dice_num1 == 5) {
				p.drawEllipse(8, 8, 10, 10);
				p.drawEllipse(8, 50, 10, 10);
				p.drawEllipse(29, 29, 10, 10);
				p.drawEllipse(49, 8, 10, 10);
				p.drawEllipse(49, 50, 10, 10);
		}
		else if (_dice_num1 == 6) {
				p.drawEllipse(8, 8, 10, 10);
				p.drawEllipse(8, 29, 10, 10);
				p.drawEllipse(8, 50, 10, 10);
				p.drawEllipse(49, 8, 10, 10);
				p.drawEllipse(49, 29, 10, 10);
				p.drawEllipse(49, 50, 10, 10);
		}

		// Draw right die according to number generated
		if (_dice_num2 == 1) {
				p.drawEllipse(96, 29, 10, 10);
		}
		else if (_dice_num2 == 2) {
				p.drawEllipse(75, 8, 10, 10);
				p.drawEllipse(117, 50, 10, 10);
		}
		else if (_dice_num2 == 3) {
		  	p.drawEllipse(75, 8, 10, 10);
				p.drawEllipse(96, 29, 10, 10);
				p.drawEllipse(117, 50, 10, 10);
		}
		else if (_dice_num2 == 4) {
				p.drawEllipse(75, 8, 10, 10);
				p.drawEllipse(75, 50, 10, 10);
				p.drawEllipse(117, 8, 10, 10);
				p.drawEllipse(117, 50, 10, 10);
		}
		else if (_dice_num2 == 5) {
				p.drawEllipse(75, 8, 10, 10);
				p.drawEllipse(75, 50, 10, 10);
				p.drawEllipse(96, 29, 10, 10);
				p.drawEllipse(117, 8, 10, 10);
				p.drawEllipse(117, 50, 10, 10);
		}
		else if (_dice_num2 == 6) {
				p.drawEllipse(75, 8, 10, 10);
				p.drawEllipse(75, 29, 10, 10);
				p.drawEllipse(75, 50, 10, 10);
				p.drawEllipse(117, 8, 10, 10);
				p.drawEllipse(117, 29, 10, 10);
				p.drawEllipse(117, 50, 10, 10);
		}

}
