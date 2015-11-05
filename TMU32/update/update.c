#include "c8051f340.h"
#include "update.h"
#include "typedef.h"
#include "uart1.h"
#include "share.h"
#include "mcuInit.h"

#define UPDATE_INFO_ADDR        0xF000                

#define APP_START_ADDR          0x0000                
#define APP_START_ADDR1         0x7000                 
#define APP_END_ADDR1           0xE000   		        
#define APP_END_ADDR            0x7000               

#define CODE_FLASH_PAGE_SIZE    0x200     
#define PMU_FRAME_DEVICE_FIX_BYTES  0x08       /*pmu设备帧固定字节(不含数据体)*/ 

#define UPDATA_APP_FLAG   0x5A5A


typedef struct{
	unsigned int   flag;           //0x5A5A
	unsigned int   appStartAddr;  
	unsigned int   bootStartAddr;  
}UpdateInfo_t;
UpdateInfo_t data updateInfo;

#define TEMP_READ_APP_LEN  64

uint8 tempReadApp[TEMP_READ_APP_LEN];
static uint16 updatePacketNum = 1 ;			//期待的下一个包序号
static uint16 updateTotalDataLen ;
static uint8 updateTimerCount; 	/*接收升级文件时，包的时间间隔*/
static uint8 updateTimerStatus;	/*是否要计时，当收到正确的包时，置1开始计时。以确保下一个包是否在规定的时间内到达*/

UART_INFO *netUartPtr;
extern UART_INFO uart1;

void fmu_cmd_init()
{
    updatePacketNum = 1;
    updateTimerCount= 0;
    updateTimerStatus = 0;
	updatePacketNum = 0;
	updateTotalDataLen = 0;

}

void earseCodeFlash(unsigned int address)
{
    char xdata* data ptr;         // a pointer to xdata located in data space, points to the first FLASH page that will be erased

    if( address > 0xFBFF )
        return;

    ptr = (char*) address;

	EA = 0;
   FLKEY = 0xA5;
    FLKEY = 0xF1;
    //FLSCL |= 1;
//	PSCTL |= 1;
    PSCTL |= 2;
    PSCTL |= 1;
    *ptr = 0xFF;
         
    //FLSCL &= ~1;
  	PSCTL &= ~1;
    PSCTL &= ~2;
 //	EA = 1;
 //   delay_ms(1);

} 



void writeCodeFlash(unsigned int address, unsigned char value)
{
    unsigned char xdata* data ptr;         // a pointer to xdata located in data space
    if( address > 0xFBFF )
        return;
    ptr = (unsigned char*) address;

    EA = 0;
    PFE0CN &= ~1;
    //FLSCL |= 1;

    PSCTL &= ~2;
    PSCTL |= 1;

    FLKEY = 0xA5;
    FLKEY = 0xF1;
    *ptr = value;

    //FLSCL &= ~1;
    PSCTL &= ~3;
    EA = 1;//???
}


void writeInnerFlash(unsigned int addr, unsigned char length, unsigned char* buf)
{
	unsigned char data j;
    unsigned int tmpAddr;
    unsigned char *tmpPtr;

	tmpAddr = addr;
    tmpPtr = buf;
	for( j=0; j< length; j++ )
	{
        writeCodeFlash(tmpAddr, *tmpPtr);
        tmpAddr ++;
        tmpPtr ++;
    }
}



void updateApp(void)
{
    unsigned int addr;
    char *ptr;
    unsigned char i;
    
	
    updateInfo.flag = UPDATA_APP_FLAG;
    updateInfo.appStartAddr = 0;
 //   updateInfo.bootStartAddr = 0;
 //   updateInfo.appPackLen = 0;


    EA = 0;
	//擦除扇区
    earseCodeFlash(UPDATE_INFO_ADDR);

    addr = UPDATE_INFO_ADDR;
    ptr = (char*)&updateInfo;
    for (i = 0; i < sizeof(UpdateInfo_t); i++,addr++,ptr++)
        writeCodeFlash(addr, *ptr);
	//软启动
	(*(void (*)( ) )0xffff)( );
//	softRst();
}
/*升级包收包时间计时*/
void fmu_cmd_update_file_timer_count(void)
{
    if(updateTimerStatus)
    {
        if(updateTimerCount++ > 10)
        {
            updatePacketNum = 1;
            updateTimerStatus = 0;
        }

		/*收到最后一个包，等待3秒进行软件升级*/
        if(updateTotalDataLen != 0 && updateTimerCount > 3)
        {
            updateApp();
            updateTotalDataLen = 0;
            updateTimerStatus = 0;
        }
    }
    else
    {
        updateTimerCount = 0;
    }
}

/*处理升级文件*/
/****************************************************
*【函数名】update_file
*【功     能】处理tmu升级文件
*【输     入】无
                            
*【输     出】无
*【作     者】陈杰
*【日     期】2014-1-11
*****************************************************/
void update_file(void)
{
	unsigned int data appStartAddr = 0;
 	unsigned int  addr;
	unsigned char *DataPart;
    unsigned char *buf_ptr;
	uint16 packetNumTmp;
 	updateTimerStatus = 1;
	DataPart	 = uart1.rxData + 1;	//消息类型和命令码
	buf_ptr = uart1.ackBuffer + 4;		//帧头数据偏移量为4
	packetNumTmp = DataPart[0]+(DataPart[1]<<8);
	memory_copy(tempReadApp,DataPart + 2,TEMP_READ_APP_LEN);
	*buf_ptr = RET_TYPE;
	buf_ptr++;
	*buf_ptr = 0x45;
	buf_ptr++;
	uart1.txLen = 9;
	if(updatePacketNum == packetNumTmp && packetNumTmp != 0xFFFF)
	{
		if((updatePacketNum %8) == 1)
		{	
			addr = (updatePacketNum-1)*64+APP_START_ADDR1;
			earseCodeFlash(addr);
			WatchDog();			//喂狗
			
		}
	  	writeInnerFlash((64*(packetNumTmp-1)+APP_START_ADDR1),TEMP_READ_APP_LEN,tempReadApp);
		updatePacketNum ++;
        *buf_ptr++ = (uint8)updatePacketNum;
        *buf_ptr++ = (uint8)(updatePacketNum >> 8);
	}else if(0xFFFF == packetNumTmp && updatePacketNum != 1)	//最后一帧数据
	{
		if((updatePacketNum %8) == 1)
		{	
			addr = (updatePacketNum-1)*64+APP_START_ADDR1;
			earseCodeFlash(addr);
			WatchDog();			//喂狗	
		}
		writeInnerFlash((TEMP_READ_APP_LEN*(updatePacketNum-1)+APP_START_ADDR1),TEMP_READ_APP_LEN,tempReadApp);
		*buf_ptr = 0xFF;
		buf_ptr++;
		*buf_ptr = 0xFF;
		buf_ptr++;	
		updateTotalDataLen = TEMP_READ_APP_LEN * updatePacketNum;
		updatePacketNum = 1;
		updateTimerStatus = 1;
	}else
	{
		updateTimerStatus = 0;
        *buf_ptr = (uint8)updatePacketNum;
		buf_ptr++;
        *buf_ptr = (uint8)(updatePacketNum >> 8);
		buf_ptr++;
	}
	updateTimerCount= 0;
}
 






 

 

 





