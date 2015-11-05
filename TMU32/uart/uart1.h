#ifndef _UART_H_
#define _UART_H_

#define UART_DEVICE_SYN1       0x0FF                //ͬ���ֽ�
#define UART_DEVICE_SYN2       0x0A6                //ͬ���ֽ�
#define RET_TYPE            0x60                 //��������
#define UART_BUFFER_LENGTH      240                  /*�����շ���������С*/ 
#define UART_RX_FRAME_OK        5      /*���յ�1֡�ı�־*/ 
#define UART_TX_FRAME_DELAY     11      /*����֮֡��ļ������λ50ms������֡���100~150ms*/
#define FRAME_DEVICE_FIX_BYTES  5       /*�豸֡�̶��ֽ�(����������)*/ 

/*ȫ˫���������ݽṹ*/
typedef struct 
{
    unsigned char txLen;
    unsigned char rxLen;
    unsigned char *txPtr;                    //����ָ�����ַ���tx����
    unsigned char *rxPtr;                    //����ָ������ַ���rx����
    unsigned char uartState;                 //��־���ν���˳��
    
    unsigned char *txData;
    unsigned char *rxData;
    unsigned char rxBuffer[UART_BUFFER_LENGTH];

    unsigned char ackLen;
    unsigned char ackBuffer[UART_BUFFER_LENGTH];         /*Ӧ����BUFFER*/

    unsigned char rxTimer;                  /*���������ֽڼ䶨ʱ��*/
    unsigned char txTimer;                  /*�������ݰ�������ƶ�ʱ��*/   

}UART_INFO;


extern UART_INFO uart1;   /*UART0 serial struct*/

extern void uart1_init(void);
extern void uart1_timeout(void);
extern void uart1_trans_enable(void);
extern void down_tx_enable(void);
extern void down_tx_disable(void);
extern void uart1_rcv_enable(void);

#endif /*_UART_H_*/
