/*
 * Display4.c
 * 
 * Sample code to control a 4 digit 7-segment LED display.
 * 
 * Copyright 2017  <redeyerob@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <wiringPi.h>

#define NUM_DISPLAY_DIGITS 4
#define ALLON 0x00
#define ALLOFF 0xFF
#define DIGIT_DELAY_MS 5000			// 1/200 sec
#define TEST_STEP_DELAY_MS 500000	// 1/2   sec

#define DIGIT_DELAY() delayMicroseconds(DIGIT_DELAY_MS) 
#define TEST_STEP_DELAY() delayMicroseconds(TEST_STEP_DELAY_MS)

#define HELP_TEXT \
	" [-t] [-c[<Value>]] [-d] [-h] [-s <secs>] [-v[<value>]]\n"\
	"	-t 	Run Display Test, loops thru all numbers on each digit\n"\
	"	-c  Runs countdown from <Value> to zero, 9999 default \n" \
	"	-d  Debug mode \n" \
	"	-h  This help\n" \
	"	-s  Time in secs to display the value, 10 sec default\n" \
	"	-v  Value to display, default 1234 \n" 
	
/* 
 * Lookup Table to determine Max Value +1 indexed by num display digits
 */
const int MaxValue[10] = {
					0,
					10,
					100,
					1000,
					10000,
					100000,
					1000000,
					10000000,
					100000000,
					1000000000};
							

/*
Display	
  Pin		GPIO		Segment
=======		=======		=========
	1		GPIO 7		Left-Bottom	
	2		GPIO 0		Bottom
	3		GPIO 2		Decimal
	4		GPIO 3		Right-Bottom
	5		GPIO 1		Middle
	6		GPIO 26		#Fourth
	7		GPIO 4		Right-Top
	8		GPIO 27		#Third
	9		GPIO 28		#Second
	10		GPIO 5		Left-Top
	11		GPIO 6		Top
	12		GPIO 29		#First
*/

/*
 * Complete list of all GPIOS used for output to the display.
 */
const unsigned char GPIO_Array[12] = {7,0,2,3,1,26,4,27,28,5,6,29};

const char DisplayDigit[4] = { 29, 28, 27, 26 };

#define	G_BOTTOM	0b11111110 
#define	G_MIDDLE	0b11111101
#define G_DECIMAL	0b11111011
#define G_R_BOTTOM	0b11110111
#define G_R_TOP		0b11101111
#define G_L_TOP		0b11011111
#define G_TOP		0b10111111
#define G_L_BOTTOM	0b01111111
 
/*
 * Lookup table of bytes indexed by numerical character.
 * Each byte represnts the bit value for the forst 8 GPIOs
 */
const char Number[10] = {	
	(char)~(G_MIDDLE & G_DECIMAL),								// 0
	(char)(G_L_TOP & G_L_BOTTOM),								// 1
	(char)~(G_L_TOP & G_R_BOTTOM & G_DECIMAL),	 				// 2
	(char)~(G_L_TOP & G_L_BOTTOM & G_DECIMAL), 					// 3
	(char)(G_L_TOP & G_R_TOP & G_MIDDLE & G_R_BOTTOM), 			// 4
	(char)~(G_R_TOP & G_L_BOTTOM & G_DECIMAL),				 	// 5
	(char)~(G_TOP & G_R_TOP & G_DECIMAL),					 	// 6
	(char)(G_TOP & G_R_TOP & G_R_BOTTOM),					 	// 7
	(char)~(G_DECIMAL),										 	// 8
	(char)~(G_L_BOTTOM & G_DECIMAL)							 	// 9
	};
	
const char DecimalPoint = {G_DECIMAL};

const char MinusSign = {G_MIDDLE};
	
static int DisplayValue = TRUE;
static int runTest = FALSE;
static int runCountDown = FALSE;
static int debugMode = FALSE;
static int displayTime = 1000000*10;	// 10 Seconds
static int countDownValue = 9999;	

/*
 * Initialize the GPIOs used to output.
 * 
 */
void InitGPIO()
{
	int i;
	
	for( i=0; i < sizeof(GPIO_Array); i++)
	{
		pinMode(GPIO_Array[i], OUTPUT);	
	}
	
}

/*
 * Select which digit to set.
*/
void DisplayOn(int Position)
{
	int i;
	
	for( i=0; i < 4; i++)
	{
		digitalWrite(DisplayDigit[i], (Position) == i);
	}	 
		
}

