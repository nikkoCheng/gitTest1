#include "typedef.h"
#include "ledStatus.h"
#include "netCommand.h"
#include "tmuProcess.h"
#include "idProcess.h"
#include "c8051f340.h"
#include "uart1.h"
#include "cmdCode.h"
#include "share.h"
#include "hardInfo.h"
#include "update.h"
#include "ds2431Process.h"
#include "oneWireProcess.h"
#include "variable.h"
#include "delay.h"
#include "crc.h"
#include "tmunum.h"

static unsigned char nextFramSendFlag;
extern unsigned char localid_buf[12][32];
			 unsigned char eid_96_temp[96]; 
			 unsigned char port_num;

void slot_address_init(void)
{
	static unsigned char temp;
	unsigned char table[9] = {0,6,5,4,3,2,1,7,8};

				temp = P0 & 0x0F;
				mainInfo.slotAddress = table[temp];
}

void slot_aging_time_init(unsigned char value)
{
    mainInfo.slotAging = DEFAULT_SLOT_AGING_TIME*value;
}

void slot_delay_time_init(void)
{
    mainInfo.slotDelay = DEFAULT_SLOT_DELAY_TIME;
}

/*50ms调用1次*/
void slot_aging_time(void)
{
    static unsigned char count = 0,i=11;
    unsigned char tempValue;
	
    tempValue = get_tmu_addr_mode();

    if( mainInfo.slotAging )     /*2秒后地址清零*/
        mainInfo.slotAging --;   

    if( mainInfo.slotDelay )
        mainInfo.slotDelay --;   /*收到地址,2秒后内允许下行发送*/

    if( mainInfo.slotAging == 0 )
		{
        if(DYNEMIC_TMU_FLAG == tempValue)
        {
            mainInfo.slotAddress =0;
            linkFmu =0;
        }
        else if(STATIC_TMU_FLAG == tempValue)
        {
            linkFmu =0;
        }
		}

	      if(DYNEMIC_TMU_FLAG == tempValue)
        {
						    if( (linkFmu) && (mainInfo.slotDelay==0) )	
								{
										down_tx_enable();
								}
								else
								{
										down_tx_disable();
									
										/*端口灯轮流亮*/
										count = (count+1) % 3;
										if( count == 0 )
										{
												i = i % PORT_ALL_NUM;
												mainInfo.ledState[i] = LED_STATE_OFF;

												i = (i + 1) % PORT_ALL_NUM;
												mainInfo.ledState[i] = LED_STATE_ON;
										}
										refresh_led();
								}
        }
        else if(STATIC_TMU_FLAG == tempValue)
        {
							if( 0 == linkFmu)
							{
												count = 0;//(count+1) % 3;
												if( count == 0 )
												{
														i = i % PORT_ALL_NUM;
														mainInfo.ledState[i] = LED_STATE_OFF;

														i = (i + 1) % PORT_ALL_NUM;
														mainInfo.ledState[i] = LED_STATE_ON;
												}
												refresh_led();						
							}						
            
        }

}


// 计算累加和校验字节
unsigned char uart_fcs(unsigned char *ptr, unsigned char len)
{
	unsigned char sum;
	unsigned char i;
	unsigned char *tempPtr;

	sum     = 0;	
	tempPtr = ptr;

	for(i=0; i<len; i++, tempPtr++)
		sum += *tempPtr;
	
	return (~sum);
}


void uart_frame_pack()
{
    unsigned char *ptr;

    ptr = uart1.ackBuffer;

    *ptr ++ = UART_DEVICE_SYN1;         /*数据包头*/
    *ptr ++ = UART_DEVICE_SYN2;
    *ptr ++ = mainInfo.slotAddress;
    *ptr ++ = uart1.txLen;

     ptr = uart1.ackBuffer + uart1.txLen - 1;
                       
    *ptr = uart_fcs(uart1.ackBuffer, uart1.txLen-1);   /*计算FCS*/

    uart1.txPtr = uart1.ackBuffer;
}

