#include "sys.h"
#include <intrins.h>


u16 count=0;
static u16 SysTick=0;


 /**
    * @brief ���ms��ʱ
    *   �����ʱ    ������T=1/206438400 
    * @param [in] 
    *      n:��ʱms
    * @param [out]
    *      ��
    * @retval 
    *      ��
    */
void delays_ms(u16 n)
{
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<17203;j++);    
}



 /**
    * @brief INIT CPU
    *   ��ʼ��OS�����裬�����г�ʼ��UART2,UART3���رտ��Ź���IO������ǿ��+/-8mA
    *   UART4��UART5����ʱ��T0 T1 T2��ֻ����UART2/UART3,��������û�п���
    * @param  ��
    *
    *   
    * @retval   ��
    * 
    */  
void INIT_CPU(void)
{
    EA=0;
    RS0=0;
    RS1=0;
    //SP=0xC0;

    CKCON=0x00;
    T2CON=0x70;
    DPC=0x00;
    PAGESEL=0x01;
    D_PAGESEL=0x02;   //DATA RAM  0x8000-0xFFFF
    MUX_SEL=0x60;   //UART2,UART2������WDT�ر�
    RAMMODE=0x00;
    PORTDRV=0x01;   //����ǿ��+/-8mA
    IEN0=0x00;      //�ر������ж�
    IEN1=0x00;
    IEN2=0x00;
    IP0=0x00;      //�ж����ȼ�Ĭ��
    IP1=0x00;

    WDT_OFF();      //�رտ��Ź�

    //IO������
    P0=0x00;
    P1=0x00;
    P2=0x00;
    P3=0x00;
    P0MDOUT=0x50;
    P1MDOUT=0xFF;
    P2MDOUT=0x00; 
    P3MDOUT=0x0F;

    //UART2����8N1  15200
    ADCON=0x80;
    SCON0=0x50;
    SREL0H=0x03;        //FCLK/64*(1024-SREL1)
    SREL0L=0xE4;

    //UART3����8N1   115200
    SCON1=0x50;
    SREL1H=0x03;       //FCLK/64*(1024-SREL1)
    SREL1L=0xC8;

    //UART4����8N1      115200
    SCON2T=0x80;
    SCON2R=0x80;
    BODE2_DIV_H=0x00;     //FCLK/8*DIV
    BODE2_DIV_L=0xE0;

    //UART5����8N1      115200
    SCON3T=0x80;
    SCON3R=0x80;
    BODE3_DIV_H=0x00;       //FCLK/8*DIV
    BODE3_DIV_L=0xE0;

    
    TMOD=0x11;          //16λ��ʱ��
    //T0
    TH0=0x00;
    TL0=0x00;
    TR0=0x00;

    //T1
    TH1=0x00;
    TL1=0x00;
    TR1=0x00;
    
    TCON=0x05;      //�ⲿ�ж��½��ش���

    //T2  Autoloadģʽ
    T2CON=0x70;
    TH2=0x00;
    TL2=0x00;
    TRL2H=0xBC;
    TRL2L=0xCD;        //1ms�Ķ�ʱ��

}

 /**
    * @brief ��ʱ��T0��ʼ��
    *        ��ʼ����ʱ��T0����ʱ���Ϊ1ms
    * @param [in] 
    *      ��
    * @param [out]
    *      ��
    * @retval 
    *      ��
    */
void T0_Init(void)
{
    TMOD|=0x01;
    TH0=T1MS>>8;        //1ms��ʱ��
    TL0=T1MS;
    ET0=1;              //������ʱ��0�ж�
    EA=1;               //�����ж�
    TR0=1;              //������ʱ��
}

 /**
    * @brief ��ʱ��T2��ʼ��
    *        ��ʼ����ʱ��T2����ʱ���Ϊ1ms
    * @param [in] 
    *      ��
    * @param [out]
    *      ��
    * @retval 
    *      ��
    */
