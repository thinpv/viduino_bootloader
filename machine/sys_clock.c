
#include "sys_io.h"
#include "reg_ccu.h"

static inline void sdelay(int loops)
{
int s;
	for(s=0;s<300;s++);
}

static void wait_pll_stable(u32_t base)
{
	u32_t rval = 0;
	u32_t time = 0x1fff;//0xfff->0x1fff

	do {
		rval = read32(base);
		time--;
	} while(time && !(rval & (1 << 28)));
}

static void clock_set_pll_cpu(u32_t clk)
{
	u32_t n, k, m, p;
	u32_t rval = 0;
	u32_t div = 0;

	if(clk > 720000000)
		clk = 720000000;

	if((clk % 24000000) == 0)
	{
		div = clk / 24000000;
		n = div - 1;
		k = 0;
		m = 0;
		p = 0;
	}
	else if((clk % 12000000) == 0)
	{
		m = 1;
		div = clk / 12000000;
		if((div % 3) == 0)
			k = 2;
		else if((div % 4) == 0)
			k = 3;
		else
			k = 1;
		n = (div / (k + 1)) - 1;
		p = 0;
	}
	else
	{
		div = clk / 24000000;
		n = div - 1;
		k = 0;
		m = 0;
		p = 0;
	}

	rval = read32(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL);
	rval &= ~((0x3 << 16) | (0x1f << 8) | (0x3 << 4) | (0x3 << 0));
	rval |= (1U << 31) | (p << 16) | (n << 8) | (k << 4) | m;
	write32(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL, rval);
	wait_pll_stable(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL);
}

void sys_clock_init(int HZ)
{
	u32_t val;

	write32(F1C100S_CCU_BASE + CCU_PLL_STABLE_TIME0, 0x1ff);
	write32(F1C100S_CCU_BASE + CCU_PLL_STABLE_TIME1, 0x1ff);

	val = read32(F1C100S_CCU_BASE + CCU_CPU_CFG);
	val &= ~(0x3 << 16);
	val |= (0x1 << 16);
	write32(F1C100S_CCU_BASE + CCU_CPU_CFG, val);
	sdelay(100);

	write32(F1C100S_CCU_BASE + CCU_PLL_VIDEO_CTRL, 0x81004107);
	sdelay(100);
	write32(F1C100S_CCU_BASE + CCU_PLL_PERIPH_CTRL, 0x80041800);
	sdelay(100);
	//write32(F1C100S_CCU_BASE + CCU_AHB_APB_CFG, 0x00003180);
	write32(F1C100S_CCU_BASE + CCU_AHB_APB_CFG, 0x00012110);
	sdelay(100);

	val = read32(F1C100S_CCU_BASE + CCU_DRAM_CLK_GATE);
	val |= (0x1 << 26) | (0x1 << 24);
	write32(F1C100S_CCU_BASE + CCU_DRAM_CLK_GATE, val);
	sdelay(100);

	clock_set_pll_cpu(HZ);
	val = read32(F1C100S_CCU_BASE + CCU_CPU_CFG);
	val &= ~(0x3 << 16);
	val |= (0x2 << 16);
	write32(F1C100S_CCU_BASE + CCU_CPU_CFG, val);
	sdelay(100);//100->200
}

