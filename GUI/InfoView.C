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

#include "InfoView.h"
#include "network/GameState.h"
#include "core/Game_Base.h"
#include "core/PlayerState.h"
#include <qimage.h>
#include <qpainter.h>
#include <cassert>
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////////
// BoardView
/////////////////////////////////////////////////////////////////////////////////////////////////
InfoView::InfoView( QWidget *parent, const char *name)
: QWidget( parent, name ), current(NULL), gamestate(NULL), propType(0)
{
  		this->setBackgroundColor(white);

		GO = new QImage("bin/deeds/GO.png");
		Free = new QImage("bin/deeds/Free.png");
		GoToJail = new QImage("bin/deeds/go_to_jail.png");
		Jail = new QImage("bin/deeds/jail.png");
		Chance = new QImage("bin/deeds/chance_deed.png");
		CommChest = new QImage("bin/deeds/cc_deed.png");
		Casino = new QImage("bin/deeds/casino_deed.png");
		RailDeed = new QImage("bin/deeds/RR_deed.png");
		LuxuryTax = new QImage("bin/deeds/lux_tax.png");
		IncomeTax = new QImage("bin/deeds/inc_tax.png");
		SalesTax = new QImage("bin/deeds/sales_tax.png");
		ElecDeed = new QImage("bin/deeds/elec_deed.png");
		WaterDeed = new QImage("bin/deeds/ww_deed.png");
		HeatDeed = new QImage("bin/deeds/heat_deed.png");
		BlackDeed = new QImage("bin/deeds/black_deed.png");
		BlueDeed = new QImage("bin/deeds/blue_deed.png");
		CyanDeed = new QImage("bin/deeds/cyan_deed.png");
		DBlueDeed = new QImage("bin/deeds/dark_blue_deed.png");
		DGreenDeed = new QImage("bin/deeds/dark_green_deed.png");
		PurpleDeed = new QImage("bin/deeds/dark_magenta_deed.png");
		DRedDeed = new QImage("bin/deeds/dark_red_deed.png");
		GreenDeed = new QImage("bin/deeds/green_deed.png");
		MagentaDeed = new QImage("bin/deeds/magenta_deed.png");
		OrangeDeed = new QImage("bin/deeds/orange_deed.png");
		PinkDeed = new QImage("bin/deeds/pink_deed.png");
		RedDeed = new QImage("bin/deeds/red_deed.png");
		YellowDeed = new QImage("bin/deeds/yellow_deed.png");
		Logo = new QImage("bin/deeds/logo.png");


}

