#include <REGX52.H>
#include "OneWire.h"
#include "LCD1602.h"
#include "DS18B20.h"
//DS18B20指令
#define DS18B20_SKIP_ROM			0xCC
#define DS18B20_CONVERT_T			0x44
#define DS18B20_READ_SCRATCHPAD 	0xBE
float T;
/**
  * @brief  DS18B20开始温度变换
  * @param  无
  * @retval 无
  */
void DS18B20_ConvertT(void)
{
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_CONVERT_T);
}

/**
  * @brief  DS18B20读取温度
  * @param  无
  * @retval 温度数值
  */
float DS18B20_ReadT(void)
{
	unsigned char TLSB,TMSB;
	int Temp;
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
	TLSB=OneWire_ReceiveByte();
	TMSB=OneWire_ReceiveByte();
	Temp=(TMSB<<8)|TLSB;
	T=Temp/16.0;
	return T;
}

void LCD1602_Show_T(void)
{
	LCD_ShowString(1,1,"T:");
	if(T<0)				//如果温度小于0
	{
		LCD_ShowChar(1,3,'-');	//显示负号
		T=-T;			//将温度变为正数
	}
	else				//如果温度大于等于0
	{
		LCD_ShowChar(1,3,'+');	//显示正号
	}
	LCD_ShowNum(1,4,T,3);		//显示温度整数部分
	LCD_ShowChar(1,7,'.');		//显示小数点
	LCD_ShowNum(1,8,(unsigned long)(T*10000)%10000,4);//显示温度小数部分
	LCD_ShowString(1,12,"^C");
}

