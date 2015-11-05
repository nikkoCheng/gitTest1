#include "hardInfo.h"
#include "tmuProcess.h"



#define DEVICE_TRAP_STATIC        0x11      /*����ODN�̹�����0x11�̶���ַ0x10��̬��ַ*/
#define DEVICE_TRAP_DYNEMIC       0x10      /*����ODN�̹�����0x11�̶���ַ0x10��̬��ַ*/
#define HARD_VERSION              0x80
#define SOFT_VERSION              0x13      //add by lee on 2014-07-31

static char deviceTable[] 
                        = {
                            DEVICE_TRAP_STATIC,
                            HARD_VERSION,       /*Ӳ���汾*///ע:10:310,40:340,80:380
                            SOFT_VERSION, /*����汾*/
                            0x14,       /*��*/
                            0x04,       /*��*/
                            0x15,       /*��*/
                            "HM-TRAP"   /*�豸�ͺ�,���15�ֽ�*/
                          };


void get_device_info(void *ptr)
{
    static char *des, *src;
    unsigned char temp;
    temp = get_tmu_addr_mode();
    des  = ptr;
    src  = &deviceTable;
    if(DYNEMIC_TMU_FLAG == temp)//�����Ǿ�̬�ģ�����̵ĵ�ַģʽ�Ƕ�̬�ģ�
		{													//�ͰѶ�̬���ͺŸ�ֵ��device Table�ĵ�һ���ֽڣ������Ĳ���
        *src = DEVICE_TRAP_DYNEMIC;
    }
    while(*src)
		{
        *des ++ = *src ++;
		}
}


