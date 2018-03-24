/**
 * Functions for each theme 
 * */

#include <unistd.h>
#include <wiringPiSPI.h>
#include <iostream>

using namespace std;

#ifndef THEMES_H
#define THEMES_H

#define SPI_SPEED	9000000
#define SPI_CHANNEL 0

#define PACKET_SIZE	(1 + (16 * 16 * 3)) // # of inputs needed by LED
#define RUN_TIME	(30 * SPI_SPEED)	// Time for each theme to run in ms

unsigned char buffer[PACKET_SIZE];

void theme1();
void theme2();
void theme3();
void theme4();
void theme5();
void theme6();

void theme1()
{
	if( wiringPiSPISetup( SPI_CHANNEL, SPI_SPEED ) == -1 )
	{
		cerr << "Could not initialize SPI." << endl;
		return;
	}

	int numChangesYellow = 256 * 2; // Two sets of color changes for yellow
	int numChangesWhite = 256 * 2;
	int sleepTimeAfterChange = RUN_TIME/9/(numChangesYellow * 2 
			+ numChangesWhite);

	for (int i = 0; i < numChangesYellow/2; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++) // Run for each index of LED board
		{
			buffer[j*3 + 1] = i;
			buffer[j*3 + 2] = i;
			buffer[j*3 + 3] = 0;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE);
		usleep((int)(sleepTimeAfterChange * 2)); // Sleep twice as long for yellow
	}

	for (int i = 0; i < numChangesWhite/2; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++) // Run for each index of LED board
		{
			buffer[j*3 + 1] = 255;
			buffer[j*3 + 2] = 255;
			buffer[j*3 + 3] = i;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE);
		usleep(sleepTimeAfterChange);
	}

	for (int i = 0; i < numChangesWhite/2; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++) // Run for each index of LED board
		{
			buffer[j*3 + 1] = 255;
			buffer[j*3 + 2] = 255;
			buffer[j*3 + 3] = 255 - i;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE);
		usleep(sleepTimeAfterChange);
	}

	for (int i = 0; i < numChangesYellow/2; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++) // Run for each index of LED board
		{
			buffer[j*3 + 1] = 255 - i;
			buffer[j*3 + 2] = 255 - i;
			buffer[j*3 + 3] = 0;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE);
		usleep(sleepTimeAfterChange * 2); // Sleep twice as long for yellow
	}

	buffer[0] = 0x72;

	wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
	usleep((int)3000000);
}

void theme2()
{
	if( wiringPiSPISetup( SPI_CHANNEL, SPI_SPEED ) == -1 )
	{
		cerr << "Could not initialize SPI." << endl;
		return;
	}

	int sleepTimeAfterChange = RUN_TIME/(9*256*2);

	//Lighting up
	// // Dark red
	for(int i = 0; i < 100; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++)
		{
			buffer[j*3 + 1] = i;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep(sleepTimeAfterChange);
	}

	// // Going to orange/yellow
	for(int i = 0; i < 100; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++)
		{
			buffer[j*3 + 1] = 100 + i;
			buffer[j*3 + 2] = i*2;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep(sleepTimeAfterChange);
	}

	// // Going to white-ish
	for(int i = 0; i < 50; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++)
		{
			buffer[j*3 + 1] = 200 + i;
			buffer[j*3 + 2] = 200 + i;
			buffer[j*3 + 3] = i*5;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep(sleepTimeAfterChange);
	}

	// // going to White
	for(int i = 0; i < 5; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++)
		{
			buffer[j*3 + 1] = 250 + i;
			buffer[j*3 + 2] = 250 + i;
			buffer[j*3 + 3] = 250 + i;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep(sleepTimeAfterChange);
	}

	// Reversing
	// // Back to white-ish
	for(int i = 0; i < 5; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++)
		{
			buffer[j*3 + 1] = 255 - i;
			buffer[j*3 + 2] = 255 - i;
			buffer[j*3 + 3] = 255 - i;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep(sleepTimeAfterChange);
	}

	// // Going to yellow
	for(int i = 0; i < 50; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++)
		{
			buffer[j*3 + 1] = 250 - i;
			buffer[j*3 + 2] = 250 - i;
			buffer[j*3 + 3] = 250 - i*5;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep(sleepTimeAfterChange);
	}

	// // Going to red 
	for(int i = 0; i < 100; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++)
		{
			buffer[j*3 + 1] = 200 - i;
			buffer[j*3 + 2] = 200 - i*2;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep(sleepTimeAfterChange);
	}

	// // Going to Black
	for(int i = 0; i < 100; i++)
	{
		buffer[0] = 0x72;

		for(int j = 0; j < 256; j++)
		{
			buffer[j*3 + 1] = 100 - i;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep(sleepTimeAfterChange);
	}

	buffer[0] = 0x72;

	wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
	usleep((int)3000000);
}

void theme3()
{
}

void theme4()
{
}

void theme5()
{
}

void theme6()
{
}

#endif
