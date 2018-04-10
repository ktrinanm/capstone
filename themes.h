/**
 * Functions for each theme 
 * */

#include <unistd.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <cmath>

using namespace std;

#ifndef THEMES_H
#define THEMES_H

#define SPI_SPEED	9000000
#define SPI_CHANNEL 0

#define PACKET_SIZE	(1 + (16 * 16 * 3)) // # of inputs needed by LED
#define RUN_TIME	(30 * 1000000)	// Run Time for each theme in microsec

unsigned char buffer[PACKET_SIZE];

void theme1();
void theme2();
void theme3();
void theme4();
void theme5();
void theme6();

void theme1()
{
	int numChangesYellow = 256 * 2; // Two sets of color changes for yellow
	int numChangesWhite = 256 * 2; // 2 sets color changes yellow -> white
	int sleepTimeAfterChange = RUN_TIME/(numChangesYellow * 2 
			+ numChangesWhite); //total run time divided by # of cycles
								// giving twice as much time for yellow
								// than white

	for (int i = 0; i < numChangesYellow/2; i++)
	{
		buffer[0] = 0x72; // Set to SOF byte

		for(int j = 0; j < 256; j++) // Run for each index of LED board
		{
			// Starting at buffer[1], add r,g,b per item on LED array
			// r = g, b = 0 -> yellow
			buffer[j*3 + 1] = i; 
			buffer[j*3 + 2] = i;
			buffer[j*3 + 3] = 0;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE);
		usleep((int)(sleepTimeAfterChange * 2)); // Sleep twice for yellow
	}

	for (int i = 0; i < numChangesWhite/2; i++)
	{
		buffer[0] = 0x72;// Set to SOF byte

		for(int j = 0; j < 256; j++) // Run for each index of LED board
		{
			// Starting at buffer[1], add r,g,b per item on LED array
			// r = g = 255, b -> 255 -> yellow to white
			buffer[j*3 + 1] = 255;
			buffer[j*3 + 2] = 255;
			buffer[j*3 + 3] = i;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE); // write GPIO
		usleep(sleepTimeAfterChange); //Sleep (microseconds)
	}

	for (int i = 0; i < numChangesWhite/2; i++)
	{
		buffer[0] = 0x72;// Set to SOF byte

		for(int j = 0; j < 256; j++) // Run for each index of LED board
		{
			// Starting at buffer[1], add r,g,b per item on LED array
			// r = g = 255, b = 255 -> 0  -> white to yellow
			buffer[j*3 + 1] = 255;
			buffer[j*3 + 2] = 255;
			buffer[j*3 + 3] = 255 - i;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE);
		usleep(sleepTimeAfterChange); // Sleep
	}

	for (int i = 0; i < numChangesYellow/2; i++)
	{
		buffer[0] = 0x72;// Set to SOF byte

		for(int j = 0; j < 256; j++) // Run for each index of LED board
		{
			// Starting at buffer[1], add r,g,b per item on LED array
			// r = g = 255 -> 0, b = 0 -> yellow to black
			buffer[j*3 + 1] = 255 - i;
			buffer[j*3 + 2] = 255 - i;
			buffer[j*3 + 3] = 0;
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE);
		usleep(sleepTimeAfterChange * 2); // Sleep twice as long for yellow
	}

	buffer[0] = 0x72;// Set to SOF byte

	wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
	usleep((int)3000000);
}

