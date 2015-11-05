#ifndef __TMUPROCESS_H__
#define __TMUPROCESS_H__

#define DYNEMIC_TMU_FLAG 1
#define STATIC_TMU_FLAG  0

extern void tmu_address_init(void);
extern void set_tmu_addr_mode(unsigned char value);
extern unsigned char get_tmu_addr_mode(void);

#endif
