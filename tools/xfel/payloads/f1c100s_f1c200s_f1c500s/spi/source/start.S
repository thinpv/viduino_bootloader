/*
 * start.S
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

	.global _start
_start:
	b reset

reset:
	ldr r0, =0x00000040
	str sp, [r0, #0]
	str lr, [r0, #4]
	mrs lr, cpsr
	str lr, [r0, #8]
	mrc p15, 0, lr, c1, c0, 0
	str lr, [r0, #12]
	mrc p15, 0, lr, c1, c0, 0
	str lr, [r0, #16]

	ldr r0, =0x00009800
	bl sys_spi_run

	mov r0, #0x4
	mov r1, #'e'
	strb r1, [r0, #0]
	mov r1, #'G'
	strb r1, [r0, #1]
	mov r1, #'O'
	strb r1, [r0, #2]
	mov r1, #'N'
	strb r1, [r0, #3]
	mov r1, #'.'
	strb r1, [r0, #4]
	mov r1, #'F'
	strb r1, [r0, #5]
	mov r1, #'E'
	strb r1, [r0, #6]
	mov r1, #'L'
	strb r1, [r0, #7]
	ldr r0, =0x00000040
	ldr sp, [r0, #0]
	ldr lr, [r0, #4]
	ldr r1, [r0, #16]
	mcr p15, 0, r1, c1, c0, 0
	ldr r1, [r0, #12]
	mcr p15, 0, r1, c1, c0, 0
	ldr r1, [r0, #8]
	msr cpsr, r1
	bx lr
