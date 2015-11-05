
#ifndef  _MCUINIT_H_
#define  _MCUINIT_H_


/*port0 外设配置*/
#define	SMBOEN			0
#define	SPI0EN			1
#define	UART0EN		2

#define  SMB0EN_1		XBR0 |= 0x1     /*SMBUS(I2C) 使能 */
#define  SMB0EN_0		XBR0 &= ~0x1E   /* SMBUS(I2C) 禁止*/
#define  SPI0EN_0		XBR0 &= ~0x2    /*SPI 禁止 */
#define  SPI0EN_1		XBR0 |= 0x2     /*SPI 使能 */
#define  UART0EN_1		XBR0 |= 0x1     /*UART0 使能 */
#define  UART0EN_0		XBR0 &= ~0x1    /* UART0 禁止*/


#define  XBARE_0        XBR1 &= ~0x40   //交叉开关禁止
#define  XBARE_1        XBR1 |= 0x40   //交叉开关使能

/*define  OSCICN*/
#define  IFC1_0_2M		OSCICN |0           
#define  IFC1_0_4M		OSCICN |1           
#define  IFC1_0_8M		OSCICN |2           
#define  IFC1_0_16M		OSCICN |3           
#define  MSCLKE_0		OSCICN&0x7F
#define  MSCLKE_1		OSCICN | 0x80
#define  CLKSL_0_INT		OSCICN & 0xF7
#define  CLKSL_1_EXT		OSCICN | 0x08


/* define OSCXCN*/
#define OSCCLK			24500000
#if(OSCCLK > 6700000)
	#define XFCN2_0		OSCXCN | 7
#elif(OSCCLK > 2200000)
	#define XFCN2_0		OSCXCN | 6
#elif(OSCCLK > 720000)
	#define XFCN2_0		OSCXCN | 5
#elif(OSCCLK > 270000)
	#define XFCN2_0		OSCXCN | 4
#elif(OSCCLK > 95000)
	#define XFCN2_0		OSCXCN | 3
#elif(OSCCLK > 30000)
	#define XFCN2_0		OSCXCN | 2
#elif(OSCCLK > 12000)
	#define XFCN2_0		OSCXCN | 1
#else 
	#define XFCN2_0		OSCXCN | 0
#endif


#define XTLVLD                  OSCXCN &0x80	


extern void Soft_WDT_Enable(void);
extern void Soft_WDT_Disnable(void);
extern void WatchDog(void);
extern void WatchDog_Init(void);
extern void mcu_f340_init(void);

#endif

