
#include "typeDef.h"
#include "variable.h"
#include "share.h"
#include "netCommand.h"

MAIN_INFO mainInfo;
unsigned char linkFmu;
    

void var_init(void)
{
    static unsigned char i;

    memory_clear((char*)&mainInfo, sizeof(mainInfo));
    slot_aging_time_init(1);

    for(i=0; i<PORT_ALL_NUM; i++)
    {
        mainInfo.ledState[i] = LED_STATE_OFF;
    }

    linkFmu =0;
    return;
}
