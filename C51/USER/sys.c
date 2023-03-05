#include "sys.h"
#include "UART.h"

static idata u16 delay_tick = 0; 



void SysDelayMs(u16 ms)
{
	u16 i,j;
	for(i=0;i<ms;i++)
			for(j=0;j<3000;j++);    
}


void SysDelayUs(u16 us)
{
	u16 i,j;
	for(i=0;i<us;i++)
			for(j=0;j<5;j++);    
}





void ReadDgusVp(u16 Addr,u8* pBuf,u8 Len16)
{
    u8 len32;
	
EA = 0;							
    ADR_M = Addr >> 9;	//½«¸ß°ËÎ»¸³Öµ¸øDGUSÖÐ¼ä¼Ä´æÆ÷£¬²¢ ³ýÒÔ2 ×ª»¯Îª32½øÖÆ£¬ËùÒÔ×óÒÆ 8+1Î»
		ADR_L = Addr >> 1;	//³ýÒÔ2 ×ª»¯Îª32½øÖÆ
    ADR_INC = 1;                 //DGUS  OS´æ´¢Æ÷µØÖ·ÔÚ¶ÁÐ´ºó×Ô¶¯¼Ó1
    RAMMODE=0xAF;               //Æô¶¯¶ÁÄ£Ê½
		while(!APP_ACK);      //µÈ´ý
    if(Addr%2)       //Ê×µØÖ·ÓÐÆ«ÒÆ£¬ÐÞÕý
    {
			APP_EN=1;
			while(APP_EN); 
			*pBuf++=DATA1;
			*pBuf++=DATA0;              
			Len16--;
    }
    len32=Len16>>1;
    while(len32--)
    {
			APP_EN=1;
			while(APP_EN);       //¶ÁÐ´Ö´ÐÐ½áÊø 
			*pBuf++=DATA3;
			*pBuf++=DATA2;
			*pBuf++=DATA1;
			*pBuf++=DATA0;                          
    }   
		if(Len16%2)
		{          
			APP_EN=1;
			while(APP_EN);       //¶ÁÐ´Ö´ÐÐ½áÊø
			*pBuf++=DATA3;
			*pBuf++=DATA2;              
		} 
    RAMMODE=0x00;           //¶ÁÐ´Íê³ÉºóRAMMODE±ØÐëÖÃÁã
EA = 1;		
}



void WriteDgusVp(u16 Addr,u8* pBuf,u8 Len16)
{
    u8 len32;
EA = 0;		
    ADR_M = Addr >> 9;	//½«¸ß°ËÎ»¸³Öµ¸øDGUSÖÐ¼ä¼Ä´æÆ÷£¬²¢ ³ýÒÔ2 ×ª»¯Îª32½øÖÆ£¬ËùÒÔ×óÒÆ 8+1Î»
		ADR_L = Addr >> 1;	//³ýÒÔ2 ×ª»¯Îª32½øÖÆ
    ADR_INC = 1;                 //DGUS  OS´æ´¢Æ÷µØÖ·ÔÚ¶ÁÐ´ºó×Ô¶¯¼Ó1
    RAMMODE = 0x8F;		////Ð´²Ù×÷
		while(!APP_ACK);      //µÈ´ý
    if(Addr%2)	//ÅÐ¶ÏÊÇ·ñÎªµ¥Êý
    {
			ADR_INC=0;			//¶ÁÐ´ºó£¬µØÖ·²»¼Ó1
			RAMMODE=0xAF;		//¶Á±äÁ¿¼Ä´æÆ÷
			APP_EN=1;				//Æô¶¯Ò»´Î¶ÁÐ´
			while(APP_EN);       //¶ÁÐ´Ö´ÐÐ½áÊø
			ADR_INC=0x01; 
			RAMMODE=0x8F;     
			DATA1=*pBuf++;
			DATA0=*pBuf++;
			APP_EN=1;
			Len16--;
    }
    len32=Len16>>1;
    while(len32--)
    {       
			DATA3=*pBuf++;
			DATA2=*pBuf++;
			DATA1=*pBuf++;
			DATA0=*pBuf++;
			APP_EN=1;
    }
		if(Len16%2)
		{
			ADR_INC=0;
			RAMMODE=0xAF;
			APP_EN=1;                //Ôö¼ÓÒ»¸ö¶Á¹ý³Ì£¬ÒÔÃâÐ´µ¥×ÖÊ±»á½«ÁíÒ»¸öµ¥×ÖÐ´0
			while(APP_EN);       //¶ÁÐ´Ö´ÐÐ½áÊø
			ADR_INC=0x01;
			RAMMODE=0x8F;         
			DATA3=*pBuf++;
			DATA2=*pBuf++;
			APP_EN=1;
		}
    RAMMODE=0x00;       //¶ÁÐ´Íê³ÉºóRAMMODE±ØÐëÖÃÁã 
EA = 1;	
}  



