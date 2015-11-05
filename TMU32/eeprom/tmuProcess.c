#include "c8051F340.h"
#include "typedef.h"
#include "ledStatus.h"
#include "idProcess.h"
#include "tmuProcess.h"
#include "share.h"
#include "netCommand.h"
#include "delay.h"

static unsigned char tmuAddrMode;

void tmu_address_init(void)
{
    unsigned char sch_version;
    unsigned char tempValue;
    unsigned char i;
    tempValue = 0;
    sch_version  = 0;
    for(i=0;i<5;i++)
    {
        delay_ms(10);
        tempValue = ((P0&0x80)>>7);
        tempValue |= ((P1&0x40)>>5);
        tempValue |= ((P1&0x80)>>5);
        sch_version = tempValue;
    }
    if(DYNEMIC_TMU_FLAG == sch_version)
    {
        set_tmu_addr_mode(DYNEMIC_TMU_FLAG);
    }
    else
    {
     		slot_address_init();
        set_tmu_addr_mode(STATIC_TMU_FLAG);
    }
}

void set_tmu_addr_mode(unsigned char value)
{
    tmuAddrMode = value;
}

unsigned char get_tmu_addr_mode(void)
{
    return tmuAddrMode;
}
