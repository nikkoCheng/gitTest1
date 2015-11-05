#include "c8051f340.h"
#include "typedef.h"
#include "ledStatus.h"
#include "idProcess.h"
#include "typeDef.h"
#include "netcommand.h"
#include "share.h"

unsigned int countnum;
unsigned char bdata bLedVar1,bLedVar2;
sbit  led1 = bLedVar1^0;
sbit  led2 = bLedVar1^1;
sbit  led3 = bLedVar1^2;
sbit  led4 = bLedVar1^3;
sbit  led5 = bLedVar1^4;
sbit  led6 = bLedVar1^5;
sbit  led7 = bLedVar1^6;
sbit  led8 = bLedVar1^7;
sbit  led9 = bLedVar2^0;
sbit  led10 = bLedVar2^1;
sbit  led11 = bLedVar2^2;
sbit  led12 = bLedVar2^3;



/* 将led状态赋值给每个端口 */
static void led_hardware_control(void)
{	
	PORT1_STATUS  = led1;
	PORT2_STATUS  = led2;
	PORT3_STATUS  = led3;
	PORT4_STATUS  = led4;
	PORT5_STATUS  = led5;
	PORT6_STATUS  = led6;
	if(led7)
		P4 |= 0x01;
	else
		P4 &= ~0x01;
	if(led8)
		P4 |= 0x02;
	else
		P4 &= ~0x02;
	if(led9)
		P4 |= 0x04;
	else
		P4 &= ~0x04;
	if(led10)
		P4 |= 0x08;
	else
		P4 &= ~0x08;
	if(led11)
		P4 |= 0x10;
	else
		P4 &= ~0x10;
	if(led12)
		P4 |= 0x20;
	else
		P4 &= ~0x20;
}

/*每隔50ms调用一次*/
bit flashQuick;
bit flashSlow; 

void refresh_led(void)
{
//    static unsigned char count=0;
	unsigned char flag = 0;
    unsigned char i;
    unsigned int tmpAll,tmpi;

    countnum ++;

	//mainInfo.ledState[8] = LED_STATE_SLOW;
	//mainInfo.ledState[9] = LED_STATE_QUICK;
	//mainInfo.ledState[10] = LED_STATE_ON;
	//mainInfo.ledState[11] = LED_STATE_OFF;
	
  /*  if( (countnum%25) == 0)
    {
 	    flashQuick = ~flashQuick;
    }
		if( (countnum%50) == 0 )
    {
        flashSlow = ~flashSlow;
			countnum = 0;
    }*/

    if( (countnum%8) == 0)
    {
 	    flashQuick = ~flashQuick;
    }
		if( (countnum%16) == 0 )
    {
        flashSlow = ~flashSlow;
			countnum = 0;
    }

    tmpAll = 0;
    tmpi = 1;
    for(i=0; i<PORT_ALL_NUM; i++)
    {
 
            switch(mainInfo.ledState[i])
            {

                case LED_STATE_OFF:
                    break;

                case LED_STATE_SLOW:
					flag |= 0x01;
                    if( flashSlow )
                        tmpAll |= tmpi;
                    break;

                case LED_STATE_QUICK:
					flag |= 0x10;
                    if( flashQuick )
                        tmpAll |= tmpi;
                    break;

                case LED_STATE_ON:
						flag |= 0x01;
                default:
                    tmpAll |= tmpi;
                    break;
            }
						
			if(flag & 0x10)
			{
					if( flashQuick )
						CPU_RUN_LED_0;
					else
						CPU_RUN_LED_1;		
			}
			else if(flag & 0x01)
			{
					 if( flashSlow )
							CPU_RUN_LED_0;
						else
							CPU_RUN_LED_1;
			}
			else
			{
						CPU_RUN_LED_1;
			}

        tmpi <<= 1;
    }
    tmpAll = ~tmpAll;           /*LED灯*/

    bLedVar1 = tmpAll % 256;
    bLedVar2 = tmpAll / 256;

    led_hardware_control();
}
