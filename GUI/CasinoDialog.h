#ifndef CASINODIALOG_H
#define CASINODIALOG_H

#include <qdialog.h>

class QWidget;
class QButtonGroup;
class QSpinBox;
class PlayerState;

class CasinoDialog : public QDialog {
	Q_OBJECT
	private:
		QButtonGroup* _group;
		QSpinBox* _spin;


	public:
		CasinoDialog(const PlayerState* player, QWidget *parent, const char* name=0);

		int get_bet_type();
		int get_wager();
};

#endif
