#include "sys_uart.h"
#include "sys_gpio.h"
#include "sys_types.h"
#include <string.h>
#include <stdio.h>
#include "sys_io.h"
#include "sys_interrupt.h"
#include "sys_delay.h"

#define vaArg(list, type) ((type *)(list += sizeof(type)))[-1]
#define vaStart(list, param) list = (INT8*)((INT)&param + sizeof(param))
	

#define UART0_ADDR 0x01c25000
#define UART1_ADDR 0x01C25400
#define UART2_ADDR 0x01c25800


/*---------------------------------------------------
串口0中断
----------------------------------------------------*/
void UART0_ISR(void)
{
	u32_t addr=UART0_ADDR;
	u32 s=read32(addr+ 0x08);
	if((s&0xff)==0xc4)//接收中断
	{
		sysprintf("d=%02x\r\n",read32(addr + 0x00));
	}
}
/*---------------------------------------------------
串口1中断
----------------------------------------------------*/
void UART1_ISR(void)
{
	u32_t addr=UART1_ADDR;
	u32 s=read32(addr+ 0x08);
	if((s&0xff)==0xc4)//接收中断
	{
		sysprintf("d=%02x\r\n",read32(addr + 0x00));
	}
}
/*---------------------------------------------------
串口2中断
----------------------------------------------------*/
void UART2_ISR(void)
{
	u32_t addr=UART2_ADDR;
	u32 s=read32(addr+ 0x08);
	if((s&0xff)==0xc4)//接收中断
	{
		sysprintf("d=%02x\r\n",read32(addr + 0x00));
	}
}

/*
串口初始化
*/
void Sys_Uart_Init(enum eUart uart,unsigned int cpu_frequency,unsigned int Baudrate,int Interrupt)
{
	u32_t addr;
	u32_t val,Div,APBclk;
	u8 LW;
	
  APBclk=cpu_frequency/4;
	Div=APBclk/Baudrate/16;
	
	//UART引脚配置
  switch(uart)
	{
		case UART0:
			addr = UART0_ADDR;
		  LW=20;
			// 配置输出脚
			GPIO_Congif(GPIOE,GPIO_Pin_0,GPIO_Mode_101,GPIO_PuPd_NOPULL);//RX
			GPIO_Congif(GPIOE,GPIO_Pin_1,GPIO_Mode_101,GPIO_PuPd_NOPULL);//TX
			break;
		case UART1:
			addr = UART1_ADDR;
		  LW=21;
			// 配置输出脚
			GPIO_Congif(GPIOA,GPIO_Pin_2,GPIO_Mode_101,GPIO_PuPd_NOPULL);//RX
			GPIO_Congif(GPIOA,GPIO_Pin_3,GPIO_Mode_101,GPIO_PuPd_NOPULL);//TX
			break;
		case UART2:
			addr = UART2_ADDR;		
		  LW=22;
			// 配置输出脚
			GPIO_Congif(GPIOE,GPIO_Pin_7,GPIO_Mode_011,GPIO_PuPd_NOPULL);//RX
			GPIO_Congif(GPIOE,GPIO_Pin_8,GPIO_Mode_011,GPIO_PuPd_NOPULL);//TX
			break;		
	}
	
  C_BIT(0x01c20068,LW);
	C_BIT(0x01c202d0,LW);
	delay_ms(1);

  S_BIT(0x01c20068,LW);
	S_BIT(0x01c202d0,LW);
  delay_ms(1);	

	// Config uart0 to xxxx-8-1-0//	
	write32(addr + 0x04, 0x01);
	write32(addr + 0x08, 0x37);
	write32(addr + 0x10, 0x0);
	
	val = read32(addr + 0x0c); //DLAB为1
	val |= (1 << 7);
	write32(addr + 0x0c, val);
	
	write32(addr + 0x00, Div & 0xff);//写波特率 APB时钟=408/4=102M{波特率=时钟/(16*除数)}
	write32(addr + 0x04, (Div >> 8) & 0xff);
	
	val = read32(addr + 0x0c);//DLAB为0
	val &= ~(1 << 7);
	write32(addr + 0x0c, val);
	
	val = read32(addr + 0x0c);//写控制位
	val &= ~0x1f;
	val |= (0x3 << 0) | (0 << 2) | (0x0 << 3);
	write32(addr + 0x0c, val);
	
	//使能中断
	if(Interrupt==1)
	{
		if(uart==UART0)IRQ_Init(IRQ_LEVEL_1,IRQ_UART0,UART0_ISR ,3);
		else if(uart==UART1)IRQ_Init(IRQ_LEVEL_1,IRQ_UART1,UART1_ISR ,3);
		else if(uart==UART2)IRQ_Init(IRQ_LEVEL_1,IRQ_UART2,UART2_ISR ,3);		
		sysSetLocalInterrupt(ENABLE_IRQ);
	}
}

