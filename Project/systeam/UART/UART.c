#include "UART.h"
#include "REGX52.h"
#include "stdio.h"

/**
  * @brief  串口初始化，9600bps@11.0592MHz
  * @param  无
  * @retval 无
  */
void UartInit(void)		
{
	//串口配置：
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//串口工作模式1：8位数据,可变波特率，使能接收
	
	//定时器1配置：(发生波特率)
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装载模式
	TL1 = 0xFD;		//设定定时器初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	TI=1;    //串口中断标志位置位  
}

/**
  * @brief  串口中断服务函数
  * @param  无
  * @retval 无
  */
void Uart_Routine() interrupt 4 
{
       if(RI)

    {
        RI= 0;   //接收器中断标志位只能由软件复位

    }

       if(TI)

    {
			  TI = 0;  //发送器中断标志位只能由软件复位

    }

}