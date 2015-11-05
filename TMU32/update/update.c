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
#define PMU_FRAME_DEVICE_FIX_BYTES  0x08       /*pmu�豸֡�̶��ֽ�(����������)*/ 

#define UPDATA_APP_FLAG   0x5A5A


typedef struct{
	unsigned int   flag;           //0x5A5A
	unsigned int   appStartAddr;  
	unsigned int   bootStartAddr;  
}UpdateInfo_t;
UpdateInfo_t data updateInfo;

#define TEMP_READ_APP_LEN  64

uint8 tempReadApp[TEMP_READ_APP_LEN];
static uint16 updatePacketNum = 1 ;			//�ڴ�����һ�������
static uint16 updateTotalDataLen ;
static uint8 updateTimerCount; 	/*���������ļ�ʱ������ʱ����*/
static uint8 updateTimerStatus;	/*�Ƿ�Ҫ��ʱ�����յ���ȷ�İ�ʱ����1��ʼ��ʱ����ȷ����һ�����Ƿ��ڹ涨��ʱ���ڵ���*/

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
	//��������
    earseCodeFlash(UPDATE_INFO_ADDR);

    addr = UPDATE_INFO_ADDR;
    ptr = (char*)&updateInfo;
    for (i = 0; i < sizeof(UpdateInfo_t); i++,addr++,ptr++)
        writeCodeFlash(addr, *ptr);
	//������
	(*(void (*)( ) )0xffff)( );
//	softRst();
}
/*�������հ�ʱ���ʱ*/
void fmu_cmd_update_file_timer_count(void)
{
    if(updateTimerStatus)
    {
        if(updateTimerCount++ > 10)
        {
            updatePacketNum = 1;
            updateTimerStatus = 0;
        }

		/*�յ����һ�������ȴ�3������������*/
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

/*���������ļ�*/
/****************************************************
*����������update_file
*����     �ܡ�����tmu�����ļ�
*����     �롿��
                            
*����     ������
*����     �ߡ��½�
*����     �ڡ�2014-1-11
*****************************************************/
void update_file(void)
{
	unsigned int data appStartAddr = 0;
 	unsigned int  addr;
	unsigned char *DataPart;
    unsigned char *buf_ptr;
	uint16 packetNumTmp;
 	updateTimerStatus = 1;
	DataPart	 = uart1.rxData + 1;	//��Ϣ���ͺ�������
	buf_ptr = uart1.ackBuffer + 4;		//֡ͷ����ƫ����Ϊ4
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
			WatchDog();			//ι��
			
		}
	  	writeInnerFlash((64*(packetNumTmp-1)+APP_START_ADDR1),TEMP_READ_APP_LEN,tempReadApp);
		updatePacketNum ++;
        *buf_ptr++ = (uint8)updatePacketNum;
        *buf_ptr++ = (uint8)(updatePacketNum >> 8);
	}else if(0xFFFF == packetNumTmp && updatePacketNum != 1)	//���һ֡����
	{
		if((updatePacketNum %8) == 1)
		{	
			addr = (updatePacketNum-1)*64+APP_START_ADDR1;
			earseCodeFlash(addr);
			WatchDog();			//ι��	
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
 






 

 

 





