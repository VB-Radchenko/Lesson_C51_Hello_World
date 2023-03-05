#ifndef __T5LOS8051_H__
#define __T5LOS8051_H__


sfr	P0		=	0x80;		/********PO��*******/
sfr	SP		=	0x81;		/********��ջָ��*******/
sfr DPL		=	0x82;		/********DPTR����ָ��*******/
sfr DPH		=	0x83;		/********DPTR����ָ��*******/
sfr PCON	=	0x87;		/********.7 UART2����������*******/
sfr TCON	=	0x88;		/********T0 T1���ƼĴ���*******/

sbit	TF1 =	TCON^7;		/********T1�жϴ���*******/
sbit	TR1	=	TCON^6;		
sbit	TF0	=	TCON^5;		/********T0�жϴ���*******/
sbit	TR0	=	TCON^4;
sbit	IE1	=	TCON^3;		/********�ⲿ�ж�1*******/
sbit	IT1	=	TCON^2;		/********�ⲿ�ж�1������ʽ	0���͵�ƽ����		1���½��ش���*******/
sbit	IE0	=	TCON^1;		/********�ⲿ�ж�0*******/
sbit	IT0	=	TCON^0;		/********�ⲿ�ж�0������ʽ	0���͵�ƽ����		1���½��ش���*******/

sfr	TMOD	=	0x89;		/********T0 T1ģʽѡ��,ͬ8051*******/
sfr	TH0 	=	0x8C;		
sfr TL0 	=	0x8A;
sfr TH1 	=	0x8D;
sfr TL1 	=	0x8B;

sfr CKCON	=	0x8E;		/********CPU����*******/
sfr	P1		=	0x90;
sfr	DPC		=	0x93;		/********MOVXָ���DPTR�ı仯ģʽ	0������ 1��+1  2��-1*******/
sfr PAGESEL	=	0x94;		/********������0x01*******/
sfr	D_PAGESEL	=	0x95;	/********������0x02*******/

sfr SCON0	=	0x98;		/********UART2���ƽӿڣ�ͬ8051*******/
sbit	TI0	=	SCON0^1;
sbit	RI0	=	SCON0^0;
sfr	SBUF0	=	0x99;		/********UART2�շ����ݽӿ�*******/
sfr	SREL0H	=	0xBA;		/********���ò����ʣ���ADCONΪ0x80ʱ*******/
sfr	SREL0L	=	0xAA;

sfr	SCON1	=	0x9B;		/********UART3���ƽӿ�*******/
sfr	SBUF1	=	0x9C;
sfr	SREL1H	=	0xBB;
sfr	SREL1L	=	0x9D;

sfr	IEN2	=	0x9A;		/********�ж�ʹ�ܿ�����SFR  .7~.1����д0		.0 UART3�ж�ʹ�ܿ���λ*******/
sfr	P2		=	0xA0;
sfr	IEN0	=	0xA8;		/********�ж�ʹ�ܿ�����0*******/
sbit	EA	=	IEN0^7;		/********�ж��ܿ���λ*******/
sbit	ET2	=	IEN0^5;		/********��ʱ��2�жϿ���λ*******/
sbit	ES0	=	IEN0^4;		/********UART2*******/
sbit	ET1	=	IEN0^3;		/********T1*******/
sbit	EX1	=	IEN0^2;		/********�ⲿ�ж�1*******/
sbit	ET0	=	IEN0^1;		/********T0*******/
sbit	EX0	=	IEN0^0;		/********�ⲿ�ж�0*******/

sfr	IP0		=	0xA9;				/********�ж����ȼ�������0*******/
sfr	P3		=	0xB0;
sfr	IEN1	=	0xB8;				/********�ж�ʹ�ܽ��ܿ�����******/
sbit	ES3R	=	IEN1^5;			/*****UART5�����ж�ʹ�ܿ���λ****/
sbit	ES3T	=	IEN1^4;			/*****UART5�����ж�ʹ�ܿ���λ****/
sbit	ES2R	=	IEN1^3;			/*****UART4�����ж�ʹ�ܿ���λ****/
sbit	ES2T	=	IEN1^2;			/*****UART4�����ж�ʹ�ܿ���λ****/
sbit	ECAN	=	IEN1^1;			/********CAN�ж�ʹ�ܿ���λ******/

sfr	IP1		=	0xB9;				/********�ж����ȼ�������0*******/
sfr	IRCON2	=	0xBF;
sfr	IRCON 	=	0xC0;
sbit	TF2	=	IRCON^6;			/********T2�жϴ�����־*******/
sfr	T2CON	=	0xC8;				/********T2���ƼĴ���********/
sbit	TR2	=	T2CON^0;			/***********T2ʹ��***********/
sfr	TRL2H	=	0xCB;
sfr	TRL2L	=	0xCA;
sfr	TH2 	=	0xCD;
sfr	TL2 	=	0xCC;

sfr	PSW		=	0xD0;
sbit	CY	=	PSW^7;
sbit	AC	=	PSW^6;
sbit	F0	=	PSW^5;
sbit	RS1	=	PSW^4;
sbit	RS0	=	PSW^3;
sbit	OV	=	PSW^2;
sbit	F1	=	PSW^1;
sbit	P	=	PSW^0;
sfr	ADCON	=	0xD8;
sfr	ACC		=	0xE0;
sfr	B 		=	0xF0;

/******Ӳ����չ����*********/
/******DGUS�����洢������*********/
sfr	RAMMODE	=	0xF8;				/******DGUS�����洢�����ʽӿڿ��ƼĴ���*********/
sbit	APP_REQ	=	RAMMODE^7;
sbit	APP_EN	=	RAMMODE^6;
sbit	APP_RW	=	RAMMODE^5;
sbit	APP_ACK	=	RAMMODE^4;
sfr ADR_H	=	0xF1;
sfr ADR_M	=	0xF2;
sfr ADR_L	=	0xF3;
sfr ADR_INC	=	0xF4;
sfr DATA3	=	0xFA;
sfr DATA2	=	0xFB;
sfr DATA1	=	0xFC;
sfr DATA0	=	0xFD;


//UART4
sfr	SCON2T	=	0x96;					/******UART4���Ϳ���********/
sfr	SCON2R	=	0x97;					/******UART4���տ���*********/
sfr	BODE2_DIV_H	=	0xD9;				/******����������********/
sfr	BODE2_DIV_L	=	0xE7;
sfr	SBUF2_TX	=	0x9E;				/******UART4�������ݽӿ�********/
sfr	SBUF2_RX	=	0x9F;				/******UART4�������ݽӿ�*********/

//UART5
sfr	SCON3T	=	0xA7;					
sfr	SCON3R	=	0xAB;
sfr	BODE3_DIV_H	=	0xAE;
sfr	BODE3_DIV_L	=	0xAF;
sfr	SBUF3_TX	=	0xAC;
sfr	SBUF3_RX	=	0xAD;

//CANͨ��
sfr	CAN_CR	=	0x8F;
sfr	CAN_IR	=	0x91;
sfr	CAN_ET	=	0xE8;

//GPIO
sfr	P0MDOUT	=	0xB7;
sfr	P1MDOUT	=	0xBC;
sfr	P2MDOUT	=	0xBD;
sfr	P3MDOUT	=	0xBE;
sfr	MUX_SEL	=	0xC9;
sfr	PORTDRV	=	0xF9;				/******�������ǿ��*********/

//MAC&DIV
sfr	MAC_MODE	=	0xE5;
sfr	DIV_MODE	=	0xE6;

//SFR��չ�ӿ�
sfr	EXADR	=	0xFE;
sfr	EXDATA	=	0xFF;





#endif


