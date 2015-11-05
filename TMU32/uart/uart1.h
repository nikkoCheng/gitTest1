#ifndef _UART_H_
#define _UART_H_

#define UART_DEVICE_SYN1       0x0FF                //同步字节
#define UART_DEVICE_SYN2       0x0A6                //同步字节
#define RET_TYPE            0x60                 //返回类型
#define UART_BUFFER_LENGTH      240                  /*串口收发缓冲器大小*/ 
#define UART_RX_FRAME_OK        5      /*接收到1帧的标志*/ 
#define UART_TX_FRAME_DELAY     11      /*发送帧之间的间隔，单位50ms，发送帧间隔100~150ms*/
#define FRAME_DEVICE_FIX_BYTES  5       /*设备帧固定字节(不含数据体)*/ 

/*全双工串口数据结构*/
typedef struct 
{
    unsigned char txLen;
    unsigned char rxLen;
    unsigned char *txPtr;                    //用于指向发送字符串tx数组
    unsigned char *rxPtr;                    //用于指向接收字符串rx数组
    unsigned char uartState;                 //标志依次接收顺序
    
    unsigned char *txData;
    unsigned char *rxData;
    unsigned char rxBuffer[UART_BUFFER_LENGTH];

    unsigned char ackLen;
    unsigned char ackBuffer[UART_BUFFER_LENGTH];         /*应答发送BUFFER*/

    unsigned char rxTimer;                  /*接收数据字节间定时器*/
    unsigned char txTimer;                  /*发送数据包间隔控制定时器*/   

}UART_INFO;


extern UART_INFO uart1;   /*UART0 serial struct*/

extern void uart1_init(void);
extern void uart1_timeout(void);
extern void uart1_trans_enable(void);
extern void down_tx_enable(void);
extern void down_tx_disable(void);
extern void uart1_rcv_enable(void);

#endif /*_UART_H_*/
