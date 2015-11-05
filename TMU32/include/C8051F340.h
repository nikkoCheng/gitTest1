#ifndef __C8051F340_H__
#define __C8051F340_H__
/*---------------------------------------------------------------------------
;
;
;
;
; 	FILE NAME  	: C8051F340.H
; 	TARGET MCUs	: C8051F310, 'F311, 'F312, 'F313, 'F314, 'F315
; 	DESCRIPTION	: Register/bit definitions for the C8051F02x product family.
;
; 	REVISION 1.1
;
;---------------------------------------------------------------------------*/

/*  BYTE Registers  */
sfr P0       =  0x80;	/* PORT 0                                                  */
sfr SP       =  0x81;	/* STACK POINTER                                           */
sfr DPL      =  0x82;	/* DATA POINTER - LOW BYTE                                 */
sfr DPH      =  0x83;	/* DATA POINTER - HIGH BYTE                                */
sfr EMI0CF   =  0x85;   /* EXTERNAL MEMORY INTERFACE (EMIF) CONFIGURATION  		   */
sfr OSCLCN   =  0x86;   /* */

sfr PCON     =  0x87;	/* POWER CONTROL                                           */
sfr TCON     =  0x88;	/* TIMER CONTROL                                           */
sfr TMOD     =  0x89;	/* TIMER MODE                                              */
sfr TL0      =  0x8A;	/* TIMER 0 - LOW BYTE                                      */
sfr TL1      =  0x8B;	/* TIMER 1 - LOW BYTE                                      */
sfr TH0      =  0x8C;	/* TIMER 0 - HIGH BYTE                                     */
sfr TH1      =  0x8D;	/* TIMER 1 - HIGH BYTE                                     */
sfr CKCON    =  0x8E;	/* CLOCK CONTROL                                           */
sfr PSCTL    =  0x8F;	/* PROGRAM STORE R/W CONTROL                               */
sfr P1       =  0x90;	/* PORT 1                                                  */
sfr TMR3CN   =  0x91;	/* TIMER 3 CONTROL                                         */
sfr TMR3RLL  =  0x92;	/* TIMER 3 RELOAD REGISTER - LOW BYTE                      */
sfr TMR3RLH  =  0x93;	/* TIMER 3 RELOAD REGISTER - HIGH BYTE                     */
sfr TMR3L    =  0x94;	/* TIMER 3 - LOW BYTE                                      */
sfr TMR3H    =  0x95;	/* TIMER 3 - HIGH BYTE                                     */

sfr SCON0    =  0x98;	/* SERIAL PORT 0 CONTROL                                   */
sfr SBUF0    =  0x99;	/* SERIAL PORT 0 BUFFER                                    */
sfr CPT1CN   =  0x9A;	/* COMPARATOR 1 CONTROL                                    */
sfr CPT0CN   =  0x9B;	/* COMPARATOR 0 CONTROL                                    */
sfr CPT1MD   =  0x9C;
sfr CPT0MD   =  0x9D;
sfr CPT1MX   =  0x9E;
sfr CPT0MX   =  0x9F; 
 
sfr P2       =  0xA0;	/* PORT 2                                                  */
sfr SPI0CFG  =  0xA1;	/* SERIAL PERIPHERAL INTERFACE 0 CONFIGURATION             */
sfr SPI0CKR  =  0xA2;	/* SERIAL PERIPHERAL INTERFACE 0 CLOCK RATE CONTROL        */
sfr SPI0DAT  =  0xA3;	/* SERIAL PERIPHERAL INTERFACE 0 DATA                      */
sfr P0MDOUT  =  0xA4;	/* PORT 0 OUTPUT MODE CONFIGURATION                        */
sfr P1MDOUT  =  0xA5;	/* PORT 1 OUTPUT MODE CONFIGURATION                        */
sfr P2MDOUT  =  0xA6;	/* PORT 2 OUTPUT MODE CONFIGURATION                        */
sfr P3MDOUT  =  0xA7;	/* PORT 3 OUTPUT MODE CONFIGURATION                        */

