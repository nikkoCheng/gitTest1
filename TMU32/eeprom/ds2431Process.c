#include "typeDef.h"
#include "oneWireProcess.h"
#include "ds2431Process.h"
#include "share.h"
#include "mcuInit.h"


#ifdef ID_ONE_WIRE

unsigned int OW_A,OW_B,OW_C,OW_D,OW_E,OW_F,OW_G,OW_H,OW_I,OW_J;

static void tick_delay(unsigned int unum)
{
    while(unum--)
    {
        _nop_();
    }
}

static void tick_delay_one(void)
{
	NOP2();
	NOP();
}
static void clear_0(void)
{
	P1 &= ~0x3F;
//	P2 &= 0xff;
	P3 &= ~0x3F;
	//P2 &= 0xff;
}
static void set_1(void)
{
	P1 |= 0x3F;
//	P2 &= 0xff;
	P3 |= 0x3F;
//	P2 &= 0xff;
}

void one_wire_set_speed(unsigned char standard)
{
#if 1  /*CPU = 340*/
    if (ONE_WIRE_STANDARD_SPEED == standard)
    {
        // Standard Speed  标准速度
	#if 1	//24M晶振
        OW_A = 8;    //6		
        OW_B = 107;   //64
        OW_C = 100;   //60
        OW_D = 13;    //10
        OW_E = 12;    //9
        OW_F = 59;    //55
        OW_G = 0;     //0
        OW_H = 900;   //510
        OW_I = 120;   //70
//        OW_J = 400;   //410
		OW_J = 640;   //410

	#else	//12M晶振
	    OW_A = 2;    //6
        OW_B = 55;   //64
        OW_C = 65;   //90
        OW_D = 10;    //10
        OW_E = 6;    //9
        OW_F = 40;    //55
        OW_G = 0;     //0
        OW_H = 400;   //510
        OW_I = 80;   //70
        OW_J = 310;   //410
	#endif
    }
    else
    {
        // Overdrive Speed  高速模式
	#if 1	//24M晶振
        OW_A = 1;    // 1			
        OW_B = 9;   //7.5
        OW_C = 10;   //7.5
        OW_D = 3;    //2.5
        OW_E = 0;    // 1
        OW_F = 8;    //7
        OW_G = 3;     //2.5
        OW_H = 110;   //70
        OW_I = 8;   //8.5
        OW_J = 62;   //40
	#else 		//12M晶振
        OW_A = 0;    // 1
        OW_B = 4;   //7.5
        OW_C = 4;   //7.5
        OW_D = 0;    //2.5
        OW_E = 0;    // 1
        OW_F = 2;    //7
        OW_G = 1;     //2.5
        OW_H = 56;   //70
        OW_I = 5;   //8.5
        OW_J = 31;   //40
	#endif
    }
#else /*CPU =  020*/
if (ONE_WIRE_STANDARD_SPEED == standard)
    {
        // Standard Speed  标准速度
        OW_A = 6;    //6
        OW_B = 100;   //64
        OW_C = 90;   //60
        OW_D = 10;    //10
        OW_E = 9;    //9
        OW_F = 55;    //55
        OW_G = 0;     //0
        OW_H = 800;   //510
        OW_I = 70;   //70
        OW_J = 400;   //410
    }
    else
    {
        // Overdrive Speed  高速模式
        OW_A = 0;    // 1
        OW_B = 8;   //7.5
        OW_C = 8;   //7.5
        OW_D = 0;    //2.5
        OW_E = 0;    // 1
        OW_F = 8;    //7
        OW_G = 0;     //2.5
        OW_H = 110;   //70
        OW_I = 10;   //8.5
        OW_J = 35;   //40
    }
#endif
}
/**************************************************************************************
获取12个端口的应答
***************************************************************************************/
unsigned int one_wire_start_get12_ack(void)
{
    unsigned int result = 0;
	set_1();
	tick_delay(OW_G);
	clear_0();
	tick_delay(OW_H);
	set_1();
 	tick_delay(OW_I);	
	////////////////////////
	//////////////////////////////
/*	result |= ONE_WIRE_WP12;
	result <<= 1;
	result |= ONE_WIRE_WP11;
	result <<= 1;
	result |= ONE_WIRE_WP10;
	result <<= 1;
	result |= ONE_WIRE_WP9;
	result <<= 1;
	result |= ONE_WIRE_WP8;
	result <<= 1;
	result |= ONE_WIRE_WP7;
	result <<= 1;
	result |= ONE_WIRE_WP6;
	result <<= 1;
	result |= ONE_WIRE_WP5;
	result <<= 1;
	result |= ONE_WIRE_WP4;
	result <<= 1;
	result |= ONE_WIRE_WP3;
	result <<= 1;
	result |= ONE_WIRE_WP2;
	result <<= 1;
	result |= ONE_WIRE_WP1;
	//set_1();*/
	result = ( P1 & (~0xC0) ) + ( (P3&(~0xC0)) * 64 );
	tick_delay(OW_J);
	WatchDog();
    return result;
}


