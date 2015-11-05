#ifndef __LEDSTATUS_H__
#define __LEDSTATUS_H__
 
#define LED_ON           0
#define LED_OFF          1

#define DOUBLE_PORT_NUM  (2*PORT_ALL_NUM)

#define TMU_MAX_NUM      20

#define LED_OFF_ALWAYS   0x01
#define LED_SLOW_FLASH   0x02
#define LED_QUICK_FLASH  0x03
#define LED_ON_ALWAYS    0x04
#define LED_AUTO_CONTROL 0x05



#ifdef DEFINE_LEFT_TAPE
/*左出纤和翻转板管脚定义*/
		#define PORT1_STATUS     P2_2
    #define PORT2_STATUS     P2_3
    #define PORT3_STATUS     P2_4
    #define PORT4_STATUS     P2_5
    #define PORT5_STATUS     P2_6
    #define PORT6_STATUS     P2_7
#else
/*右出纤管脚定义*/
    #define PORT1_STATUS     P0_0
    #define PORT2_STATUS     P0_1
    #define PORT3_STATUS     P0_2
    #define PORT4_STATUS     P0_3
    #define PORT5_STATUS     P0_6
    #define PORT6_STATUS     P0_7
    #define PORT7_STATUS     P2_4
    #define PORT8_STATUS     P2_5 
    #define PORT9_STATUS     P2_6 
    #define PORT10_STATUS    P2_7
    #define PORT11_STATUS    P3_4 
    #define PORT12_STATUS    P3_3
#endif

extern unsigned int countnum;
extern void refresh_led(void);


#endif
