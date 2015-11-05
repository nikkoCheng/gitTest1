#include "tmunum.h"
#include "mcuInit.h"
#include "update.h"


void write_tmunum_info(TMUNUM_INFO * ptr)
{
	WatchDog();
	earseCodeFlash(TMU_NUM_INFO_ADDR);
	WatchDog();
	writeInnerFlash(TMU_NUM_INFO_ADDR,sizeof(TMUNUM_INFO), ptr);	
}
void read_tmunum_info(TMUNUM_INFO * ptr)
{
	TMUNUM_INFO code * data ptrCode;
	ptrCode = (TMUNUM_INFO code *)TMU_NUM_INFO_ADDR;	
	*ptr = *ptrCode;
	WatchDog();
}
