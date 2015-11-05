#ifndef  _DEFINE_H_
#define  _DEFINE_H_

#if 0
		#define ID_USB                  /*标签采用USB接口的24C02*/
		#define DEFINE_LEFT_TAPE        /*左出纤的管脚定义*/
#else
		#define ID_ONE_WIRE              /*标签采用单总线的DS2431*/
		#define DEFINE_LEFT_TAPE        /*左出纤的管脚定义*/
#endif


typedef unsigned char  uint8; 
typedef unsigned int   uint16;
typedef unsigned long  uint32;

typedef unsigned char U8;
typedef unsigned int   U16;
typedef unsigned long U32;

typedef signed char  S8;
typedef signed int   S16;
typedef signed long  S32;


union myunion
{                   /*定义一个联合*/ 
    signed int whole; 
    struct         /*在联合中定义一个结构*/
    {              
        signed char highByte; 
        signed char lowByte; 
    }half; 
}; 

#define PORT_ALL_NUM     12     /*端口数量*/

//////////////////////////////////////////////////
#define PORT_NULL    0
#define PORT_OK      1
#define PORT_ERROR   2
#define PORT_TEMP    3
///////////////////////////////////////////////////


enum ENUM_ALARM_DATA_STATUS
{
    ENUM_ALARM_DATA_STATUS_NULL_OK    = 0x11,       /*端口无标签正常*/
    ENUM_ALARM_DATA_STATUS_NULL_ERR   = 0x01,       /*端口无标签异常*/
    ENUM_ALARM_DATA_STATUS_ID_OK      = 0x12,       /*端口有标签正常*/
    ENUM_ALARM_DATA_STATUS_ID_ERR     = 0x02,        /*端口有标签异常*/
		ENUM_ALARM_DATA_STATUS_ID_WRONG   = 0x07
	};


#define LED_STATE_OFF           1
#define LED_STATE_QUICK         2
#define LED_STATE_SLOW          3
#define LED_STATE_ON            4

enum ENUM_LED_STATUS
{
    ENUM_LED_STATUS_OFF = 1,           /*没有告警*/
	  ENUM_LED_STATUS_QUICK = 2,         
		ENUM_LED_STATUS_SLOW = 3,
    ENUM_LED_STATUS_ON = 4,            
    ENUM_LED_STATUS_AUTO = 6          /*盘管理器自动判断*/
};


/*此数据结构仅用于主程序中，不能用于中断服务程序*/



typedef struct ID_STRUCT_32
{
    unsigned char local_id[128];
		unsigned char desig_id[32];
		unsigned char eid_32_flag;
		unsigned char eid_96_flag;
		unsigned char eid_96_send_flag;
		unsigned char err_num;
		unsigned char null_num;
    unsigned char state;
}ID_STRUCT_32;


typedef struct MAIN_INFO_STRUCT
{    
    unsigned char slotAddress;         /*盘地址*/
    unsigned char slotAging;           /*2秒后地址清零*/
    unsigned char slotDelay;           /*收到地址,2秒后内允许下行发送*/
		enum ENUM_ALARM_DATA_STATUS portState[PORT_ALL_NUM];           /*端口状态*/
		enum ENUM_LED_STATUS rcvLedState[PORT_ALL_NUM];   /*接收到的架管理器LED灯状态*/
    unsigned char ledState[PORT_ALL_NUM];       /*端口灯的状态*/
    ID_STRUCT_32 id[PORT_ALL_NUM];           /*12个端口的ID*/
}MAIN_INFO;


/*50ms计数时间间隔*/
#define DEFAULT_SLOT_AGING_TIME         1500/60
#define DEFAULT_SLOT_DELAY_TIME         1560/60      /*此值必须大于DEFAULT_SLOT_AGING_TIME*/

extern void _nop_ (void);
#define NOP() _nop_()
#define NOP1()   NOP()
#define NOP2()   NOP1();NOP1()
#define NOP4()   NOP2();NOP2()
#define NOP8()   NOP4();NOP4()
#define NOP10() NOP8();NOP2()
#define NOP16() NOP8();NOP8()

#define    OK           0
#define    ERROR        1

extern MAIN_INFO mainInfo;
extern unsigned char linkFmu;

#endif
