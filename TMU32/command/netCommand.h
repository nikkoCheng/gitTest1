#ifndef __NETCOMMAND_H__
#define __NETCOMMAND_H__

#define CPU_RUN_LED_1  (P4 |= 0x40)
#define CPU_RUN_LED_0  (P4 &= ~0x40)

extern unsigned char port_num;

extern void slot_address_init(void);
extern unsigned char uart_fcs(unsigned char *ptr, unsigned char len);
extern void uart_process(void);

extern void slot_aging_time_init(unsigned char value);
extern void slot_aging_time(void);

extern void port_state_process(void);
extern void uart1_wr_label(void);
extern void uart1_rd_label(void);
extern void read_id_96(void);
extern unsigned char port_status_1_port_process(unsigned char lPortTmp);
extern unsigned char inquire_port_change(void);
extern unsigned char inquire_port_96(void);
#endif