sfr IE       =  0xA8;	/* INTERRUPT ENABLE                                        */
sfr CLKSEL   =  0xA9;
sfr EMI0CN   =  0xAA;   /* EXTERNAL MEMORY INTERFACE CONTROL                       */
sfr SBCON1   =  0xAC;   /* UART1BAUD CONTROL                                       */
sfr PFE0CN   =  0xAF;   /*Ô¤È¡ÒýÇæ¿ØÖÆ¼Ä´æÆ÷*/

sfr P3       =  0xB0;	/* PORT 3                                                  */
sfr OSCXCN   =  0xB1;	/* EXTERNAL OSCILLATOR CONTROL                             */
sfr OSCICN   =  0xB2;	/* INTERNAL OSCILLATOR CONTROL                             */
sfr OSCICL   =  0xB3;
sfr SBRLL1   =  0xB4;   /* UART1 BAUD CONTROL LOW                                  */
sfr SBRLH1   =  0xB5;   /* UART1 BAUD CONTROL HUIGH                                */
sfr FLSCL    =  0xB6;	/* FLASH MEMORY TIMING PRESCALER                           */
sfr FLKEY    =  0xB7;

sfr IP       =  0xB8;	/* INTERRUPT PRIORITY                                      */
sfr CLKMUL   =  0xB9;
sfr AMX0N    =  0xBA;
sfr AMX0P    =  0xBB;
sfr ADC0CF   =  0xBC;	/* ADC 0 CONFIGURATION                                     */
sfr ADC0L    =  0xBD;	/* ADC 0 DATA - LOW BYTE                                   */
sfr ADC0H    =  0xBE;	/* ADC 0 DATA - HIGH BYTE                                  */

sfr SMB0CN   =  0xC0;	/* SMBUS 0 CONTROL                                         */
sfr SMB0CF   =  0xC1;
sfr SMB0DAT  =  0xC2;	/* SMBUS 0 DATA                                            */
sfr ADC0GTL  =  0xC3;	/* ADC 0 GREATER-THAN REGISTER - LOW BYTE                  */
sfr ADC0GTH  =  0xC4;	/* ADC 0 GREATER-THAN REGISTER - HIGH BYTE                 */
sfr ADC0LTL  =  0xC5;	/* ADC 0 LESS-THAN REGISTER - LOW BYTE                     */
sfr ADC0LTH  =  0xC6;	/* ADC 0 LESS-THAN REGISTER - HIGH BYTE                    */

sfr P4       =  0xC7;
sfr TMR2CN   =  0xC8;
sfr REG01CN  =  0xC9;   /* Voltage Regulator Control Default0:Voltage Regulator Enabled*/
sfr TMR2RLL  =  0xCA;
sfr TMR2RLH  =  0xCB;
sfr TMR2L    =  0xCC;
sfr TMR2H    =  0xCD;

sfr PSW      =  0xD0;	/* PROGRAM STATUS WORD                                     */
sfr REF0CN   =  0xD1;	/* VOLTAGE REFERENCE 0 CONTROL                             */
sfr SCON1    =  0xD2;   /* SERIAL PORT 1 CONTROL                                   */
sfr SBUF1    =  0xD3;   /* SERIAL PORT 1 SBUF                                      */
sfr P0SKIP   =  0xD4;
sfr P1SKIP   =  0xD5;
sfr P2SKIP   =  0xD6;

sfr PCA0CN   =  0xD8;	/* PCA 0 COUNTER CONTROL                                   */
sfr PCA0MD   =  0xD9;	/* PCA 0 COUNTER MODE                                      */
sfr PCA0CPM0 =  0xDA;	/* CONTROL REGISTER FOR PCA 0 MODULE 0                     */
sfr PCA0CPM1 =  0xDB;	/* CONTROL REGISTER FOR PCA 0 MODULE 1                     */
sfr PCA0CPM2 =  0xDC;	/* CONTROL REGISTER FOR PCA 0 MODULE 2                     */
sfr PCA0CPM3 =  0xDD;	/* CONTROL REGISTER FOR PCA 0 MODULE 3                     */
sfr PCA0CPM4 =  0xDE;	/* CONTROL REGISTER FOR PCA 0 MODULE 4                     */

