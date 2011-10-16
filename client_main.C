#include <qapplication.h>
#include <qcdestyle.h>
#include <qwindowsstyle.h>
#include <qplatinumstyle.h>
#include "GUI/Yloponom.h"

#include <iostream>


#include <unistd.h>

//-------------------------------------------------------
// Main method that reads and executes the program
//-------------------------------------------------------
int main( int argc, char ** argv )
{

	bool abrt = false;
	if(0 != access("data.dat", R_OK)) {
		std::cerr << "client must be executed in same directory as data.dat file." << std::endl;
		abrt = true;
	}

	if(0 != access("bin", R_OK)) {
		std::cerr << "client must be executed in same directory as 'bin' folder." << std::endl;
		abrt = true;
	}

	if(abrt) abort();

    QApplication a( argc, argv );

	QApplication::setStyle( new QPlatinumStyle );

    Yloponom ylop;
    a.setMainWidget( &ylop );
    ylop.show( );

	int r = a.exec( );

    return r;
}
