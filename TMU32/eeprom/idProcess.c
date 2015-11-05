#include "typeDef.h"
#include "c8051f340.h"
#include "mcuInit.h"
#include "ledStatus.h"
#include "idProcess.h"
#include "netCommand.h"
#include "share.h"
#include "oneWireProcess.h"
#include "crc.h"

unsigned char localid_buf[12][32];
unsigned char error_id_temp[12][32];
unsigned char id_check_null(unsigned char *ptr)
{
    unsigned char i;
    
    for(i=0; i<32; i++)
    {
        if( *ptr ++ )
            return ERROR;
    }

    return OK;
}


void refresh_current_id(void)
{
		unsigned char lport=0;
		WatchDog();
for(lport=0;lport<12;lport++)
{
    if( id_check_null(localid_buf[lport]) )
    {//端口插入了标签//
			/////////////////////////////////////	
			mainInfo.id[lport].null_num = 0;
			/////////////////////////////////////////
       if( localid_buf[lport][31] == CRC8_Table(localid_buf[lport],31)) 
				{
						mainInfo.id[lport].err_num = 0;
            mainInfo.id[lport].state = PORT_OK;
        }
        else 
        {
					if(mainInfo.id[lport].err_num == 0)
					{
								mainInfo.id[lport].err_num ++;	
								memory_copy(error_id_temp[lport],localid_buf[lport],32);
								mainInfo.id[lport].state = PORT_TEMP;
					}
					else
					{
								if(memory_compare(error_id_temp[lport], localid_buf[lport], 32))
								{
										memory_copy(error_id_temp[lport],localid_buf[lport],32);
								}
								else
								{		
										mainInfo.id[lport].err_num ++;
										if(mainInfo.id[lport].err_num >= 10)
										{												
														mainInfo.id[lport].err_num = 0;
														mainInfo.id[lport].state = PORT_ERROR;											
										}
								}
					}
        }
    }
    else
    {//端口没有插入标签//
						
						mainInfo.id[lport].null_num++;
						if(mainInfo.id[lport].null_num >=3)
						{
								mainInfo.id[lport].null_num = 0;
								mainInfo.id[lport].state = PORT_NULL;
						}
						else
						{
							mainInfo.id[lport].state = PORT_TEMP;
						}
    }
		
		if(mainInfo.id[lport].state != PORT_TEMP)
		{
					if(memory_compare(mainInfo.id[lport].local_id, localid_buf[lport], 32))
					{
									mainInfo.id[lport].eid_32_flag = 1;
									memory_copy(mainInfo.id[lport].local_id,localid_buf[lport],32);
									if(mainInfo.id[lport].state != PORT_NULL)//如果端口有标签
									{
											mainInfo.id[lport].eid_96_flag = 1; 
											mainInfo.id[lport].eid_96_send_flag = 0; 
									}
									else
									{
											mainInfo.id[lport].eid_96_send_flag = 1; 
											memory_clear(&mainInfo.id[lport].local_id[32],96);
									}
					}
		}
	}
}
void read_id(unsigned char type,unsigned char len,unsigned char* buf)
{
	unsigned char i,j;
	static unsigned char buff[12][16];
	memory_clear((unsigned char*)buff,sizeof(buff));
	if(type == 0)	//读前32字节数据
	{
		one_wire_read12_memory_block(len*16,(unsigned char *)buff,16);
		for(i=0;i<12;i++)
		{
			for(j=0;j<16;j++)
			{
					buf[i*32+len*16+j] = buff[i][j];
			}	
		}
	}
}


