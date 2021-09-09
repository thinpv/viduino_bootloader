#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dma.h>

// #define FIC 1

// #include "slip.h"

int boot_main(int argc, char **argv)
{
	sys_uart_putc('m');
	sys_uart_putc('a');
	sys_uart_putc('i');
	sys_uart_putc('n');
	sys_uart_putc('\r');
	sys_uart_putc('\n');
	printf("boot_main:\r\n");
	// sys_init();
	// unsigned int CPU_Frequency_hz=408*1000000;//HZ

	// sys_dram_init(32*FIC);
	// sys_clock_init(CPU_Frequency_hz);
	// sys_spi_flash_init();
	// Sys_Uart_Init(UART0,CPU_Frequency_hz,115200,0);
	// Sys_SET_UART_DBG(UART0);

	// sysprintf("\r\n\r\n\r\n");
	// sysprintf("F1C%d00S BOOT START...\r\n",FIC);
	// sysprintf("XiaoTaoQiKJ [2020-07-25]...\r\n");
	// sysprintf("QQ:718595426\r\n\r\n");
	// sysprintf("CPU Frequency:%d Hz\r\n",CPU_Frequency_hz);
	// sysprintf("DRAM Size: %d MB\r\n",32*FIC);

	// sys_print_init();
	// sys_print_c('A');
	// sys_print_c('\r');
	// sys_print_c('\n');
	// sys_print_s("sys_copyself\r\n");
	// printf("Boot to Bootloader\r\n");
	// sys_copyself();

	// /* Do initial mem pool */
	// do_init_mem_pool();
	// do_init_dma_pool();

	// // sys_print_init();
	// irq_init();
	// timer0_set();

	// sys_spi_flash_init();
	// printf("Boot to Bootloader\r\n");
	// printf("Old app data len: %ld\r\n", sys_spi_flash_read_data_size());
	// slip_start();
	// sys_spi_flash_exit();
	printf("Flash finish\r\n");
	// sys_print_exit();

	while (1)
		;

	return 0;
}

void __fatal_error(const char *msg)
{
	while (1)
		;
}

void __cxa_pure_virtual()
{
	while (1)
		;
}

#ifndef NDEBUG
void __assert_func(const char *file, int line, const char *func, const char *expr)
{
	//printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
	__fatal_error("Assertion failed");
}
#endif

int printf(const char *format, ...)
{
}

void interrupt_handle_exception(void *regs)
{
}