#ifndef TOKENS_H
#define TOKENS_H

#include <qpainter.h>
#include <qcolor.h>

class QImage;


class Tokens {
private:
	QImage * TokenArray[8];
	QImage * TokenArraySide[8];

public:
	Tokens ();
	virtual ~Tokens();

	QImage* get_token(int id);

	void draw_token(QPainter &,int,int,bool);

	QColor get_color_of(int tokenID);
};

#endif
