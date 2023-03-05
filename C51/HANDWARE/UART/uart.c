
#include "sys.h"
#include "T5LOS8051.h"
#include "uart.h"
#include "GPIO.h"
#include "timer.h"
#include "stdio.h"
#include "string.h"





//全局变量定义
Typdef_Uart uart[6];
u8  uart_txAck[6] = {0x5A, 0xA5, 0x03, 0x82, 0x4F, 0x4B };
u8 xdata uart_txBuf[255] = {0x5A, 0xA5, 0x00, 0x83, 0x4F, 0x4B};
u8 xdata uart_busy=0;
u8 xdata UART2_message[9]={0x5A,0xA5,0x06,0x83,0x25,0x00,0x01,0x00,0x00};


/*CRC校验*/
u16 crc16(u8 *dat, u16 length)  
{  
    u8 i;  
    u16 crc = 0xffff;        // Initial value  
    while(length--)  
    {  
        crc ^= *dat++;            // crc ^= *dat; dat++;  
        for (i = 0; i < 8; ++i)  
        {  
            if (crc & 1)  
                crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005  
            else  
                crc = (crc >> 1);  
        }
    }
    return crc;  
}
/*比较两个字符串并根据比较结果返回整
str1=str2，则返回零；不等则返回正数*/
u8 strcmp_qiao( u8 *str1, u8 *str2,u8 Num)
{
	u8 ret=0 ,i = 0;
	for(i=0;i<Num;i++)
	{
			if(*(str1+i)!= *(str2+i))
			{
				ret++;
			}
	}
	return ret;
}


 /**
   串口2初始化
    */
void UART2_Init(u32 bdt)
{
	u16 tmp;	
	tmp=1024-FOSC/64/bdt;	 	
	MUX_SEL    |= 0x40; //复用选择口， 选择复用为UART2， 接口引出到 P0.4、P0.5

	SetPinOut(GPIO_PORT_0,GPIO_PIN_4);
	SetPinIn(GPIO_PORT_0,GPIO_PIN_5);
	
	SCON2 = 0x50;	//0011 0000 ,即将SM2，REN置1,SM2：只有当接收到第9位数据（RB8）为1时，才把接收到的前8位数据送入SBUF，且置位RI 发出中断申请 ，即发送方式为8N1   ，
	ADCON = 0x80;	//波特率发生器选择，要使用  SREL0H:L 自行设置 
		 
	SREL2H = (u8)(tmp>>8);        
	SREL2L = (u8)tmp;
	ES2 = 1;
	EA = 1;

	uart[2].Tx_Num 	= 0;
	uart[2].Tx_H		= 0;
	uart[2].Tx_F  	= 0;	
	uart[2].Rx_Num	= 0;
	uart[2].Rx_T 		= 0;
	uart[2].Rx_F	 	= 0;
	memset(&uart[2].Tx_Data,0,UART_SIZE);
	memset(&uart[2].Rx_Data,0,UART_MAX_LEN);
}
void UART3_Init(u32 bdt)		//8N1 115200
{
	u16 tmp;	
	tmp=1024-FOSC/32/bdt;
	
	MUX_SEL|=0X20;
	SetPinOut(GPIO_PORT_0,GPIO_PIN_6);
  SetPinIn(GPIO_PORT_0,GPIO_PIN_7);
	
	SCON3 = 0X50;
	SREL3H =(u8)(tmp>>8);
	SREL3L =(u8)tmp;
	IEN2 |= 1;
	
	uart[3].Tx_Num 	= 0;
	uart[3].Tx_H		= 0;
	uart[3].Tx_F  	= 0;	
	uart[3].Rx_Num	= 0;
	uart[3].Rx_T 		= 0;
	uart[3].Rx_F	 	= 0;
	memset(&uart[3].Tx_Data,0,UART_SIZE);
	memset(&uart[3].Rx_Data,0,UART_MAX_LEN);
}

