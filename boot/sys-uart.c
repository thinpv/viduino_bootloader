/*
 * sys-uart.c
 *
 * Copyright(c) 2007-2021 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

// #include <xboot.h>
#include <types.h>
#include <io.h>

void sys_uart_init(void)
{
	virtual_addr_t addr;
	u32_t val;

	/* Config GPIOE1 and GPIOE0 to txd0 and rxd0 */
	addr = 0x01c20890 + 0x00;
	val = read32(addr);
	val &= ~(0xf << ((1 & 0x7) << 2));
	val |= ((0x5 & 0x7) << ((1 & 0x7) << 2));
	write32(addr, val);

	val = read32(addr);
	val &= ~(0xf << ((0 & 0x7) << 2));
	val |= ((0x5 & 0x7) << ((0 & 0x7) << 2));
	write32(addr, val);

	/* Open the clock gate for uart0 */
	addr = 0x01c20068;
	val = read32(addr);
	val |= 1 << 20;
	write32(addr, val);

	/* Deassert uart0 reset */
	addr = 0x01c202d0;
	val = read32(addr);
	val |= 1 << 20;
	write32(addr, val);

	/* Config uart0 to 115200-8-1-0 */
	addr = 0x01c25000;
	write32(addr + 0x04, 0x0);
	write32(addr + 0x08, 0xf7);
	write32(addr + 0x10, 0x0);
	val = read32(addr + 0x0c);
	val |= (1 << 7);
	write32(addr + 0x0c, val);
	write32(addr + 0x00, 0x36 & 0xff);
	write32(addr + 0x04, (0x36 >> 8) & 0xff);
	val = read32(addr + 0x0c);
	val &= ~(1 << 7);
	write32(addr + 0x0c, val);
	val = read32(addr + 0x0c);
	val &= ~0x1f;
	val |= (0x3 << 0) | (0 << 2) | (0x0 << 3);
	write32(addr + 0x0c, val);
}

void sys_uart_putc(char c)
{
	virtual_addr_t addr = 0x01c25000;

	while((read32(addr + 0x7c) & (0x1 << 1)) == 0);
	write32(addr + 0x00, c);
}

// VOID sysPutString(INT8 *string)
// {
// 		while (*string != '\0')
// 		{
// 			Sys_Uart_Putc(*string);
// 			string++;
// 		}

// }

// static VOID sysPutRepChar(INT8 c, INT count)
// {
// 	while (count--)
// 	Sys_Uart_Putc(c);
// }


// static VOID sysPutStringReverse(INT8 *s, INT index)
// {
// 	while ((index--) > 0)
// 	Sys_Uart_Putc(s[index]);
// }

// static VOID sysPutNumber(INT value, INT radix, INT width, INT8 fill)
// {
// 	INT8    buffer[40];
// 	INT     bi = 0;
// 	UINT32  uvalue;
// 	UINT16  digit;
// 	UINT16  left = FALSE;
// 	UINT16  negative = FALSE;

// 	if (fill == 0)
// 	    	fill = ' ';

// 	if (width < 0)
// 	{
// 		width = -width;
// 		left = TRUE;
// 	}

// 	if (width < 0 || width > 80)
// 	    	width = 0;

// 	if (radix < 0)
// 	{
// 		radix = -radix;
// 		if (value < 0)
// 		{
// 			negative = TRUE;
// 			value = -value;
// 	    	}
// 	}

// 	uvalue = value;

// 	do
// 	{
// 		if (radix != 16)
// 		{
// 			digit = uvalue % radix;
// 			uvalue = uvalue / radix;
// 		}
// 		else
// 		{
// 			digit = uvalue & 0xf;
// 			uvalue = uvalue >> 4;
// 		}
// 		buffer[bi] = digit + ((digit <= 9) ? '0' : ('A' - 10));
// 		bi++;

// 		if (uvalue != 0)
// 		{
// 			if ((radix == 10)
// 			    && ((bi == 3) || (bi == 7) || (bi == 11) | (bi == 15)))
// 			{
// 				buffer[bi++] = ',';
// 			}
// 		}
// 	}
// 	while (uvalue != 0);

// 	if (negative)
// 	{
// 		buffer[bi] = '-';
// 		bi += 1;
// 	}

// 	if (width <= bi)
// 		sysPutStringReverse(buffer, bi);
// 	else
// 	{
// 		width -= bi;
// 		if (!left)
// 			sysPutRepChar(fill, width);
// 		sysPutStringReverse(buffer, bi);
// 		if (left)
// 		    	sysPutRepChar(fill, width);
// 	}
// }


// static INT8 *FormatItem(INT8 *f, INT a)
// {
// 	INT8   c;
// 	INT    fieldwidth = 0;
// 	INT    leftjust = FALSE;
// 	INT    radix = 0;
// 	INT8   fill = ' ';

// 	if (*f == '0')
// 		fill = '0';

// 	while ((c = *f++) != 0)
// 	{
// 		if (c >= '0' && c <= '9')
// 		{
// 			fieldwidth = (fieldwidth * 10) + (c - '0');
// 		}
// 		else
// 			switch (c)
// 			{
// 				case '\000':
// 					return (--f);
// 				case '%':
// 				    	Sys_Uart_Putc('%');
// 				    	return (f);
// 				case '-':
// 				    	leftjust = TRUE;
// 				    	break;
// 				case 'c':
// 				{
// 				        if (leftjust)
// 				        	Sys_Uart_Putc(a & 0x7f);

// 				        if (fieldwidth > 0)
// 				            	sysPutRepChar(fill, fieldwidth - 1);

// 				        if (!leftjust)
// 				            	Sys_Uart_Putc(a & 0x7f);
// 				        return (f);
// 				}
// 				case 's':
// 				{
// 				        if (leftjust)
// 				        	sysPutString((PINT8)a);

// 				        if (fieldwidth > strlen((PINT8)a))
// 				            	sysPutRepChar(fill, fieldwidth - strlen((PINT8)a));

// 				        if (!leftjust)
// 				           	sysPutString((PINT8)a);
// 				        return (f);
// 				}
// 				case 'd':
// 				case 'i':
// 				   	 radix = -10;
// 				break;
// 				case 'u':
// 				    	radix = 10;
// 				break;
// 				case 'x':
// 				    	radix = 16;
// 				break;
// 				case 'X':
// 				    	radix = 16;
// 				break;
// 				case 'o':
// 				    	radix = 8;
// 				break;
// 				default:
// 				    	radix = 3;
// 				break;      /* unknown switch! */
// 			}
// 		if (radix)
// 		    break;
// 	}

// 	if (leftjust)
// 	    	fieldwidth = -fieldwidth;

// 	sysPutNumber(a, radix, fieldwidth, fill);

// 	return (f);
// }


// void sysprintf(char * pcStr,...)
// {
// 	char *argP;

// 	vaStart(argP, pcStr);       /* point at the end of the format string */
// 	while (*pcStr)
// 	{                       /* this works because args are all ints */
// 		if (*pcStr == '%')
// 					pcStr = FormatItem(pcStr + 1, vaArg(argP, INT));
// 		else
// 					Sys_Uart_Putc(*pcStr++);
// 	}
// }
