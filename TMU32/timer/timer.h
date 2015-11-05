/***********************************************************
*  mcuInit.h
*
*
*  xhb   2004-02-08   created it
***********************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_


#if 1
	#define OSC_CLK     10833502  //12000000   /*ÄÚ²¿Õñµ´Æ÷*/
	#define MACHINE_CLK 9028     //10000      /*OSC_CLK/1200*/
#endif


#define INTERVAL    2                 // 2  
#define TIME_RELOAD 0xFFFF - MACHINE_CLK/10 * INTERVAL
//#define TIME_RELOAD  0xF19A

/*serial port macro define*/
#define BPS_TEMP    384

#if 1
#undef UART_PCON
#else
#define  UART_PCON
#endif


#ifdef UART_PCON
#define UART_CLK_TEMP   (MACHINE_CLK+8*BPS_TEMP)/(16*BPS_TEMP)
#else
#define UART_CLK_TEMP   (MACHINE_CLK+16*BPS_TEMP)/(32*BPS_TEMP)
#endif
#define UART_CLK        0x100 - (UART_CLK_TEMP * 12 +3)





/*constant parameter declare*/

#define TIME0_INIT  0x01        /*16bits timer*/
#define TIME1_INIT  0x20        /*8bits auto-reload timer*/


#define SCON_INIT   0x50        /*8bits */

#define UART_TIMEOUT    (90 / INTERVAL)   /*80ms*/


/*configure CKCON*/
/*CKCON  :clock control register*/
/*1:  system clock devided by 1; 0: system clock devided by 12*/
#if 0
#define T4M_1	CKCON | 0x40
#define T4M_0	CKCON & 0xBF
#define T2M_1	CKCON | 0x20
#define T2M_0	CKCON & 0xDF
#define T1M_1	CKCON | 0x10
#define T1M_0	CKCON & 0xEF
#define T0M_1	CKCON | 0x08
#define T0M_0	CKCON & 0xF7
#else
#define T4M_1	1<<6
#define T4M_0	0<<6
#define T2M_1	1<<5
#define T2M_0	0<<5
#define T1M_1	1<<4
#define T1M_0	0<<4
#define T0M_1	1<<3
#define T0M_0	0<<3

#endif
#define  TIMER_CKCON_CFR	CKCON += T4M_0+T2M_0+T1M_0+T0M_0  /*12clk  0*/



/*configure TMOD*/
/*TMOD  :timer mode register*/
/*TIMER1:*/
#define  CT1_1	1<<6               /*TMOD.6 :timer1 is counter*/
#define  CT1_0	0<<6			/*timer1 is timer*/

#define  MODE0_13BIT       0			/*TMOD[5:4] :timer1 is counter*/
#define  MODE1_16BIT	1
#define  MODE2_8BITRELOAD   2
#define  MODE3_TIMER1_STOP         3
#define  MODE3_TIMER0_D8         3

#define  MODE_TIMER1_SEL	 MODE2_8BITRELOAD<<4

#define  CT0_1	1<<2
#define	CT0_0	0<<2
#define	MODE_TIMER0_SEL	MODE1_16BIT

//#define	TIMER_TMOD_CFR	TMOD = CT1_0 +CT0_0 +MODE_TIMER1_SEL +MODE_TIMER0_SEL
#define	TIMER_TMOD_CFR	TMOD = 0x21

extern bit flag10ms;
extern bit flag20ms;
extern bit flag30ms;
extern bit flag60ms;

extern void _nop_       (void);
extern void Timer_Init(void);

#endif  /*_MCUINIT_H_*/



