#include "timer.h"
#include "c8051F340.h"
#include "ledStatus.h"
#include "variable.h"

bit flag10ms;
bit flag20ms;
bit flag30ms;
bit flag60ms;

void Timer_Init(void)
{
	TIMER_CKCON_CFR;
	TIMER_TMOD_CFR;

	TH0 = 0xF0;//TIME_RELOAD>>8;
	TL0 = 0x5F;//TIME_RELOAD;
	TR0 = 1;        
	ET0 = 1;             
}

void ISRtime0(void) interrupt 1 using 1
{
    static unsigned char data time50Count = 0;

   // TH0 = TIME_RELOAD>>8;
   // TL0 = TIME_RELOAD;
		TH0 = 0xF0;
		TL0 = 0x5F;
    time50Count ++;

    if((0 == (time50Count % 5)))
    {
        flag10ms = 1;
		//	if(linkFmu == 1)
		//	{refresh_led();}
    }
		if((0 == (time50Count % 10)))
		{
				flag20ms = 1;
		}
		if((0 == (time50Count % 15)))
    {
        flag30ms = 1;
				if(linkFmu == 1)
				{refresh_led();}
    }  
    if(((time50Count%30)== 0))
    {
				time50Count = 0;
        flag60ms = 1;
    }	   	
}


