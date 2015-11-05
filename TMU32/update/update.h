#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "typeDef.h"


/**
 * @brief 更新处理汉，
 *        程序在启动时读取更新标识位，如果需要更新按更新信息，
 *        擦除原有程序，读取Flash中APP应用程序HEX文件，写入到片内Flash上，
 *        最后跳转到0xDE000（APP启动入口地址）
 */
extern void update_file(void);
extern void fmu_cmd_init();
extern void fmu_cmd_update_file_timer_count(void);


extern void earseCodeFlash(unsigned int address);
extern void writeCodeFlash(unsigned int address, unsigned char value);
extern void writeInnerFlash(unsigned int addr, unsigned char length, unsigned char* buf);

#endif