void T2_Init(void)
{
    T2CON=0x70;
    TH2=0x00;
    TL2=0x00;
    TRL2H=0xBC;
    TRL2L=0xCD;        //1ms�Ķ�ʱ��
    IEN0|=0x20;     //������ʱ��2
    TR2=0x01;
}


 /**
    * @brief ��DGUS II VP����
    *        һ�ζ�д������4���ֽڣ���Ϊ��дlen����������Ϊ��λ��
    *       ���Զ�д���ݷ�Ϊ4�в�ͬ����������ͬ����д��һ��
    * @param [in] 
    *      addr:VP�׵�ַ
    *      len: ��VP�ֳ���
    * @param [out]
    *      buf:��ȡ���ݱ��棬buf�ֽڳ���Ϊlen*2
    * @retval 
    *      ��
    */  
void read_dgusii_vp(int addr,char* buf,int len)
{
    int OS_addr=0,OS_addr_offset=0;
    int OS_len=0,OS_len_offset=0;
    EA=0;
    OS_addr=addr/2;
    OS_addr_offset=addr%2;
    ADR_H=(u8)(OS_addr>>16)&0xFF;
    ADR_M=(u8)(OS_addr>>8)&0xFF;
    ADR_L=(u8)OS_addr&0xFF;
    ADR_INC=1;                 //DGUS  OS�洢����ַ�ڶ�д���Զ���1
    RAMMODE=0xAF;               //������ģʽ
    if(OS_addr_offset==1)       //�׵�ַ��ƫ�ƣ�����
    {
        while(APP_ACK==0);      //�ȴ�
        APP_EN=1;
        while(APP_EN==1); 
        *buf++=DATA1;
        *buf++=DATA0;              
        len--;
        OS_addr_offset=0;
    }
    OS_len=len/2;
    OS_len_offset=len%2;
    if(OS_len_offset==1)
    {
         OS_len++;
    }
    while(OS_len--)
    {
        if((OS_len_offset==1)&&(OS_len==0))
        {           
            while(APP_ACK==0);
            APP_EN=1;
            while(APP_EN==1);       //��дִ�н���
            *buf++=DATA3;
            *buf++=DATA2;           
            break;    
        } 
        while(APP_ACK==0);
        APP_EN=1;
        while(APP_EN==1);       //��дִ�н��� 
        *buf++=DATA3;
        *buf++=DATA2;
        *buf++=DATA1;
        *buf++=DATA0;                          
    }   
    RAMMODE=0x00;           //��д��ɺ�RAMMODE��������
    EA=1;         
}


 /**
    * @brief дDGUS II VP����
    *        һ�ζ�д������4���ֽڣ���Ϊ��дlen����������Ϊ��λ��
    *       ���Զ�д���ݷ�Ϊ4�в�ͬ����������ͬ����д��һ��
    * @param [in] 
    *      addr:VP�׵�ַ
    *      len: дVP�ֳ���
    * @param [out]
    *      buf:��ȡ���ݱ��棬buf�ֽڳ���Ϊlen*2
    * @retval 
    *      ��
    */ 