void UART4_Init(u32 bdt)	   //8N1 115200
{
	u16 tmp;
	SCON4T = 0X80;
	SCON4R = 0X80;
	tmp=FOSC/8/bdt;
	BODE4_DIV_H = (u8)(tmp>>8);
	BODE4_DIV_L = (u8)tmp;
	
	IEN1 |= 0X0C;
	uart[4].Tx_Num 	= 0;
	uart[4].Tx_H		= 0;
	uart[4].Tx_F  	= 0;	
	uart[4].Rx_Num	= 0;
	uart[4].Rx_T 		= 0;
	uart[4].Rx_F	 	= 0;
	memset(&uart[4].Tx_Data,0,UART_SIZE);
	memset(&uart[4].Rx_Data,0,UART_MAX_LEN);
}
void UART5_Init(u32 bdt)
{
	u16 tmp;
	SCON5T = 0X80;
	SCON5R = 0X80;
		
	tmp=FOSC/8/bdt;
	BODE5_DIV_H = (u8)(tmp>>8);
	BODE5_DIV_L = (u8)tmp;;
	
	IEN1 |= 0X30;
	uart[5].Tx_Num 	= 0;
	uart[5].Tx_H		= 0;
	uart[5].Tx_F  	= 0;	
	uart[5].Rx_Num	= 0;
	uart[5].Rx_T 		= 0;
	uart[5].Rx_F	 	= 0;
	memset(&uart[5].Tx_Data,0,UART_SIZE);
	memset(&uart[5].Rx_Data,0,UART_MAX_LEN);
}
void UART2_ISR_PC(void)    interrupt 4
{
    EA = 0;							//中断总控制位 关闭
    if(RI2)							//接收中断标志位 该位由硬件置位 ,软件清零
    {		
				RI2 = 0; 
				uart[2].Rx_F	 	= 1;
				uart[2].Rx_Data[uart[2].Rx_Num]	= SBUF2;
				uart[2].Rx_Num++;
				uart[2].Rx_Num %=UART_MAX_LEN; 
				uart[2].Rx_T = 3;	
    }
		if(TI2==1)													//发送中断标志位 该位由硬件置位 ,软件清零
		{
				TI2=0;							
				if(uart[2].Tx_H != uart[2].Tx_Num)
				{
					SBUF2 = uart[2].Tx_Data[uart[2].Tx_Num];	
					uart[2].Tx_Num++;
					uart[2].Tx_Num %=UART_SIZE;
				}
				else
				{
					uart[2].Tx_F=0;	
				}
		}
    EA = 1;														//中断总控制位 关闭
}
/*发送一个字节*/
void UART2_Sendbyte(u8 dat)
{       
	while(uart[2].Tx_F==1);     	 //  uart2_busy 如果为1就等待，不为1则执行下面
	uart[2].Tx_F=1;									//
	SBUF2 = dat;        
}


char putchar(u8 chr)
{
	UART2_Sendbyte(chr);
	return 0;
}

void uart3_Isr() interrupt 16
{
	EA = 0;
	if(SCON3&0x01)
	{
		SCON3 = 0x50;
		SCON3 &= 0xFE;
		uart[3].Rx_F = 1;	
		uart[3].Rx_T = 3;	
		uart[3].Rx_Data[uart[3].Rx_Num] = SBUF3;
		uart[3].Rx_Num++;
		uart[3].Rx_Num %=UART_MAX_LEN; 
	}
	if(SCON3&0x02)
	{
		SCON3 = 0x50;
		SCON3 &= 0xFD;
		if(uart[3].Tx_H != uart[3].Tx_Num)
		{
			SBUF3 = uart[3].Tx_Data[uart[3].Tx_Num];	
			uart[3].Tx_Num++;
			uart[3].Tx_Num %=UART_SIZE;
		}
	else
	{
		uart[3].Tx_F=0;
	}
	}
	EA = 1;
}

void UART3_Sendbyte(u8 dat)
{       
	while(uart[3].Tx_F==1);     	 //  uart2_busy 如果为1就等待，不为1则执行下面
	uart[3].Tx_F=1;									//
	SBUF3 = dat;        
}



