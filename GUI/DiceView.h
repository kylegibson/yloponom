#ifndef DICEVIEW_H
#define DICEVIEW_H

#include <qwidget.h>

class DiceView : public QWidget {
	Q_OBJECT
	public:
		DiceView (QWidget *parent=0, const char *name=0);
		void paintEvent(QPaintEvent *pe);

	private:
		int _dice_num1;
		int _dice_num2;

	public slots:
		void change_dice(int, int);
};

#endif
