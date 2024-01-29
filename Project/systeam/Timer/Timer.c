#include "Timer.h"
#include <REGX52.H>
#include "DS1302.h"
#include "DS18B20.h"
#include "UART.h"
unsigned int  Count_i1=0;//��ʱ����������
unsigned char Count_k1=0;//���鸳ֵ����������
unsigned char Time_Record_Array[10][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
float Temperature_Record_Array[10]={0,0,0,0,0,0,0,0,0,0};
void Timer0Init(void)		//50����@12.000MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0xB0;		  //���ö�ʱ��ֵ
	TH0 = 0x3C;		  //���ö�ʱ��ֵ
	ET0=1;          //�򿪶�ʱ�� 0 �ж�����	
  EA=1;           //�����ж�	
	TF0 = 0;		    //���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
}

void Timer0() interrupt 1
{
	TL0 = 0xB0;		//���ö�ʱ��ֵ
	TH0 = 0x3C;		//���ö�ʱ��ֵ
  Count_i1++;	
  if(Count_i1%200==0)	
  {	
		DS1302_ReadTime();               //��ȡʱ��
		DS18B20_ReadT();	               //��ȡ�¶�
		DS18B20_ConvertT();	             //ת���¶�
    printf("�¶ȣ�%2.4f^C  ",T);
		printf("ʱ�䣺%2.0f:%2.0f:%2.0f\n",(float)DS1302_Time[3],(float)DS1302_Time[4],(float)DS1302_Time[5]);
  }
	if(Count_i1%400==0)
	{
		Count_i1=0;
		Time_Record_Array[Count_k1][0]=DS1302_Time[3];
		Time_Record_Array[Count_k1][1]=DS1302_Time[4];
		Time_Record_Array[Count_k1][2]=DS1302_Time[5];
		Temperature_Record_Array[Count_k1]=T;
		Count_k1++;
		if(Count_k1==10)
		{
			Count_k1=0;
		}
	}
}



