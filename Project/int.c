#include "int.h"
#include "REGX52.h"
#include "Delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"
#include "LCD1602.h"
#include "DS1302.h"
void Int1Init()
{
	//����INT1
	IT1=1;//�����س�����ʽ���½��أ�
	EX1=1;//��INT1���ж�����	
	EA=1;//�����ж�	
}


void Int1()	interrupt 2		//�ⲿ�ж�1���жϺ���
{
		Delay(20);
		if(P3^3==0)
		{ 

			
		}
}