sfr ACC      =  0xE0;	/* ACCUMULATOR                                             */
sfr XBR0     =  0xE1;	/* DIGITAL CROSSBAR CONFIGURATION REGISTER 0               */
sfr XBR1     =  0xE2;	/* DIGITAL CROSSBAR CONFIGURATION REGISTER 1               */
sfr XBR2     =  0xE3;	/* DIGITAL CROSSBAR CONFIGURATION REGISTER 2               */
sfr IT01CF   =  0xE4;
sfr SMOD1    =  0xE5;
sfr EIE1     =  0xE6;	/* EXTERNAL INTERRUPT ENABLE 1                             */
sfr EIE2     =  0xE7;	/* EXTERNAL INTERRUPT ENABLE 2                             */

sfr ADC0CN   =  0xE8;	/* ADC 0 CONTROL                                           */
sfr PCA0CPL1 =	0xE9;   /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 1 - LOW BYTE  */
sfr PCA0CPH1 =	0xEA;  	/* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 1 - HIGH BYTE */
sfr PCA0CPL2 =	0xEB;   /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 2 - LOW BYTE  */
sfr PCA0CPH2 =	0xEC;  	/* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 2 - HIGH BYTE */
sfr PCA0CPL3 =	0xED;   /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 3 - LOW BYTE  */
sfr PCA0CPH3 =	0xEE;  	/* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 3 - HIGH BYTE */
sfr RSTSRC   =	0xEF;   /* RESET SOURCE                                            */

sfr B        =	0xF0;   /* B REGISTER                                              */
sfr P0MDIN   =  0xF1;
sfr P1MDIN   =  0xF2;   /* PORT 1 INPUT MODE                                       */
sfr P2MDIN   =  0xF3;
sfr P3MDIN   =  0xF4;
sfr P4MDIN   =  0xF5;
sfr EIP1     =	0xF6;   /* EXTERNAL INTERRUPT PRIORITY REGISTER 1                  */
sfr EIP2     =  0xF7;   /* */

sfr SPI0CN   =	0xF8;   /* SERIAL PERIPHERAL INTERFACE 0 CONTROL                   */
sfr PCA0L    =  0xF9;	/* PCA 0 TIMER - LOW BYTE                                  */
sfr PCA0H    =	0xFA;  	/* PCA 0 TIMER - HIGH BYTE                                 */
sfr PCA0CPL0 =	0xFB;   /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 0 - LOW BYTE  */
sfr PCA0CPH0 =	0xFC;  	/* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 0 - HIGH BYTE */
sfr PCA0CPL4 =	0xFD;   /* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 4 - LOW BYTE  */
sfr PCA0CPH4 =	0xFE;  	/* CAPTURE/COMPARE REGISTER FOR PCA 0 MODULE 4 - HIGH BYTE */
sfr VDM0CN   =  0xFF;   /* VDD¼àÊÓÆ÷¿ØÖÆ¼Ä´æÆ÷*/


/*  BIT Registers  */

/*  TCON  0x88 */
sbit TF1   = TCON ^ 7;              /* TIMER 1 OVERFLOW FLAG      */
sbit TR1   = TCON ^ 6;              /* TIMER 1 ON/OFF CONTROL     */
sbit TF0   = TCON ^ 5;              /* TIMER 0 OVERFLOW FLAG      */
sbit TR0   = TCON ^ 4;              /* TIMER 0 ON/OFF CONTROL     */
sbit IE1   = TCON ^ 3;              /* EXT. INTERRUPT 1 EDGE FLAG */
sbit IT1   = TCON ^ 2;              /* EXT. INTERRUPT 1 TYPE      */
sbit IE0   = TCON ^ 1;              /* EXT. INTERRUPT 0 EDGE FLAG */
sbit IT0   = TCON ^ 0;              /* EXT. INTERRUPT 0 TYPE      */

/*  SCON0  0x98 */
sbit SM00  = SCON0 ^ 7;             /* SERIAL MODE CONTROL BIT 0           */
sbit SM10  = SCON0 ^ 6;             /* SERIAL MODE CONTROL BIT 1           */
sbit SM20  = SCON0 ^ 5;             /* MULTIPROCESSOR COMMUNICATION ENABLE */
sbit REN0  = SCON0 ^ 4;             /* RECEIVE ENABLE                      */
sbit TB80  = SCON0 ^ 3;             /* TRANSMIT BIT 8                      */
sbit RB80  = SCON0 ^ 2;             /* RECEIVE BIT 8                       */
sbit TI0   = SCON0 ^ 1;             /* TRANSMIT INTERRUPT FLAG             */
sbit RI0   = SCON0 ^ 0;             /* RECEIVE INTERRUPT FLAG              */