/*
串口输出字符
*/
void Sys_Uart_Put(enum eUart uart,char c)
{
u32 addr;	
  switch(uart)
	{
		case UART0:
			addr = UART0_ADDR;
			break;
		case UART1:
			addr = UART1_ADDR;
			break;
		case UART2:
			addr = UART2_ADDR;		
			break;		
	}	
	while((read32(addr + 0x7c) & (0x1 << 1)) == 0);
	write32(addr + 0x0, c);
}

/*-----------------------------打印相关----------------------------------*/
/*
设置调试口
*/
int UART_DBG=0;
void Sys_SET_UART_DBG(enum eUart uart)
{
	UART_DBG = uart;
}
/*
输出一个字
*/
void Sys_Uart_Putc(char c)
{
	Sys_Uart_Put((enum eUart)UART_DBG,c);
}

VOID sysPutString(INT8 *string)
{
		while (*string != '\0')
		{
			Sys_Uart_Putc(*string);
			string++;
		}

}

static VOID sysPutRepChar(INT8 c, INT count)
{
	while (count--)
	Sys_Uart_Putc(c);
}


static VOID sysPutStringReverse(INT8 *s, INT index)
{
	while ((index--) > 0)
	Sys_Uart_Putc(s[index]);
}

static VOID sysPutNumber(INT value, INT radix, INT width, INT8 fill)
{
	INT8    buffer[40];
	INT     bi = 0;
	UINT32  uvalue;
	UINT16  digit;
	UINT16  left = FALSE;
	UINT16  negative = FALSE;

	if (fill == 0)
	    	fill = ' ';

	if (width < 0)
	{
		width = -width;
		left = TRUE;
	}

	if (width < 0 || width > 80)
	    	width = 0;

	if (radix < 0)
	{
		radix = -radix;
		if (value < 0)
		{
			negative = TRUE;
			value = -value;
	    	}
	}

	uvalue = value;

	do
	{
		if (radix != 16)
		{
			digit = uvalue % radix;
			uvalue = uvalue / radix;
		}
		else
		{
			digit = uvalue & 0xf;
			uvalue = uvalue >> 4;
		}
		buffer[bi] = digit + ((digit <= 9) ? '0' : ('A' - 10));
		bi++;

		if (uvalue != 0)
		{
			if ((radix == 10)
			    && ((bi == 3) || (bi == 7) || (bi == 11) | (bi == 15)))
			{
				buffer[bi++] = ',';
			}
		}
	}
	while (uvalue != 0);

	if (negative)
	{
		buffer[bi] = '-';
		bi += 1;
	}

	if (width <= bi)
		sysPutStringReverse(buffer, bi);
	else
	{
		width -= bi;
		if (!left)
			sysPutRepChar(fill, width);
		sysPutStringReverse(buffer, bi);
		if (left)
		    	sysPutRepChar(fill, width);
	}
}


static INT8 *FormatItem(INT8 *f, INT a)
{
	INT8   c;
	INT    fieldwidth = 0;
	INT    leftjust = FALSE;
	INT    radix = 0;
	INT8   fill = ' ';

	if (*f == '0')
		fill = '0';

	while ((c = *f++) != 0)
	{
		if (c >= '0' && c <= '9')
		{
			fieldwidth = (fieldwidth * 10) + (c - '0');
		}
		else
			switch (c)
			{
				case '\000':
					return (--f);
				case '%':
				    	Sys_Uart_Putc('%');
				    	return (f);
				case '-':
				    	leftjust = TRUE;
				    	break;
				case 'c':
				{
				        if (leftjust)
				        	Sys_Uart_Putc(a & 0x7f);

				        if (fieldwidth > 0)
				            	sysPutRepChar(fill, fieldwidth - 1);

				        if (!leftjust)
				            	Sys_Uart_Putc(a & 0x7f);
				        return (f);
				}
				case 's':
				{
				        if (leftjust)
				        	sysPutString((PINT8)a);

				        if (fieldwidth > strlen((PINT8)a))
				            	sysPutRepChar(fill, fieldwidth - strlen((PINT8)a));

				        if (!leftjust)
				           	sysPutString((PINT8)a);
				        return (f);
				}
				case 'd':
				case 'i':
				   	 radix = -10;
				break;
				case 'u':
				    	radix = 10;
				break;
				case 'x':
				    	radix = 16;
				break;
				case 'X':
				    	radix = 16;
				break;
				case 'o':
				    	radix = 8;
				break;
				default:
				    	radix = 3;
				break;      /* unknown switch! */
			}
		if (radix)
		    break;
	}

	if (leftjust)
	    	fieldwidth = -fieldwidth;

	sysPutNumber(a, radix, fieldwidth, fill);

	return (f);
}


void sysprintf(char * pcStr,...)
{
	char *argP;

	vaStart(argP, pcStr);       /* point at the end of the format string */
	while (*pcStr)
	{                       /* this works because args are all ints */
		if (*pcStr == '%')
					pcStr = FormatItem(pcStr + 1, vaArg(argP, INT));
		else
					Sys_Uart_Putc(*pcStr++);
	}
}


