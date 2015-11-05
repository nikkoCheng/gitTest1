
#ifndef     _SHARE_H_
#define     _SHARE_H

#define    OK           0
#define    ERROR        1

extern void memory_clear(char *ptr, unsigned int number);
extern void memory_fill(char *ptr, unsigned int number, char value);
extern void memory_copy(unsigned char  *dest, unsigned char *src, int len);
extern unsigned char memory_compare(unsigned char *ptr1, unsigned char *ptr2, unsigned char len);

#endif