void sys_timer2_isr()	interrupt 5
{
	TF2=0;
	if(delay_tick)
		delay_tick--;
}



//==============================================================================
//--------------------------------FUNCTIONS-------------------------------------
//==============================================================================

 union
{
   u8       byte[4];
   u16      word[2];
   u32      words;
}data buf;

u16 code union_0_buff = 0;
u16 code union_1_buff = 1;

u16 GetPageID()
{
	  u16  nPage;
    ReadDgusVp(PIC_NOW,(u8*)(&nPage),1);	
	  return nPage;
}

void PageChange(u16 PageID)
{
	u8 buf[4];
	buf[0] = 0x5a;
	buf[1] = 0x01;
	buf[2] = (u8)(PageID >> 8);
	buf[3] = (u8)PageID;
  WriteDgusVp(PIC_SET,buf,2);
}

/****************************************************************************/
u16 ReadDgus(u16 Dgus_Addr)
{	   	   	
	u16 R_Dgus=0;
EA = 0;
			ADR_H = 0x00;
			ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//¶Á²Ù×÷
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			if(Dgus_Addr%2)	R_Dgus = (DATA1<<8)+DATA0; 
			else   R_Dgus=(DATA3<<8)+DATA2;
			RAMMODE = 0;
EA = 1;
			return   R_Dgus;		
}
/****************************************************************************/
void WriteDgus(u16 Dgus_Addr,u16 Val)
{      
EA = 0;
			ADR_H = 0x00;
			ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//¶Á²Ù×÷
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
		   	ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0x8F;		//Ð´²Ù×÷
		    while(!APP_ACK);
			if(Dgus_Addr%2){DATA1=(u8)(Val>>8); DATA0=(u8)(Val); }
			else { DATA3 =(u8)(Val>>8);	DATA2 =(u8)(Val);}
		    APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
EA = 1;
}


//==============================================================================
//---------------------------------- INIT---------------------------------------
//==============================================================================

void SysTimer2Init()
{
	T2CON = 0x70;
	TH2 = 0x00;
	TL2 = 0x00;
	TRL2H = 0xBC;	
	TRL2L = 0xCD;       
	IEN0 |= 0x20;	
	TR2 = 0x01;
	EA = 1;
}

void InitCFG(void)
{
		CKCON=0x00;		
    T2CON=0x70;		
    DPC=0x00;				
    PAGESEL=0x01;			//64KB 
    D_PAGESEL=0x02;   //DATA RAM  0x8000-0xFFFF
    MUX_SEL=0x60;   	//UART2,UART3
    PORTDRV=0x01;   	//+/-8mA
		RAMMODE=0x00;			//DGUS 
}
void InitInt(void)
{
    IEN0=0x00;      	//
		IEN1=0x00;				//
    IEN2=0x00;				//
    IP0=0x00;      		// 
    IP1=0x00;					//	
}

//GPIO  
void InitGPIO(void)
{
	PORTDRV=0x01;   			//+/-8mA		
  P0         = 0x00;		//·							
  P1         = 0x00;
  P2         = 0x00;
  P3         = 0x00;
}
void EX0_ISR_PC(void)    interrupt 0
{
    EA=0;
    EA=1;
}
void EX1_ISR_PC(void)    interrupt 2
{
    EA=0;
    EA=1;
}
void InitCPU(void)
{
  EA=0;
  InitCFG();      
  InitInt();
  InitGPIO();
	InitUart();
  SysTimer2Init();
  //InitTimer();

  EA=1;
}


//==============================================================================
//---------------------------------END FILE-------------------------------------
//==============================================================================
