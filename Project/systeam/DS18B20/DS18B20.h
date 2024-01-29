#ifndef __DS18B20_H__
#define __DS18B20_H__
extern float T;
void DS18B20_ConvertT(void);
float DS18B20_ReadT(void);
void LCD1602_Show_T(void);
#endif
