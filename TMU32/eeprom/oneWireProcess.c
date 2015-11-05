#include "typeDef.h"
#include "oneWireProcess.h"
#include "share.h"
#include "mcuInit.h"
#include "ds2431Process.h"

#ifdef ID_ONE_WIRE

/*one_wire_standard  为1时表示标准速度，为0时表示高速模式*/
static unsigned char one_wire_standard;    

/*one_wire_is_overdrive ，用于高速模式时是否需要强制使DS8431为标准模式再设置为高速模式。
该值为1时表示已经进入高速模式，无需重新设定；为0则需重新设定*/
static unsigned char one_wire_is_overdrive[12];

/****************************************************
*【函数名】one_wire_init
*【功     能】设置DS2431通信模式
*【输     入】standard    为1时表示标准模式，为0时表示高速模式
                            
*【输     出】无
*【作     者】wuyanan
*【日     期】2013-06-15
*****************************************************/

void one_wire_init(enum ONE_WIRE_STANDARD speed)
{
    one_wire_standard = speed;
    memory_clear(one_wire_is_overdrive, sizeof(one_wire_is_overdrive));
    one_wire_set_speed(speed);
}

/****************************************************
*【函数名】one_wire_write_eight_byte
*【功     能】将8字节数据写入DS2431,目标地址最末3 bit 必须为0
*【输     入】addr    目标地址
                            dat     将要写入的数据
                            cPortNum    端口
                            
*【输     出】无
*【作     者】wuyanan
*【日     期】2013-06-15
*****************************************************/
unsigned char one_wire_write_eight_byte( unsigned char cPortNum, unsigned int addr, unsigned char *dat)
{
    unsigned char *temp_data;
    unsigned char temp_num;   
    unsigned char i;   
    union myunion union1;
    unsigned char temp_reg[3];

    temp_num = 8;
    union1.whole = addr;
    temp_data = dat;
	
    memory_clear(temp_reg,3);
	
    if(union1.half.lowByte & 0x07)//目标地址最后三位为0
    {
        return ERROR;
    }

    if( ONE_WIRE_STANDARD_SPEED == one_wire_standard || ONE_WIRE_OVERDRIVE_SPEED == one_wire_is_overdrive[cPortNum])
    {
        if( one_wire_start_get_ack(cPortNum))
        {
            one_wire_is_overdrive[cPortNum] = ONE_WIRE_STANDARD_SPEED;
            return ERROR; 
        }
		
        one_wire_write_byte(OW_SKIP_ROM, cPortNum, one_wire_is_overdrive[cPortNum]);
    }
    else
    {
        /*由于不知道DS2431目前处于什么模式，所以先强制使其为标准模式，
              在标准模式下，将其设置为高速模式*/
        one_wire_set_speed(ONE_WIRE_STANDARD_SPEED);
	
        if(one_wire_start_get_ack(cPortNum))
        {
            one_wire_is_overdrive[cPortNum] = ONE_WIRE_STANDARD_SPEED;
			one_wire_set_speed(ONE_WIRE_OVERDRIVE_SPEED);
            return ERROR; 
        }
		
        one_wire_write_byte(OW_OVERDRIVE_SKIP_ROM, cPortNum, ONE_WIRE_STANDARD_SPEED);
		
        one_wire_set_speed(ONE_WIRE_OVERDRIVE_SPEED);
		one_wire_is_overdrive[cPortNum] = ONE_WIRE_OVERDRIVE_SPEED;
    }
	
	/*将数据写入暂存器*/
    one_wire_write_byte(OW_WRITE_SCRATCHPAD, cPortNum, one_wire_is_overdrive[cPortNum]);
    one_wire_write_byte(union1.half.lowByte, cPortNum, one_wire_is_overdrive[cPortNum]);
    one_wire_write_byte(union1.half.highByte, cPortNum, one_wire_is_overdrive[cPortNum]);
	
    while(temp_num--)
    {   
        one_wire_write_byte(*temp_data++, cPortNum, one_wire_is_overdrive[cPortNum]);
        WatchDog();
    }
	
	/*获取OW_COPY_SCRATCHPAD命令所需的三字节模板:目标地址和结束偏移量/数据状态字*/
    if( one_wire_start_get_ack(cPortNum))
    {
        one_wire_is_overdrive[cPortNum] = ONE_WIRE_STANDARD_SPEED;
        return ERROR;
    }
	
    one_wire_write_byte(OW_SKIP_ROM, cPortNum, one_wire_is_overdrive[cPortNum]);
    one_wire_write_byte(OW_READ_SCRATCHPAD, cPortNum, one_wire_is_overdrive[cPortNum]);
    for ( i = 0; i < 3; i++)
    {
        temp_reg[i] = one_wire_read_byte(cPortNum, one_wire_is_overdrive[cPortNum]);
        WatchDog();
    }
	
    /*将暂存器中数据复制到目标地址*/
    if( one_wire_start_get_ack(cPortNum))
    {
        one_wire_is_overdrive[cPortNum] = ONE_WIRE_STANDARD_SPEED;
        return ERROR; 
    }
	
    one_wire_write_byte(OW_SKIP_ROM, cPortNum, one_wire_is_overdrive[cPortNum]);
	
    one_wire_write_byte(OW_COPY_SCRATCHPAD, cPortNum, one_wire_is_overdrive[cPortNum]);
    one_wire_write_byte(temp_reg[0], cPortNum, one_wire_is_overdrive[cPortNum]);
    one_wire_write_byte(temp_reg[1], cPortNum, one_wire_is_overdrive[cPortNum]);
    one_wire_write_byte(temp_reg[2], cPortNum, one_wire_is_overdrive[cPortNum]);
	
    return OK;
}


