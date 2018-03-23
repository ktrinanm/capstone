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

using namespace std;

int main()
{
	while(true)
	{
		theme1();
		theme2();
		theme3();
		theme4();
		theme5();
		theme6();
	}
}

