#include "c8051F340.h"
#include "typedef.h"
#include "mcuInit.h"
#include "timer.h"


/************************/
void Soft_WDT_Enable(void)
{
    PCA0MD |= 0x60;
}

void Soft_WDT_Disnable(void)
{
    PCA0MD &= (~0x40);
}

void WatchDog(void)
{
    PCA0CPH4 = 0x00;
    Soft_WDT_Enable();
}

void WatchDog_Init(void)
{
    PCA0MD &= 0xE0;
    PCA0MD |= 0x80;

    CR = 1;

    PCA0CPL4 |= 0xFF;

    WatchDog();
}


/************************/


void Port_IO_Init(void)
{
#ifdef DEFINE_LEFT_TAPE
	P0MDIN  |= 0xF0;
	P0MDOUT &= ~0x10;	//P0.4������Ϊ©����·
	P0MDOUT |= 0x40;	//P0.6 ����Ϊ����

	P1MDIN  |= 0xFF;
	P1MDOUT &= ~0x3F;
	
	P2MDIN  |= 0x20;
	P2MDIN &= ~0x03;	//P2.0 P2.1
	P2MDOUT &= ~0x58;

	P3MDIN  |= 0x3F;
	P3MDOUT &= ~0x3F;
	P4MDIN  = ~0x00;

	P0SKIP = 0x8F;
	P1SKIP = 0x7F;
	P2SKIP = 0x03;	

	CPT0CN = 0x00;	//�Ƚ�����ֹ
	CPT0MX = 0x00;	//�Ƚ���P2.1 P2.0
	CPT0MD = 0x00;	//�����Ӧ
#else
  P0MDOUT = 0x00;     //P0.1,P0.3,P0.7 is OC 
	P1MDOUT = 0;        //P1.1,P1.3,P1.5,P1.7 is OC
	P2MDOUT = 0;        //P2.1,P2.3,P2.5,P2.7 is OC
	P3MDOUT = 0x6;      //P3.1,P3.2��SCL���趨��Ϊ���
#endif

    XBR0 = 0;
  	XBR1 = 0;
		XBR0 |= 0x20; //�Ƚ���0�����˿�����
  	UART0EN_1;		//����0:tx,rx�����˿�����
		XBARE_1;           
		
}

/*sysClock initialization 16M*/
void  SysClk_Init(void)
{
	

  OSCLCN |= 0x83;  //�ڲ���������ʹ��
	OSCICN = 0xC3;		//12M
	CLKMUL |= 0x80;
	CLKMUL &= 0xFC;
	CLKMUL |= 0xc0;
	while(~CLKMUL&0x20);
	CLKSEL |=  0x02;
	
}

void Voltage_Regulator_Init(void)
{
    REG01CN |= 0x80;//��ѹ����ֹ
}

void mcu_f340_init(void)
{
	Soft_WDT_Disnable();
 	SysClk_Init();
	Port_IO_Init();
	Timer_Init();  
	Soft_WDT_Disnable();
//    Voltage_Regulator_Init();//��ѹ����ֹ
    
}