/*  IE  0xA8 */
sbit EA    = IE ^ 7;                /* GLOBAL INTERRUPT ENABLE      */
sbit ET2   = IE ^ 5;                /* TIMER 2 INTERRUPT ENABLE     */
sbit ES0   = IE ^ 4;                /* UART0 INTERRUPT ENABLE       */
sbit ET1   = IE ^ 3;                /* TIMER 1 INTERRUPT ENABLE     */
sbit EX1   = IE ^ 2;                /* EXTERNAL INTERRUPT 1 ENABLE  */
sbit ET0   = IE ^ 1;                /* TIMER 0 INTERRUPT ENABLE     */
sbit EX0   = IE ^ 0;                /* EXTERNAL INTERRUPT 0 ENABLE  */

/*  IP  0xB8 */
sbit PT2   = IP ^ 5;                /* TIMER 2 PRIORITY					*/
sbit PS    = IP ^ 4;                /* SERIAL PORT PRIORITY				*/
sbit PT1   = IP ^ 3;                /* TIMER 1 PRIORITY					*/
sbit PX1   = IP ^ 2;                /* EXTERNAL INTERRUPT 1 PRIORITY	*/
sbit PT0   = IP ^ 1;                /* TIMER 0 PRIORITY					*/
sbit PX0   = IP ^ 0;                /* EXTERNAL INTERRUPT 0 PRIORITY	*/

/* SMB0CN 0xC0 */
sbit BUSY     =   SMB0CN ^ 7;       /* SMBUS 0 BUSY                    */
sbit ENSMB    =   SMB0CN ^ 6;       /* SMBUS 0 ENABLE                  */
sbit STA      =   SMB0CN ^ 5;       /* SMBUS 0 START FLAG              */
sbit STO      =   SMB0CN ^ 4;       /* SMBUS 0 STOP FLAG               */
sbit SI       =   SMB0CN ^ 3;       /* SMBUS 0 INTERRUPT PENDING FLAG  */
sbit AA       =   SMB0CN ^ 2;       /* SMBUS 0 ASSERT/ACKNOWLEDGE FLAG */
sbit SMBFTE   =   SMB0CN ^ 1;       /* SMBUS 0 FREE TIMER ENABLE       */
sbit SMBTOE   =   SMB0CN ^ 0;       /* SMBUS 0 TIMEOUT ENABLE          */

/*  TMR2CN  0xC8 */
sbit TF2H  = TMR2CN ^ 7;             /* TIMER 2 OVERFLOW HIGH BYTE FLAG */
sbit TF2L  = TMR2CN ^ 6;             /* TIMER 2 OVERFLOW LOW BYTE FLAG */
sbit TF2LEN= TMR2CN ^ 5;             /* TIMER 2 LOW BYTE INTERRUPT ENABLE BIT   */
sbit T2SPLIT= TMR2CN ^ 3;            /* TIMER 2 DOUBLE 8 BITS MODE ENABLE BIT */
sbit TR2   = TMR2CN ^ 2;             /* TIMER 2 ON/OFF CONTROL       */
sbit T2XCLK= TMR2CN ^ 0;             /* TIMER 2 EXT CLOCK SELECT    */

/*  PSW  */
sbit CY    = PSW ^ 7;               /* CARRY FLAG              */
sbit AC    = PSW ^ 6;               /* AUXILIARY CARRY FLAG    */
sbit F0    = PSW ^ 5;               /* USER FLAG 0             */
sbit RS1   = PSW ^ 4;               /* REGISTER BANK SELECT 1  */
sbit RS0   = PSW ^ 3;               /* REGISTER BANK SELECT 0  */
sbit OV    = PSW ^ 2;               /* OVERFLOW FLAG           */
sbit F1    = PSW ^ 1;               /* USER FLAG 1             */
sbit P     = PSW ^ 0;               /* ACCUMULATOR PARITY FLAG */