/****************************************************
*【函数名】one_wire_read_memory_block
*【功     能】从DS2431中读取数据(从目标地址最多可连续读至0X008F 处)
*【输     入】 cPortNum    端口
                            addr    目标地址
                            recvPtr        接受读取数据的指针
                            len      读取长度(字节)
                           
                            
*【输     出】无
*【作     者】wuyanan
*【日     期】2013-06-15
*****************************************************/
unsigned char one_wire_read_memory_block( unsigned char cPortNum, int desAddr, unsigned char*recvPtr, unsigned char len)
{
    int i ;
    union myunion union1;

    union1.whole = desAddr;
	
    if(!recvPtr)
    {
        return ERROR;
    }
	
    if( ONE_WIRE_STANDARD_SPEED == one_wire_standard || ONE_WIRE_OVERDRIVE_SPEED == one_wire_is_overdrive[cPortNum])
    {
        if( one_wire_start_get_ack(cPortNum))
        {
            one_wire_is_overdrive[cPortNum] = ONE_WIRE_STANDARD_SPEED;
            return ERROR; 
        }
		
        one_wire_write_byte(OW_SKIP_ROM, cPortNum, one_wire_is_overdrive[cPortNum]);
    }
    else
    {
        //由于不知道DS2431目前处于什么模式，所以先强制使其为标准模式，在标准模式下，将其设置为高速模式//
        one_wire_set_speed(ONE_WIRE_STANDARD_SPEED);
	
        if( one_wire_start_get_ack(cPortNum))
        {
            one_wire_is_overdrive[cPortNum] = ONE_WIRE_STANDARD_SPEED;
			one_wire_set_speed(ONE_WIRE_OVERDRIVE_SPEED);
            return ERROR; 
        }
		
        one_wire_write_byte(OW_OVERDRIVE_SKIP_ROM, cPortNum, ONE_WIRE_STANDARD_SPEED);
		
        one_wire_set_speed(ONE_WIRE_OVERDRIVE_SPEED);
		one_wire_is_overdrive[cPortNum] = ONE_WIRE_OVERDRIVE_SPEED;
    }
/*		
if(one_wire_is_overdrive[cPortNum] == ONE_WIRE_STANDARD_SPEED)
	{
					one_wire_set_speed(ONE_WIRE_STANDARD_SPEED);
					if( one_wire_start_get_ack(cPortNum) == 0 )
					{				
								one_wire_write_byte(OW_OVERDRIVE_SKIP_ROM, cPortNum, ONE_WIRE_STANDARD_SPEED);
						
								one_wire_set_speed(ONE_WIRE_OVERDRIVE_SPEED);
								one_wire_is_overdrive[cPortNum] = ONE_WIRE_OVERDRIVE_SPEED;
				 }
				 else
				 {							
								one_wire_is_overdrive[cPortNum] = ONE_WIRE_OVERDRIVE_SPEED;
								return ERROR;
					}
		}	
		else
		{
						one_wire_set_speed(ONE_WIRE_OVERDRIVE_SPEED);
						if( one_wire_start_get_ack(cPortNum) == 0 )
						{		
									
									one_wire_write_byte(OW_OVERDRIVE_SKIP_ROM, cPortNum, ONE_WIRE_OVERDRIVE_SPEED);
					 }
					 else
					 {
							one_wire_is_overdrive[cPortNum] = ONE_WIRE_STANDARD_SPEED;
							return ERROR;
						}				
		}
	*/
    one_wire_write_byte(OW_READ_MEMORY, cPortNum, one_wire_is_overdrive[cPortNum]);		
    one_wire_write_byte(union1.half.lowByte, cPortNum, one_wire_is_overdrive[cPortNum]);
    one_wire_write_byte(union1.half.highByte, cPortNum, one_wire_is_overdrive[cPortNum]);

    for(i = 0; i < len;i++)
    {
        *recvPtr++ = one_wire_read_byte(cPortNum, one_wire_is_overdrive[cPortNum]);
        WatchDog();
    }

    return OK;

}