/*
 * Display a integer value, multiplexig across all digits.
*/
void DisplayInt(int Value, int DisplayTimeMS )
{
	int i, j;
	int V[NUM_DISPLAY_DIGITS];
	int Count;
	
	// Convert DisplayTimeMS to number of loops
	Count = DisplayTimeMS / (NUM_DISPLAY_DIGITS * DIGIT_DELAY_MS );
	
	// Ignore part of value greater than the number of digits that
	// can be displayed
	Value = Value % MaxValue[NUM_DISPLAY_DIGITS];
	
	for( j=3; j > -1; j--)
	{
		V[j] = Value % 10;
		if( debugMode) 
		{
			printf("Value = %d V[ %d ] = %d\n", Value, j, V[j]);
		}
		Value = Value/10;
	}
	
	for( i=0; i<Count; i++ )
	{
		for( j=0; j < NUM_DISPLAY_DIGITS; j++)
		{
			DisplayOn(j);
			digitalWriteByte(Number[V[j]]);
			DIGIT_DELAY();
		}
	}
	digitalWriteByte(ALLOFF);	
}

/*
 * Display a minus sign across all digits.
*/
void DisplayAllMinus(int DisplayTimeMS )
{
	int i, j;
	int Count;
	
	// Convert DisplayTimeMS to number of loops
	Count = DisplayTimeMS / (NUM_DISPLAY_DIGITS * DIGIT_DELAY_MS );
	
	for( i=0; i<Count; i++ )
	{
		for( j=0; j < NUM_DISPLAY_DIGITS; j++)
		{
			DisplayOn(j);
			digitalWriteByte(MinusSign);
			DIGIT_DELAY();
		}
	}
	digitalWriteByte(ALLOFF);	
}

/*
 *  Test routine for the display.
*/
void Test()
{
	int i, j;
	int temp;
	
	printf("Starting Display Test\n");
	
	digitalWriteByte(ALLON);
	TEST_STEP_DELAY();
	
	for( j=0; j < NUM_DISPLAY_DIGITS; j++)
	{
		DisplayOn(j);
		for( i=0; i < 10; i++)
		{
			digitalWriteByte(Number[i]);
			TEST_STEP_DELAY();
		}
		digitalWriteByte(DecimalPoint);
		TEST_STEP_DELAY();
	}
	
	digitalWriteByte(ALLOFF);
	TEST_STEP_DELAY();	
}

/*
 * Countdown from specified value
*/
void CountDown()
{
	int T = countDownValue;
	
	for(T; T > 0; T--)
	{
		DisplayInt(T, 50000);
	}
	DisplayAllMinus(1000000);
}


/* 
 * Parse the command line options.
*/
int ParseCmdLine(int argc, char **argv)
{
	int Value = 1234;
	int opt;
	
	while ((opt = getopt(argc, argv, "hc::tds:v::")) != -1)
	{
		switch (opt) 
		{
			case 't':
				DisplayValue = FALSE;
				runTest = TRUE;
				break;
			
			case 'c':
				DisplayValue = FALSE;
				runCountDown = TRUE;
				if( optarg != NULL)
				{
					countDownValue = atoi(optarg);
					if( countDownValue == 0 )
					{
						printf( "Invalid countdown value: %s\n", optarg);
						exit(EXIT_FAILURE);
					}
				}
				break;
				
			case 'd':
				debugMode = TRUE;
				break;
				
			case 'v':
				DisplayValue = TRUE;
				if( optarg != NULL)
				{
					Value = atoi(optarg);
				}
				break;
				
			case 's':
			DisplayValue = TRUE;
				displayTime = atoi(optarg)*1000000;
				if( displayTime == 0 )
				{
					printf( "Invalid display time: %s\n", optarg);
					exit(EXIT_FAILURE);
				}
				break;
				
			case '?':
			case 'h':
			default:
				printf("Usage: %s %s", argv[0],	HELP_TEXT);
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	return(Value);
}

/* 
 * Demo use of the 7 segment display.
*/
int main(int argc, char **argv)
{
	int rc = 0;
	int Value = 0;
	
	Value = ParseCmdLine(argc, argv);
	
	
	rc = wiringPiSetup();
	if( rc != 0)
	{
		printf("Error calling wiringPiSetup() rc= %d\n", rc);
		exit(EXIT_FAILURE);
	}
	
	if( runTest )
	{
		Test();
	}
	
	if( runCountDown ) 
	{
		CountDown();
	}
	
	if( DisplayValue )
	{
		DisplayInt(Value, displayTime);
	}
	
	exit(EXIT_SUCCESS);
}
