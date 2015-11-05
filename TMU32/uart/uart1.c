#include "c8051f340.h"
#include "typeDef.h"
#include "timer.h"
#include "uart1.h"

UART_INFO uart1;

void uart1_init(void)
{
    /*initialize serial port*/
#ifdef UART_PCON
    PCON |= 0x80;
#endif

    uart1.uartState = 0;
    uart1.rxLen = 0;
    uart1.txLen = 0;
    uart1.txPtr = uart1.ackBuffer;

    SCON0  = SCON_INIT;
    TMOD = (TMOD & 0x0F) | TIME1_INIT;
    TH1   = 0xE6; //24M频率E6,12M要改为F3 
    ES0 = 1;        /*允许串口中断*/
    TR1 = 1;        /*波特率发生器工作*/   
    REN0 = 1;        /*允许接收数据*/
}


/*********************************
*
*每10ms在定时器中断中调用一次。
*********************************/
void uart1_timeout(void) using 1
{
	if( uart1.rxTimer )
        uart1.rxTimer --;

	if( (uart1.rxTimer==0) && (uart1.uartState != UART_RX_FRAME_OK) )
    {
        uart1.uartState = 0;
        uart1.rxLen = 0;
        REN0 = 1;
    }

    if( uart1.txTimer )
        uart1.txTimer --;

}

void uart1_rcv_enable(void)
{
    REN0 = 1;
}

/*串口发送数据*/
void uart1_trans_enable(void)
{
    uart1.txTimer = UART_TX_FRAME_DELAY;
    TI0 = 1;//发送中断标志
}


/*串口接收中断函数*/
void ISRuart(void) interrupt 4 using 3
/*设备与设备通信串口中断服务程序*/
{    
    unsigned char uart1Tmp;

    if(RI0)//RI为接收中断位，1接收，0不接收
    {
        RI0 = 0;
		uart1.rxTimer = uart1.rxBuffer;
		uart1Tmp = SBUF0;

        switch ( uart1.uartState )
        {
            case 0:                       /*SYN1*/
                if( uart1Tmp == UART_DEVICE_SYN1 )
                {
                    uart1.uartState = 1;
                    uart1.rxPtr = uart1.rxBuffer;
                    *uart1.rxPtr ++ = uart1Tmp;

                    uart1.txData = uart1.ackBuffer + 5;//+5是到数据体？
                    uart1.rxData = uart1.rxBuffer  + 5;

                }
                break;
    
            case 1:                     /*SYN2*/
                if( uart1Tmp == UART_DEVICE_SYN2 )
                {
                    uart1.uartState ++;
                    *uart1.rxPtr ++ = uart1Tmp;
                }
                else if( uart1Tmp == UART_DEVICE_SYN1 )
                {
                    uart1.uartState = 1;
                    uart1.rxPtr = uart1.rxBuffer;
                    *uart1.rxPtr ++ = uart1Tmp;
                }
                else
                   uart1.uartState = 0;
                break;
    
            case 2:                                       /*同步字3:0xFF*/
                

                *uart1.rxPtr ++ = uart1Tmp;
                uart1.uartState ++;

                break;
    
            case 3:                                       /*LENGTH*/
                uart1.uartState ++;
                *uart1.rxPtr ++ = uart1Tmp;
                uart1.rxLen = ( uart1Tmp ) - 4;   /*uart1.rxLen = (SBUF - 4) & 0x1F;*/
                break;
    
            case 4:                              /*type,date and FCS*/
                *uart1.rxPtr ++ = uart1Tmp;
                uart1.rxLen --;
                if( uart1.rxLen == 0 )
                {                              /*the last byte "FCS"*/
                    REN0 = 0;                     /*收到一帧数据，禁止再接收*/
                    uart1.uartState = UART_RX_FRAME_OK;
                } 
                break; 

            default:
                if( uart1.uartState != UART_RX_FRAME_OK)
                    uart1.uartState = 0;
                break;            
        }
    }

    if(TI0)
    {
        TI0 = 0;

        if( uart1.txLen != 0 )
        {
            SBUF0 = *uart1.txPtr;
            uart1.txPtr ++;
            uart1.txLen --;
        }
        else
            uart1.txTimer = UART_TX_FRAME_DELAY;
    }
}

void down_tx_enable(void)
{
	CPT0CN = 0x80;	//比较器使能
}

void down_tx_disable(void)
{
	CPT0CN = 0x00;	//比较器禁止
}