/*命令CMD1  查询模块型号*/
void uart1_set_slot_address(void)
{   
    unsigned char *ptr;
    unsigned char tempValue;
		unsigned char code * data ptrCode;

    uart1.txLen = 2 + FRAME_DEVICE_FIX_BYTES + 22 + 6;    //消息类型和命令码2个，除去数据体外的5字节，22是协议里的               /*长度*/
    uart1.ackBuffer[4] = RET_TYPE;             /*类型*/
    uart1.ackBuffer[5] = CMD_SET_SLOT_ADDRESS;

    nextFramSendFlag = 0;
    tempValue = get_tmu_addr_mode();
	

    if(DYNEMIC_TMU_FLAG == tempValue)
    {
        mainInfo.slotAddress = uart1.rxBuffer[6];//动态地址
        linkFmu = 1;//盘在线
    }
    else if(mainInfo.slotAddress == uart1.rxBuffer[6])
    {
        slot_address_init();
        linkFmu = 1;				
    }
		
	WatchDog();
	earseCodeFlash(TMU_NUM_INFO_ADDR);
	WatchDog();
	writeInnerFlash(TMU_NUM_INFO_ADDR, 10, x);
  ptrCode = (char code *)TMU_NUM_INFO_ADDR;
		
		
		
    ptr = &uart1.ackBuffer[6];
    get_device_info(ptr);
}




void uart1_rd_label(void)
{
	unsigned char *srcPtr,*destPtr;
 	unsigned char cPortNum,len,addr;
	uart1.ackBuffer[4] = RET_TYPE;             /*类型*/
    uart1.ackBuffer[5] = CMD_RD_LABEL;
	srcPtr = &uart1.rxBuffer[6];
	destPtr = &uart1.ackBuffer[6];
	cPortNum = *srcPtr++;
	addr = *srcPtr++;
	len  = *srcPtr++;
	memory_clear(destPtr,len);
	one_wire_read_memory_block( cPortNum,addr, destPtr, len);
	uart1.txLen = len+7;
}
/*命令写标签*/
void uart1_wr_label(void)
{
	unsigned char *srcPtr;
	unsigned char cPortNum,len,addr;
	unsigned char i = 0;
    unsigned char tempArry[96];
	uart1.ackBuffer[4] = RET_TYPE;             /*类型*/
    uart1.ackBuffer[5] = CMD_WR_LABEL;

	srcPtr = &uart1.rxBuffer[6];
	cPortNum = *srcPtr++;
	addr = *srcPtr++;
	len  = *srcPtr++;
	memory_copy(tempArry,srcPtr,len);
	
	uart1.txLen = 7;
  uart_frame_pack();
  uart1_trans_enable();
	while(uart1.txLen);
	while(len)
	{
		one_wire_write_eight_byte( cPortNum-1,addr+i*8, &tempArry[i*8]);
		delay_ms(10);
		len = len-8;
		i++;	
	}	
	
	mainInfo.id[cPortNum-1].eid_32_flag = 1;
	mainInfo.id[cPortNum-1].eid_96_flag = 1; 
	mainInfo.id[cPortNum-1].eid_96_send_flag = 0;
}




