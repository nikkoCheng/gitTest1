#ifndef __IDPROCESS_H__
#define __IDPROCESS_H__

#if 0
#define ID_TYPE_JUMPER          1
#define ID_TYPE_PIGTAIL         2
#define ID_TYPE_CABLE           3
#define ID_TYPE_SPLITER_ALL     4
#define ID_TYPE_SPLITER_2       5
#define ID_TYPE_SPLITER_4       6
#define ID_TYPE_SPLITER_8       7
#define ID_TYPE_SPLITER_16      8
#define ID_TYPE_SPLITER_32      9
#define ID_TYPE_SPLITER_64      10
#define ID_TYPE_SPLITER_128     11
#endif

enum ENUM_ID_TYPE
{
    ID_TYPE_ALL = 0,
    ID_TYPE_JUMPER = 1,
    ID_TYPE_PIGTAIL = 2,
    ID_TYPE_CABLE = 3,
    ID_TYPE_SPLITER_ALL = 4,
    ID_TYPE_SPLITER_2  = 5,
    ID_TYPE_SPLITER_4  = 6,
    ID_TYPE_SPLITER_8  = 7,
    ID_TYPE_SPLITER_16  = 8,
    ID_TYPE_SPLITER_32  = 9,
    ID_TYPE_SPLITER_64  = 10,
    ID_TYPE_SPLITER_128 = 11,
	ID_TYPE_NO_COUPLE = 0X7F
};


extern unsigned char localid_buf[12][32];

extern unsigned char id_check_null(unsigned char *ptr);
extern void refresh_current_id(void);
extern void read_id(unsigned char type,unsigned char len,unsigned char* buf);
#endif
