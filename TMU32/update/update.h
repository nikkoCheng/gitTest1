#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "typeDef.h"


/**
 * @brief ���´�����
 *        ����������ʱ��ȡ���±�ʶλ�������Ҫ���°�������Ϣ��
 *        ����ԭ�г��򣬶�ȡFlash��APPӦ�ó���HEX�ļ���д�뵽Ƭ��Flash�ϣ�
 *        �����ת��0xDE000��APP������ڵ�ַ��
 */
extern void update_file(void);
extern void fmu_cmd_init();
extern void fmu_cmd_update_file_timer_count(void);


extern void earseCodeFlash(unsigned int address);
extern void writeCodeFlash(unsigned int address, unsigned char value);
extern void writeInnerFlash(unsigned int addr, unsigned char length, unsigned char* buf);

#endif