/*命令CMD3  轮询命令*/
void uart1_poll_current(void)
{
    unsigned char *srcPtr, *destPtr;
    unsigned char i;
    unsigned int  tmp;	
		unsigned char addr = 0;
		unsigned char addr_temp[3]={0,0,0};


    uart1.ackBuffer[4] = RET_TYPE;             //类型//
    uart1.ackBuffer[5] = CMD_RD_CURRENT;

    //接收LED灯状态//
    srcPtr = &uart1.rxBuffer[6];
    destPtr = &mainInfo.rcvLedState[0];//
    for(i=0; i<PORT_ALL_NUM; i++)
    {
        *destPtr++ = *srcPtr++;
    }
		addr = uart1.rxBuffer[18];//////////////////
		if(addr<1 || addr>12)
		{return;}
		srcPtr = &uart1.rxBuffer[19];
		memory_copy(mainInfo.id[addr-1].desig_id,srcPtr,32);	
		srcPtr += 32; 
		memory_copy(mainInfo.id[(addr)%12].desig_id,srcPtr,32);
		srcPtr += 32; 
		memory_copy(mainInfo.id[(addr+1)%12].desig_id,srcPtr,32);
		srcPtr += 32; 
		
		port_state_process();
				
    srcPtr = &uart1.ackBuffer[6];
		port_num = addr - 1;
		
		addr_temp[0] = inquire_port_change();
		if(0xFE != addr_temp[0])
		{
				*srcPtr++ = addr_temp[0] + 1;
				
				addr_temp[1] = inquire_port_change();
				if(addr_temp[1] == 0xFE )
				{
					addr_temp[1] = port_num;
					port_num = (port_num+1)%12;
				}
				*srcPtr++ = addr_temp[1] + 1;
					
				
				addr_temp[2] = inquire_port_change();
				if(addr_temp[2] == 0xFE )
				{
					addr_temp[2] = port_num;
					port_num = (port_num+1)%12;
				}
				*srcPtr++ = addr_temp[2] + 1;
				
				*srcPtr++ = mainInfo.portState[addr_temp[0]];
				*srcPtr++ = mainInfo.portState[addr_temp[1]];
				*srcPtr++ = mainInfo.portState[addr_temp[2]];
				
					
				memory_copy(srcPtr,(unsigned char *)&mainInfo.id[addr_temp[0]].local_id,32);
				srcPtr += 32;
				memory_copy(srcPtr,(unsigned char *)&mainInfo.id[addr_temp[1]].local_id,32);
				srcPtr += 32;
				memory_copy(srcPtr,(unsigned char *)&mainInfo.id[addr_temp[2]].local_id,32);
				srcPtr += 32;
		}
		else
		{
				addr_temp[0] = inquire_port_96();
				if( 0 != addr_temp[0] )
				{
					*srcPtr++ = 0xF0 + addr_temp[0];
					memory_copy(srcPtr,&mainInfo.id[addr_temp[0]-1].local_id[32],96);					
					srcPtr += 96;
				}
				else
				{
											addr_temp[0] = port_num;
											addr_temp[1] = port_num + 1;
											addr_temp[2] = port_num + 2;
											
											*srcPtr++ = addr_temp[0] + 1;
											*srcPtr++ = addr_temp[1] + 1;
											*srcPtr++ = addr_temp[2] + 1;
											
											*srcPtr++ = mainInfo.portState[addr_temp[0]];
											*srcPtr++ = mainInfo.portState[addr_temp[1]];
											*srcPtr++ = mainInfo.portState[addr_temp[2]];
											
												
											memory_copy(srcPtr,(unsigned char *)&mainInfo.id[addr_temp[0]].local_id,32);
											srcPtr += 32;
											memory_copy(srcPtr,(unsigned char *)&mainInfo.id[addr_temp[1]].local_id,32);
											srcPtr += 32;
											memory_copy(srcPtr,(unsigned char *)&mainInfo.id[addr_temp[2]].local_id,32);
											srcPtr += 32;
				}
		}
		
				
    tmp = srcPtr - &uart1.ackBuffer;   
    uart1.txLen = tmp + 1;  //长度//		
}

