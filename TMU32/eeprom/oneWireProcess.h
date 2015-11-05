#ifndef __ONEWIREPROCESS_H__
#define __ONEWIREPROCESS_H__

#define OW_READ_ROM            0x33
#define OW_MATCH_ROM           0x55
#define OW_SEARCH_ROM          0xF0
#define OW_SKIP_ROM            0xCC
#define OW_RESUME              0xA5
#define OW_OVERDRIVE_SKIP_ROM  0x3C
#define OW_OVERDRIVE_MATH_ROM  0x69

#define OW_WRITE_SCRATCHPAD    0x0F
#define OW_READ_SCRATCHPAD     0xAA
#define OW_COPY_SCRATCHPAD     0x55
#define OW_READ_MEMORY         0xF0

enum ONE_WIRE_STANDARD
{
    ONE_WIRE_STANDARD_SPEED  = 0,  /*标准模式*/
    ONE_WIRE_OVERDRIVE_SPEED = 1   /*高速模式*/
};

extern void one_wire_init(enum ONE_WIRE_STANDARD speed);

extern unsigned char one_wire_write_eight_byte(unsigned char cPortNum, unsigned int desAddr, unsigned char *dat);

extern unsigned char one_wire_read_memory_block( unsigned char cPortNum, int addr, unsigned char*recvPtr, unsigned char len);
extern unsigned char one_wire_read12_memory_block(int desAddr,unsigned char*recvPtr,unsigned char len);


#endif
