#include <REGX52.H>
#include "DS1302.h"
#include "LCD1602.h"
//引脚定义
sbit DS1302_SCLK=P3^6;
sbit DS1302_IO=P3^4;
sbit DS1302_CE=P3^5;

//寄存器写入地址/指令定义
#define DS1302_SECOND		0x80
#define DS1302_MINUTE		0x82
#define DS1302_HOUR			0x84
#define DS1302_DATE			0x86
#define DS1302_MONTH		0x88
#define DS1302_DAY			0x8A
#define DS1302_YEAR			0x8C
#define DS1302_WP			  0x8E

//时间数组，索引0~6分别为年、月、日、时、分、秒、星期
unsigned char DS1302_Time[]={22,06,01,0,0,0,6};
/**
  * @brief  DS1302初始化
  * @param  无
  * @retval 无
  */
void DS1302_Init(void)
{
	DS1302_CE=0;
	DS1302_SCLK=0;
}

/**
  * @brief  DS1302写一个字节
  * @param  Command 命令字/地址
  * @param  Data 要写入的数据
  * @retval 无
  */
void DS1302_WriteByte(unsigned char Command,Data)
{
	unsigned char i;
	DS1302_CE=1;
	for(i=0;i<8;i++)
	{
		DS1302_IO=Command&(0x01<<i);
		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}
	for(i=0;i<8;i++)
	{
		DS1302_IO=Data&(0x01<<i);
		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}
	DS1302_CE=0;
}

/**
  * @brief  DS1302读一个字节
  * @param  Command 命令字/地址
  * @retval 读出的数据
  */
unsigned char DS1302_ReadByte(unsigned char Command)
{
	unsigned char i,Data=0x00;
	Command|=0x01;	//将指令转换为读指令
	DS1302_CE=1;
	for(i=0;i<8;i++)
	{
		DS1302_IO=Command&(0x01<<i);
		DS1302_SCLK=0;
		DS1302_SCLK=1;
	}
	for(i=0;i<8;i++)
	{
		DS1302_SCLK=1;
		DS1302_SCLK=0;
		if(DS1302_IO){Data|=(0x01<<i);}
	}
	DS1302_CE=0;
	DS1302_IO=0;	//读取后将IO设置为0，否则读出的数据会出错
	return Data;
}

/**
  * @brief  DS1302设置时间，调用之后，DS1302_Time数组的数字会被设置到DS1302中
  * @param  无
  * @retval 无
  */
void DS1302_SetTime(void)
{
	DS1302_WriteByte(DS1302_WP,0x00);
	DS1302_WriteByte(DS1302_YEAR,DS1302_Time[0]/10*16+DS1302_Time[0]%10);//十进制转BCD码后写入
	DS1302_WriteByte(DS1302_MONTH,DS1302_Time[1]/10*16+DS1302_Time[1]%10);
	DS1302_WriteByte(DS1302_DATE,DS1302_Time[2]/10*16+DS1302_Time[2]%10);
	DS1302_WriteByte(DS1302_HOUR,DS1302_Time[3]/10*16+DS1302_Time[3]%10);
	DS1302_WriteByte(DS1302_MINUTE,DS1302_Time[4]/10*16+DS1302_Time[4]%10);
	DS1302_WriteByte(DS1302_SECOND,DS1302_Time[5]/10*16+DS1302_Time[5]%10);
	DS1302_WriteByte(DS1302_DAY,DS1302_Time[6]/10*16+DS1302_Time[6]%10);
	DS1302_WriteByte(DS1302_WP,0x80);
}

/**
  * @brief  DS1302读取时间，调用之后，DS1302中的数据会被读取到DS1302_Time数组中
  * @param  无
  * @retval 无
  */
void DS1302_ReadTime(void)
{
	unsigned char Temp;
	Temp=DS1302_ReadByte(DS1302_YEAR);
	DS1302_Time[0]=Temp/16*10+Temp%16;//BCD码转十进制后读取
	Temp=DS1302_ReadByte(DS1302_MONTH);
	DS1302_Time[1]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_DATE);
	DS1302_Time[2]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_HOUR);
	DS1302_Time[3]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_MINUTE);
	DS1302_Time[4]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_SECOND);
	DS1302_Time[5]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_DAY);
	DS1302_Time[6]=Temp/16*10+Temp%16;
}

void LCD1602_Show_Time(void)
{
	LCD_ShowNum(2,1,DS1302_Time[0],2);
	LCD_ShowString(2,3,",");
	LCD_ShowNum(2,4,DS1302_Time[1],2);
	LCD_ShowString(2,6,",");
	LCD_ShowNum(2,7,DS1302_Time[2],2);
	LCD_ShowNum(2,9,DS1302_Time[3],2);
	LCD_ShowString(2,11,":");
	LCD_ShowNum(2,12,DS1302_Time[4],2);
	LCD_ShowString(2,14,":");
	LCD_ShowNum(2,15,DS1302_Time[5],2);
}