//-----------------------------------------------------------------------------------------------
// Draw initial image in upper left corner of game window
//-----------------------------------------------------------------------------------------------
void InfoView::paintEvent(QPaintEvent *)
{
	if (current == NULL) {
		return;
	}

	QPixmap buffer( width(), height() );
	buffer.fill (white);

	// draw onto second buffer
	QPainter p(&buffer);

	QFont font;

		if (propType == 1) { // deed

				p.drawImage(0, 0, *current);

				QColor color = current->pixel(10, 10);
				if(color.red() < 140 and color.green() < 140 and color.blue() <= 255) {
						color.setRgb(255, 255, 255);
				}
				else {
					color.setRgb(0);
				}
				p.setPen(QColor(color));
				font.setPixelSize(15);
				p.setFont(font);
				p.drawText(14, 35, deedInfo.title);
				p.setPen(black);
				font.setPixelSize(11);
				p.setFont(font);
				QString price;
				int y = 69;
				for (int i = 0; i < 5; ++i) {
						price = QString::number(deedInfo.costs[i]);
						price.prepend("$");
						p.drawText(120, y, price);
						y += 12;
				}
				price = QString::number(deedInfo.costs[5]);
				price.prepend("$");
				p.drawText(120, 141, price);
				price = QString::number(deedInfo.price/2);
				price.prepend("$");
				p.drawText(130, 153, price);
				price = QString::number(deedInfo.house);
				price.prepend("$");
				font.setPixelSize(9);
				p.setFont(font);
				p.drawText(103, 164, price);
				price = QString::number(deedInfo.hotel);
				price.prepend("$");
				p.drawText(67, 176, price);

				price = QString::number(deedInfo.price);
				price.prepend("Cost: $");
				font.setPixelSize(18);
				p.setFont(font);
				p.drawText(50, 255, price);
		}
		else if (propType == 2) {

				p.drawImage(0, 0, *current);
				font.setPixelSize(10);
				p.setFont(font);
				p.drawText( 15, 71, deedInfo.title);
				font.setPixelSize(12);
				p.setFont(font);
				p.drawText(147, 92, QString::number(deedInfo.costs[0]));
				int y = 124;
				for (int i = 1; i < 8; ++i) {
						p.drawText(147, y, QString::number(deedInfo.costs[i]));
						y += 16;
				}
				QString	price = QString::number(deedInfo.price);
				price.prepend("Cost: $");
				font.setPixelSize(18);
				p.setFont(font);
				p.drawText(50, 255, price);
		}
		else if (propType == 3) {

				p.drawImage(0, 0, *current);
		}

		else if (propType == 5) {

				p.drawImage(0, 0, *current);
				font.setPixelSize(21);
				p.setFont(font);
				p.drawText(113, 90, QString::number(deedInfo.price));
		}
		else if (propType == 6) {

				p.drawImage(0, 0, *current);
				QString	price = QString::number(deedInfo.price);
				price.prepend("Cost: $");
				font.setPixelSize(18);
				p.setFont(font);
				p.drawText(50, 255, price);
		}
		else if (propType == 7) {

				p.drawImage(0, 0, *current);
				font.setPixelSize(18);
				p.setFont(font);
				p.drawText(123, 50, QString::number(deedInfo.price));
		}
		else {return;}


	// Done drawing to the temporary buffer
	p.end();

	// Draw to the window
	p.begin(this);
	p.drawPixmap( 0, 0, buffer);
	p.end();

}

void InfoView::game_state_updated(net::GameState * state)
{
	if(state == NULL) return;
	gamestate = state;
	repaint(false);
}

