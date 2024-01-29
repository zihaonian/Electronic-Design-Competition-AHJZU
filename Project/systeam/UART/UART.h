#ifndef __UART_H__
#define __UART_H__

#ifndef EOF
 #define EOF -1
#endif

#ifndef NULL
 #define NULL ((void *) 0)
#endif

#ifndef _SIZE_T
 #define _SIZE_T
 typedef unsigned int size_t;
#endif

#pragma SAVE
#pragma REGPARMS
extern void UartInit(void);
extern int printf   (const char *, ...);      //用这个	 模板：printf("%.4f\n",str);	
 
#pragma RESTORE

//管脚定义：

//函数声明：

#endif




