#include "UART.h"
#include "REGX52.h"
#include "stdio.h"

/**
  * @brief  ���ڳ�ʼ����9600bps@11.0592MHz
  * @param  ��
  * @retval ��
  */
void UartInit(void)		
{
	//�������ã�
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//���ڹ���ģʽ1��8λ����,�ɱ䲨���ʣ�ʹ�ܽ���
	
	//��ʱ��1���ã�(����������)
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ģʽ
	TL1 = 0xFD;		//�趨��ʱ����ֵ
	TH1 = 0xFD;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	TI=1;    //�����жϱ�־λ��λ  
}

/**
  * @brief  �����жϷ�����
  * @param  ��
  * @retval ��
  */
void Uart_Routine() interrupt 4 
{
       if(RI)

    {
        RI= 0;   //�������жϱ�־λֻ���������λ

    }

       if(TI)

    {
			  TI = 0;  //�������жϱ�־λֻ���������λ

    }

}