//-----------------------------------------------------------------------------------------------
// Gets specific property and puts its info in upper left corner of game window
//-----------------------------------------------------------------------------------------------
void InfoView::display_deed(int ID)//, int specID)
{
	if(gamestate == NULL) return;

		const Game_Base & base = gamestate->get_base();
		const std::string * name = base.name(ID);

		if (name == NULL) {return;}

		deedInfo.title = *name;

		for (int i = 0; i<8; ++i) {
				deedInfo.costs[i]= base.rent(ID, i);
		}
		deedInfo.price = base.price(ID);
		deedInfo.house = base.upgrade(ID, 0);
		deedInfo.hotel = base.upgrade(ID, 4);
		if (ID == 0) {
				propType = 7;
				current = GO;
		}
		else if (ID == 1) {
				propType = 1;
				current = PurpleDeed;
		}
		else if (ID == 2) {
				propType = 3;
				current = CommChest;
		}
		else if (ID == 3) {
				propType = 1;
				current = PurpleDeed;
		}
		else if (ID == 4) {
				propType = 3;
				current = IncomeTax;
		}
		else if (ID == 5) {
				current = RailDeed;
				propType = 2;
		}
		else if (ID == 6) {
				propType = 1;
				current = CyanDeed;
		}
		else if (ID == 7) {
				propType = 3;
				current = Chance;
		}
		else if (ID == 8) {
				propType = 1;
				current = CyanDeed;
		}
		else if (ID == 9) {
				propType = 1;
				current = CyanDeed;
		}
		else if (ID == 10) {
				propType = 3;
				current = Jail;
		}
		else if (ID == 11) {
				propType = 1;
				current = MagentaDeed;
		}
		else if (ID == 12) {
				propType = 6;
				current = ElecDeed;
		}
		else if (ID == 13) {
				propType = 1;
				current = MagentaDeed;
		}
		else if (ID == 14) {
				propType = 1;
				current = MagentaDeed;
		}
		else if (ID == 15) {
				current = RailDeed;
				propType = 2;
		}
		else if (ID == 16) {
				propType = 1;
				current = OrangeDeed;
		}
		else if (ID == 17) {
				propType = 3;
				current = CommChest;
		}
		else if (ID == 18) {
				propType = 1;
				current = OrangeDeed;
		}
		else if (ID == 19) {
				propType = 1;
				current = OrangeDeed;
		}
		else if (ID == 20) {
				propType = 3;
				current = Free;
		}
		else if (ID == 21) {
				propType = 1;
				current = RedDeed;
		}
		else if (ID == 22) {
				propType = 3;
				current = Chance;
		}
		else if (ID == 23) {
				propType = 1;
				current = RedDeed;
		}
		else if (ID == 24) {
				propType = 1;
				current = RedDeed;
		}
		else if (ID == 25) {
				current = RailDeed;
				propType = 2;
		}
		else if (ID == 26) {
				propType = 1;
				current = YellowDeed;
		}
		else if (ID == 27) {
				propType = 1;
				current = YellowDeed;
		}
		else if (ID == 28) {
				propType = 6;
				current = WaterDeed;
		}
		else if (ID == 29) {
				propType = 1;
				current = YellowDeed;
		}
		else if (ID == 30) {
				propType = 3;
				current = GoToJail;
		}
		else if (ID == 31) {
				propType = 1;
				current = DGreenDeed;
		}
		else if (ID == 32) {
				propType = 1;
				current = DGreenDeed;
		}
		else if (ID == 33) {
				propType = 3;
				current = CommChest;
		}
		else if (ID == 34) {
				propType = 1;
				current = DGreenDeed;
		}
		else if (ID == 35) {
				current = RailDeed;
				propType = 2;
		}
		else if (ID == 36) {
				propType = 3;
				current = Chance;
		}
		else if (ID == 37) {
				propType = 1;
				current = BlueDeed;
		}
		else if (ID == 38) {
				propType = 5;
				current = LuxuryTax;
		}
		else if (ID == 39) {
				propType = 1;
				current = BlueDeed;
		}
		else if (ID == 40) {
				propType = 7;
				current = GO;
		}
		else if (ID == 41) {
				propType = 6;
				current = HeatDeed;
		}
		else if (ID == 42) {
				propType = 1;
				current = DRedDeed;
		}
		else if (ID == 43) {
				current = RailDeed;
				propType = 2;
		}
		else if (ID == 44) {
				propType = 1;
				current = DRedDeed;
		}
		else if (ID == 45) {
				propType = 1;
				current = DRedDeed;
		}
		else if (ID == 46) {
				propType = 3;
				current = GoToJail;
		}
		else if (ID == 47) {
				propType = 3;
				current = Chance;
		}
		else if (ID == 48) {
				propType = 1;
				current = GreenDeed;
		}
		else if (ID == 49) {
				current = RailDeed;
				propType = 2;
		}
		else if (ID == 50) {
				propType = 1;
				current = GreenDeed;
		}
		else if (ID == 51) {
				propType = 1;
				current = GreenDeed;
		}
		else if (ID == 52) {
				propType = 3;
				current = Free;
		}
		else if (ID == 53) {
				propType = 3;
				current = SalesTax;
		}
		else if (ID == 54) {
				propType = 1;
				current = DBlueDeed;
		}
		else if (ID == 55) {
				current = RailDeed;
				propType = 2;
		}
		else if (ID == 56) {
				propType = 1;
				current = DBlueDeed;
		}
		else if (ID == 57) {
				propType = 3;
				current = CommChest;
		}
		else if (ID == 58) {
				propType = 3;
				current = Casino;
		}
		else if (ID == 59) {
				propType = 1;
				current = PinkDeed;
		}
		else if (ID == 60) {
				propType = 1;
				current = PinkDeed;
		}
		else if (ID == 61) {
				current = RailDeed;
				propType = 2;
		}
		else if (ID == 62) {
				propType = 1;
				current = BlackDeed;
		}
		else if (ID == 63) {
				propType = 1;
				current = BlackDeed;
		}
		else if (ID == 99) {
				current = current;
		}
		repaint(false);
}
