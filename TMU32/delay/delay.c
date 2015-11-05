#include "delay.h"
#include <intrins.h>

void delay_ms(unsigned int delayMs)
{
    unsigned int delayLoop;
    unsigned int ll;
    for(ll=0; ll<delayMs; ll++)
    {
       // delayLoop = 0x500;
			delayLoop = 0x6B8;
        while( delayLoop )
        {
            delayLoop --;
            _nop_();
        }
    }
}