void theme2()
{
	int sleepTimeAfterChange = RUN_TIME/(256*2);

	// Lighting up
	for(int i = 0; i < 256; i++)
	{
		buffer[0] = 0x72; // Set SOF byte

		for(int j =0; j < 256; j++)
		{
			buffer[j*3 + 1] = i; // Red increases throughout

			if(i >= 100 && i < 200) // Red -> Orange -> Yellow
			{
				buffer[j*3 + 2] = (i-100)*2;
			}
			else if(i >= 200 && i < 250) // Yellow -> gray
			{
				buffer[j*3 + 2] = i;
				buffer[j*3 + 3] = (i - 200)*5;
			}
			else if(i >= 250) // Gray -> White
			{
				buffer[j*3 + 2] = i;
				buffer[j*3 + 3] = i;
			}
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep( sleepTimeAfterChange );
	}

	usleep((int)1000000); //Sleep (microseconds)

	// Reversing/Going to Black
	for(int i = 0; i < 256; i++)
	{
		buffer[0] = 0x72; // Set SOF byte

		for(int j =0; j < 256; j++)
		{
			buffer[j*3 + 1] = 255 - i; // Red increases throughout

			if(i < 6) // White -> Gray
			{
				buffer[j*3 + 2] = 255 - i;
				buffer[j*3 + 3] = 255 - i;
			}
			else if(i >= 6 && i < 56) // Gray -> yellow
			{
				buffer[j*3 + 2] = 255 - i;
				buffer[j*3 + 3] = 250 - (i-5)*5;
			}
			else if(i >= 56 && i < 156) // Yellow -> Orange -> Red
			{
				buffer[j*3 + 2] = 200 - (i-55)*2;
			}
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep( sleepTimeAfterChange );
	}

	buffer[0] = 0x72;// Set to SOF byte

	//set back to 0
	wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
	usleep((int)1000000); //Sleep (microseconds)
}

void theme3()
{
	int sleepTimeAfterChange = RUN_TIME/(256*2);

	// Light up
	for(int i = 0; i < 256; i++)
	{
		buffer[0] = 0x72; // Set SOF byte

		//light up inner 256
		if( i >= (256/8)*7 )
		{
			for(int x = 0; x < 16; x++)
			{
				for(int y = 0; y < 16; y++)
				{
					buffer[(x*16 + y)*3 + 1] = (i - 224)*8;

					// 236.5 = 224+(100/8)
					// 249 = 236.5+(100/8)
					// 254.5 = 249+(50/8)
					if( i >= 236 && i < 249 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 236)*2*8;
					}
					else if( i >= 249 && i < 254 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 224)*8;
						buffer[(x*16 + y)*3 + 3] = (i - 249)*5*8;
					}
					else if( i >= 254 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 224)*8;
						buffer[(x*16 + y)*3 + 3] = (i - 224)*8;
					}
				}
			}
		}


		//light up inner 196
		if( i >= (256/8)*6 )
		{
			for(int x = 1; x < 15; x++)
			{
				for(int y = 1; y < 15; y++)
				{
					buffer[(x*16 + y)*3 + 1] = (i - 192)*4;

					// 217 = 192+(100*2/8)
					// 242 = 217+(100*2/8)
					// 254.5 = 235+(50*2/8)
					if( i >= 217 && i < 242 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 217)*2*(8.0/2);
					}
					else if( i >= 242 && i < 254 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 192)*(8.0/2);
						buffer[(x*16 + y)*3 + 3] = (i - 242)*5*(8.0/2);
					}
					else if( i >= 254 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 192)*(8.0/2);
						buffer[(x*16 + y)*3 + 3] = (i - 192)*(8.0/2);
					}
				}
			}
		}


		//light up inner 144
		if( i >= (256/8)*5 )
		{
			for(int x = 2; x < 14; x++)
			{
				for(int y = 2; y < 14; y++)
				{
					buffer[(x*16 + y)*3 + 1] = (i - 160)*(8.0/3);

					// 197.5 = 160+(100*3/8)
					// 235 = 197.5+(100*3/8)
					// 253.75 = 235+(50*3/8)
					if( i >= 197 && i < 235 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 197)*2*(8.0/3);
					}
					else if( i >= 235 && i < 253 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 160)*(8.0/3);
						buffer[(x*16 + y)*3 + 3] = (i - 235)*5*(8.0/3);
					}
					else if( i >= 253 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 160)*(8.0/3);
						buffer[(x*16 + y)*3 + 3] = (i - 160)*(8.0/3);
					}
				}
			}
		}

		//light up inner 100
		if( i >= (256/8)*4 )
		{
			for(int x = 3; x < 13; x++)
			{
				for(int y = 3; y < 13; y++)
				{
					buffer[(x*16 + y)*3 + 1] = (i - 128)*2; // 8/4 = 2

					// 178 = 128 + 100*4/8
					// 228 = 178 + 100*4/8
					// 253 = 228 + 50*4/8
					if( i >= 178 && i < 228 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 178)*2*2;
					}
					else if( i >= 228 && i < 253 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 128)*2;
						buffer[(x*16 + y)*3 + 3] = (i - 228)*5*2;
					}
					else if( i >= 253 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 128)*2;
						buffer[(x*16 + y)*3 + 3] = (i - 128)*2;
					}
				}
			}
		}

		//light up inner 64
		if( i >= (256/8)*3 )
		{
			for(int x = 4; x < 12; x++)
			{
				for(int y = 4; y < 12; y++)
				{
					buffer[(x*16 + y)*3 + 1] = (i - 96)*(8.0/5);

					// 158.5 = 96+(100*5/8)
					// 221 = 158.5+(100*5/8)
					// 252.25 = 221+(50*5/8)
					if( i >= 158 && i < 221 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 158)*2*(8.0/5);
					}
					else if( i >= 221 && i < 252 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 96)*(8.0/5);
						buffer[(x*16 + y)*3 + 3] = (i - 221)*5*(8.0/5);
					}
					else if( i >= 252 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 96)*(8.0/5);
						buffer[(x*16 + y)*3 + 3] = (i - 96)*(8.0/5);
					}
				}
			}
		}

		//light up inner 36
		if( i >= (256/8)*2 )
		{
			for(int x = 5; x < 11; x++)
			{
				for(int y = 5; y < 11; y++)
				{
					buffer[(x*16 + y)*3 + 1] = (i - 64)*(4.0/3);

					// 139 = 64+(100*6/8)
					// 214 = 139+(100*6/8)
					// 251.5 = 214+(50*6/8)
					if( i >= 139 && i < 214 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 139)*2*(4.0/3);
					}
					else if( i >= 214 && i < 251 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 64)*(4.0/3);
						buffer[(x*16 + y)*3 + 3] = (i - 214)*5*(4.0/3);
					}
					else if( i >= 251 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 64)*(4.0/3);
						buffer[(x*16 + y)*3 + 3] = (i - 64)*(4.0/3);
					}
				}
			}
		}


		//light up inner 16
		if( i >= 256/8 )
		{
			for(int x = 6; x < 10; x++)
			{
				for(int y = 6; y < 10; y++)
				{
					buffer[(x*16 + y)*3 + 1] = (i - 32)*(8.0/7);

					// 119 = 32+(100*7/8) (truncated)
					// 207 = 119.5+(100*7/8)
					// 250.75 = 207+(50*7/8)
					if( i >= 119 && i < 207 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 119)*2*(8.0/7);
					}
					else if( i >= 207 && i < 250 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 32)*(8.0/7);
						buffer[(x*16 + y)*3 + 3] =(i - 207)*5*(8.0/7);
					}
					else if( i >= 250 )
					{
						buffer[(x*16 + y)*3 + 2] = (i - 32)*(8.0/7);
						buffer[(x*16 + y)*3 + 3] = (i - 32)*(8.0/7);
					}
				}
			}
		}

		// Light up starting at inner 4
		for(int x = 7; x < 9; x++)
		{
			for(int y = 7; y < 9; y++)
			{
				buffer[(x*16 + y)*3 + 1] = i;

				if( i >= 100 && i < 200 )
				{
					buffer[(x*16 + y)*3 + 2] = (i - 100)*2;
				}
				else if( i >= 200 && i < 250 )
				{
					buffer[(x*16 + y)*3 + 2] = i;
					buffer[(x*16 + y)*3 + 3] = (i - 200)*5;
				}
				else if( i >= 250 && i < 256 )
				{ 
					buffer[(x*16 + y)*3 + 2] = i;
					buffer[(x*16 + y)*3 + 3] = i;
				}
			}
		}

		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep( sleepTimeAfterChange );
	}

	// Dim to black
	for(int i = 0; i < 256; i++)
	{
		buffer[0] = 0x72; // Set SOF byte

		// dim inner 256
		if( i >= (256/8)*7 )
		{
			for(int x = 0; x < 16; x++)
			{
				for(int y = 0; y < 16; y++)
				{
					buffer[(x*16 + y)*3 + 1] = 255 - (i - 224)*8;

					// 224.625 = 224+(5/8)
					// 230.875 = 224.625+(50/8)
					// 243.375 = 230.875+(100/8)
					if( i < 225 )
					{
						buffer[(x*16 + y)*3 + 2] = 255;
						buffer[(x*16 + y)*3 + 3] = 255;
					}
					else if( i >= 225 && i < 230 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 224)*8.0;
						buffer[(x*16 + y)*3 + 3] = 255 - (i - 225)*5*8.0;
					}
					else if( i >= 230 && i < 243 )
					{
						buffer[(x*16 + y)*3 + 2] = 215 - (i - 230)*2*8;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
					else
					{
						buffer[(x*16 + y)*3 + 2] = 0;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
				}
			}
		}
		else
		{
			for(int x = 0; x < 16; x++)
			{
				for(int y = 0; y < 16; y++)
				{
					
					buffer[(x*16 + y)*3 + 1] = 255;
					buffer[(x*16 + y)*3 + 2] = 255;
					buffer[(x*16 + y)*3 + 3] = 255;
				}
			}
		}

		// dim inner 196
		if( i >= (256/8)*6 )
		{
			for(int x = 1; x < 15; x++)
			{
				for(int y = 1; y < 15; y++)
				{
					buffer[(x*16 + y)*3 + 1] = 255 - (i - 192)*4;

					// 193.25 = 192+(5*2/8)
					// 205.75 = 193.25+(50*2/8)
					// 230.75 = 205.75+(100*2/8)
					if( i < 193 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 192)*8.0/2;
						buffer[(x*16 + y)*3 + 3] = 255 - (i - 192)*8.0/2;
					}
					else if( i >= 193 && i < 205 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 192)*8.0/2;
						buffer[(x*16 + y)*3 + 3] = 255 - (i - 193)*5*8.0/2;
					}
					else if( i >= 205 && i < 230 )
					{
						buffer[(x*16 + y)*3 + 2] = 207 - (i - 205)*2*8.0/2;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
					else
					{
						buffer[(x*16 + y)*3 + 2] = 0;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
				}
			}
		}
		else
		{
			for(int x = 1; x < 15; x++)
			{
				for(int y = 1; y < 15; y++)
				{
					
					buffer[(x*16 + y)*3 + 1] = 255;
					buffer[(x*16 + y)*3 + 2] = 255;
					buffer[(x*16 + y)*3 + 3] = 255;
				}
			}
		}

		// dim inner 144
		if( i >= (256/8)*5 )
		{
			for(int x = 2; x < 14; x++)
			{
				for(int y = 2; y < 14; y++)
				{
					buffer[(x*16 + y)*3 + 1] = 255 - (i - 160)*(8.0/3);
					
					// 161.875 = 160+(5*3/8)
					// 180.625 = 161.875+(50*3/8)
					// 218.125 = 180.625+(100*3/8)
					if( i < 161 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 160)*8.0/3;
						buffer[(x*16 + y)*3 + 3] = 255 - (i - 160)*8.0/3;
					}
					else if( i >= 161 && i < 180 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 160)*8.0/3;
						buffer[(x*16 + y)*3 + 3] = 255 - (i - 161)*5*8.0/3;
					}
					else if( i >= 180 && i < 218 )
					{
						buffer[(x*16 + y)*3 + 2] = 204 - (i - 180)*2*8.0/3;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
					else
					{
						buffer[(x*16 + y)*3 + 2] = 0;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
				}
			}
		}
		else
		{
			for(int x = 2; x < 14; x++)
			{
				for(int y = 2; y < 14; y++)
				{
					
					buffer[(x*16 + y)*3 + 1] = 255;
					buffer[(x*16 + y)*3 + 2] = 255;
					buffer[(x*16 + y)*3 + 3] = 255;
				}
			}
		}

		//dim inner 100
		if( i >= (256/8)*4 )
		{
			for(int x = 3; x < 13; x++)
			{
				for(int y = 3; y < 13; y++)
				{
					buffer[(x*16 + y)*3 + 1] = 255 - (i - 128)*2; // 8/4 = 2

					// 130.5 = 128+(5*4/8)
					// 155.35 = 130.5+(50*4/8)
					// 205.35 = 130.375+(100*4/8)
					if( i < 130 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 128)*8.0/4;
						buffer[(x*16 + y)*3 + 3] = 255 - (i - 128)*8.0/4;
					}
					else if( i >= 130 && i < 155 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 128)*8.0/4;
						buffer[(x*16 + y)*3 + 3] = 253 -(i - 130)*5*8.0/4;
					}
					else if( i >= 155 && i < 205 )
					{
						buffer[(x*16 + y)*3 + 2] = 203 -(i - 155)*2*8.0/4;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
					else
					{
						buffer[(x*16 + y)*3 + 2] = 0;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
				}
			}
		}
		else
		{
			for(int x = 3; x < 13; x++)
			{
				for(int y = 3; y < 13; y++)
				{
					
					buffer[(x*16 + y)*3 + 1] = 255;
					buffer[(x*16 + y)*3 + 2] = 255;
					buffer[(x*16 + y)*3 + 3] = 255;
				}
			}
		}



		// dim inner 64
		if( i >= (256/8)*3 )
		{
			for(int x = 4; x < 12; x++)
			{
				for(int y = 4; y < 12; y++)
				{
					buffer[(x*16 + y)*3 + 1] = 255 - (i - 96)*(8.0/5);

					// 99.125 = 96+(5*5/8)
					// 130.375 = 99.125+(50*5/8)
					// 192.875 = 130.375+(100*5/8)
					if( i < 99 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 96)*8.0/5;
						buffer[(x*16 + y)*3 + 3] = 255 - (i - 96)*8.0/5;
					}
					else if( i >= 99 && i < 130 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 96)*8.0/5;
						buffer[(x*16 + y)*3 + 3] = 251 - (i - 99)*5*8.0/5;
					}
					else if( i >= 130 && i < 192 )
					{
						buffer[(x*16 + y)*3 + 2] = 202 -(i - 130)*2*8.0/5;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
					else
					{
						buffer[(x*16 + y)*3 + 2] = 0;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
				}
			}
		}
		else
		{
			for(int x = 4; x < 12; x++)
			{
				for(int y = 4; y < 12; y++)
				{
					
					buffer[(x*16 + y)*3 + 1] = 255;
					buffer[(x*16 + y)*3 + 2] = 255;
					buffer[(x*16 + y)*3 + 3] = 255;
				}
			}
		}


		// dim inner 36
		if( i >= (256/8)*2 )
		{
			for(int x = 5; x < 11; x++)
			{
				for(int y = 5; y < 11; y++)
				{
					buffer[(x*16 + y)*3 + 1] = 255 - (i - 64)*4.0/3;

					// 67.75 = 64+(5*6/8)
					// 105.25 = 67.75+(50*6/8)
					// 180.25 = 105.25+(100*6/8)
					if( i < 67 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 64)*8.0/6;
						buffer[(x*16 + y)*3 + 3] = 255 - (i - 64)*8.0/6;
					}
					else if( i >= 67 && i < 105 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i - 64)*8.0/6;
						buffer[(x*16 + y)*3 + 3] = 252 - (i - 67)*5*8.0/6;
					}
					else if( i >= 105 && i < 180 )
					{
						buffer[(x*16 + y)*3 + 2] = 201 -(i - 105)*2*8.0/6;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
					else
					{
						buffer[(x*16 + y)*3 + 2] = 0;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
				}
			}
		}
		else
		{
			for(int x = 5; x < 11; x++)
			{
				for(int y = 5; y < 11; y++)
				{
					
					buffer[(x*16 + y)*3 + 1] = 255;
					buffer[(x*16 + y)*3 + 2] = 255;
					buffer[(x*16 + y)*3 + 3] = 255;
				}
			}
		}

		// dim inner 16
		if( i >= 256/8 )
		{
			for(int x = 6; x < 10; x++)
			{
				for(int y = 6; y < 10; y++)
				{
					buffer[(x*16 + y)*3 + 1] = 255 - (i - 32)*8.0/7;

					// 36.375 = 32+(5*7/8)
					// 80.125 = 36.375+(50*7/8)
					// 167.625 = 80.125+(100*7/8)
					if( i < 36 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i-32)*8.0/7;
						buffer[(x*16 + y)*3 + 3] = 255 - (i-32)*8.0/7;
					}
					else if( i >= 36 && i < 80 )
					{
						buffer[(x*16 + y)*3 + 2] = 255 - (i-32)*8.0/7;
						buffer[(x*16 + y)*3 + 3] = 251 - (i-36)*5*(8.0/7);
					}
					else if( i >= 80 && i < 167 )
					{
						buffer[(x*16 + y)*3 + 2] = 201 - (i-80)*2*(8.0/7);
						buffer[(x*16 + y)*3 + 3] = 0;
					}
					else
					{
						buffer[(x*16 + y)*3 + 2] = 0;
						buffer[(x*16 + y)*3 + 3] = 0;
					}
				}
			}
		}
		else
		{
			for(int x = 6; x < 10; x++)
			{
				for(int y = 6; y < 10; y++)
				{
					
					buffer[(x*16 + y)*3 + 1] = 255;
					buffer[(x*16 + y)*3 + 2] = 255;
					buffer[(x*16 + y)*3 + 3] = 255;
				}
			}
		}

		// Dim starting at inner 4
		for(int x = 7; x < 9; x++)
		{
			for(int y = 7; y < 9; y++)
			{

				buffer[(x*16 + y)*3 + 1] = 255-i;

				if( i < 5 )
				{
					buffer[(x*16 + y)*3 + 2] = 255 - i;
					buffer[(x*16 + y)*3 + 3] = 255 - i;
				}
				else if( i >= 5 && i < 55 )
				{
					buffer[(x*16 + y)*3 + 2] = 255 - i;
					buffer[(x*16 + y)*3 + 3] = 250 - (i - 5)*5;
				}
				else if( i >= 55 && i < 155 )
				{
					buffer[(x*16 + y)*3 + 2] = 200 - (i - 55)*2;
					buffer[(x*16 + y)*3 + 3] = 0;
				}
				else
				{
					buffer[(x*16 + y)*3 + 2] = 0;
					buffer[(x*16 + y)*3 + 3] = 0;
				}
			}
		}
	
		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep( sleepTimeAfterChange );
	}

	buffer[0] = 0x72;// Set to SOF byte

	//set back to 0
	wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
	usleep((int)1000000); //Sleep (microseconds)
}

