#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dma.h>
#include <F1C100S.h>
#include <sys-spiflash.h>
#include <arm32.h>

#include <timer.h>
#include <sys-uart.h>
#include <slip.h>

#include "Precomp.h"
#include "Alloc.h"
#include "LzmaDec.h"
#include <md5sum.h>

extern unsigned char __out_start[];
extern unsigned char __in_start[];
extern void jump_to_app(void *entry);

#ifndef VERSION
#define VERSION "2.0.0"
#endif

static MD5state md5state;
static byte digest[16];

static uint8_t retry = 0;

typedef struct
{
	char magic[8];	   /* viduino   */
	uint32_t in_size;  /* size   */
	uint32_t out_size; /* lzma     */
	uint8_t md5[16];   /* md5   */
} image_header_t;

int boot_main(int argc, char **argv)
{
	uint8_t boot_to_app = 0;
	uint32_t ram = (*(uint32_t *)0x5c) & 0xFFFFFF;
	irq_init();
	do_init_mem_pool();
	timer0_set();
	sys_spi_flash_init();

	printf("\r\nViduino bootloader " VERSION " ram %dMB\r\n", ram);
	gpio_set_dir(GPIOF, 2, GPIO_DIRECTION_INPUT);
	gpio_set_pull(GPIOF, 2, GPIO_PULL_UP);

	for (uint8_t i = 0; i < 5; i++)
	{
		boot_to_app = gpio_get_value(GPIOF, 2);
		if (boot_to_app)
			break;
	}
	gpio_set_pull(GPIOF, 2, GPIO_PULL_NONE);
boot_to_app:
	if (boot_to_app && retry < 5)
	{
		image_header_t header;
		uint8_t *inBuffer = __in_start;
		size_t inSize;
		uint8_t *outBuffer = __out_start;
		size_t outSize = 0;
		ELzmaStatus status;

		printf("Boot to App\r\n");
		sys_spi_flash_read(FLASH_APP_START_ADDRESS, (uint8_t *)&header, 32);
		if (strcmp("viduino", header.magic) == 0)
		{
			printf("in_size: %d\r\n", header.in_size);
			printf("out_size: %d\r\n", header.out_size);
			printf("md5: ");
			for (uint8_t i = 0; i < 16; i++)
				printf("%02x", header.md5[i]);
			printf("\r\n");
			if (header.out_size > 0)
			{
				sys_spi_flash_read(FLASH_APP_START_ADDRESS + 32, __in_start, header.in_size);
				for (int i = 0; i < 4; i++)
					outSize |= inBuffer[LZMA_PROPS_SIZE + i] << 8 * i;
				inSize = header.in_size - (LZMA_PROPS_SIZE + 8);
				inBuffer += (LZMA_PROPS_SIZE + 8);
				SRes res = LzmaDecode(outBuffer,
									  &outSize,
									  inBuffer,
									  &inSize,
									  __in_start,
									  LZMA_PROPS_SIZE,
									  LZMA_FINISH_END,
									  &status,
									  (ISzAlloc *)&g_Alloc);
				if (res == SZ_OK)
				{
					printf("inSize: %d\r\n", inSize);
					printf("outSize: %d\r\n", outSize);
					if (outSize != header.out_size)
					{
						printf("file size not match\r\n");
						retry++;
						goto boot_to_app;
					}
				}
				else
				{
					printf("unzip lzma error: %d\r\n", res);
					retry++;
					goto boot_to_app;
				}
				memset(__in_start, 0, header.in_size);
			}
			else
			{
				sys_spi_flash_read(FLASH_APP_START_ADDRESS + 32, __out_start, header.in_size);
				outSize = header.in_size;
			}
#if 0 // dump data
			printf("data: ");
			for (uint32_t i = 0; i < outSize; i++)
			{
				if (i % 16 == 0)
					printf("\r\n0x%02x: ", i);
				else if (i % 4 == 0)
					printf(" ");
				printf("%02x", __out_start[i]);
			}
			printf("\r\n");
#endif
			md5sum_init(&md5state);
			md5(__out_start, outSize, digest, &md5state);
			for (int i = 0; i < 16; i++)
			{
				if (digest[i] != header.md5[i])
				{
					printf("md5 not match: ");
					for (uint8_t i = 0; i < 16; i++)
						printf("%02x", digest[i]);
					printf("\r\n");
					retry++;
					goto boot_to_app;
				}
			}
			printf("md5 match\r\n");
			sys_spi_flash_exit();
			// util_disable_interrupt();
			void (*fw_func)(void);
			fw_func = (void (*)(void))(__out_start);
			fw_func();
		}
	}
	printf("Boot to Bootloader\r\n");
	slip_start();
	sys_spi_flash_exit();

	while (1)
	{
		printf("loop\r\n");
		delay(50);
	}
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
	// printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
	__fatal_error("Assertion failed");
}
#endif
