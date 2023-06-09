#ifndef __IRQ_H__
#define __IRQ_H__

typedef void (*sys_pvFunPtr)();

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

/* Max number of interruptions */
#define INTERRUPTS_MAX (64)
/* a group num */
#define GROUP_NUM (32)
/* Interrupt Source */
#define F1C100S_IRQ_NMI (0)
#define F1C100S_IRQ_UART0 (1)
#define F1C100S_IRQ_UART1 (2)
#define F1C100S_IRQ_UART2 (3)
#define F1C100S_IRQ_SPDIF (5)
#define F1C100S_IRQ_CIR (6)
#define F1C100S_IRQ_I2C0 (7)
#define F1C100S_IRQ_I2C1 (8)
#define F1C100S_IRQ_I2C2 (9)
#define F1C100S_IRQ_SPI0 (10)
#define F1C100S_IRQ_SPI1 (11)
#define F1C100S_IRQ_TIMER0 (13)
#define F1C100S_IRQ_TIMER1 (14)
#define F1C100S_IRQ_TIMER2 (15)
#define F1C100S_IRQ_WDOG (16)
#define F1C100S_IRQ_RSB (17)
#define F1C100S_IRQ_DMA (18)
#define F1C100S_IRQ_TP (20)
#define F1C100S_IRQ_AUDIO (21)
#define F1C100S_IRQ_KEYADC (22)
#define F1C100S_IRQ_MMC0 (23)
#define F1C100S_IRQ_MMC1 (24)
#define F1C100S_IRQ_USBOTG (26)
#define F1C100S_IRQ_TVD (27)
#define F1C100S_IRQ_TVE (28)
#define F1C100S_IRQ_LCD (29)
#define F1C100S_IRQ_DEFE (30)
#define F1C100S_IRQ_DEBE (31)
#define F1C100S_IRQ_CSI (32)
#define F1C100S_IRQ_DEITLA (33)
#define F1C100S_IRQ_VE (34)
#define F1C100S_IRQ_I2S (35)
#define F1C100S_IRQ_GPIOD (38)
#define F1C100S_IRQ_GPIOE (39)
#define F1C100S_IRQ_GPIOF (40)

#define F1C100S_IRQ_GPIOD0 (64)
#define F1C100S_IRQ_GPIOD1 (65)
#define F1C100S_IRQ_GPIOD2 (66)
#define F1C100S_IRQ_GPIOD3 (67)
#define F1C100S_IRQ_GPIOD4 (68)
#define F1C100S_IRQ_GPIOD5 (69)
#define F1C100S_IRQ_GPIOD6 (70)
#define F1C100S_IRQ_GPIOD7 (71)
#define F1C100S_IRQ_GPIOD8 (72)
#define F1C100S_IRQ_GPIOD9 (73)
#define F1C100S_IRQ_GPIOD10 (74)
#define F1C100S_IRQ_GPIOD11 (75)
#define F1C100S_IRQ_GPIOD12 (76)
#define F1C100S_IRQ_GPIOD13 (77)
#define F1C100S_IRQ_GPIOD14 (78)
#define F1C100S_IRQ_GPIOD15 (79)
#define F1C100S_IRQ_GPIOD17 (80)
#define F1C100S_IRQ_GPIOD18 (81)
#define F1C100S_IRQ_GPIOD19 (82)
#define F1C100S_IRQ_GPIOD20 (83)
#define F1C100S_IRQ_GPIOD21 (84)

#define F1C100S_IRQ_GPIOE0 (96)
#define F1C100S_IRQ_GPIOE1 (97)
#define F1C100S_IRQ_GPIOE2 (98)
#define F1C100S_IRQ_GPIOE3 (99)
#define F1C100S_IRQ_GPIOE4 (100)
#define F1C100S_IRQ_GPIOE5 (101)
#define F1C100S_IRQ_GPIOE6 (102)
#define F1C100S_IRQ_GPIOE7 (103)
#define F1C100S_IRQ_GPIOE8 (104)
#define F1C100S_IRQ_GPIOE9 (105)
#define F1C100S_IRQ_GPIOE10 (106)
#define F1C100S_IRQ_GPIOE11 (107)
#define F1C100S_IRQ_GPIOE12 (108)

#define F1C100S_IRQ_GPIOF0 (128)
#define F1C100S_IRQ_GPIOF1 (129)
#define F1C100S_IRQ_GPIOF2 (130)
#define F1C100S_IRQ_GPIOF3 (131)
#define F1C100S_IRQ_GPIOF4 (132)
#define F1C100S_IRQ_GPIOF5 (133)

#define FIQ_LEVEL_0 0
#define IRQ_LEVEL_1 1
#define IRQ_LEVEL_2 2
#define IRQ_LEVEL_3 3
#define IRQ_LEVEL_4 4
#define IRQ_LEVEL_5 5
#define IRQ_LEVEL_6 6
#define IRQ_LEVEL_7 7

/* The parameters for sysSetLocalInterrupt() use */
#define ENABLE_IRQ 0x7F
#define ENABLE_FIQ 0xBF
#define ENABLE_FIQ_IRQ 0x3F
#define DISABLE_IRQ 0x80
#define DISABLE_FIQ 0x40
#define DISABLE_FIQ_IRQ 0xC0

	extern sys_pvFunPtr sysIrqHandlerTable[];

	void irq_register(int32_t nIntTypeLevel, int32_t eIntNo, sys_pvFunPtr pvNewISR, uint8_t Priority);
	void irq_enable(int32_t eIntNo);
	void irq_disable(int32_t eIntNo);
	void irq_init();
	int32_t irq_handle();

#ifdef __cplusplus
}
#endif

#endif