/* PCA0CN D8H */
sbit CF    =   PCA0CN ^ 7;          /* PCA 0 COUNTER OVERFLOW FLAG   */
sbit CR    =   PCA0CN ^ 6;          /* PCA 0 COUNTER RUN CONTROL BIT */
sbit CCF4  =   PCA0CN ^ 4;          /* PCA 0 MODULE 4 INTERRUPT FLAG */
sbit CCF3  =   PCA0CN ^ 3;          /* PCA 0 MODULE 3 INTERRUPT FLAG */
sbit CCF2  =   PCA0CN ^ 2;          /* PCA 0 MODULE 2 INTERRUPT FLAG */
sbit CCF1  =   PCA0CN ^ 1;          /* PCA 0 MODULE 1 INTERRUPT FLAG */
sbit CCF0  =   PCA0CN ^ 0;          /* PCA 0 MODULE 0 INTERRUPT FLAG */

/* ADC0CN E8H */
sbit AD0EN     =   ADC0CN ^ 7;      /* ADC 0 ENABLE                              */
sbit AD0TM     =   ADC0CN ^ 6;      /* ADC 0 TRACK MODE                          */
sbit AD0INT    =   ADC0CN ^ 5;      /* ADC 0 CONVERISION COMPLETE INTERRUPT FLAG */
sbit AD0BUSY   =   ADC0CN ^ 4;      /* ADC 0 BUSY FLAG                           */
sbit AD0CM1    =   ADC0CN ^ 3;      /* ADC 0 START OF CONVERSION MODE BIT 1      */
sbit AD0CM0    =   ADC0CN ^ 2;      /* ADC 0 START OF CONVERSION MODE BIT 0      */
sbit AD0WINT   =   ADC0CN ^ 1;      /* ADC 0 WINDOW COMPARE INTERRUPT FLAG       */
sbit AD0LJST   =   ADC0CN ^ 0;      /* ADC 0 RIGHT JUSTIFY DATA BIT              */

/* SPI0CN F8H */
sbit SPIF     =   SPI0CN ^ 7;       /* SPI 0 INTERRUPT FLAG			*/
sbit WCOL     =   SPI0CN ^ 6;       /* SPI 0 WRITE COLLISION FLAG	*/
sbit MODF     =   SPI0CN ^ 5;       /* SPI 0 MODE FAULT FLAG		*/
sbit RXOVRN   =   SPI0CN ^ 4;       /* SPI 0 RX OVERRUN FLAG		*/
sbit TXBSY    =   SPI0CN ^ 3;       /* SPI 0 TX BUSY FLAG			*/
sbit SLVSEL   =   SPI0CN ^ 2;       /* SPI 0 SLAVE SELECT			*/
sbit MSTEN    =   SPI0CN ^ 1;       /* SPI 0 MASTER ENABLE			*/
sbit SPIEN    =   SPI0CN ^ 0;       /* SPI 0 SPI ENABLE				*/




/*------------------------------------------------
P0 Bit Registers
------------------------------------------------*/
sbit P0_0 = 0x80;
sbit P0_1 = 0x81;
sbit P0_2 = 0x82;
sbit P0_3 = 0x83;
sbit P0_4 = 0x84;
sbit P0_5 = 0x85;
sbit P0_6 = 0x86;
sbit P0_7 = 0x87;



/*------------------------------------------------
P1 Bit Registers
------------------------------------------------*/
sbit P1_0 = 0x90;
sbit P1_1 = 0x91;
sbit P1_2 = 0x92;
sbit P1_3 = 0x93;
sbit P1_4 = 0x94;
sbit P1_5 = 0x95;
sbit P1_6 = 0x96;
sbit P1_7 = 0x97;




/*------------------------------------------------
P2 Bit Registers
------------------------------------------------*/
sbit P2_0 = 0xA0;
sbit P2_1 = 0xA1;
sbit P2_2 = 0xA2;
sbit P2_3 = 0xA3;
sbit P2_4 = 0xA4;
sbit P2_5 = 0xA5;
sbit P2_6 = 0xA6;
sbit P2_7 = 0xA7;

/*------------------------------------------------
P3 Bit Registers
------------------------------------------------*/
sbit P3_0 = 0xB0;
sbit P3_1 = 0xB1;
sbit P3_2 = 0xB2;
sbit P3_3 = 0xB3;
sbit P3_4 = 0xB4;
sbit P3_5 = 0xB5;
sbit P3_6 = 0xB6;
sbit P3_7 = 0xB7;

#endif
