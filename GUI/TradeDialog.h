#ifndef TRADEDIALOG_H
#define TRADEDIALOG_H

#include <qdialog.h>
#include <vector>
#include <vector>

class QWidget;
class QString;
class QSpinBox;
class QListBox;
class PlayerState;
namespace net{
	class GameState;
}

class TradeDialog : public QDialog {
	Q_OBJECT
	public:
		TradeDialog(net::GameState *, const PlayerState*, const PlayerState*, QWidget *parent, const char * name = 0);

		unsigned get_offer_money();
		unsigned get_request_money();

		unsigned get_offer_props(std::vector<unsigned> &);
		unsigned get_request_props(std::vector<unsigned> &);

	private:
		net::GameState *gamestate;
		QSpinBox* _my_spin;
		QSpinBox* _their_spin;

		QListBox* _my_list;
		QListBox* _their_list;

		std::vector<unsigned> _my_trans;
		std::vector<unsigned> _their_trans;
};

#endif