void theme4()
{
	int sleepTimeAfterChange = RUN_TIME/(256*2);

	for(int i = 0; i < 511; i++)
	{
		double brightness = -fabs((i/255.0)-1)+1;// absolute value?
		buffer[0] = 0x72; // Set SOF byte

		for(int x = 0; x < 16; x++)
		{
			for(int y = 0; y < 16; y++)
			{
				buffer[(x*16+y)*3+1] = 255*brightness;

				if(x == 0 || x == 15 || y == 0 || y == 15)
				{
					buffer[(x*16+y)*3+2] = (i%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-(i%255))*brightness;
				}
				if((x == 1 && (y > 0 && y < 15)) 
						|| (x == 14 && (y > 0 && y < 15)) 
						|| (y == 1 && (x > 0 && x < 15)) 
						|| (y == 14 && (x > 0 && x < 15)))
				{
					buffer[(x*16+y)*3+2] = ((i+32)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+32)%255))*brightness;
				}
				if((x == 2 && (y > 1 && y < 14)) 
						|| (x == 13 && (y > 1 && y < 14)) 
						|| (y == 2 && (x > 1 && x < 14)) 
						|| (y == 13 && (x > 1 && x < 14)))
				{
					buffer[(x*16+y)*3+2] = ((i+64)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+64)%255))*brightness;
				}
				if((x == 3 && (y > 2 && y < 13))
						|| (x == 12 && (y > 2 && y < 13)) 
						|| (y == 3 && (x > 2 && x < 13)) 
						|| (y == 12 && (x > 2 && x < 13)))
				{
					buffer[(x*16+y)*3+2] = ((i+96)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+96)%255))*brightness;
				}
				if((x == 4 && (y > 3 && y < 12)) 
						|| (x == 11 && (y > 3 && y < 12)) 
						|| (y == 4  && (x > 3 && x < 12))
						|| (y == 11 && (x > 3 && x < 12)))
				{
					buffer[(x*16+y)*3+2] = ((i+128)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+128)%255))*brightness;
				}
				if((x == 5 && (y > 4 && y < 11)) 
						|| (x == 10 && (y > 4 && y < 11)) 
						|| (y == 5 && (x > 4 && x < 11)) 
						|| (y == 10 && (x > 4 && x < 11)))
				{
					buffer[(x*16+y)*3+2] = ((i+160)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+160)%255))*brightness;
				}
				if((x == 6 && (y > 5 && y < 10)) 
						|| (x == 9 && (y > 5 && y < 10)) 
						|| (y == 6 && (x > 5 && x < 10)) 
						|| (y == 9 && (x > 5 && x < 10)))
				{
					buffer[(x*16+y)*3+2] = ((i+192)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+192)%255))*brightness;
				}
				if((x == 7 && (y > 6 && y < 9)) 
						|| (x == 8 && (y > 6 && y < 9)) 
						|| (y == 7 && (x > 6 && x < 9)) 
						|| (y == 8 && (x > 6 && x < 9)))
				{
					buffer[(x*16+y)*3+2] = ((i+224)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+224)%255))*brightness;
				}
			}
		}
		
		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep( sleepTimeAfterChange );
	}

	buffer[0] = 0x72;// Set to SOF byte

	//set back to 0
	wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
	usleep((int)1000000); //Sleep (microseconds)
}

