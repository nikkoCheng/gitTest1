#ifndef __DS2431PROCESS_H__
#define __DS2431PROCESS_H__

#include "c8051F340.h"

#if 1 
/*左出纤和翻转板管脚定义*/
sbit  ONE_WIRE_WP1  = P1^0;
sbit  ONE_WIRE_WP2  = P1^1;
sbit  ONE_WIRE_WP3  = P1^2;
sbit  ONE_WIRE_WP4  = P1^3;
sbit  ONE_WIRE_WP5  = P1^4;
sbit  ONE_WIRE_WP6  = P1^5;
sbit  ONE_WIRE_WP7  = P3^0;
sbit  ONE_WIRE_WP8  = P3^1;
sbit  ONE_WIRE_WP9  = P3^2;
sbit  ONE_WIRE_WP10 = P3^3;
sbit  ONE_WIRE_WP11 = P3^4;
sbit  ONE_WIRE_WP12 = P3^5;
#else
/*右出纤管脚定义*/
sbit  ONE_WIRE_WP1  = P1^0;
sbit  ONE_WIRE_WP2  = P1^1;
sbit  ONE_WIRE_WP3  = P1^2;
sbit  ONE_WIRE_WP4  = P1^3;
sbit  ONE_WIRE_WP5  = P1^4;
sbit  ONE_WIRE_WP6  = P1^5;
sbit  ONE_WIRE_WP7  = P1^6;
sbit  ONE_WIRE_WP8  = P1^7;
sbit  ONE_WIRE_WP9  = P2^0;
sbit  ONE_WIRE_WP10 = P2^1;
sbit  ONE_WIRE_WP11 = P2^2;
sbit  ONE_WIRE_WP12 = P2^3;
#endif

extern void one_wire_set_speed(unsigned char standard);
extern unsigned char one_wire_start_get_ack(unsigned char cPortNum);
extern void one_wire_write_byte(unsigned char dat, unsigned char cPortNum, unsigned char speed);
extern unsigned char one_wire_read_byte(unsigned char cPortNum, unsigned char speed);
extern void tick_delay(unsigned int unum);
extern unsigned int one_wire_start_get12_ack(void);
extern void one_wire_write12_byte(unsigned char dat,unsigned char speed);
extern unsigned char* one_wire_read12_byte(unsigned char speed);

#endif

