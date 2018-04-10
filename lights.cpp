/**
 * 	Rise and Shine Alarm Pi
 * 	Braden Roberts and Katrina Mehring
 * 	Spring 2018 CSIS Capstone
 *
 * 	This project is a Proof of Concept of a Natural Light Alarm clock. 
 * 	It utilizes a Raspberry Pi 3B and a Pimoroni Unicorn Hat HD.
 *
 **/
#include "themes.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	if( wiringPiSPISetup( SPI_CHANNEL, SPI_SPEED ) == -1 )
	{
		cerr << "Could not initialize SPI." << endl;
		return 1;
	}

	switch(stoi(argv[1]))
	{
		case 1:
			theme1();
			break;
		case 2:
			theme2();
			break;
		case 3:
			theme3();
			break;
		case 4:
			theme4();
			break;
		case 5:
			theme5();
			break;
		default:
			cerr << "Not a valid theme" << endl;
	}
}

