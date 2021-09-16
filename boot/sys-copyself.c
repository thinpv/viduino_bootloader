/*
 * sys-copyself.c
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
#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

extern unsigned char __image_start[];
extern unsigned char __image_end[];
extern unsigned char __heap_start[];
extern void sys_mmu_init(void);

struct zdesc_t
{												 /* Total 256 bytes */
	uint8_t magic[4];			 /* ZB??, I for bind id, E for encrypt image */
	uint8_t key[32];			 /* Aes256 encrypt key (hardcode or efuse suggested) */
	uint8_t sha256[32];		 /* Sha256 hash */
	uint8_t signature[64]; /* Ecdsa256 signature of sha256 */
	uint8_t csize[4];			 /* Compress size of image */
	uint8_t dsize[4];			 /* Decompress size of image */
	uint8_t public[33];		 /* Ecdsa256 public key (hardcode suggested) */
	uint8_t majoy;				 /* Majoy version */
	uint8_t minior;				 /* Minior version */
	uint8_t patch;				 /* Patch version */
	uint8_t message[80];	 /* Message additionally */
};

void sys_copyself(void)
{
	struct zdesc_t *z;
	uint32_t csize, dsize;
	void *mem, *tmp;
	uint32_t size;
	z = (struct zdesc_t *)__heap_start;
	mem = (void *)__image_start;
	tmp = (void *)z + sizeof(struct zdesc_t);
	size = __image_end - __image_start;
	sys_mmu_init();
	sys_spi_flash_init();
	sys_spi_flash_read(24576, z, sizeof(struct zdesc_t));
	if ((z->magic[0] == 'Z') && (z->magic[1] == 'B') && ((z->magic[2] == 'I') || (z->magic[2] == 0)) && ((z->magic[3] == 'E') || (z->magic[3] == 0)))
	{
		sys_uart_putc('z');
		sys_crypt((char *)z->key, (char *)z->sha256, sizeof(struct zdesc_t) - 36);
		{
			sys_uart_putc('2');
			csize = (z->csize[0] << 24) | (z->csize[1] << 16) | (z->csize[2] << 8) | (z->csize[3] << 0);
			dsize = (z->dsize[0] << 24) | (z->dsize[1] << 16) | (z->dsize[2] << 8) | (z->dsize[3] << 0);
			sys_spi_flash_read(24576 + sizeof(struct zdesc_t), mem, csize);
			// {
			// 	if (z->magic[3] == 'E')
			// 	{
			// 		sys_crypt((char *)z->key, tmp, csize);
			// 	}
			// 	sys_decompress(tmp, csize, mem, dsize);
			// }
		}
	}
	else
	{
		sys_spi_flash_read(0, mem, size);
	}
	sys_spi_flash_exit();
}
