#ifndef INFOVIEW_H
#define INFOVIEW_H

#include <qwidget.h>
#include "core/PlayerState.h"

class QImage;
struct deedOverlay{
		QString title;
		int costs[9];
		int price;
		int house;
		int hotel;
};

namespace net{
	class GameState;
}


class InfoView : public QWidget {
Q_OBJECT
  public:
		QImage * Chance;
		QImage * CommChest;
		QImage * GO;
		QImage * Free;
		QImage * GoToJail;
		QImage * Jail;
		QImage * Casino;
		QImage * LuxuryTax;
		QImage * IncomeTax;
		QImage * SalesTax;
		QImage * RailDeed;
		QImage * ElecDeed;
		QImage * WaterDeed;
		QImage * HeatDeed;
		QImage * Logo;
		QImage * BlackDeed;
		QImage * BlueDeed;
		QImage * CyanDeed;
		QImage * DBlueDeed;
		QImage * DGreenDeed;
		QImage * PurpleDeed;
		QImage * DRedDeed;
		QImage * GreenDeed;
		QImage * MagentaDeed;
		QImage * OrangeDeed;
		QImage * PinkDeed;
		QImage * RedDeed;
		QImage * YellowDeed;
		QImage * current;

    InfoView (QWidget *parent=0, const char *name=0 );
    void paintEvent(QPaintEvent *pe);
	public slots:

	void display_deed(int);

	void game_state_updated(net::GameState *);

	private:
		struct deedOverlay deedInfo;
		net::GameState * gamestate;
		int propType;
};

#endif
