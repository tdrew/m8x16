/*
	Name     : fifo.h
	function : Basic Fifo functionality for storing multiples of same size
	Author   : Tim Drew
	Date     : 26th April 2020
	Details  : Designed for Arduino
*/

/*
      Buffer
    +-+-+-+-+-+-+-+-+
	|0|1|2|3|4|5|6|7|
    +-+-+-+-+-+-+-+-+
	
	+-+
	|0|  Entry Index
	+-+
	
	+-+
	|0|  Exit Index
	+-+

	+-+
	|0|  Count of used cells
	+-+

*/
#define FIFO_SIZE 32

unsigned char fifo[FIFO_SIZE];
unsigned char fifo_entry;
unsigned char fifo_exit;
unsigned char fifo_cnt;

void fifo_init()
{
	memset(fifo, 0x0, sizeof(fifo));
	fifo_entry = 0;
	fifo_exit = 0;
	fifo_cnt = 0;
}

unsigned char fifo_next( unsigned char index)
{
	unsigned char x = (index + 1) % FIFO_SIZE;
	return(x);
}

boolean fifo_full()
{
	if(fifo_cnt == FIFO_SIZE)
		return(true);
	return(false);
}

boolean fifo_empty()
{
	if(!fifo_cnt)
		return(true);
	return(false);
}

// Add a null terminated string
boolean fifo_add(unsigned char *ele)
{
	while(*ele && !fifo_full())
	{
		fifo[fifo_entry] = *ele++;
		fifo_entry = fifo_next(fifo_entry);
		if(fifo_cnt < FIFO_SIZE)
			fifo_cnt++;
	}
	if(!*ele)
		return(true);
	return(false);
}


// Add a single character
boolean fifo_add(unsigned char ele)
{
	if(fifo_full())
		return(false);
		
	fifo[fifo_entry] = ele;
	fifo_entry = fifo_next(fifo_entry);
	if(fifo_cnt < FIFO_SIZE)
		fifo_cnt++;
	return(true);
}

boolean fifo_get(unsigned char *ele)
{
	if(fifo_empty())
		return(false);
		
	*ele = fifo[fifo_exit];
	fifo_exit = fifo_next(fifo_exit);
	if(fifo_cnt > 0)
		fifo_cnt--;
	return(true);
}

void fifo_status()
{
	unsigned char x;
	unsigned char y;
	Serial.print("----> FIFO Size:"); Serial.println(sizeof(fifo), DEC);
	
	Serial.print("Entry:"); Serial.print(fifo_entry);
	Serial.print(", Exit:"); Serial.print(fifo_exit);
	Serial.print(", Size:"); Serial.println(fifo_cnt);
	Serial.print("[ ");
	for(x = 0; x < FIFO_SIZE; x++)
	{
		Serial.print(fifo[x],HEX); Serial.print(" ");
	}
	Serial.println("]");
}