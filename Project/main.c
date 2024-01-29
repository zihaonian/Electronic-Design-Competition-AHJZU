/*****头文件区***************************************************************/
#include <REGX52.H>
#include "LCD1602.h"
#include "DS18B20.h"
#include "Delay.h"
#include "MatrixKey.h"
#include "DS1302.h"
#include "Timer.h"
#include "UART.h"
/*******************************************************************************/

/*****变量定义区***************************************************************/
unsigned char Count_k=0;
unsigned char Count_i=0;
unsigned char Count_j=0;
/*******************************************************************************/

/*******************************************************************************
* Name          main
* Functiom	    .......
* Input         Non
* Output    	  Non
* Author        年志豪 
* University    安徽建筑大学
* Decoding      GB2312
* Time          2022年6月14日
*******************************************************************************/
void main()
{
	DS1302_Init();                       //时钟模块初始化
	LCD_Init();                          //LCD1602模块初始化
	Timer0Init();                        //定时器0初始化
	UartInit();                          //串口初始化
	DS1302_SetTime();                    //时钟时间赋初值
	DS18B20_ConvertT();		               //上电先转换一次温度，防止第一次读数据错误
	LCD_ShowString(1,1,"N1:20210040216");//学号显示
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
				Count_i++;                       //模式控制计数器变量自增
				matrixkey_return=0;              //矩阵键值清零
			}
			if(Count_i%2==0)                   //模式1(学号显示模式)
			{
				LCD_Init();                      //LCD1602模块初始化
				LCD_ShowString(1,1,"N1:20210040216");
				LCD_ShowString(2,1,"N2:20203100142");
			}
			if(Count_i%2==1)                   //模式2(温度时间显示模式)
			{
				LCD_Init();                      //LCD1602模块初始化
				DS18B20_ReadT();	               //读取温度
				DS18B20_ConvertT();	             //转换温度
				LCD1602_Show_T();                //显示温度
				DS1302_ReadTime();               //读取时间
				LCD1602_Show_Time();             //显示时间
			}
			Delay(500);
		}
		
		if(Count_k%3==1)                     //模式3(时间设置模式)
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
		if(Count_k%3==2)                        //模式4(时间温度数据回查模式)
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
				

				LCD_ShowChar(2,11,'+');	//显示正号
				LCD_ShowNum(2,12,Temperature_Record_Array[Count_j%10],2);		//显示温度整数部分
				LCD_ShowChar(2,14,'.');		//显示小数点
				LCD_ShowNum(2,15,(unsigned long)(Temperature_Record_Array[Count_j%10]*10000)%10000,2);//显示温度小数部分
	      if(matrixkey_return==11)
				{	
					printf("\n\n温度时间回显 S11触发\n");
					printf("第%2.0f次时间温度记录\n",(float)(Count_j%10)+1);
					printf("温度：%2.4f^C  ",Temperature_Record_Array[Count_j%10]);
					printf("时间：%2.0f:%2.0f:%2.0f\n\n\n",(float)Time_Record_Array[Count_j%10][0],(float)Time_Record_Array[Count_j%10][1],(float)Time_Record_Array[Count_j%10][2]);
				}
				if(matrixkey_return==15){matrixkey_return=0;Count_k++;break;}
			}
		}
	}
}