void uart4t_Isr() interrupt 10
{
	EA = 0;
	SCON4T &= 0XFE;						
	if(uart[4].Tx_H != uart[4].Tx_Num)
	{
		SBUF4_TX = uart[4].Tx_Data[uart[4].Tx_Num];	
		uart[4].Tx_Num++;
		uart[4].Tx_Num %=UART_SIZE;
	}
	else
	{
		uart[4].Tx_F=0;
	}
	EA = 1;
}
void uart4r_Isr() interrupt 11
{
	EA = 0;
	SCON4R &= 0XFE;
	uart[4].Rx_F= 1;
	uart[4].Rx_T = 3;	
	uart[4].Rx_Data[uart[4].Rx_Num] = SBUF4_RX;
	uart[4].Rx_Num++;
	uart[4].Rx_Num %=UART_MAX_LEN;
	EA = 1;
}

void UART4_Sendbyte(u8 dat)
{       
	while(uart[4].Tx_F==1);     	 //  uart2_busy 如果为1就等待，不为1则执行下面
	uart[4].Tx_F=1;									//
	SBUF4_TX = dat;  
}

void uart5t_Isr() interrupt 12
{
	EA = 0;
	SCON5T &= 0XFE;
	if(uart[5].Tx_H != uart[5].Tx_Num)
	{
		SBUF5_TX = uart[5].Tx_Data[uart[5].Tx_Num];	
		uart[5].Tx_Num++;
		uart[5].Tx_Num %=UART_SIZE;
	}
	else
	{
		uart[5].Tx_F=0;
	}
	EA = 1;
}
void uart5r_Isr() interrupt 13
{
	EA = 0;
	SCON5R &= 0XFE;
	uart[5].Rx_F= 1;
	uart[5].Rx_T = 3;	
	uart[5].Rx_Data[uart[5].Rx_Num] = SBUF5_RX;
	uart[5].Rx_Num++;
	uart[5].Rx_Num %=UART_MAX_LEN;
	EA = 1;
}



/* 发送通讯消息 */
void Uatr_Send_CMD(u16 VP,u16 VP_buff)
{
	u8 i,str;
	str=2;
	UART2_message[4]=(VP&0xFF00)>>8;
	UART2_message[5]=VP&0xFF;
	UART2_message[6]=(VP_buff&0xFF00)>>8;
	UART2_message[7]=VP_buff&0xFF;
	
	for(i=0;i<8;i++)
	{
		uart[str].Tx_Data[uart[str].Tx_H] = UART2_message[i];
		uart[str].Tx_H++;
		uart[str].Tx_H %=UART_SIZE;
	}
	if(uart[str].Tx_F == 0)
	{
		uart[str].Tx_F = 1;
		SBUF2 = uart[str].Tx_Data[uart[str].Tx_Num];
		uart[str].Tx_Num++;
		uart[str].Tx_Num %=UART_SIZE;
	}
}

/*串口2-5发送数据 ,num */
void Uatr_Send_Data(u8 st,const u8 *buf,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		uart[st].Tx_Data[uart[st].Tx_H] = *buf++;
		uart[st].Tx_H++;
		uart[st].Tx_H %=UART_SIZE;
	}

	switch(st)
	{
		case 2:
			if(uart[2].Tx_F == 0)
			{
				uart[2].Tx_F = 1;
				SBUF2 = uart[2].Tx_Data[uart[2].Tx_Num];
				uart[2].Tx_Num++;
				uart[2].Tx_Num %=UART_SIZE;
			}
			break;
		case 3:
			if(uart[3].Tx_F == 0)
			{
				uart[3].Tx_F = 1;
				SBUF3 = uart[3].Tx_Data[uart[3].Tx_Num];
				uart[3].Tx_Num++;
				uart[3].Tx_Num %=UART_SIZE;
			}
			break;
		case 4:
			if(uart[4].Tx_F == 0)
			{

				uart[4].Tx_F = 1;
				SBUF4_TX = uart[4].Tx_Data[uart[4].Tx_Num];
				uart[4].Tx_Num++;
				uart[4].Tx_Num %=UART_SIZE;
			}
			break;
		case 5:
			if(uart[5].Tx_F == 0)
			{
				uart[5].Tx_F = 1;
				SBUF5_TX = uart[5].Tx_Data[uart[5].Tx_Num];
				uart[5].Tx_Num++;
				uart[5].Tx_Num %=UART_SIZE;
			}
			break;
		default:
			break;
	}

}


