#include "Timer.h"
#include <REGX52.H>
#include "DS1302.h"
#include "DS18B20.h"
#include "UART.h"
unsigned int  Count_i1=0;//计时计数器变量
unsigned char Count_k1=0;//数组赋值计数器变量
unsigned char Time_Record_Array[10][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
float Temperature_Record_Array[10]={0,0,0,0,0,0,0,0,0,0};
void Timer0Init(void)		//50毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0xB0;		  //设置定时初值
	TH0 = 0x3C;		  //设置定时初值
	ET0=1;          //打开定时器 0 中断允许	
  EA=1;           //打开总中断	
	TF0 = 0;		    //清除TF0标志
	TR0 = 1;				//定时器0开始计时
}

void Timer0() interrupt 1
{
	TL0 = 0xB0;		//设置定时初值
	TH0 = 0x3C;		//设置定时初值
  Count_i1++;	
  if(Count_i1%200==0)	
  {	
		DS1302_ReadTime();               //读取时间
		DS18B20_ReadT();	               //读取温度
		DS18B20_ConvertT();	             //转换温度
    printf("温度：%2.4f^C  ",T);
		printf("时间：%2.0f:%2.0f:%2.0f\n",(float)DS1302_Time[3],(float)DS1302_Time[4],(float)DS1302_Time[5]);
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



