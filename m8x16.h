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
#define TITLE     "  Message Board 8 x 16 LED Matrix driver (Monochrome)"
#define SUBTITLE  "   --> By Tim Drew 11th May 2020"
#define SEPERATOR "======================================================="

// Use these pins to tlk to the 8 x 16 display
#define IIC_SCL  A5
#define IIC_SDA  A4

// Buffer constants
#define MAX_FRAMES    4
#define FRAME_SIZE    16
#define BUFF_SIZE     (MAX_FRAMES * FRAME_SIZE)
#define MSG_BUFF_SIZE 32

#define MODE_FRAME    0x01
#define MODE_SCROLL   0x02
#define MODE_STATIC   0x03
