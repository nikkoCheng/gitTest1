#include "typeDef.h"
#include "oneWireProcess.h"
#include "share.h"
#include "mcuInit.h"
#include "ds2431Process.h"

#ifdef ID_ONE_WIRE

/*one_wire_standard  Ϊ1ʱ��ʾ��׼�ٶȣ�Ϊ0ʱ��ʾ����ģʽ*/
static unsigned char one_wire_standard;    

/*one_wire_is_overdrive �����ڸ���ģʽʱ�Ƿ���Ҫǿ��ʹDS8431Ϊ��׼ģʽ������Ϊ����ģʽ��
��ֵΪ1ʱ��ʾ�Ѿ��������ģʽ�����������趨��Ϊ0���������趨*/
static unsigned char one_wire_is_overdrive[12];

/****************************************************
*����������one_wire_init
*����     �ܡ�����DS2431ͨ��ģʽ
*����     �롿standard    Ϊ1ʱ��ʾ��׼ģʽ��Ϊ0ʱ��ʾ����ģʽ
                            
*����     ������
*����     �ߡ�wuyanan
*����     �ڡ�2013-06-15
*****************************************************/

void one_wire_init(enum ONE_WIRE_STANDARD speed)
{
    one_wire_standard = speed;
    memory_clear(one_wire_is_overdrive, sizeof(one_wire_is_overdrive));
    one_wire_set_speed(speed);
}

/****************************************************
*����������one_wire_write_eight_byte
*����     �ܡ���8�ֽ�����д��DS2431,Ŀ���ַ��ĩ3 bit ����Ϊ0
*����     �롿addr    Ŀ���ַ
                            dat     ��Ҫд�������
                            cPortNum    �˿�
                            
*����     ������
*����     �ߡ�wuyanan
*����     �ڡ�2013-06-15
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
	
    if(union1.half.lowByte & 0x07)//Ŀ���ַ�����λΪ0
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
        /*���ڲ�֪��DS2431Ŀǰ����ʲôģʽ��������ǿ��ʹ��Ϊ��׼ģʽ��
              �ڱ�׼ģʽ�£���������Ϊ����ģʽ*/
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
	
	/*������д���ݴ���*/
    one_wire_write_byte(OW_WRITE_SCRATCHPAD, cPortNum, one_wire_is_overdrive[cPortNum]);
    one_wire_write_byte(union1.half.lowByte, cPortNum, one_wire_is_overdrive[cPortNum]);
    one_wire_write_byte(union1.half.highByte, cPortNum, one_wire_is_overdrive[cPortNum]);
	
    while(temp_num--)
    {   
        one_wire_write_byte(*temp_data++, cPortNum, one_wire_is_overdrive[cPortNum]);
        WatchDog();
    }
	
	/*��ȡOW_COPY_SCRATCHPAD������������ֽ�ģ��:Ŀ���ַ�ͽ���ƫ����/����״̬��*/
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
	
    /*���ݴ��������ݸ��Ƶ�Ŀ���ַ*/
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
*����������one_wire_read_memory_block
*����     �ܡ���DS2431�ж�ȡ����(��Ŀ���ַ������������0X008F ��)
*����     �롿 cPortNum    �˿�
                            addr    Ŀ���ַ
                            recvPtr        ���ܶ�ȡ���ݵ�ָ��
                            len      ��ȡ����(�ֽ�)
                           
                            
*����     ������
*����     �ߡ�wuyanan
*����     �ڡ�2013-06-15
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
        //���ڲ�֪��DS2431Ŀǰ����ʲôģʽ��������ǿ��ʹ��Ϊ��׼ģʽ���ڱ�׼ģʽ�£���������Ϊ����ģʽ//
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
*����������one_wire_read12_memory_block
*����     �ܡ���12��DS2431�ж�ȡ����(��Ŀ���ַ������������0X008F ��)
*����     �롿 ecvPtr        ���ܶ�ȡ���ݵ�ָ��

                                                     
*����     ������
*����     �ߡ��½�
*����     �ڡ�2013-12-10
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
*����������one_wire_get_rom
*����     �ܡ���ȡDS2431��64 bit ���ROM��
*����     �롿 cPortNum    �˿�
                            recvPtr        ���ܶ�ȡ���ݵ�ָ��
                           
                            
*����     ������
*����     �ߡ�wuyanan
*����     �ڡ�2013-06-15
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
        /*���ڲ�֪��DS2431Ŀǰ����ʲôģʽ��������ǿ��ʹ��Ϊ��׼ģʽ��
              �ڱ�׼ģʽ�£���������Ϊ����ģʽ*/
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

