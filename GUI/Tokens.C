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

#include "Tokens.h"
#include <cstdio>
#include <qimage.h>
#include <qcolor.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
// Tokens
/////////////////////////////////////////////////////////////////////////////////////////////////
Tokens::Tokens() 
{

	char buff[20];
	for (int i=0; i<8; i++){
		snprintf(buff,20,"bin/tokens/%d.png",i+1);
		TokenArray[i]= new QImage(buff);
		snprintf(buff,20,"bin/tokens/%d-s.png",i+1);
		TokenArraySide[i]= new QImage(buff);
	}

}

Tokens::~Tokens()
{
	for (int i=0; i<8; i++){
		delete TokenArray[i];
		delete TokenArraySide[i];
	}
}

QColor Tokens::get_color_of(int tokenID)
{
	QColor color;
	QImage * i = get_token(tokenID);
	if(i != NULL) {
		color = i->pixel(12,7);
	}
	return color;
}

QImage* Tokens::get_token(int id)
{
	if(id < 0 || id > 7) {
		return NULL;
	}
	return TokenArray[id];
}

void Tokens::draw_token(QPainter &p, int player, int pos, bool jail)
{
	if (pos < 0){
		return;
	}

	if(player < 0) {
		return;
	}

	++player;
	++pos;

	if (player == 1) {
		if (pos == 1) {
				p.drawImage(582, 582, *TokenArray[0]);
		}
		if (pos == 2) {
				p.drawImage(511, 582, *TokenArray[0]);
		}
		if (pos == 3) {
				p.drawImage(461, 582, *TokenArray[0]);
		}
		if (pos == 4) {
				p.drawImage(411, 582, *TokenArray[0]);
		}
		if (pos == 5) {
				p.drawImage(361, 582, *TokenArray[0]);
		}
		if (pos == 6) {
				p.drawImage(311, 582, *TokenArray[0]);
		}
		if (pos == 7) {
				p.drawImage(261, 582, *TokenArray[0]);
		}
		if (pos == 8) {
				p.drawImage(211, 582, *TokenArray[0]);
		}
		if (pos == 9) {
				p.drawImage(161, 582, *TokenArray[0]);
		}
		if (pos == 10) {
				p.drawImage(111, 582, *TokenArray[0]);
		}
		if (pos == 11) {
			if(jail){
				p.drawImage(52, 561, *TokenArray[0]);
			}
			else{
				p.drawImage(27, 561, *TokenArray[0]);
			}
		}
		if (pos == 12) {
				p.drawImage(66, 511, *TokenArraySide[0]);
		}
		if (pos == 13) {
				p.drawImage(66, 461, *TokenArraySide[0]);
		}
		if (pos == 14) {
				p.drawImage(66, 411, *TokenArraySide[0]);
		}
		if (pos == 15) {
				p.drawImage(66, 361, *TokenArraySide[0]);
		}
		if (pos == 16) {
				p.drawImage(66, 311, *TokenArraySide[0]);
		}
		if (pos == 17) {
				p.drawImage(66, 261, *TokenArraySide[0]);
		}
		if (pos == 18) {
				p.drawImage(66, 211, *TokenArraySide[0]);
		}
		if (pos == 19) {
				p.drawImage(66, 161, *TokenArraySide[0]);
		}
		if (pos == 20) {
				p.drawImage(66, 111, *TokenArraySide[0]);
		}
		if (pos == 21) {
				p.drawImage(73, 76, *TokenArray[0]);
		}
		if (pos == 22) {
				p.drawImage(136, 66, *TokenArray[0]);
		}
		if (pos == 23) {
				p.drawImage(186, 66, *TokenArray[0]);
		}
		if (pos == 24) {
				p.drawImage(236, 66, *TokenArray[0]);
		}
		if (pos == 25) {
				p.drawImage(286, 66, *TokenArray[0]);
		}
		if (pos == 26) {
				p.drawImage(336, 66, *TokenArray[0]);
		}
		if (pos == 27) {
				p.drawImage(386, 66, *TokenArray[0]);
		}
		if (pos == 28) {
				p.drawImage(436, 66, *TokenArray[0]);
		}
		if (pos == 29) {
				p.drawImage(486, 66, *TokenArray[0]);
		}
		if (pos == 30) {
				p.drawImage(536, 66, *TokenArray[0]);
		}
		//if (pos == 31) {
		//}
		if (pos == 32) {
				p.drawImage(582, 136, *TokenArraySide[0]);
		}
		if (pos == 33) {
				p.drawImage(582, 186, *TokenArraySide[0]);
		}
		if (pos == 34) {
				p.drawImage(582, 236, *TokenArraySide[0]);
		}
		if (pos == 35) {
				p.drawImage(582, 286, *TokenArraySide[0]);
		}
		if (pos == 36) {
				p.drawImage(582, 336, *TokenArraySide[0]);
		}
		if (pos == 37) {
				p.drawImage(582, 386, *TokenArraySide[0]);
		}
		if (pos == 38) {
				p.drawImage(582, 436, *TokenArraySide[0]);
		}
		if (pos == 39) {
				p.drawImage(582, 486, *TokenArraySide[0]);
		}
		if (pos == 40) {
				p.drawImage(582, 536, *TokenArraySide[0]);
		}
		if (pos == 41) {
				p.drawImage(482, 482, *TokenArray[0]);
		}
		if (pos == 42) {
				p.drawImage(411, 482, *TokenArray[0]);
		}
		if (pos == 43) {
				p.drawImage(361, 482, *TokenArray[0]);
		}
		if (pos == 44) {
				p.drawImage(311, 482, *TokenArray[0]);
		}
		if (pos == 45) {
				p.drawImage(261, 482, *TokenArray[0]);
		}
		if (pos == 46) {
				p.drawImage(211, 482, *TokenArray[0]);
		}
		//if (pos == 47) {
		//}
		if (pos == 48) {
				p.drawImage(166, 411, *TokenArraySide[0]);
		}
		if (pos == 49) {
				p.drawImage(166, 361, *TokenArraySide[0]);
		}
		if (pos == 50) {
				p.drawImage(166, 311, *TokenArraySide[0]);
		}
		if (pos == 51) {
				p.drawImage(166, 261, *TokenArraySide[0]);
		}
		if (pos == 52) {
				p.drawImage(166, 211, *TokenArraySide[0]);
		}
		if (pos == 53) {
				p.drawImage(173, 176, *TokenArray[0]);
		}
		if (pos == 54) {
				p.drawImage(236, 166, *TokenArray[0]);
		}
		if (pos == 55) {
				p.drawImage(286, 166, *TokenArray[0]);
		}
		if (pos == 56) {
				p.drawImage(336, 166, *TokenArray[0]);
		}
		if (pos == 57) {
				p.drawImage(386, 166, *TokenArray[0]);
		}
		if (pos == 58) {
				p.drawImage(436, 166, *TokenArray[0]);
		}
		if (pos == 59) {
				p.drawImage(492, 176, *TokenArray[0]);
		}
		if (pos == 60) {
				p.drawImage(482, 236, *TokenArraySide[0]);
		}
		if (pos == 61) {
				p.drawImage(482, 286, *TokenArraySide[0]);
		}
		if (pos == 62) {
				p.drawImage(482, 336, *TokenArraySide[0]);
		}
		if (pos == 63) {
				p.drawImage(482, 386, *TokenArraySide[0]);
		}
		if (pos == 64) {
				p.drawImage(482, 436, *TokenArraySide[0]);
		}
}

if (player == 2) {
		if (pos == 1) {
				p.drawImage(607, 582, *TokenArray[1]);
		}
		if (pos == 2) {
				p.drawImage(536, 582, *TokenArray[1]);
		}
		if (pos == 3) {
				p.drawImage(486, 582, *TokenArray[1]);
		}
		if (pos == 4) {
				p.drawImage(436, 582, *TokenArray[1]);
		}
		if (pos == 5) {
				p.drawImage(386, 582, *TokenArray[1]);
		}
		if (pos == 6) {
				p.drawImage(336, 582, *TokenArray[1]);
		}
		if (pos == 7) {
				p.drawImage(286, 582, *TokenArray[1]);
		}
		if (pos == 8) {
				p.drawImage(236, 582, *TokenArray[1]);
		}
		if (pos == 9) {
				p.drawImage(186, 582, *TokenArray[1]);
		}
		if (pos == 10) {
				p.drawImage(136, 582, *TokenArray[1]);
		}
		if (pos == 11) {
			if(jail){
				p.drawImage(77, 561, *TokenArray[1]);
			}
			else{
				p.drawImage(27, 574, *TokenArray[1]);
			}
		}
		if (pos == 12) {
				p.drawImage(66, 536, *TokenArraySide[1]);
		}
		if (pos == 13) {
				p.drawImage(66, 486, *TokenArraySide[1]);
		}
		if (pos == 14) {
				p.drawImage(66, 436, *TokenArraySide[1]);
		}
		if (pos == 15) {
				p.drawImage(66, 386, *TokenArraySide[1]);
		}
		if (pos == 16) {
				p.drawImage(66, 336, *TokenArraySide[1]);
		}
		if (pos == 17) {
				p.drawImage(66, 286, *TokenArraySide[1]);
		}
		if (pos == 18) {
				p.drawImage(66, 236, *TokenArraySide[1]);
		}
		if (pos == 19) {
				p.drawImage(66, 186, *TokenArraySide[1]);
		}
		if (pos == 20) {
				p.drawImage(66, 136, *TokenArraySide[1]);
		}
		if (pos == 21) {
				p.drawImage(48, 76, *TokenArray[1]);
		}
		if (pos == 22) {
				p.drawImage(111, 66, *TokenArray[1]);
		}
		if (pos == 23) {
				p.drawImage(161, 66, *TokenArray[1]);
		}
		if (pos == 24) {
				p.drawImage(211, 66, *TokenArray[1]);
		}
		if (pos == 25) {
				p.drawImage(261, 66, *TokenArray[1]);
		}
		if (pos == 26) {
				p.drawImage(311, 66, *TokenArray[1]);
		}
		if (pos == 27) {
				p.drawImage(361, 66, *TokenArray[1]);
		}
		if (pos == 28) {
				p.drawImage(411, 66, *TokenArray[1]);
		}
		if (pos == 29) {
				p.drawImage(461, 66, *TokenArray[1]);
		}
		if (pos == 30) {
				p.drawImage(511, 66, *TokenArray[1]);
		}
		//if (pos == 31) {
		//}
		if (pos == 32) {
				p.drawImage(582, 111, *TokenArraySide[1]);
		}
		if (pos == 33) {
				p.drawImage(582, 161, *TokenArraySide[1]);
		}
		if (pos == 34) {
				p.drawImage(582, 211, *TokenArraySide[1]);
		}
		if (pos == 35) {
				p.drawImage(582, 261, *TokenArraySide[1]);
		}
		if (pos == 36) {
				p.drawImage(582, 311, *TokenArraySide[1]);
		}
		if (pos == 37) {
				p.drawImage(582, 361, *TokenArraySide[1]);
		}
		if (pos == 38) {
				p.drawImage(582, 411, *TokenArraySide[1]);
		}
		if (pos == 39) {
				p.drawImage(582, 461, *TokenArraySide[1]);
		}
		if (pos == 40) {
				p.drawImage(582, 511, *TokenArraySide[1]);
		}
		if (pos == 41) {
				p.drawImage(507, 482, *TokenArray[1]);
		}
		if (pos == 42) {
				p.drawImage(436, 482, *TokenArray[1]);
		}
		if (pos == 43) {
				p.drawImage(386, 482, *TokenArray[1]);
		}
		if (pos == 44) {
				p.drawImage(336, 482, *TokenArray[1]);
		}
		if (pos == 45) {
				p.drawImage(286, 482, *TokenArray[1]);
		}
		if (pos == 46) {
				p.drawImage(236, 482, *TokenArray[1]);
		}
		//if (pos == 47) {
		//}
		if (pos == 48) {
				p.drawImage(166, 436, *TokenArraySide[1]);
		}
		if (pos == 49) {
				p.drawImage(166, 386, *TokenArraySide[1]);
		}
		if (pos == 50) {
				p.drawImage(166, 336, *TokenArraySide[1]);
		}
		if (pos == 51) {
				p.drawImage(166, 286, *TokenArraySide[1]);
		}
		if (pos == 52) {
				p.drawImage(166, 236, *TokenArraySide[1]);
		}
		if (pos == 53) {
				p.drawImage(148, 176, *TokenArray[1]);
		}
		if (pos == 54) {
				p.drawImage(211, 166, *TokenArray[1]);
		}
		if (pos == 55) {
				p.drawImage(261, 166, *TokenArray[1]);
		}
		if (pos == 56) {
				p.drawImage(311, 166, *TokenArray[1]);
		}
		if (pos == 57) {
				p.drawImage(361, 166, *TokenArray[1]);
		}
		if (pos == 58) {
				p.drawImage(411, 166, *TokenArray[1]);
		}
		if (pos == 59) {
				p.drawImage(467, 176, *TokenArray[1]);
		}
		if (pos == 60) {
				p.drawImage(482, 211, *TokenArraySide[1]);
		}
		if (pos == 61) {
				p.drawImage(482, 261, *TokenArraySide[1]);
		}
		if (pos == 62) {
				p.drawImage(482, 311, *TokenArraySide[1]);
		}
		if (pos == 63) {
				p.drawImage(482, 361, *TokenArraySide[1]);
		}
		if (pos == 64) {
				p.drawImage(482, 411, *TokenArraySide[1]);
		}

}

if (player == 3) {
		if (pos == 1) {
				p.drawImage(582, 595, *TokenArray[2]);
		}
		if (pos == 2) {
				p.drawImage(511, 595, *TokenArray[2]);
		}
		if (pos == 3) {
				p.drawImage(461, 595, *TokenArray[2]);
		}
		if (pos == 4) {
				p.drawImage(411, 595, *TokenArray[2]);
		}
		if (pos == 5) {
				p.drawImage(361, 595, *TokenArray[2]);
		}
		if (pos == 6) {
				p.drawImage(311, 595, *TokenArray[2]);
		}
		if (pos == 7) {
				p.drawImage(261, 595, *TokenArray[2]);
		}
		if (pos == 8) {
				p.drawImage(211, 595, *TokenArray[2]);
		}
		if (pos == 9) {
				p.drawImage(161, 595, *TokenArray[2]);
		}
		if (pos == 10) {
				p.drawImage(111, 595, *TokenArray[2]);
		}
		if (pos == 11) {
			if(jail){
				p.drawImage(52, 595, *TokenArray[2]);
			}
			else{
				p.drawImage(27, 587, *TokenArray[2]);
			}
		}
		if (pos == 12) {
				p.drawImage(53, 511, *TokenArraySide[2]);
		}
		if (pos == 13) {
				p.drawImage(53, 461, *TokenArraySide[2]);
		}
		if (pos == 14) {
				p.drawImage(53, 411, *TokenArraySide[2]);
		}
		if (pos == 15) {
				p.drawImage(53, 361, *TokenArraySide[2]);
		}
		if (pos == 16) {
				p.drawImage(53, 311, *TokenArraySide[2]);
		}
		if (pos == 17) {
				p.drawImage(53, 261, *TokenArraySide[2]);
		}
		if (pos == 18) {
				p.drawImage(53, 211, *TokenArraySide[2]);
		}
		if (pos == 19) {
				p.drawImage(53, 161, *TokenArraySide[2]);
		}
		if (pos == 20) {
				p.drawImage(53, 111, *TokenArraySide[2]);
		}
		if (pos == 21) {
				p.drawImage(73, 63, *TokenArray[2]);
		}
		if (pos == 22) {
				p.drawImage(136, 53, *TokenArray[2]);
		}
		if (pos == 23) {
				p.drawImage(186, 53, *TokenArray[2]);
		}
		if (pos == 24) {
				p.drawImage(236, 53, *TokenArray[2]);
		}
		if (pos == 25) {
				p.drawImage(286, 53, *TokenArray[2]);
		}
		if (pos == 26) {
				p.drawImage(336, 53, *TokenArray[2]);
		}
		if (pos == 27) {
				p.drawImage(386, 53, *TokenArray[2]);
		}
		if (pos == 28) {
				p.drawImage(436, 53, *TokenArray[2]);
		}
		if (pos == 29) {
				p.drawImage(486, 53, *TokenArray[2]);
		}
		if (pos == 30) {
				p.drawImage(536, 53, *TokenArray[2]);
		}
		//if (pos == 31) {
		//}
		if (pos == 32) {
				p.drawImage(595, 136, *TokenArraySide[2]);
		}
		if (pos == 33) {
				p.drawImage(595, 186, *TokenArraySide[2]);
		}
		if (pos == 34) {
				p.drawImage(595, 236, *TokenArraySide[2]);
		}
		if (pos == 35) {
				p.drawImage(595, 286, *TokenArraySide[2]);
		}
		if (pos == 36) {
				p.drawImage(595, 336, *TokenArraySide[2]);
		}
		if (pos == 37) {
				p.drawImage(595, 386, *TokenArraySide[2]);
		}
		if (pos == 38) {
				p.drawImage(595, 436, *TokenArraySide[2]);
		}
		if (pos == 39) {
				p.drawImage(595, 486, *TokenArraySide[2]);
		}
		if (pos == 40) {
				p.drawImage(595, 536, *TokenArraySide[2]);
		}
		if (pos == 41) {
				p.drawImage(482, 495, *TokenArray[2]);
		}
		if (pos == 42) {
				p.drawImage(411, 495, *TokenArray[2]);
		}
		if (pos == 43) {
				p.drawImage(361, 495, *TokenArray[2]);
		}
		if (pos == 44) {
				p.drawImage(311, 495, *TokenArray[2]);
		}
		if (pos == 45) {
				p.drawImage(261, 495, *TokenArray[2]);
		}
		if (pos == 46) {
				p.drawImage(211, 495, *TokenArray[2]);
		}
		//if (pos == 47) {
		//}
		if (pos == 48) {
				p.drawImage(153, 411, *TokenArraySide[2]);
		}
		if (pos == 49) {
				p.drawImage(153, 361, *TokenArraySide[2]);
		}
		if (pos == 50) {
				p.drawImage(153, 311, *TokenArraySide[2]);
		}
		if (pos == 51) {
				p.drawImage(153, 261, *TokenArraySide[2]);
		}
		if (pos == 52) {
				p.drawImage(153, 211, *TokenArraySide[2]);
		}
		if (pos == 53) {
				p.drawImage(173, 163, *TokenArray[2]);
		}
		if (pos == 54) {
				p.drawImage(236, 153, *TokenArray[2]);
		}
		if (pos == 55) {
				p.drawImage(286, 153, *TokenArray[2]);
		}
		if (pos == 56) {
				p.drawImage(336, 153, *TokenArray[2]);
		}
		if (pos == 57) {
				p.drawImage(386, 153, *TokenArray[2]);
		}
		if (pos == 58) {
				p.drawImage(436, 153, *TokenArray[2]);
		}
		if (pos == 59) {
				p.drawImage(492, 163, *TokenArray[2]);
		}
		if (pos == 60) {
				p.drawImage(495, 236, *TokenArraySide[2]);
		}
		if (pos == 61) {
				p.drawImage(495, 286, *TokenArraySide[2]);
		}
		if (pos == 62) {
				p.drawImage(495, 336, *TokenArraySide[2]);
		}
		if (pos == 63) {
				p.drawImage(495, 386, *TokenArraySide[2]);
		}
		if (pos == 64) {
				p.drawImage(495, 436, *TokenArraySide[2]);
		}

}

if (player == 4) {
		if (pos == 1) {
				p.drawImage(607, 595, *TokenArray[3]);
		}
		if (pos == 2) {
				p.drawImage(536, 595, *TokenArray[3]);
		}
		if (pos == 3) {
				p.drawImage(486, 595, *TokenArray[3]);
		}
		if (pos == 4) {
				p.drawImage(436, 595, *TokenArray[3]);
		}
		if (pos == 5) {
				p.drawImage(386, 595, *TokenArray[3]);
		}
		if (pos == 6) {
				p.drawImage(336, 595, *TokenArray[3]);
		}
		if (pos == 7) {
				p.drawImage(286, 595, *TokenArray[3]);
		}
		if (pos == 8) {
				p.drawImage(236, 595, *TokenArray[3]);
		}
		if (pos == 9) {
				p.drawImage(186, 595, *TokenArray[3]);
		}
		if (pos == 10) {
				p.drawImage(136, 595, *TokenArray[3]);
		}
		if (pos == 11) {
			if(jail){
				p.drawImage(77, 595, *TokenArray[3]);
			}
			else{
				p.drawImage(27, 600, *TokenArray[3]);
			}
		}
		if (pos == 12) {
				p.drawImage(53, 536, *TokenArraySide[3]);
		}
		if (pos == 13) {
				p.drawImage(53, 486, *TokenArraySide[3]);
		}
		if (pos == 14) {
				p.drawImage(53, 436, *TokenArraySide[3]);
		}
		if (pos == 15) {
				p.drawImage(53, 386, *TokenArraySide[3]);
		}
		if (pos == 16) {
				p.drawImage(53, 336, *TokenArraySide[3]);
		}
		if (pos == 17) {
				p.drawImage(53, 286, *TokenArraySide[3]);
		}
		if (pos == 18) {
				p.drawImage(53, 236, *TokenArraySide[3]);
		}
		if (pos == 19) {
				p.drawImage(53, 186, *TokenArraySide[3]);
		}
		if (pos == 20) {
				p.drawImage(53, 136, *TokenArraySide[3]);
		}
		if (pos == 21) {
				p.drawImage(48, 63, *TokenArray[3]);
		}
		if (pos == 22) {
				p.drawImage(111, 53, *TokenArray[3]);
		}
		if (pos == 23) {
				p.drawImage(161, 53, *TokenArray[3]);
		}
		if (pos == 24) {
				p.drawImage(211, 53, *TokenArray[3]);
		}
		if (pos == 25) {
				p.drawImage(261, 53, *TokenArray[3]);
		}
		if (pos == 26) {
				p.drawImage(311, 53, *TokenArray[3]);
		}
		if (pos == 27) {
				p.drawImage(361, 53, *TokenArray[3]);
		}
		if (pos == 28) {
				p.drawImage(411, 53, *TokenArray[3]);
		}
		if (pos == 29) {
				p.drawImage(461, 53, *TokenArray[3]);
		}
		if (pos == 30) {
				p.drawImage(511, 53, *TokenArray[3]);
		}
		//if (pos == 31) {
		//}
		if (pos == 32) {
				p.drawImage(595, 111, *TokenArraySide[3]);
		}
		if (pos == 33) {
				p.drawImage(595, 161, *TokenArraySide[3]);
		}
		if (pos == 34) {
				p.drawImage(595, 211, *TokenArraySide[3]);
		}
		if (pos == 35) {
				p.drawImage(595, 261, *TokenArraySide[3]);
		}
		if (pos == 36) {
				p.drawImage(595, 311, *TokenArraySide[3]);
		}
		if (pos == 37) {
				p.drawImage(595, 361, *TokenArraySide[3]);
		}
		if (pos == 38) {
				p.drawImage(595, 411, *TokenArraySide[3]);
		}
		if (pos == 39) {
				p.drawImage(595, 461, *TokenArraySide[3]);
		}
		if (pos == 40) {
				p.drawImage(595, 511, *TokenArraySide[3]);
		}
		if (pos == 41) {
				p.drawImage(507, 495, *TokenArray[3]);
		}
		if (pos == 42) {
				p.drawImage(436, 495, *TokenArray[3]);
		}
		if (pos == 43) {
				p.drawImage(386, 495, *TokenArray[3]);
		}
		if (pos == 44) {
				p.drawImage(336, 495, *TokenArray[3]);
		}
		if (pos == 45) {
				p.drawImage(286, 495, *TokenArray[3]);
		}
		if (pos == 46) {
				p.drawImage(236, 495, *TokenArray[3]);
		}
		//if (pos == 47) {
		//}
		if (pos == 48) {
				p.drawImage(153, 436, *TokenArraySide[3]);
		}
		if (pos == 49) {
				p.drawImage(153, 386, *TokenArraySide[3]);
		}
		if (pos == 50) {
				p.drawImage(153, 336, *TokenArraySide[3]);
		}
		if (pos == 51) {
				p.drawImage(153, 286, *TokenArraySide[3]);
		}
		if (pos == 52) {
				p.drawImage(153, 236, *TokenArraySide[3]);
		}
		if (pos == 53) {
				p.drawImage(148, 163, *TokenArray[3]);
		}
		if (pos == 54) {
				p.drawImage(211, 153, *TokenArray[3]);
		}
		if (pos == 55) {
				p.drawImage(261, 153, *TokenArray[3]);
		}
		if (pos == 56) {
				p.drawImage(311, 153, *TokenArray[3]);
		}
		if (pos == 57) {
				p.drawImage(361, 153, *TokenArray[3]);
		}
		if (pos == 58) {
				p.drawImage(411, 153, *TokenArray[3]);
		}
		if (pos == 59) {
				p.drawImage(467, 163, *TokenArray[3]);
		}
		if (pos == 60) {
				p.drawImage(495, 211, *TokenArraySide[3]);
		}
		if (pos == 61) {
				p.drawImage(495, 261, *TokenArraySide[3]);
		}
		if (pos == 62) {
				p.drawImage(495, 311, *TokenArraySide[3]);
		}
		if (pos == 63) {
				p.drawImage(495, 361, *TokenArraySide[3]);
		}
		if (pos == 64) {
				p.drawImage(495, 411, *TokenArraySide[3]);
		}

}

if (player == 5) {
		if (pos == 1) {
				p.drawImage(582, 608, *TokenArray[4]);
		}
		if (pos == 2) {
				p.drawImage(511, 608, *TokenArray[4]);
		}
		if (pos == 3) {
				p.drawImage(461, 608, *TokenArray[4]);
		}
		if (pos == 4) {
				p.drawImage(411, 608, *TokenArray[4]);
		}
		if (pos == 5) {
				p.drawImage(361, 608, *TokenArray[4]);
		}
		if (pos == 6) {
				p.drawImage(311, 608, *TokenArray[4]);
		}
		if (pos == 7) {
				p.drawImage(261, 608, *TokenArray[4]);
		}
		if (pos == 8) {
				p.drawImage(211, 608, *TokenArray[4]);
		}
		if (pos == 9) {
				p.drawImage(161, 608, *TokenArray[4]);
		}
		if (pos == 10) {
				p.drawImage(111, 608, *TokenArray[4]);
		}
		if (pos == 11) {
			if(jail){
				p.drawImage(52, 608, *TokenArray[4]);
			}
			else{
				p.drawImage(27, 613, *TokenArray[4]);
			}
		}
		if (pos == 12) {
				p.drawImage(40, 511, *TokenArraySide[4]);
		}
		if (pos == 13) {
				p.drawImage(40, 461, *TokenArraySide[4]);
		}
		if (pos == 14) {
				p.drawImage(40, 411, *TokenArraySide[4]);
		}
		if (pos == 15) {
				p.drawImage(40, 361, *TokenArraySide[4]);
		}
		if (pos == 16) {
				p.drawImage(40, 311, *TokenArraySide[4]);
		}
		if (pos == 17) {
				p.drawImage(40, 261, *TokenArraySide[4]);
		}
		if (pos == 18) {
				p.drawImage(40, 211, *TokenArraySide[4]);
		}
		if (pos == 19) {
				p.drawImage(40, 161, *TokenArraySide[4]);
		}
		if (pos == 20) {
				p.drawImage(40, 111, *TokenArraySide[4]);
		}
		if (pos == 21) {
				p.drawImage(73, 50, *TokenArray[4]);
		}
		if (pos == 22) {
				p.drawImage(136, 40, *TokenArray[4]);
		}
		if (pos == 23) {
				p.drawImage(186, 40, *TokenArray[4]);
		}
		if (pos == 24) {
				p.drawImage(236, 40, *TokenArray[4]);
		}
		if (pos == 25) {
				p.drawImage(286, 40, *TokenArray[4]);
		}
		if (pos == 26) {
				p.drawImage(336, 40, *TokenArray[4]);
		}
		if (pos == 27) {
				p.drawImage(386, 40, *TokenArray[4]);
		}
		if (pos == 28) {
				p.drawImage(436, 40, *TokenArray[4]);
		}
		if (pos == 29) {
				p.drawImage(486, 40, *TokenArray[4]);
		}
		if (pos == 30) {
				p.drawImage(536, 40, *TokenArray[4]);
		}
		//if (pos == 31) {
		//}
		if (pos == 32) {
				p.drawImage(608, 136, *TokenArraySide[4]);
		}
		if (pos == 33) {
				p.drawImage(608, 186, *TokenArraySide[4]);
		}
		if (pos == 34) {
				p.drawImage(608, 236, *TokenArraySide[4]);
		}
		if (pos == 35) {
				p.drawImage(608, 286, *TokenArraySide[4]);
		}
		if (pos == 36) {
				p.drawImage(608, 336, *TokenArraySide[4]);
		}
		if (pos == 37) {
				p.drawImage(608, 386, *TokenArraySide[4]);
		}
		if (pos == 38) {
				p.drawImage(608, 436, *TokenArraySide[4]);
		}
		if (pos == 39) {
				p.drawImage(608, 486, *TokenArraySide[4]);
		}
		if (pos == 40) {
				p.drawImage(608, 536, *TokenArraySide[4]);
		}
		if (pos == 41) {
				p.drawImage(482, 508, *TokenArray[4]);
		}
		if (pos == 42) {
				p.drawImage(411, 508, *TokenArray[4]);
		}
		if (pos == 43) {
				p.drawImage(361, 508, *TokenArray[4]);
		}
		if (pos == 44) {
				p.drawImage(311, 508, *TokenArray[4]);
		}
		if (pos == 45) {
				p.drawImage(261, 508, *TokenArray[4]);
		}
		if (pos == 46) {
				p.drawImage(211, 508, *TokenArray[4]);
		}
		//if (pos == 47) {
		//}
		if (pos == 48) {
				p.drawImage(140, 411, *TokenArraySide[4]);
		}
		if (pos == 49) {
				p.drawImage(140, 361, *TokenArraySide[4]);
		}
		if (pos == 50) {
				p.drawImage(140, 311, *TokenArraySide[4]);
		}
		if (pos == 51) {
				p.drawImage(140, 261, *TokenArraySide[4]);
		}
		if (pos == 52) {
				p.drawImage(140, 211, *TokenArraySide[4]);
		}
		if (pos == 53) {
				p.drawImage(173, 150, *TokenArray[4]);
		}
		if (pos == 54) {
				p.drawImage(236, 140, *TokenArray[4]);
		}
		if (pos == 55) {
				p.drawImage(286, 140, *TokenArray[4]);
		}
		if (pos == 56) {
				p.drawImage(336, 140, *TokenArray[4]);
		}
		if (pos == 57) {
				p.drawImage(386, 140, *TokenArray[4]);
		}
		if (pos == 58) {
				p.drawImage(436, 140, *TokenArray[4]);
		}
		if (pos == 59) {
				p.drawImage(492, 150, *TokenArray[4]);
		}
		if (pos == 60) {
				p.drawImage(508, 236, *TokenArraySide[4]);
		}
		if (pos == 61) {
				p.drawImage(508, 286, *TokenArraySide[4]);
		}
		if (pos == 62) {
				p.drawImage(508, 336, *TokenArraySide[4]);
		}
		if (pos == 63) {
				p.drawImage(508, 386, *TokenArraySide[4]);
		}
		if (pos == 64) {
				p.drawImage(508, 436, *TokenArraySide[4]);
		}

}

if (player == 6) {
		if (pos == 1) {
				p.drawImage(607, 608, *TokenArray[5]);
		}
		if (pos == 2) {
				p.drawImage(536, 608, *TokenArray[5]);
		}
		if (pos == 3) {
				p.drawImage(486, 608, *TokenArray[5]);
		}
		if (pos == 4) {
				p.drawImage(436, 608, *TokenArray[5]);
		}
		if (pos == 5) {
				p.drawImage(386, 608, *TokenArray[5]);
		}
		if (pos == 6) {
				p.drawImage(336, 608, *TokenArray[5]);
		}
		if (pos == 7) {
				p.drawImage(286, 608, *TokenArray[5]);
		}
		if (pos == 8) {
				p.drawImage(236, 608, *TokenArray[5]);
		}
		if (pos == 9) {
				p.drawImage(186, 608, *TokenArray[5]);
		}
		if (pos == 10) {
				p.drawImage(136, 608, *TokenArray[5]);
		}
		if (pos == 11) {
			if(jail){
				p.drawImage(77, 608, *TokenArray[5]);
			}
			else{
				p.drawImage(27, 630, *TokenArray[5]);
			}
		}
		if (pos == 12) {
				p.drawImage(40, 536, *TokenArraySide[5]);
		}
		if (pos == 13) {
				p.drawImage(40, 486, *TokenArraySide[5]);
		}
		if (pos == 14) {
				p.drawImage(40, 436, *TokenArraySide[5]);
		}
		if (pos == 15) {
				p.drawImage(40, 386, *TokenArraySide[5]);
		}
		if (pos == 16) {
				p.drawImage(40, 336, *TokenArraySide[5]);
		}
		if (pos == 17) {
				p.drawImage(40, 286, *TokenArraySide[5]);
		}
		if (pos == 18) {
				p.drawImage(40, 236, *TokenArraySide[5]);
		}
		if (pos == 19) {
				p.drawImage(40, 186, *TokenArraySide[5]);
		}
		if (pos == 20) {
				p.drawImage(40, 136, *TokenArraySide[5]);
		}
		if (pos == 21) {
				p.drawImage(48, 50, *TokenArray[5]);
		}
		if (pos == 22) {
				p.drawImage(111, 40, *TokenArray[5]);
		}
		if (pos == 23) {
				p.drawImage(161, 40, *TokenArray[5]);
		}
		if (pos == 24) {
				p.drawImage(211, 40, *TokenArray[5]);
		}
		if (pos == 25) {
				p.drawImage(261, 40, *TokenArray[5]);
		}
		if (pos == 26) {
				p.drawImage(311, 40, *TokenArray[5]);
		}
		if (pos == 27) {
				p.drawImage(361, 40, *TokenArray[5]);
		}
		if (pos == 28) {
				p.drawImage(411, 40, *TokenArray[5]);
		}
		if (pos == 29) {
				p.drawImage(461, 40, *TokenArray[5]);
		}
		if (pos == 30) {
				p.drawImage(511, 40, *TokenArray[5]);
		}
		//if (pos == 31) {
		//}
		if (pos == 32) {
				p.drawImage(608, 111, *TokenArraySide[5]);
		}
		if (pos == 33) {
				p.drawImage(608, 161, *TokenArraySide[5]);
		}
		if (pos == 34) {
				p.drawImage(608, 211, *TokenArraySide[5]);
		}
		if (pos == 35) {
				p.drawImage(608, 261, *TokenArraySide[5]);
		}
		if (pos == 36) {
				p.drawImage(608, 311, *TokenArraySide[5]);
		}
		if (pos == 37) {
				p.drawImage(608, 361, *TokenArraySide[5]);
		}
		if (pos == 38) {
				p.drawImage(608, 411, *TokenArraySide[5]);
		}
		if (pos == 39) {
				p.drawImage(608, 461, *TokenArraySide[5]);
		}
		if (pos == 40) {
				p.drawImage(608, 511, *TokenArraySide[5]);
		}
		if (pos == 41) {
				p.drawImage(507, 508, *TokenArray[5]);
		}
		if (pos == 42) {
				p.drawImage(436, 508, *TokenArray[5]);
		}
		if (pos == 43) {
				p.drawImage(386, 508, *TokenArray[5]);
		}
		if (pos == 44) {
				p.drawImage(336, 508, *TokenArray[5]);
		}
		if (pos == 45) {
				p.drawImage(286, 508, *TokenArray[5]);
		}
		if (pos == 46) {
				p.drawImage(236, 508, *TokenArray[5]);
		}
		//if (pos == 47) {
		//}
		if (pos == 48) {
				p.drawImage(140, 436, *TokenArraySide[5]);
		}
		if (pos == 49) {
				p.drawImage(140, 386, *TokenArraySide[5]);
		}
		if (pos == 50) {
				p.drawImage(140, 336, *TokenArraySide[5]);
		}
		if (pos == 51) {
				p.drawImage(140, 286, *TokenArraySide[5]);
		}
		if (pos == 52) {
				p.drawImage(140, 236, *TokenArraySide[5]);
		}
		if (pos == 53) {
				p.drawImage(148, 150, *TokenArray[5]);
		}
		if (pos == 54) {
				p.drawImage(211, 140, *TokenArray[5]);
		}
		if (pos == 55) {
				p.drawImage(261, 140, *TokenArray[5]);
		}
		if (pos == 56) {
				p.drawImage(311, 140, *TokenArray[5]);
		}
		if (pos == 57) {
				p.drawImage(361, 140, *TokenArray[5]);
		}
		if (pos == 58) {
				p.drawImage(411, 140, *TokenArray[5]);
		}
		if (pos == 59) {
				p.drawImage(467, 150, *TokenArray[5]);
		}
		if (pos == 60) {
				p.drawImage(508, 211, *TokenArraySide[5]);
		}
		if (pos == 61) {
				p.drawImage(508, 261, *TokenArraySide[5]);
		}
		if (pos == 62) {
				p.drawImage(508, 311, *TokenArraySide[5]);
		}
		if (pos == 63) {
				p.drawImage(508, 361, *TokenArraySide[5]);
		}
		if (pos == 64) {
				p.drawImage(508, 411, *TokenArraySide[5]);
		}

}

if (player == 7) {
		if (pos == 1) {
				p.drawImage(582, 621, *TokenArray[6]);
		}
		if (pos == 2) {
				p.drawImage(511, 621, *TokenArray[6]);
		}
		if (pos == 3) {
				p.drawImage(461, 621, *TokenArray[6]);
		}
		if (pos == 4) {
				p.drawImage(411, 621, *TokenArray[6]);
		}
		if (pos == 5) {
				p.drawImage(361, 621, *TokenArray[6]);
		}
		if (pos == 6) {
				p.drawImage(311, 621, *TokenArray[6]);
		}
		if (pos == 7) {
				p.drawImage(261, 621, *TokenArray[6]);
		}
		if (pos == 8) {
				p.drawImage(211, 621, *TokenArray[6]);
		}
		if (pos == 9) {
				p.drawImage(161, 621, *TokenArray[6]);
		}
		if (pos == 10) {
				p.drawImage(111, 621, *TokenArray[6]);
		}
		if (pos == 11) {
			if(jail){
				p.drawImage(52, 621, *TokenArray[6]);
			}
			else{
				p.drawImage(52, 630, *TokenArray[6]);
			}
		}
		if (pos == 12) {
				p.drawImage(27, 511, *TokenArraySide[6]);
		}
		if (pos == 13) {
				p.drawImage(27, 461, *TokenArraySide[6]);
		}
		if (pos == 14) {
				p.drawImage(27, 411, *TokenArraySide[6]);
		}
		if (pos == 15) {
				p.drawImage(27, 361, *TokenArraySide[6]);
		}
		if (pos == 16) {
				p.drawImage(27, 311, *TokenArraySide[6]);
		}
		if (pos == 17) {
				p.drawImage(27, 261, *TokenArraySide[6]);
		}
		if (pos == 18) {
				p.drawImage(27, 211, *TokenArraySide[6]);
		}
		if (pos == 19) {
				p.drawImage(27, 161, *TokenArraySide[6]);
		}
		if (pos == 20) {
				p.drawImage(27, 111, *TokenArraySide[6]);
		}
		if (pos == 21) {
				p.drawImage(73, 37, *TokenArray[6]);
		}
		if (pos == 22) {
				p.drawImage(136, 27, *TokenArray[6]);
		}
		if (pos == 23) {
				p.drawImage(186, 27, *TokenArray[6]);
		}
		if (pos == 24) {
				p.drawImage(236, 27, *TokenArray[6]);
		}
		if (pos == 25) {
				p.drawImage(286, 27, *TokenArray[6]);
		}
		if (pos == 26) {
				p.drawImage(336, 27, *TokenArray[6]);
		}
		if (pos == 27) {
				p.drawImage(386, 27, *TokenArray[6]);
		}
		if (pos == 28) {
				p.drawImage(436, 27, *TokenArray[6]);
		}
		if (pos == 29) {
				p.drawImage(486, 27, *TokenArray[6]);
		}
		if (pos == 30) {
				p.drawImage(536, 27, *TokenArray[6]);
		}
		//if (pos == 31) {
		//}
		if (pos == 32) {
				p.drawImage(621, 136, *TokenArraySide[6]);
		}
		if (pos == 33) {
				p.drawImage(621, 186, *TokenArraySide[6]);
		}
		if (pos == 34) {
				p.drawImage(621, 236, *TokenArraySide[6]);
		}
		if (pos == 35) {
				p.drawImage(621, 286, *TokenArraySide[6]);
		}
		if (pos == 36) {
				p.drawImage(621, 336, *TokenArraySide[6]);
		}
		if (pos == 37) {
				p.drawImage(621, 386, *TokenArraySide[6]);
		}
		if (pos == 38) {
				p.drawImage(621, 436, *TokenArraySide[6]);
		}
		if (pos == 39) {
				p.drawImage(621, 486, *TokenArraySide[6]);
		}
		if (pos == 40) {
				p.drawImage(621, 536, *TokenArraySide[6]);
		}
		if (pos == 41) {
				p.drawImage(482, 521, *TokenArray[6]);
		}
		if (pos == 42) {
				p.drawImage(411, 521, *TokenArray[6]);
		}
		if (pos == 43) {
				p.drawImage(361, 521, *TokenArray[6]);
		}
		if (pos == 44) {
				p.drawImage(311, 521, *TokenArray[6]);
		}
		if (pos == 45) {
				p.drawImage(261, 521, *TokenArray[6]);
		}
		if (pos == 46) {
				p.drawImage(211, 521, *TokenArray[6]);
		}
		//if (pos == 47) {
		//}
		if (pos == 48) {
				p.drawImage(127, 411, *TokenArraySide[6]);
		}
		if (pos == 49) {
				p.drawImage(127, 361, *TokenArraySide[6]);
		}
		if (pos == 50) {
				p.drawImage(127, 311, *TokenArraySide[6]);
		}
		if (pos == 51) {
				p.drawImage(127, 261, *TokenArraySide[6]);
		}
		if (pos == 52) {
				p.drawImage(127, 211, *TokenArraySide[6]);
		}
		if (pos == 53) {
				p.drawImage(173, 137, *TokenArray[6]);
		}
		if (pos == 54) {
				p.drawImage(236, 127, *TokenArray[6]);
		}
		if (pos == 55) {
				p.drawImage(286, 127, *TokenArray[6]);
		}
		if (pos == 56) {
				p.drawImage(336, 127, *TokenArray[6]);
		}
		if (pos == 57) {
				p.drawImage(386, 127, *TokenArray[6]);
		}
		if (pos == 58) {
				p.drawImage(436, 127, *TokenArray[6]);
		}
		if (pos == 59) {
				p.drawImage(492, 137, *TokenArray[6]);
		}
		if (pos == 60) {
				p.drawImage(521, 236, *TokenArraySide[6]);
		}
		if (pos == 61) {
				p.drawImage(521, 286, *TokenArraySide[6]);
		}
		if (pos == 62) {
				p.drawImage(521, 336, *TokenArraySide[6]);
		}
		if (pos == 63) {
				p.drawImage(521, 386, *TokenArraySide[6]);
		}
		if (pos == 64) {
				p.drawImage(521, 436, *TokenArraySide[6]);
		}

		
}

if (player == 8) {
		if (pos == 1) {
				p.drawImage(607, 621, *TokenArray[7]);
		}
		if (pos == 2) {
				p.drawImage(536, 621, *TokenArray[7]);
		}
		if (pos == 3) {
				p.drawImage(486, 621, *TokenArray[7]);
		}
		if (pos == 4) {
				p.drawImage(436, 621, *TokenArray[7]);
		}
		if (pos == 5) {
				p.drawImage(386, 621, *TokenArray[7]);
		}
		if (pos == 6) {
				p.drawImage(336, 621, *TokenArray[7]);
		}
		if (pos == 7) {
				p.drawImage(286, 621, *TokenArray[7]);
		}
		if (pos == 8) {
				p.drawImage(236, 621, *TokenArray[7]);
		}
		if (pos == 9) {
				p.drawImage(186, 621, *TokenArray[7]);
		}
		if (pos == 10) {
				p.drawImage(136, 621, *TokenArray[7]);
		}
		if (pos == 11) {
			if(jail){
				p.drawImage(77, 621, *TokenArray[7]);
			}
			else{
				p.drawImage(77, 630, *TokenArray[7]);
			}
		}
		if (pos == 12) {
				p.drawImage(27, 536, *TokenArraySide[7]);
		}
		if (pos == 13) {
				p.drawImage(27, 486, *TokenArraySide[7]);
		}
		if (pos == 14) {
				p.drawImage(27, 436, *TokenArraySide[7]);
		}
		if (pos == 15) {
				p.drawImage(27, 386, *TokenArraySide[7]);
		}
		if (pos == 16) {
				p.drawImage(27, 336, *TokenArraySide[7]);
		}
		if (pos == 17) {
				p.drawImage(27, 286, *TokenArraySide[7]);
		}
		if (pos == 18) {
				p.drawImage(27, 236, *TokenArraySide[7]);
		}
		if (pos == 19) {
				p.drawImage(27, 186, *TokenArraySide[7]);
		}
		if (pos == 20) {
				p.drawImage(27, 136, *TokenArraySide[7]);
		}
		if (pos == 21) {
				p.drawImage(48, 37, *TokenArray[7]);
		}
		if (pos == 22) {
				p.drawImage(111, 27, *TokenArray[7]);
		}
		if (pos == 23) {
				p.drawImage(161, 27, *TokenArray[7]);
		}
		if (pos == 24) {
				p.drawImage(211, 27, *TokenArray[7]);
		}
		if (pos == 25) {
				p.drawImage(261, 27, *TokenArray[7]);
		}
		if (pos == 26) {
				p.drawImage(311, 27, *TokenArray[7]);
		}
		if (pos == 27) {
				p.drawImage(361, 27, *TokenArray[7]);
		}
		if (pos == 28) {
				p.drawImage(411, 27, *TokenArray[7]);
		}
		if (pos == 29) {
				p.drawImage(461, 27, *TokenArray[7]);
		}
		if (pos == 30) {
				p.drawImage(511, 27, *TokenArray[7]);
		}
		//if (pos == 31) {
		//}
		if (pos == 32) {
				p.drawImage(621, 111, *TokenArraySide[7]);
		}
		if (pos == 33) {
				p.drawImage(621, 161, *TokenArraySide[7]);
		}
		if (pos == 34) {
				p.drawImage(621, 211, *TokenArraySide[7]);
		}
		if (pos == 35) {
				p.drawImage(621, 261, *TokenArraySide[7]);
		}
		if (pos == 36) {
				p.drawImage(621, 311, *TokenArraySide[7]);
		}
		if (pos == 37) {
				p.drawImage(621, 361, *TokenArraySide[7]);
		}
		if (pos == 38) {
				p.drawImage(621, 411, *TokenArraySide[7]);
		}
		if (pos == 39) {
				p.drawImage(621, 461, *TokenArraySide[7]);
		}
		if (pos == 40) {
				p.drawImage(621, 511, *TokenArraySide[7]);
		}
		if (pos == 41) {
				p.drawImage(507, 521, *TokenArray[7]);
		}
		if (pos == 42) {
				p.drawImage(436, 521, *TokenArray[7]);
		}
		if (pos == 43) {
				p.drawImage(386, 521, *TokenArray[7]);
		}
		if (pos == 44) {
				p.drawImage(336, 521, *TokenArray[7]);
		}
		if (pos == 45) {
				p.drawImage(286, 521, *TokenArray[7]);
		}
		if (pos == 46) {
				p.drawImage(236, 521, *TokenArray[7]);
		}
		//if (pos == 47) {
		//}
		if (pos == 48) {
				p.drawImage(127, 436, *TokenArraySide[7]);
		}
		if (pos == 49) {
				p.drawImage(127, 386, *TokenArraySide[7]);
		}
		if (pos == 50) {
				p.drawImage(127, 336, *TokenArraySide[7]);
		}
		if (pos == 51) {
				p.drawImage(127, 286, *TokenArraySide[7]);
		}
		if (pos == 52) {
				p.drawImage(127, 236, *TokenArraySide[7]);
		}
		if (pos == 53) {
				p.drawImage(148, 137, *TokenArray[7]);
		}
		if (pos == 54) {
				p.drawImage(211, 127, *TokenArray[7]);
		}
		if (pos == 55) {
				p.drawImage(261, 127, *TokenArray[7]);
		}
		if (pos == 56) {
				p.drawImage(311, 127, *TokenArray[7]);
		}
		if (pos == 57) {
				p.drawImage(361, 127, *TokenArray[7]);
		}
		if (pos == 58) {
				p.drawImage(411, 127, *TokenArray[7]);
		}
		if (pos == 59) {
				p.drawImage(467, 137, *TokenArray[7]);
		}
		if (pos == 60) {
				p.drawImage(521, 211, *TokenArraySide[7]);
		}
		if (pos == 61) {
				p.drawImage(521, 261, *TokenArraySide[7]);
		}
		if (pos == 62) {
				p.drawImage(521, 311, *TokenArraySide[7]);
		}
		if (pos == 63) {
				p.drawImage(521, 361, *TokenArraySide[7]);
		}
		if (pos == 64) {
				p.drawImage(521, 411, *TokenArraySide[7]);
		}
	}


}

