#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dma.h>

// #define FIC 1

// #include "slip.h"

#include "Precomp.h"
#include "Alloc.h"
#include "LzmaDec.h"

// #include <_acAlloc.h>
// #include <_aclv_demo_music.h>

#define SIZE_4K 0x1000
#define SIZE_64K 0x10000

#define FLASH_APP_LEN_START_ADDRESS (0x00200000 - SIZE_4K)
#define FLASH_APP_START_ADDRESS 0x00200000

extern void sys_spinor_init(void);
extern void sys_spinor_exit(void);
extern void sys_spinor_read(int addr, void *buf, int count);

extern unsigned char __out_start[];
extern unsigned char __in_start[];
extern void jump_to_app(uint32_t entry);

int boot_main(int argc, char **argv)
{
	printf("boot_main:\r\n");

	do_init_mem_pool();

	char *inBuffer = __in_start;
	uint32_t inSize;
	char *outBuffer = __out_start; //malloc(outSize);
	size_t outSize;
	ELzmaStatus status;
	char *p;

	printf("copy data from spi\r\n");
	sys_spinor_init();
	sys_spinor_read(FLASH_APP_LEN_START_ADDRESS, (uint8_t *)&inSize, 4);
	sys_spinor_read(FLASH_APP_START_ADDRESS, inBuffer, inSize);
	sys_spinor_exit();

	outSize = ((size_t)((unsigned char)inBuffer[LZMA_PROPS_SIZE]) +
						 ((size_t)((unsigned char)inBuffer[LZMA_PROPS_SIZE + 1]) << 8) +
						 ((size_t)((unsigned char)inBuffer[LZMA_PROPS_SIZE + 2]) << 16));
	printf("outSize: %d\r\n", outSize);

	inSize -= (LZMA_PROPS_SIZE + 8);
	inBuffer += (LZMA_PROPS_SIZE + 8);

	printf("start decode\r\n");
	/* Decompress stage2 into memory */
	SRes res = LzmaDecode(outBuffer,
												&outSize,
												inBuffer,
												&inSize,
												inBuffer - (LZMA_PROPS_SIZE + 8),
												LZMA_PROPS_SIZE,
												LZMA_FINISH_END,
												&status,
												(ISzAlloc *)&g_Alloc);

	printf("res: %d\r\n", res);

	jump_to_app(outBuffer);

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

// int printf(const char *format, ...)
// {
// }

void interrupt_handle_exception(void *regs)
{
}