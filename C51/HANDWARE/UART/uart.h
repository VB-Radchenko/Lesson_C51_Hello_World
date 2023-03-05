#ifndef __UART_H_
#define __UART_H_

#include "t5los8051.h"
#include "sys.h"

#define UART_SIZE 256
#define UART_MAX_LEN 1024
#define Type_Communication  1		//1.迪文8283通讯   //2.mudbus

typedef struct UART
{
	u8 	 Tx_Num;									//已发数据计数
	u8 	 Tx_H;										//总发送计数
	u8 	 Tx_F;										//发送标记
	u8   Tx_Data[UART_SIZE];			//发送缓存
	u16  Rx_Num;									//接收数据计数
	u8 	 Rx_T;										//接收数据延迟
	u8 	 Rx_F;										//接收标记	
	u8   Rx_Data[UART_MAX_LEN];	//接收缓存
	u8	 Rx_S;										//modbus接收状态位
}Typdef_Uart;


/*mady by qiao*/
extern Typdef_Uart uart[6];
extern u8 xdata uart_txAck[6] ;
extern u8 xdata uart_txBuf[255];
extern u8 xdata One_CMD_Buff[255];//用来存取一帧数据
extern u8 xdata uart_busy;
extern u8 xdata UART2_message[9];






/*串口2*/
void UART2_Init(u32 bdt);
void UART2_Sendbyte(u8 dat);
/*串口3*/
void UART3_Init(u32 bdt);
void UART3_Sendbyte(u8 dat);
/*串口4*/
void UART4_Init(u32 bdt);	
void UART4_Sendbyte(u8 dat);
/*串口5*/
void UART5_Init(u32 bdt);	
void UART5_Sendbyte(u8 dat);

u16 crc16(u8 *dat, u16 length);  
u8 strcmp_qiao( u8 *str1, u8 *str2,u8 Num);
void Describe_Modbus(u8 st);
void Describe_8283(u8 st);
void Communication_CMD(u8 st);
void Uatr_Send_Data(u8 st,const u8 *buf,u8 len);
void InitUart(void);
void Uatr_Send_CMD(u16 VP,u16 VP_buff);


/*额外定义*/






#endif