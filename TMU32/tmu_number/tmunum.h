/***********************************************************
*  tmunum.h
*
*
*  nikko   2015-11-02   created it
***********************************************************/

#ifndef _TMUNUM_H_
#define _TMUNUM_H_

#define TMU_NUM_INFO_ADDR        0xF400 
typedef struct
{
	unsigned char c[6];
}TMUNUM_INFO;

extern void write_tmunum_info(TMUNUM_INFO * ptr);
extern void read_tmunum_info(TMUNUM_INFO * ptr);



#endif  