void uart1_command(void)
{
  char cTmp;
	static unsigned char update_tmp;	//升级标志位，实现指示灯闪烁
    unsigned char tempValue;
#if 1
    cTmp = uart_fcs(uart1.rxBuffer, uart1.rxBuffer[3]);
    if( cTmp) /*校验是否正确*/
        return;
#endif

    switch ( uart1.rxBuffer[5] )
    {   
        case CMD_SET_SLOT_ADDRESS:      /*设置盘号*/
					
            tempValue = get_tmu_addr_mode();
            if(DYNEMIC_TMU_FLAG == tempValue)//如果盘地址为动态地址
            {
                if( (mainInfo.slotAddress == 0) && (uart1.rxBuffer[6] != 0) )
                { 
                    slot_delay_time_init();
                    uart1_set_slot_address();
                }
                else if((uart1.rxBuffer[6] == mainInfo.slotAddress) && (uart1.rxBuffer[6] != 0) )
                { 
                    uart1_set_slot_address(); 
                }
            }
            else if(STATIC_TMU_FLAG == tempValue)//如果盘地址为静态地址
            {
                if( (linkFmu == 0) && (uart1.rxBuffer[6] == mainInfo.slotAddress) )
                { 
                    slot_delay_time_init();
										linkFmu = 1;
										uart1_set_slot_address();
                }
								else if((linkFmu == 1) && (uart1.rxBuffer[6] == mainInfo.slotAddress))
								{
									linkFmu = 1;
									uart1_set_slot_address();
								}
            }
            break;

        case CMD_RD_CURRENT:            /*查询当前端口id*/

			if( ((uart1.rxBuffer[2]&0x0F) != 0) && ((uart1.rxBuffer[2]&0x0F) == mainInfo.slotAddress) )
        {
						uart1_poll_current();
				}
            break;
		case CMD_RD_LABEL:              /*读标签*/
			if( ((uart1.rxBuffer[2]&0x0F) != 0) && ((uart1.rxBuffer[2]&0x0F) == mainInfo.slotAddress) )
			{            
				uart1_rd_label();
			}
			break;
		case CMD_WR_LABEL:				/*写标签*/	
            if( ((uart1.rxBuffer[2]&0x0F) != 0) && ((uart1.rxBuffer[2]&0x0F) == mainInfo.slotAddress) )
			{
	            uart1_wr_label();
			}
            break;	

		case CMD_UPDATE:				/*升级文件处理*/
			if( ((uart1.rxBuffer[2]&0x0F) != 0) && ((uart1.rxBuffer[2]&0x0F) == mainInfo.slotAddress) )
			{
				if(update_tmp)
				{
					CPU_RUN_LED_1;
					update_tmp = 0;
				}else
				{
					CPU_RUN_LED_0;
					update_tmp = 1;
				}
				update_file();
			}			
			break;
        default:
			CPU_RUN_LED_1;
            break;
    }   
}

void uart_process(void)
{
    unsigned char tempValue;
	
	if(uart1.uartState==UART_RX_FRAME_OK )
	{
        uart1_command();
        tempValue = get_tmu_addr_mode();

        if(STATIC_TMU_FLAG == tempValue)//如果是静态地址，检测该地址数据才初始化盘在线状态
        {
            if(mainInfo.slotAddress == (uart1.rxBuffer[2]&0x0F))
            {
                slot_aging_time_init(2);
            }
            if(linkFmu)//盘在线，则回复命令处理结果，否则不回复
            {
                if( uart1.txLen )
                {
                    uart_frame_pack();
                    uart1_trans_enable();
                } 
							//	while(uart1.txLen);
								//delay_ms(5);
							
								//read_id_96();
            }
        }
        else if(DYNEMIC_TMU_FLAG == tempValue)//动态地址盘
        {
            slot_aging_time_init(1);
            if( uart1.txLen )
            {
                uart_frame_pack();
                uart1_trans_enable();
            } 
					//	while(uart1.txLen);
					//	read_id_96();
        }
		    uart1.uartState = 0;
	}

    uart1_rcv_enable();
}

void read_id_96(void)
{
	unsigned char i;
	for(i=0;i<12;i++)
	{
			if(mainInfo.id[i].eid_96_flag == 1)
			{
				one_wire_read_memory_block( i,32, eid_96_temp, 96);
				memory_copy(&mainInfo.id[i].local_id[32],eid_96_temp,96);	
				mainInfo.id[i].eid_96_flag = 0;
				mainInfo.id[i].eid_96_send_flag = 1;
				break;				
			}			
	}
}