/*获取DS2431应答信号，若返回0则获取DS2431应答信号成功*/
unsigned char one_wire_start_get_ack(unsigned char cPortNum)
{
    unsigned char result = 1;
	
    switch(cPortNum)
    {
        case 0:
        {
            ONE_WIRE_WP1 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP1 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP1 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP1;
            tick_delay(OW_J);
            break;
        }
        case 1:
        {
            ONE_WIRE_WP2 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP2 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP2 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP2;
            tick_delay(OW_J);
            break;
        }
        case 2:
        {
            ONE_WIRE_WP3 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP3 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP3 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP3;
            tick_delay(OW_J);
            break;
		}
        case 3:
        {
            ONE_WIRE_WP4 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP4 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP4 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP4;
            tick_delay(OW_J);
            break;
        }
        case 4:
        {
            ONE_WIRE_WP5 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP5 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP5 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP5;
            tick_delay(OW_J);
            break;
        }
        case 5:
        {
            ONE_WIRE_WP6 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP6 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP6 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP6;
            tick_delay(OW_J);
            break;
        }
        case 6:
        {
            ONE_WIRE_WP7 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP7 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP7 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP7;
            tick_delay(OW_J);
            break;
		}
        case 7:
        {
            ONE_WIRE_WP8 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP8 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP8 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP8;
            tick_delay(OW_J);
            break;
        }
        case 8:
        {
            ONE_WIRE_WP9 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP9 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP9 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP9;
            tick_delay(OW_J);
            break;
        }
        case 9:
        {
            ONE_WIRE_WP10 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP10 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP10 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP10;
            tick_delay(OW_J);
            break;
        }
        case 10:
        {
            ONE_WIRE_WP11 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP11 = 0;
            tick_delay(OW_H);
	
            ONE_WIRE_WP11 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP11;
            tick_delay(OW_J);
            break;
        }
        case 11:
        {
            ONE_WIRE_WP12 = 1;
            tick_delay(OW_G);
	
            ONE_WIRE_WP12 = 0;
            tick_delay(OW_H);
			
            ONE_WIRE_WP12 = 1;
            tick_delay(OW_I);
	
            result= ONE_WIRE_WP12;
            tick_delay(OW_J);
            break;
        }
        default:break;
    }
	WatchDog();
    return result;
}


/****************************************************************************
读12端口1位数据
*****************************************************************************/
unsigned int one_wire_read12_bit(unsigned char speed)
{
    unsigned result = 0;
	EA = 0;

	if(ONE_WIRE_OVERDRIVE_SPEED == speed)
	{
		clear_0();
		tick_delay_one();
		set_1();
		tick_delay_one();
		result |= ONE_WIRE_WP12;
		result <<= 1;
		result |= ONE_WIRE_WP11;
		result <<=1;
		result |= ONE_WIRE_WP10;
		result <<=1;
		result |= ONE_WIRE_WP9;
		result <<=1;
		result |= ONE_WIRE_WP8;
		result <<=1;
		result |= ONE_WIRE_WP7;
		result <<=1;
		result |= ONE_WIRE_WP6;
		result <<=1;
		result |= ONE_WIRE_WP5;
		result <<=1;
		result |= ONE_WIRE_WP4;
		result <<=1;
		result |= ONE_WIRE_WP3;
		result <<=1;
		result |= ONE_WIRE_WP2;
		result <<=1;
		result |= ONE_WIRE_WP1;
 		EA = 1;
		tick_delay(OW_F);
		set_1();
	}
	else
	{
		clear_0();
		tick_delay(OW_A);
//		nop();
		set_1();
		tick_delay(OW_E);
		result |= ONE_WIRE_WP12;
		result <<= 1;
		result |= ONE_WIRE_WP11;
		result <<=1;
		result |= ONE_WIRE_WP10;
		result <<=1;
		result |= ONE_WIRE_WP9;
		result <<=1;
		result |= ONE_WIRE_WP8;
		result <<=1;
		result |= ONE_WIRE_WP7;
		result <<=1;
		result |= ONE_WIRE_WP6;
		result <<=1;
		result |= ONE_WIRE_WP5;
		result <<=1;
		result |= ONE_WIRE_WP4;
		result <<=1;
		result |= ONE_WIRE_WP3;
		result <<=1;
		result |= ONE_WIRE_WP2;
		result <<=1;
		result |= ONE_WIRE_WP1;
		EA = 1;
		set_1();
	}
    tick_delay(OW_D); 
    return result;
}


