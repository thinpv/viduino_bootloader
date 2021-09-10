#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dma.h>
#include <F1C100S.h>
#include <sys-spiflash.h>

#include "Precomp.h"
#include "Alloc.h"
#include "LzmaDec.h"

extern unsigned char __out_start[];
extern unsigned char __in_start[];
extern void jump_to_app(uint32_t entry);

int boot_main(int argc, char **argv)
{
	uint8_t boot_to_app = 0;
	printf("boot_main:\r\n");

	do_init_mem_pool();
	irq_init();
	timer0_set();

	gpio_set_dir(GPIOF, 2, GPIO_DIRECTION_INPUT);
	gpio_set_pull(GPIOF, 2, GPIO_PULL_UP);
	for (uint8_t i = 0; i < 5; i++)
	{
		if (gpio_get_value(GPIOF, 2) != 0)
		{
			boot_to_app = 1;
			break;
		}
	}
	gpio_set_pull(GPIOF, 2, GPIO_PULL_NONE);
	gpio_set_dir(GPIOE, 2, GPIO_DIRECTION_OUTPUT);
	gpio_set_value(GPIOE, 2, 0);
	if (boot_to_app)
	{
		uint32_t dataSize;
		uint8_t *inBuffer = __in_start;
		uint32_t inSize;
		uint8_t *outBuffer = __out_start;
		size_t outSize = 0;
		ELzmaStatus status;
		char *p;

		printf("Boot to App\r\n");
		sys_spi_flash_init();
		dataSize = sys_spi_flash_read_data_size();
		sys_spi_flash_read(FLASH_APP_START_ADDRESS, inBuffer, dataSize);
		sys_spi_flash_exit();
		printf("dataSize: %d\r\n", dataSize);

		for (int i = 0; i < 4; i++)
		{
			outSize |= inBuffer[LZMA_PROPS_SIZE + i] << 8 * i;
		}
		printf("outSize: %d\r\n", outSize);

		inSize = dataSize - (LZMA_PROPS_SIZE + 8);
		inBuffer += (LZMA_PROPS_SIZE + 8);

		printf("start decode\r\n");
		int time = millis();
		/* Decompress stage2 into memory */
		SRes res = LzmaDecode(outBuffer,
													&outSize,
													inBuffer,
													&inSize,
													__in_start,
													LZMA_PROPS_SIZE,
													LZMA_FINISH_END,
													&status,
													(ISzAlloc *)&g_Alloc);
		printf("res: %d, time: %d\r\n", res, millis() - time);
		printf("inSize: %d\r\n", inSize);
		// sys_print_exit();
		if (res != 0)
		{
			memcpy(__out_start, __in_start, dataSize);
		}
		jump_to_app(outBuffer);
	}
	else
	{
		printf("Boot to Bootloader\r\n");
		sys_spi_flash_init();
		slip_start();
		sys_spi_flash_exit();
	}

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
