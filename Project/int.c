#include "int.h"
#include "REGX52.h"
#include "Delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"
#include "LCD1602.h"
#include "DS1302.h"
void Int1Init()
{
	//设置INT1
	IT1=1;//跳变沿出发方式（下降沿）
	EX1=1;//打开INT1的中断允许。	
	EA=1;//打开总中断	
}


void Int1()	interrupt 2		//外部中断1的中断函数
{
		Delay(20);
		if(P3^3==0)
		{ 

			
		}
}