/****************************************************
*【函数名】one_wire_read12_memory_block
*【功     能】从12个DS2431中读取数据(从目标地址最多可连续读至0X008F 处)
*【输     入】 ecvPtr        接受读取数据的指针

                                                     
*【输     出】无
*【作     者】陈杰
*【日     期】2013-12-10
*****************************************************/
//unsigned char xx = 0;
unsigned char one_wire_read12_memory_block( int desAddr,unsigned char*recvPtr,unsigned char len)
{
    int i , j, n = 0;
		unsigned char* ret;
    union myunion union1;

    union1.whole = desAddr;
	
    if(!recvPtr)
    {
        return ERROR;
    }
    if( n = one_wire_start_get12_ack())
    {
		//		xx++;
    }

	one_wire_write12_byte(OW_SKIP_ROM, 0);
	one_wire_write12_byte(OW_READ_MEMORY, 0);		
    one_wire_write12_byte(union1.half.lowByte,0);
    one_wire_write12_byte(union1.half.highByte,0);

	for(i = 0; i < len;i++)
	{	
 		ret = one_wire_read12_byte(0);
		for(j = 0;j<12;j++)
		{
			if(n&(1<<j))
			{
				recvPtr[j*len+i] = 0;
			}else
			{
				recvPtr[j*len+i] = ret[j];
			}
		}
	    WatchDog();
	}

    return OK;
}
/****************************************************
*【函数名】one_wire_get_rom
*【功     能】获取DS2431的64 bit 光刻ROM码
*【输     入】 cPortNum    端口
                            recvPtr        接受读取数据的指针
                           
                            
*【输     出】无
*【作     者】wuyanan
*【日     期】2013-06-15
*****************************************************/

unsigned char one_wire_get_rom( unsigned char cPortNum, unsigned char *recvPtr)
{
    unsigned char *temp_data;
    unsigned char temp_num;   
    unsigned char i;   

    temp_num = 8;
    temp_data = recvPtr;

    if( ONE_WIRE_STANDARD_SPEED == one_wire_standard || ONE_WIRE_OVERDRIVE_SPEED == one_wire_is_overdrive[cPortNum])
    {
        /*由于不知道DS2431目前处于什么模式，所以先强制使其为标准模式，
              在标准模式下，将其设置为高速模式*/
        one_wire_set_speed(ONE_WIRE_STANDARD_SPEED);
	
        if( one_wire_start_get_ack(cPortNum))
        {
            one_wire_is_overdrive[cPortNum] = ONE_WIRE_STANDARD_SPEED;
			one_wire_set_speed(ONE_WIRE_OVERDRIVE_SPEED);
            return ERROR; 
        }

        one_wire_write_byte(OW_OVERDRIVE_SKIP_ROM, cPortNum, ONE_WIRE_STANDARD_SPEED);
		
        one_wire_set_speed(ONE_WIRE_OVERDRIVE_SPEED);
		one_wire_is_overdrive[cPortNum] = ONE_WIRE_OVERDRIVE_SPEED;
    }
	
    if( one_wire_start_get_ack(cPortNum))
    {
        one_wire_is_overdrive[cPortNum] = ONE_WIRE_STANDARD_SPEED;
        return ERROR; 
    }
	
    one_wire_write_byte(OW_READ_ROM, cPortNum, one_wire_is_overdrive[cPortNum]);
	
    for ( i = 0; i < 8; i++)
    {
        *temp_data++ = one_wire_read_byte(cPortNum, one_wire_is_overdrive[cPortNum]);
        WatchDog();
    }
		
    return OK;
}

#endif

