#include "c8051F340.h"
#include "typedef.h"
#include "update.h"
#include "variable.h"
#include "uart1.h"
#include "mcuInit.h"
#include "ledStatus.h"
#include "idProcess.h"
#include "tmuProcess.h"
#include "netCommand.h"
#include "share.h"
#include "oneWireProcess.h"
#include "crc.h"
#include "delay.h"
#include "timer.h"
#include "tmunum.h"

void main(void)
{
		//////////////////////
			TMUNUM_INFO xx;
		TMUNUM_INFO  yy;
	///////////////////////////
    unsigned char index;   
		static unsigned char num_byte = 0;
		char y =0;
		EA = 0;
    Soft_WDT_Disnable();

    mcu_f340_init();

    delay_ms(10);

    var_init();
		fmu_cmd_init();
    uart1_init();

#ifdef ID_ONE_WIRE
    one_wire_init( ONE_WIRE_STANDARD_SPEED);
#endif

    delay_ms(10);
    tmu_address_init(); 				    
    EA = 1;     

    WatchDog_Init();

    index = 0;
		countnum = 0;
		port_num = 0;
		
		while(1)
		{		
				xx.c[0] = 2;
				xx.c[1] = 2;
				xx.c[2] = 2;
				xx.c[3] = 2;
				xx.c[4] = 2;
				xx.c[5] = 2;
				write_tmunum_info(&xx);
				read_tmunum_info(&yy);
		}
while(1)
{
	
	char x[10]={1,2,3,4,5,6,7,8,9,10};
	char temp[10];
	int i;
	unsigned char code * data ptrCode;
	x[0] = y++;
	WatchDog();
	earseCodeFlash(TMU_NUM_INFO_ADDR);
	WatchDog();
	writeInnerFlash(TMU_NUM_INFO_ADDR, 10, x);
  ptrCode = (char code *)TMU_NUM_INFO_ADDR;
	for(i = 0;i<10;i++)
	{
		temp[i] = 0;
	}
	for (i = 0; i <10; i++)
	{
		temp[i] = *ptrCode++;
	}
		WatchDog();
}

 while(1)
    { 	
        WatchDog();
        uart_process();

				if(flag30ms)
				{
					flag30ms = 0;
						if(num_byte == 0)
						{
							num_byte = 1;
							read_id(0,0,(unsigned char*)localid_buf);
							WatchDog();
						}
						else
						{
								num_byte = 0;
								read_id(0,1,(unsigned char*)localid_buf);
								WatchDog();
								refresh_current_id();
								memory_clear((unsigned char*)localid_buf,sizeof(localid_buf));
								read_id_96();
						}
				}
					
        if( flag60ms )             
        {
            flag60ms = 0;
            index ++; 
						uart1_timeout();				
					  port_state_process();
            slot_aging_time();	
			
					if(index >= 20)
					{
							index = 0;
							fmu_cmd_update_file_timer_count();
					}
        }
    }
}