void theme5()
{
	int sleepTimeAfterChange = RUN_TIME/(256*2);

	for(int i = 0; i < 511; i++)
	{
		double brightness = -fabs((i/255.0)-1)+1;// absolute value?
		buffer[0] = 0x72; // Set SOF byte

		for(int x = 0; x < 16; x++)
		{
			for(int y = 0; y < 16; y++)
			{
				buffer[(x*16+y)*3+2] = 0;

				if(x == 0 || x == 15 || y == 0 || y == 15)
				{
					buffer[(x*16+y)*3+1] = (i%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-(i%255))*brightness;
				}
				if((x == 1 && (y > 0 && y < 15)) 
						|| (x == 14 && (y > 0 && y < 15)) 
						|| (y == 1 && (x > 0 && x < 15)) 
						|| (y == 14 && (x > 0 && x < 15)))
				{
					buffer[(x*16+y)*3+1] = ((i+32)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+32)%255))*brightness;
				}
				if((x == 2 && (y > 1 && y < 14)) 
						|| (x == 13 && (y > 1 && y < 14)) 
						|| (y == 2 && (x > 1 && x < 14)) 
						|| (y == 13 && (x > 1 && x < 14)))
				{
					buffer[(x*16+y)*3+1] = ((i+64)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+64)%255))*brightness;
				}
				if((x == 3 && (y > 2 && y < 13))
						|| (x == 12 && (y > 2 && y < 13)) 
						|| (y == 3 && (x > 2 && x < 13)) 
						|| (y == 12 && (x > 2 && x < 13)))
				{
					buffer[(x*16+y)*3+1] = ((i+96)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+96)%255))*brightness;
				}
				if((x == 4 && (y > 3 && y < 12)) 
						|| (x == 11 && (y > 3 && y < 12)) 
						|| (y == 4  && (x > 3 && x < 12))
						|| (y == 11 && (x > 3 && x < 12)))
				{
					buffer[(x*16+y)*3+1] = ((i+128)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+128)%255))*brightness;
				}
				if((x == 5 && (y > 4 && y < 11)) 
						|| (x == 10 && (y > 4 && y < 11)) 
						|| (y == 5 && (x > 4 && x < 11)) 
						|| (y == 10 && (x > 4 && x < 11)))
				{
					buffer[(x*16+y)*3+1] = ((i+160)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+160)%255))*brightness;
				}
				if((x == 6 && (y > 5 && y < 10)) 
						|| (x == 9 && (y > 5 && y < 10)) 
						|| (y == 6 && (x > 5 && x < 10)) 
						|| (y == 9 && (x > 5 && x < 10)))
				{
					buffer[(x*16+y)*3+1] = ((i+192)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+192)%255))*brightness;
				}
				if((x == 7 && (y > 6 && y < 9)) 
						|| (x == 8 && (y > 6 && y < 9)) 
						|| (y == 7 && (x > 6 && x < 9)) 
						|| (y == 8 && (x > 6 && x < 9)))
				{
					buffer[(x*16+y)*3+1] = ((i+224)%255)*brightness;
					buffer[(x*16+y)*3+3] = (255-((i+224)%255))*brightness;
				}
			}
		}
		
		wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
		usleep( sleepTimeAfterChange );
	}

	buffer[0] = 0x72;// Set to SOF byte

	//set back to 0
	wiringPiSPIDataRW( SPI_CHANNEL, buffer, PACKET_SIZE );
	usleep((int)1000000); //Sleep (microseconds)
}
#endif
