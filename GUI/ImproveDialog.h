#ifndef IMPROVEDIALOG_H
#define IMPROVEDIALOG_H

#include <qdialog.h>
#include <vector>

class QWidget;
class QString;
class QComboBox;
class QGridLayout;
class PlayerState;
namespace net{
	class GameState;
}

class ImproveDialog : public QDialog {
	Q_OBJECT
	public:

		ImproveDialog(net::GameState *, const PlayerState *, QWidget *, const char * name =0);

		unsigned get_level() const;
		unsigned get_property() const;

	private:
		net::GameState *_gamestate;
		QComboBox *_groupList;
		QComboBox *_levelList;
		QString *_me;
		QGridLayout *_grid;

		std::vector<unsigned> _translate;

		void _populate_group_list(const PlayerState *);

	public slots:

		void item_selected(int);
};

#endif