unsigned char one_wire_read_bit(unsigned char cPortNum, unsigned char speed)
{
    unsigned result;
	EA = 0;

	if(ONE_WIRE_OVERDRIVE_SPEED == speed)
	{
	    switch(cPortNum)
	    {
	        case 0:
	        {
	            ONE_WIRE_WP1 = 0;
	            tick_delay_one();
		
	            /*Releases the bus*/
	            ONE_WIRE_WP1 = 1;
	            tick_delay_one();

	            /*Sample the bit value from the slave*/
	            result = ONE_WIRE_WP1;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP1 = 1;
	            break;
	        }
	        case 1:
	        {
	            ONE_WIRE_WP2 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP2 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP2;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP2 = 1;
	            break;
	        }
	        case 2:
	        {
	            ONE_WIRE_WP3 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP3 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP3;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP3 = 1;
	            break;
	        }
	        case 3:
	        {
	            ONE_WIRE_WP4 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP4 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP4;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP4 = 1;
	            break;
	        }
	        case 4:
	        {
	            ONE_WIRE_WP5 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP5 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP5;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP5 = 1;
	            break;
	        }
	        case 5:
	        {
	            ONE_WIRE_WP6 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP6 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP6;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP6 = 1;
	            break;
	        }
	        case 6:
	        {
	            ONE_WIRE_WP7 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP7 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP7;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP7 = 1;
	            break;
	        }
	        case 7:
	        {
	            ONE_WIRE_WP8 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP8 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP8;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP8 = 1;
	            break;
	        }
	        case 8:
	        {
	            ONE_WIRE_WP9 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP9 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP9;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP9 = 1;
	            break;
	        }
	        case 9:
	        {
	            ONE_WIRE_WP10 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP10 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP10;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP10 = 1;
	            break;
	        }
	        case 10:
	        {
	            ONE_WIRE_WP11 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP11 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP11;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP11 = 1;
	            break;
	        }
	        case 11:
	        {
	            ONE_WIRE_WP12 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP12 = 1;
	            tick_delay_one();

	            result = ONE_WIRE_WP12;
	            EA = 1;
	            tick_delay(OW_F);
		
	            ONE_WIRE_WP12 = 1;
	            break;
	        }
	        default:
	            EA = 1;
	            break;
	    }
	}
	else
	{
		switch(cPortNum)
		{
			case 0:
			{
				ONE_WIRE_WP1 = 0;
				tick_delay(OW_A);
		
				/*Releases the bus*/
				ONE_WIRE_WP1 = 1;
				tick_delay(OW_E);

				/*Sample the bit value from the slave*/
				result = ONE_WIRE_WP1;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP1 = 1;
				break;
			}
			case 1:
			{
				ONE_WIRE_WP2 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP2 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP2;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP2 = 1;
				break;
			}
			case 2:
			{
				ONE_WIRE_WP3 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP3 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP3;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP3 = 1;
				break;
			}
			case 3:
			{
				ONE_WIRE_WP4 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP4 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP4;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP4 = 1;
				break;
			}
			case 4:
			{
				ONE_WIRE_WP5 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP5 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP5;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP5 = 1;
				break;
			}
			case 5:
			{
				ONE_WIRE_WP6 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP6 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP6;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP6 = 1;
				break;
			}
			case 6:
			{
				ONE_WIRE_WP7 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP7 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP7;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP7 = 1;
				break;
			}
			case 7:
			{
				ONE_WIRE_WP8 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP8 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP8;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP8 = 1;
				break;
			}
			case 8:
			{
				ONE_WIRE_WP9 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP9 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP9;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP9 = 1;
				break;
			}
			case 9:
			{
				ONE_WIRE_WP10 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP10 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP10;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP10 = 1;
				break;
			}
			case 10:
			{
				ONE_WIRE_WP11 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP11 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP11;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP11 = 1;
				break;
			}
			case 11:
			{
				ONE_WIRE_WP12 = 0;
				tick_delay(OW_A);
		
				ONE_WIRE_WP12 = 1;
				tick_delay(OW_E);

				result = ONE_WIRE_WP12;
				EA = 1;
				tick_delay(OW_F);
		
				ONE_WIRE_WP12 = 1;
				break;
			}
			default:
				EA = 1;
				break;
		}
	}
    tick_delay(OW_D); 
    return result;
}