#if(Type_Communication==1)	
/* 使用队列 解析8283指令*/
void Describe_8283(u8 st)
{
		u8 N=0;	bit Flog=1;
			while(Flog)
			{						
				if((uart[st].Rx_Data[N]==0x5a)&&(uart[st].Rx_Data[N+1]==0xa5))   //5A A5 0007 82 1000 0001 0002
					{	 
							if(uart[st].Rx_Data[N+3]==0x82)
							 {	    
									u8 i=0;
									for(i=0;i<((uart[st].Rx_Data[N+2]-3)/2);i++)
								 {																																	
										WriteDgus(((uart[st].Rx_Data[N+4]<<8)+uart[st].Rx_Data[N+5]+i),((uart[st].Rx_Data[N+6+2*i]<<8)+uart[st].Rx_Data[N+7+2*i]));
								 }
										Uatr_Send_Data(st,uart_txAck,6);
							 }								  
							else if(uart[st].Rx_Data[N+3]==0x83)
							 {	    							 
									u8 i=0;u16 Temp=0;
									for(i=0;i<uart[st].Rx_Data[N+6];i++)
									{
											Temp=ReadDgus(((uart[st].Rx_Data[N+4]<<8)+uart[st].Rx_Data[N+5]+i));
										 uart_txBuf[7+2*i]=(u8)(Temp>>8);
										 uart_txBuf[8+2*i]=(u8)(Temp);
									}
										uart_txBuf[4] = uart[st].Rx_Data[N+4];
										uart_txBuf[5] = uart[st].Rx_Data[N+5];	
										uart_txBuf[6] = uart[st].Rx_Data[N+6];	
										uart_txBuf[2]	= 2*uart[st].Rx_Data[N+6]+4;
										Uatr_Send_Data(st,uart_txBuf,uart_txBuf[2]+3);
							 }
							N= N+(uart[st].Rx_Data[N+2]+3);					 
					}
					else
					{	  memset(uart[st].Rx_Data,0,N);				//处理完情况缓存区
							Flog=0;
					}		
			}
}
#elif(Type_Communication==2)	
void Describe_Modbus(u8 st)
{
	u8 cmd;
	u16 modbus_data,modbus_len,modbus_crc;
		if(((uart[st].Rx_Data[0]==0x01))&&(uart[st].Rx_Num >=uart[st].Rx_Data[2]+5)&&(uart[st].Rx_Num >=7))
					{				
							modbus_len=uart[st].Rx_Data[2]+5;
							modbus_crc=crc16(uart[st].Rx_Data,modbus_len-2);			//crc校验
							if(modbus_crc==((uart[st].Rx_Data[modbus_len-1]<<8)|(uart[st].Rx_Data[modbus_len-2])))
							{
								uart[st].Rx_S=0x5A;
							}
							else
							{
								uart[st].Rx_S=0;
								uart[st].Rx_Num=0;
							}
						}
					else
					{
						memset(uart[st].Rx_Data,0,uart[st].Rx_Num);
						uart[st].Rx_S=0;
						uart[st].Rx_Num=0;
					}
	if(uart[st].Rx_S==0x5A)
	{			
			cmd=uart[st].Rx_Data[1];
			switch(cmd)
			{		
				
				case	0x03:		//		
					if(uart[st].Rx_Data[0] == 0x01)
					{
						write_dgus_vp(0x10A0,(u8*)&uart[st].Rx_Data[3],16);					
					}	
			
					break;
				default:
					break;
			}
			memset(uart[st].Rx_Data,0,uart[st].Rx_Num);
			uart[st].Rx_S=0;
			uart[st].Rx_Num=0;			
		}
}	
#endif	

void Communication_CMD(u8 st)
{
	if((uart[st].Rx_F==1 )&&(uart[st].Rx_T==0))
			{				
				
#if(Type_Communication==1)		
				Describe_8283(st);
#elif(Type_Communication==2)		
				Describe_Modbus(st);
#endif					
				uart[st].Rx_F=0;
				uart[st].Rx_Num=0;
			} 
}
/*****************************************************************************
串口总配置*/
void InitUart(void)
{
		UART2_Init(115200);
		UART3_Init(1612800);
//		UART4_Init(115200);
//		UART5_Init(115200);
}








