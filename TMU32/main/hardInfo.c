#include "hardInfo.h"
#include "tmuProcess.h"



#define DEVICE_TRAP_STATIC        0x11      /*智能ODN盘管理器0x11固定地址0x10动态地址*/
#define DEVICE_TRAP_DYNEMIC       0x10      /*智能ODN盘管理器0x11固定地址0x10动态地址*/
#define HARD_VERSION              0x80
#define SOFT_VERSION              0x13      //add by lee on 2014-07-31

static char deviceTable[] 
                        = {
                            DEVICE_TRAP_STATIC,
                            HARD_VERSION,       /*硬件版本*///注:10:310,40:340,80:380
                            SOFT_VERSION, /*软件版本*/
                            0x14,       /*年*/
                            0x04,       /*月*/
                            0x15,       /*日*/
                            "HM-TRAP"   /*设备型号,最多15字节*/
                          };


void get_device_info(void *ptr)
{
    static char *des, *src;
    unsigned char temp;
    temp = get_tmu_addr_mode();
    des  = ptr;
    src  = &deviceTable;
    if(DYNEMIC_TMU_FLAG == temp)//本来是静态的，如果盘的地址模式是动态的，
		{													//就把动态的型号赋值给device Table的第一个字节，其他的不变
        *src = DEVICE_TRAP_DYNEMIC;
    }
    while(*src)
		{
        *des ++ = *src ++;
		}
}


