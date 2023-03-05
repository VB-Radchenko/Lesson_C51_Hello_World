#ifndef __UART_H_
#define __UART_H_

#include "t5los8051.h"
#include "sys.h"

#define UART_SIZE 256
#define UART_MAX_LEN 1024
#define Type_Communication  1		//1.����8283ͨѶ   //2.mudbus

typedef struct UART
{
	u8 	 Tx_Num;									//�ѷ����ݼ���
	u8 	 Tx_H;										//�ܷ��ͼ���
	u8 	 Tx_F;										//���ͱ��
	u8   Tx_Data[UART_SIZE];			//���ͻ���
	u16  Rx_Num;									//�������ݼ���
	u8 	 Rx_T;										//���������ӳ�
	u8 	 Rx_F;										//���ձ��	
	u8   Rx_Data[UART_MAX_LEN];	//���ջ���
	u8	 Rx_S;										//modbus����״̬λ
}Typdef_Uart;


/*mady by qiao*/
extern Typdef_Uart uart[6];
extern u8 xdata uart_txAck[6] ;
extern u8 xdata uart_txBuf[255];
extern u8 xdata One_CMD_Buff[255];//������ȡһ֡����
extern u8 xdata uart_busy;
extern u8 xdata UART2_message[9];






/*����2*/
void UART2_Init(u32 bdt);
void UART2_Sendbyte(u8 dat);
/*����3*/
void UART3_Init(u32 bdt);
void UART3_Sendbyte(u8 dat);
/*����4*/
void UART4_Init(u32 bdt);	
void UART4_Sendbyte(u8 dat);
/*����5*/
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


/*���ⶨ��*/






#endif