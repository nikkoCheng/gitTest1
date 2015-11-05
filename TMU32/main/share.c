#include "share.h"

unsigned char memory_compare(unsigned char *ptr1, unsigned char *ptr2, unsigned char len)
{
    unsigned char i;

    for(i=0; i<len; i++)
    {
        if( *ptr1 == *ptr2 )
        {
            ptr1 ++;
            ptr2 ++;
        }
        else
				{
            return ERROR;
				}
    }
    return OK;
}

void memory_clear(char *ptr, unsigned int number)
{
    unsigned int count;    
    char *ramPtr;   

    count = number;

    ramPtr = ptr;

    do
    {
        *ramPtr++ = 0;
        count --;
    }while( count!=0 );
}

void memory_copy(unsigned char  *dest, unsigned char *src, int len)
{
		while(len--)
		{
			*dest++ = *src++;
		}
}