void write_dgusii_vp(int addr,u8* buf,int len)
{
    int OS_addr=0,OS_addr_offset=0;
    int OS_len=0,OS_len_offset=0;
    EA=0;
    OS_addr=addr/2;
    OS_addr_offset=addr%2; 
    ADR_H=(u8)(OS_addr>>16)&0xFF;
    ADR_M=(u8)(OS_addr>>8)&0xFF;
    ADR_L=(u8)OS_addr&0xFF;
    ADR_INC=0x01;                 //DGUS  OS�洢����ַ�ڶ�д���Զ���1
    RAMMODE=0x8F;               //����дģʽ 
    if(OS_addr_offset==1)
    {
        ADR_INC=0;                     
        RAMMODE=0xAF;
        while(APP_ACK==0);
        APP_EN=1;
        while(APP_EN==1);       //��дִ�н���
        ADR_INC=0x01; 
        RAMMODE=0x8F;
        while(APP_ACK==0);      
        DATA1=*buf++;
        DATA0=*buf++;
        APP_EN=1;
        while(APP_EN==1);       //��дִ�н���
        len--;
        OS_addr_offset=0;
    }
    OS_len=len/2;
    OS_len_offset=len%2; 
    if(OS_len_offset==1)
    {
         OS_len++;
    } 
    while(OS_len--)
    {
        if((OS_len_offset==1)&&(OS_len==0))
        {
            ADR_INC=0;
            RAMMODE=0xAF;
            while(APP_ACK==0);
            APP_EN=1;                //����һ�������̣�����д����ʱ�Ὣ��һ������д0
            while(APP_EN==1);       //��дִ�н���
            ADR_INC=0x01;
            RAMMODE=0x8F;
            while(APP_ACK==0);           
            DATA3=*buf++;
            DATA2=*buf++;
            APP_EN=1;
            while(APP_EN==1);       //��дִ�н���
            break;
        }
        while(APP_ACK==0);        
        DATA3=*buf++;
        DATA2=*buf++;
        DATA1=*buf++;
        DATA0=*buf++;
        APP_EN=1;
        while(APP_EN==1);       //��дִ�н���
    } 
    RAMMODE=0x00;       //��д��ɺ�RAMMODE��������
    EA=1;  
}  




 /**
    * @brief �жϴ��������жϺ��Ѿ�������ˣ�����Ҫ�ٽ����޸�
    *        using n:���Բ���Ҫ����ΪOS CPU�Ĵ����Ѿ��ܿ��ˣ�������Ҫ�ж�Ƕ��
    *       �Ѿ��ĳ��˽ӿں����Ķ�ʱ���Ƿ���Ҫ����interrupt n�������Ҫ��ȷ���£��а��Ӻ󣬿��Բ�����
    * @param [in] 
    *      ��
    * @param [out]
    *      ��
    * @retval 
    *      ��
    */ 
void EX0_ISR_PC(void)    interrupt 0
{
    EA=0;
    //code
    EA=1;
}


 /**
    * @brief ��ʱ��T0�жϴ�����
    *        �û��Լ�����
    * @param [in] 
    *      ��
    * @param [out]
    *      ��
    * @retval 
    *      ��
    */
void T0_ISR_PC(void)    interrupt 1
{
    static u16 i=0;
    EA=0;
    TH0=T1MS>>8;
    TL0=T1MS;
    i++;
    if(i>=1000)
    {
        i=0;
        P1_0=!P1_0;        
        count++; 
    }          
    EA=1;
}

void EX1_ISR_PC(void)    interrupt 2
{
    EA=0;
    //code
    EA=1;
}

void T1_ISR_PC(void)    interrupt 3
{
    EA=0;
    //code
    EA=1;
}

void UART2_ISR_PC(void)    interrupt 4
{
    EA=0;
    //code
    EA=1;
}


 /**
    * @brief ��ʱ��T2��ʱ
    *   ��ʱ��T2��ʱ����׼��ʱ 
    * @param [in] 
    *      n:��ʱms
    * @param [out]
    *      ��
    * @retval 
    *      ��
    */
void delay_ms(u16 n)
{
    SysTick=n;
    while(SysTick)
    {
        if(ET2==0)
        {   
            SysTick--;
            ET2=1;
        }
    }    
}

void T2_ISR_PC(void)    interrupt 5
{
    TF2=0;    
    SysTick--;    
}


void CAN_ISR_PC(void)    interrupt 9
{
    EA=0;
    //code
    EA=1;
}

void UART4_TX_ISR_PC(void)    interrupt 10
{
    EA=0;
    //code
    EA=1;
}

void UART4_RX_ISR_PC(void)    interrupt 11
{
    EA=0;
    //code
    EA=1;
}

void UART5_TX_ISR_PC(void)    interrupt 12
{
    EA=0;
    //code
    EA=1;
}

void UART5_RX_ISR_PC(void)    interrupt 13
{
    EA=0;
    //code
    EA=1;
}

void UART3_ISR_PC(void)    interrupt 16
{
    EA=0;
    //code
    EA=1;
}