void port_state_process(void)
{
    unsigned char i;
    if( !linkFmu )	
        return;

    for(i=0; i<PORT_ALL_NUM; i++)
    {
        switch(mainInfo.rcvLedState[i])
        {
            case ENUM_LED_STATUS_OFF:
            case ENUM_LED_STATUS_SLOW:
            case ENUM_LED_STATUS_QUICK:
            case ENUM_LED_STATUS_ON: 							
									mainInfo.portState[i] = port_status_1_port_process(i);
									mainInfo.ledState[i] = mainInfo.rcvLedState[i];
									break;
						
            case ENUM_LED_STATUS_AUTO:
									mainInfo.portState[i] = port_status_1_port_process(i);
									break;
						default:
									mainInfo.ledState[i] = LED_STATE_OFF;
									break;
        }
    }
    
}

unsigned char port_status_1_port_process(unsigned char lPortTmp)
{
    unsigned char *ptrNative, *ptrDesig;
    unsigned char curAlarmStatus;
	
    ptrNative = mainInfo.id[lPortTmp].local_id;
    ptrDesig = mainInfo.id[lPortTmp].desig_id;
	
    if( !memory_compare(ptrNative, ptrDesig, 32) )/**-标签相同*/
    {
        if( id_check_null(mainInfo.id[lPortTmp].local_id) )
        {
						curAlarmStatus = ENUM_ALARM_DATA_STATUS_ID_OK;	//端口有标签正常
				}
				else
				{
						curAlarmStatus = ENUM_ALARM_DATA_STATUS_NULL_OK;//端口无标签正常
				}
				mainInfo.ledState[lPortTmp] = LED_STATE_OFF;
    }
	else	/*标签不相同*/
    {
        if( id_check_null(mainInfo.id[lPortTmp].local_id) )
        {
					if( mainInfo.id[lPortTmp].local_id[31] != CRC8_Table(mainInfo.id[lPortTmp].local_id,31) )
					{	
							curAlarmStatus = ENUM_ALARM_DATA_STATUS_ID_WRONG;//端口标签无效
							mainInfo.ledState[lPortTmp] = LED_STATE_QUICK;
					}
					else
					{
						curAlarmStatus = ENUM_ALARM_DATA_STATUS_ID_ERR;	//端口有标签异常
						mainInfo.ledState[lPortTmp] = LED_STATE_QUICK;
					}
				}
				else
				{
						curAlarmStatus = ENUM_ALARM_DATA_STATUS_NULL_ERR;//端口无标签异常
						mainInfo.ledState[lPortTmp] = LED_STATE_QUICK;					
				}
			}
    return curAlarmStatus;
}

unsigned char inquire_port_change(void)
{
	unsigned char i;
	for(i=0;i<12;i++)
	{
	//	if(mainInfo.id[port_num].eid_32_flag  == 1 || mainInfo.portState[port_num] == ENUM_ALARM_DATA_STATUS_NULL_ERR || mainInfo.portState[port_num] == ENUM_ALARM_DATA_STATUS_ID_ERR)
		if(mainInfo.id[port_num].eid_32_flag  == 1 )
		{
				mainInfo.id[port_num].eid_32_flag = 0;
				i = port_num;
				port_num = (port_num+1)%12;
				return i;
			}
			
			port_num = (port_num+1)%12;
	}
	
	return 0xFE;
}

unsigned char inquire_port_96(void)
{
	unsigned char i;
	for(i=0;i<12;i++)
	{
			if( 1 == mainInfo.id[i].eid_96_send_flag )
			{
				mainInfo.id[i].eid_96_send_flag = 0;
				return i+1;
			}		
	}
	
	return 0;
}