void one_wire_write_1(unsigned char cPortNum, unsigned char speed)
{
	if(ONE_WIRE_OVERDRIVE_SPEED == speed)
	{

	    switch(cPortNum)
	    {
	        case 0:
	        {
	            EA = 0;
	            ONE_WIRE_WP1 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP1 = 1;
	            EA = 1;
	            break;
	        }
	        case 1:
	        {
	            EA = 0;
	            ONE_WIRE_WP2 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP2 = 1;
	            EA = 1;
	            break;
	        }
	        case 2:
	        {
	            EA = 0;
	            ONE_WIRE_WP3 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP3 = 1;
	            EA = 1;
	            break;
	        }
	        case 3:
	        {
	            EA = 0;
	            ONE_WIRE_WP4 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP4 = 1;
	            EA = 1;
	            break;
	        }
	        case 4:
	        {
	            EA = 0;
	            ONE_WIRE_WP5 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP5 = 1;
	            EA = 1;
	            break;
	        }
	        case 5:
	        {
	            EA = 0;
	            ONE_WIRE_WP6 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP6 = 1;
	            EA = 1;
	            break;
	        }
	        case 6:
	        {
	            EA = 0;
	            ONE_WIRE_WP7 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP7 = 1;
	            EA = 1;
	            break;
	        }
	        case 7:
	        {
	            EA = 0;
	            ONE_WIRE_WP8 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP8 = 1;
	            EA = 1;
	            break;
	        }
	        case 8:
	        {
	            EA = 0;
	            ONE_WIRE_WP9 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP9 = 1;
	            EA = 1;
	            break;
	        }
	        case 9:
	        {
	            EA = 0;
	            ONE_WIRE_WP10 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP10 = 1;
	            EA = 1;
	            break;
	        }
	        case 10:
	        {
	            EA = 0;
	            ONE_WIRE_WP11 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP11 = 1;
	            EA = 1;
	            break;
	        }
	        case 11:
	        {
	            EA = 0;
	            ONE_WIRE_WP12 = 0;
	            tick_delay_one();
		
	            ONE_WIRE_WP12 = 1;
	            EA = 1;
	            break;
	        }
	        default:
	            EA = 1;
	            break;
			
	    }
	}
	else
	{
	    switch(cPortNum)
	    {
	        case 0:
	        {
	            EA = 0;
	            ONE_WIRE_WP1 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP1 = 1;
	            EA = 1;
	            break;
	        }
	        case 1:
	        {
	            EA = 0;
	            ONE_WIRE_WP2 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP2 = 1;
	            EA = 1;
	            break;
	        }
	        case 2:
	        {
	            EA = 0;
	            ONE_WIRE_WP3 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP3 = 1;
	            EA = 1;
	            break;
	        }
	        case 3:
	        {
	            EA = 0;
	            ONE_WIRE_WP4 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP4 = 1;
	            EA = 1;
	            break;
	        }
	        case 4:
	        {
	            EA = 0;
	            ONE_WIRE_WP5 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP5 = 1;
	            EA = 1;
	            break;
	        }
	        case 5:
	        {
	            EA = 0;
	            ONE_WIRE_WP6 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP6 = 1;
	            EA = 1;
	            break;
	        }
	        case 6:
	        {
	            EA = 0;
	            ONE_WIRE_WP7 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP7 = 1;
	            EA = 1;
	            break;
	        }
	        case 7:
	        {
	            EA = 0;
	            ONE_WIRE_WP8 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP8 = 1;
	            EA = 1;
	            break;
	        }
	        case 8:
	        {
	            EA = 0;
	            ONE_WIRE_WP9 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP9 = 1;
	            EA = 1;
	            break;
	        }
	        case 9:
	        {
	            EA = 0;
	            ONE_WIRE_WP10 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP10 = 1;
	            EA = 1;
	            break;
	        }
	        case 10:
	        {
	            EA = 0;
	            ONE_WIRE_WP11 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP11 = 1;
	            EA = 1;
	            break;
	        }
	        case 11:
	        {
	            EA = 0;
	            ONE_WIRE_WP12 = 0;
	            tick_delay(OW_A);
		
	            ONE_WIRE_WP12 = 1;
	            EA = 1;
	            break;
	        }
	        default:
	            EA = 1;
	            break;
			
	    }
	}
    tick_delay(OW_B);
}
/****************************************************************************
12个端口都写1

*****************************************************************************/
void one_wire_write12_1(unsigned char speed)
{
	if(ONE_WIRE_OVERDRIVE_SPEED == speed)
	{
		EA = 0;
		clear_0();
		tick_delay_one();
		set_1();
 		EA = 1;
	    
	}
	else
	{
		EA = 0;
		clear_0();

		tick_delay(OW_A);

		set_1();
 		EA = 1;		
	}
    tick_delay(OW_B);
}
/***************************************************************************
12端口写0
***************************************************************************/
void one_wire_write12_0(void)
{
	EA = 0;
	clear_0();

	tick_delay(OW_C);

	set_1();
	EA = 1;
    tick_delay(OW_D);
}
void one_wire_write_0(unsigned char cPortNum, unsigned char speed)
{

    switch(cPortNum)
    {

        case 0:
        {
            EA = 0;
            ONE_WIRE_WP1 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP1 = 1;
            EA = 1;
            break;
        }
        case 1:
        {
            EA = 0;
            ONE_WIRE_WP2 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP2 = 1;
            EA = 1;
            break;
        }
        case 2:
        {
            EA = 0;
            ONE_WIRE_WP3 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP3 = 1;
            EA = 1;
            break;
        }
        case 3:
        {
            EA = 0;
            ONE_WIRE_WP4 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP4 = 1;
            EA = 1;
            break;
        }
        case 4:
        {
            EA = 0;
            ONE_WIRE_WP5 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP5 = 1;
            EA = 1;
            break;
        }
        case 5:
        {
            EA = 0;
            ONE_WIRE_WP6 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP6 = 1;
            EA = 1;
            break;
        }
        case 6:
        {
            EA = 0;
            ONE_WIRE_WP7 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP7 = 1;
            EA = 1;
            break;
        }
        case 7:
        {
            EA = 0;
            ONE_WIRE_WP8 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP8 = 1;
            EA = 1;
            break;
        }
        case 8:
        {
            EA = 0;
            ONE_WIRE_WP9 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP9 = 1;
            EA = 1;
            break;
        }
        case 9:
        {
            EA = 0;
            ONE_WIRE_WP10 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP10 = 1;
            EA = 1;
            break;
        }
        case 10:
        {
            EA = 0;
            ONE_WIRE_WP11 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP11 = 1;
            EA = 1;
            break;
        }
        case 11:
        {
            EA = 0;
            ONE_WIRE_WP12 = 0;
            tick_delay(OW_C);
	
            ONE_WIRE_WP12 = 1;
            EA = 1;
            break;
        }
        default:
            EA = 1;
            break;
    }
	
    tick_delay(OW_D);
}
/**************************************************************************************
12个端口写1个字节
***************************************************************************************/
void one_wire_write12_byte(unsigned char dat,unsigned char speed)
{
    char i;
    unsigned char temp;

    temp = dat;
    for(i=0;i<8;i++)
    {
        if(temp & 0x01)
        {
            one_wire_write12_1(speed);
        }
        else
        {
            one_wire_write12_0();
        }
  
        temp >>= 1;
    }
}

