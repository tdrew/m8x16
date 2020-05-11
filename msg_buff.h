/*
	Name     : msg_buff.h
	function : Basic character buffer, non-destructive.
	Author   : Tim Drew
	Date     : 11th May 2020
	Details  : Designed for Arduino
*/

/*
      Buffer
    +-+-+-+-+-+-+-+-+
	|0|1|2|3|4|5|6|7|
    +-+-+-+-+-+-+-+-+
	
	+-+
	|0|  msg_pos
	+-+
	
	+-+
	|0|  msg_len
	+-+

*/
#define FIFO_SIZE 32

unsigned char msg_buff[MSG_BUFF_SIZE];

unsigned char msg_pos;
unsigned char msg_len;

void msg_init()
{
	memset(&msg_buff[0], 0x0, sizeof(msg_buff));
	msg_pos = 0;
	msg_len = 0;
}

unsigned char msg_next( unsigned char index)
{
	unsigned char x = (index + 1) % MSG_BUFF_SIZE;
	return(x);
}

boolean msg_full()
{
	if(msg_len == MSG_BUFF_SIZE)
		return(true);
	return(false);
}

boolean msg_empty()
{
	if(!msg_len)
		return(true);
	return(false);
}

// Add a null terminated string
boolean msg_add(unsigned char *ele)
{
	while(*ele && !msg_full())
	{
		msg_buff[msg_len] = *ele++;
		if(msg_len < MSG_BUFF_SIZE)
			msg_len++;
	}
	if(!*ele)
		return(true);
	return(false);
}

// Add a single character
boolean msg_add(unsigned char ele)
{
	if(msg_full())
		return(false);
		
	msg_buff[msg_len] = ele;
	if(msg_len < MSG_BUFF_SIZE)
		msg_len++;
	return(true);
}

// get one char from the buffer, loop around message
boolean msg_get(unsigned char *ele)
{
	if(msg_empty())
		return(false);
		
	*ele = msg_buff[msg_pos];
	msg_pos = msg_next(msg_pos);
	return(true);
}

void msg_status()
{
	unsigned char x;

	Serial.print("----> MSG_BUFF Size:"); Serial.println(sizeof(msg_buff), DEC);
	
	Serial.print("len:"); Serial.print(msg_len);
	Serial.print(", pos"); Serial.println(msg_pos);
	Serial.print("[ ");
	for(x = 0; x < MSG_BUFF_SIZE; x++)
	{
		Serial.print(msg_buff[x],HEX); Serial.print(" ");
	}
	Serial.println("]");
}