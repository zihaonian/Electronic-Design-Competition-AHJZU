/*****ͷ�ļ���***************************************************************/
#include <REGX52.H>
#include "LCD1602.h"
#include "DS18B20.h"
#include "Delay.h"
#include "MatrixKey.h"
#include "DS1302.h"
#include "Timer.h"
#include "UART.h"
/*******************************************************************************/

/*****����������***************************************************************/
unsigned char Count_k=0;
unsigned char Count_i=0;
unsigned char Count_j=0;
/*******************************************************************************/

/*******************************************************************************
* Name          main
* Functiom	    .......
* Input         Non
* Output    	  Non
* Author        ��־�� 
* University    ���ս�����ѧ
* Decoding      GB2312
* Time          2022��6��14��
*******************************************************************************/
void main()
{
	DS1302_Init();                       //ʱ��ģ���ʼ��
	LCD_Init();                          //LCD1602ģ���ʼ��
	Timer0Init();                        //��ʱ��0��ʼ��
	UartInit();                          //���ڳ�ʼ��
	DS1302_SetTime();                    //ʱ��ʱ�丳��ֵ
	DS18B20_ConvertT();		               //�ϵ���ת��һ���¶ȣ���ֹ��һ�ζ����ݴ���
	LCD_ShowString(1,1,"N1:20210040216");//ѧ����ʾ
	LCD_ShowString(2,1,"N2:20203100142");
	while(1)
	{
		matrixkey_return=MatrixKey();
		if(matrixkey_return==15)
		{
			Count_k++;
		}
		if(Count_k%3==0)
		{
			if(matrixkey_return==16)     
			{
				Count_i++;                       //ģʽ���Ƽ�������������
				matrixkey_return=0;              //�����ֵ����
			}
			if(Count_i%2==0)                   //ģʽ1(ѧ����ʾģʽ)
			{
				LCD_Init();                      //LCD1602ģ���ʼ��
				LCD_ShowString(1,1,"N1:20210040216");
				LCD_ShowString(2,1,"N2:20203100142");
			}
			if(Count_i%2==1)                   //ģʽ2(�¶�ʱ����ʾģʽ)
			{
				LCD_Init();                      //LCD1602ģ���ʼ��
				DS18B20_ReadT();	               //��ȡ�¶�
				DS18B20_ConvertT();	             //ת���¶�
				LCD1602_Show_T();                //��ʾ�¶�
				DS1302_ReadTime();               //��ȡʱ��
				LCD1602_Show_Time();             //��ʾʱ��
			}
			Delay(500);
		}
		
		if(Count_k%3==1)                     //ģʽ3(ʱ������ģʽ)
		{	
			matrixkey_return=0;
			LCD_Init();
			LCD_ShowString(1,1,"MODE:Setting Time");
			LCD1602_Show_Time();
			while(1)
			{
				matrixkey_return=MatrixKey(); 
				if(matrixkey_return==1){DS1302_Time[0]++;matrixkey_return=0;}
				if(matrixkey_return==5){DS1302_Time[0]--;matrixkey_return=0;}
				
				if(matrixkey_return==2){DS1302_Time[1]++;matrixkey_return=0;}
				if(matrixkey_return==6){DS1302_Time[1]--;matrixkey_return=0;}
				
				if(matrixkey_return==3){DS1302_Time[2]++;matrixkey_return=0;}
				if(matrixkey_return==7){DS1302_Time[2]--;matrixkey_return=0;}
				
				if(matrixkey_return==4){DS1302_Time[3]++;matrixkey_return=0;}
				if(matrixkey_return==8){DS1302_Time[3]--;matrixkey_return=0;}
				
				if(matrixkey_return==9){DS1302_Time[4]++;matrixkey_return=0;}
				if(matrixkey_return==13){DS1302_Time[4]--;matrixkey_return=0;}
				
				if(matrixkey_return==10){DS1302_Time[5]++;matrixkey_return=0;}
				if(matrixkey_return==14){DS1302_Time[5]--;matrixkey_return=0;}
				
				DS1302_SetTime();
				LCD1602_Show_Time();
				
				if(matrixkey_return==15||matrixkey_return==16){break;}
			}	
			Count_k++;
		}
		if(Count_k%3==2)                        //ģʽ4(ʱ���¶����ݻز�ģʽ)
		{
			matrixkey_return=0;
			LCD_Init();
			LCD_ShowString(1,1,"MODE:");
			LCD_ShowString(1,8,"Record");
			LCD_ShowNum(1,6,(Count_j%10)+1,2);
			LCD_ShowNum(2,1,Time_Record_Array[Count_j%10][0],2);
			LCD_ShowString(2,3,":");
      LCD_ShowNum(2,4,Time_Record_Array[Count_j%10][1],2);
			LCD_ShowString(2,6,":");
			LCD_ShowNum(2,7,Time_Record_Array[Count_j%10][2],2);
			while(1)
			{
				matrixkey_return=MatrixKey();
				if(matrixkey_return==8){Count_j++;matrixkey_return=0;}
				if(matrixkey_return==12){Count_j--;matrixkey_return=0;}
				LCD_ShowNum(1,6,(Count_j%10)+1,2);
				LCD_ShowNum(2,1,Time_Record_Array[Count_j%10][0],2);
				LCD_ShowString(2,3,":");
        LCD_ShowNum(2,4,Time_Record_Array[Count_j%10][1],2);
				LCD_ShowString(2,6,":");
				LCD_ShowNum(2,7,Time_Record_Array[Count_j%10][2],2);
				

				LCD_ShowChar(2,11,'+');	//��ʾ����
				LCD_ShowNum(2,12,Temperature_Record_Array[Count_j%10],2);		//��ʾ�¶���������
				LCD_ShowChar(2,14,'.');		//��ʾС����
				LCD_ShowNum(2,15,(unsigned long)(Temperature_Record_Array[Count_j%10]*10000)%10000,2);//��ʾ�¶�С������
	      if(matrixkey_return==11)
				{	
					printf("\n\n�¶�ʱ����� S11����\n");
					printf("��%2.0f��ʱ���¶ȼ�¼\n",(float)(Count_j%10)+1);
					printf("�¶ȣ�%2.4f^C  ",Temperature_Record_Array[Count_j%10]);
					printf("ʱ�䣺%2.0f:%2.0f:%2.0f\n\n\n",(float)Time_Record_Array[Count_j%10][0],(float)Time_Record_Array[Count_j%10][1],(float)Time_Record_Array[Count_j%10][2]);
				}
				if(matrixkey_return==15){matrixkey_return=0;Count_k++;break;}
			}
		}
	}
}