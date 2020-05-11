/*
 * Message Board
 * Creation    : 08/05/2020
 * Author      : Tim Drew
 * Filename    : m8x16.ino
 * Hardware    : Arduino Due
 * Description : Control application for 8x16 Monochrome Led Display Matrix
 *             :
 *             :
 *
 * History     :
 */
//  Arduino Due
//  512 K Flash
//  96 K Ram
//  0 K EEProm
//  12 Analogue I/P
//  2  DAC
//  54 Digital I/O [12 are PWM]
//  84 MHz Clk, 3V3
//
// Digital I/O pins
//
//
// Pin  0 - Rx \ serial 0 comms, programming
// Pin  1 - Tx / and debug
// Pin  2 -
// Pin  3 -
// Pin  4 -
// Pin  5 -
// Pin  6 -
// Pin  7 -
//-------------
// Pin  8 -
// Pin  9 -
// Pin 10 -
// Pin 11 -
// Pin 12 -
// Pin 13 - built in Led -> Heartbeat
// GND
// AREF

//
// Analog I/P 10 bit
//

// Analog read takes approx. 100 uS
// A0 - A15
// Pin A0  - Light intensity
// Pin A1  - 5V sensor
// Pin A2  - 3V3 sensor
// Pin A3  - Vin sensor
// Pin A4  -
// Pin A5  -
//
#include <timer.h>
#include "char_gen.h"
#include "m8x16.h"
#include "i2c_bitbash.h"
#include "fifo.h"
#include "msg_buff.h"

auto timer = timer_create_default(); // create a timer with default settings
Timer<> default_timer;				 // save as above


//data display from right to left, from bottom to top, HIGH level display.

unsigned char data_line = 0;
unsigned char delay_count = 0;
unsigned char buff[BUFF_SIZE];
unsigned char cur_frame;
unsigned char *fptr;

unsigned char disp_mode;
unsigned int  display_rate;		// 100 mS steps 

//
// Prpototypes
//
void IIC_start();
void IIC_send(unsigned char send_data);
void IIC_end();

unsigned char table[4][16] = {
 {0x00,0x00,0x00,0x00,0x26,0x41,0x86,0x80,0x80,0x80,0x86,0x41,0x26,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x42,0x84,0x42,0x22,0x1C,0x00,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x00,0x20,0x44,0x42,0x84,0x80,0x84,0x42,0x44,0x20,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x00,0xC0,0x40,0xF8,0xD8,0x7E,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00}};

//
// ISR Related functions
//
void shift_left()
{
	register unsigned char *ptr;
	register byte i;
	
	for(i = 0, ptr = &buff[0]; i < BUFF_SIZE-1; i++, ptr++)
		*ptr = *(ptr + 1);
	*ptr = 0x00;			// Back fill with blank chars	
}

void next_frame()
{
	if(++cur_frame == MAX_FRAMES)
	{
		cur_frame = 0;	
		fptr = &buff[0];
	}
	else
		fptr = &buff[cur_frame * FRAME_SIZE];
}

//
// Initialisation functions
//

bool show_status(void *msg)
{
	const char *m = (const char *)msg;
	Serial.println(m);
	fifo_status();
	msg_status();
	return true;
}

void init_frame_buff()
{
	cur_frame = 0;
	fptr = &buff[0];
	memset(&buff[0], 0x00, sizeof(buff));	
}


void setup()
{
	//unsigned char test_buff[] = "Hello World 12345\n";
	
	delay(5000);
	Serial.begin(115200);
	Serial.println();
	Serial.println(TITLE);
	Serial.println(SUBTITLE);
	Serial.println(SEPERATOR);

	// Setup IO Ports
	pinMode(IIC_SCL,OUTPUT);
	pinMode(IIC_SDA,OUTPUT);
	digitalWrite(IIC_SCL,LOW);
	digitalWrite(IIC_SDA,LOW);

	// Setup data structures
	init_frame_buff();
	fifo_init();
	msg_init();
	fifo_status();
	msg_status();
	//strcpy(&msg_buff[0], "Hello world 12345\0");
	//fifo_add();	// Add a test message
	//fifo_status();
	
	// Show status every second
	timer.every(2000, show_status, (void *)"\nShow Status");
	Serial.println("Startup complete ...\n");
	//msg_add(&test_buff[0]);
	//msg_status();
	//msg_add('!');
	//msg_get(&test_buff[0]);
	//Serial.println("\nGot char:");Serial.println(test_buff[0],HEX);
	//msg_get(&test_buff[0]);
	//Serial.println("\nGot char:");Serial.println(test_buff[0],HEX);
	//Serial.println("Test complete ...\n");
}
/*----------------------------------------------------------------*/

void loop()
{
	timer.tick(); // tick the timer

    /**************set the address plus 1***************/
    IIC_start();
    IIC_send(0x40);// set the address plus 1 automatically
    IIC_end();
    /************end the process of address plus 1 *****************/
    
    /************set the data display*****************/
    IIC_start();
    IIC_send(0xc0);// set the initial address as 0
    for(char i = 0;i < 16;i++)
    {
       IIC_send(table[data_line][i]);// send the display data
    }
    
    if(++delay_count >= 10)
    {
      delay_count = 0;
      data_line++;
      if(data_line >= 4)
      {
        data_line = 0;
      }
    }

    IIC_end();
    /************end the data display*****************/
    /*************set the brightness display***************/
    IIC_start();
    IIC_send(0x8A);// set the brightness display
    IIC_end();
    /*************end the brightness display***************/
    delay(100);
}

/*----------------------------------------------------------------*/