void one_wire_write_byte(unsigned char dat, unsigned char cPortNum, unsigned char speed)
{
    char i;
    unsigned char temp;

    temp = dat;
    for(i=0;i<8;i++)
    {
        if(temp & 0x01)
        {
            one_wire_write_1(cPortNum, speed);
        }
        else
        {
            one_wire_write_0(cPortNum, speed);
        }
  
        temp >>= 1;
    }
}

/****************************************************************************
读12个端口1个字节
*****************************************************************************/
unsigned char* one_wire_read12_byte(unsigned char speed)
{
    int i,j,ret;
		static unsigned char result[12];
	memory_clear(result,12);
    for ( i = 0; i < 8; i++)
    {

		ret = one_wire_read12_bit( speed);
		for(j = 0;j < 12;j++)
		{
        	WatchDog();
			result[j] >>= 1;
        	if (ret&(1<<j))
        	{
        	    result[j] |= 0x80;
        	}
		}
    }
	
    return result;
}
/*---------------------------------------------------------------------
 * Read 1 -wire data byte and return it
 *---------------------------------------------------------------------*/
unsigned char one_wire_read_byte(unsigned char cPortNum, unsigned char speed)
{
    int i;
    unsigned result;
    result = 0;

    for ( i = 0; i < 8; i++)
    {	
        result >>= 1;

        if (one_wire_read_bit(cPortNum, speed))
        {
            result |= 0x80;
        }
    }
	
    return result;
}

#endif

