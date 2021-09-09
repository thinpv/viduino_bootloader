#include "sys_lcd.h"
#include "reg_debe.h"
#include "reg_tcon.h"
#include "sys_interrupt.h"
#include "sys_uart.h"
#include "sys_gpio.h"
#include "sys_delay.h"
#include "sys_io.h"
#include <stdlib.h>
// #include "main.h"


void TCON_ISR(void);

/*
TCON设置
*/
void Tcon_Init(struct fb_f1c100s_pdata_t * pdat)
{
  struct f1c100s_tcon_reg_t *tcon=((struct f1c100s_tcon_reg_t *)pdat->virttcon);
	u32_t val;
	int F=0;
//-----------------------------------------LCD_TYPE_RGB_480_272-------------------------------------------------	
#ifdef LCD_TYPE_RGB_480_272
	F=20;
#endif
//-----------------------------------------LCD_TYPE_RGB_800_480-------------------------------------------------	
#ifdef LCD_TYPE_RGB_800_480
  //设置video时钟到390MHZ 
	u8 N=65,M=4;F=9;  //(24MHz*N)/M/F=43.33MHZ
	C_BIT(CCU_Base_Address+0x010,31);
	write32(CCU_Base_Address+0x010,((N-1)<<8)|((M-1)<<0)|(3<<24));
	S_BIT(CCU_Base_Address+0x010,31);
	delay_ms(1);
#endif
//-----------------------------------------LCD_TYPE_RGB_1024_600-------------------------------------------------	
#ifdef LCD_TYPE_RGB_1024_600
  //设置video时钟到390MHZ 
	u8 N=65,M=4;F=7;  //(24MHz*N)/M/F=55.7MHZ
	C_BIT(CCU_Base_Address+0x010,31);
	write32(CCU_Base_Address+0x010,((N-1)<<8)|((M-1)<<0)|(3<<24));
	S_BIT(CCU_Base_Address+0x010,31);
	delay_ms(1);	
#endif	
//-----------------------------------------VGA 设置Vedio到65MHZ-------------------------------------------------
#ifdef LCD_TYPE_VGA_1024_768_60HZ
  //设置video时钟到390MHZ 
	u8 N=65,M=4;F=6;  //(24MHz*N)/M/F=65MHZ
	C_BIT(CCU_Base_Address+0x010,31);
	write32(CCU_Base_Address+0x010,((N-1)<<8)|((M-1)<<0)|(3<<24));
	S_BIT(CCU_Base_Address+0x010,31);
	delay_ms(1);
#endif	

//-----------------------------------------
#ifdef LCD_TYPE_VGA_1024_768_60HZ	
	int ht = 1344;
	int hbp = 296;
	int hspw=136;
	
	int vt = 1612/2;	
	int vbp = 34;
	int vspw=6;
#else
	int hbp = pdat->timing.h_sync_len + pdat->timing.h_back_porch;
	int ht = pdat->width + pdat->timing.h_front_porch + hbp;
	int hspw=pdat->timing.h_sync_len;
	

	int vbp = pdat->timing.v_sync_len + pdat->timing.v_back_porch;
	int vt = pdat->height + pdat->timing.v_front_porch + vbp;		
	int vspw=pdat->timing.v_sync_len;	
#endif 

	//-----------------------------------------TCON时钟与复位-------------------------------------------------
	//使能LCD时钟
	write32(CCU_BUS_CLK_GATING_REG1,read32(CCU_BUS_CLK_GATING_REG1)|(1)<<4);
//	//使能TCON时钟
	write32(CCU_TCON_CLK_REG,read32(CCU_TCON_CLK_REG)|(u64_t)(1)<<31);
	//使能LCD复位
	write32(CCU_BUS_SOFT_RST_REG1,read32(CCU_BUS_SOFT_RST_REG1)|((1)<<4));
	delay_ms(1);

	//-----------------------------------------TCON相关设置-------------------------------------------------
	//设置为TCON0
	val = read32((virtual_addr_t)&tcon->ctrl);
	val &= ~(0x1 << 0);
	write32((virtual_addr_t)&tcon->ctrl, val);
  //使能TCON 31 
	val = (pdat->timing.v_front_porch + pdat->timing.v_back_porch + pdat->timing.v_sync_len);
	write32((virtual_addr_t)&tcon->tcon0_ctrl,((u64_t)0x1 << 31) |(val & 0x1f) << 4);
	//设置时钟
	val = F; // 5< DCLKDIV <96
	write32((virtual_addr_t)&tcon->tcon0_dclk, ((u64_t)0xf << 28) | (val << 0));	
	
	//设置宽高
	write32((virtual_addr_t)&tcon->tcon0_timing_active, ((pdat->width - 1) << 16) | ((pdat->height - 1) << 0));
  //设置HT+HBP
	write32((virtual_addr_t)&tcon->tcon0_timing_h, ((ht - 1) << 16) | ((hbp - 1) << 0));
	//设置VT+VBP
	write32((virtual_addr_t)&tcon->tcon0_timing_v, ((vt * 2) << 16) | ((vbp - 1) << 0));
	//设置时钟宽度
	write32((virtual_addr_t)&tcon->tcon0_timing_sync, ((hspw - 1) << 16) | ((vspw - 1) << 0));
  //RB取反-方便布线
#if 0	
  S_BIT(TCON_Base_Address+0x40,23);
#endif
  //设置模式
	if(pdat->mode==1)//rgb
	{
		write32((virtual_addr_t)&tcon->tcon0_hv_intf, 0);
		write32((virtual_addr_t)&tcon->tcon0_cpu_intf, 0);	
	}
	else//cpu
	{
		//设置为8080模式
		write32(TCON_Base_Address+0x40,read32(TCON_Base_Address+0x40)|(1)<<24);	
		//设置输入源
		write32(TCON_Base_Address+0x40,read32(TCON_Base_Address+0x40)|(0)<<0);//[3-白色数据][2-DMA][0-DE]		
		//设置为16位模式+自动 
		write32(TCON_Base_Address+0x60,(u64_t)(4)<<29 | (u64_t)(1)<<28);	
	}
  //FRM
	if(pdat->bits_per_pixel == 18 || pdat->bits_per_pixel == 16)
	{
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[0], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[1], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[2], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[3], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[4], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[5], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[0], 0x01010000);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[1], 0x15151111);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[2], 0x57575555);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[3], 0x7f7f7777);
		write32((virtual_addr_t)&tcon->tcon0_frm_ctrl, (pdat->bits_per_pixel == 18) ? (((u64_t)1 << 31) | (0 << 4)) : (((u64_t)1 << 31) | (5 << 4)));
	}
  //极性控制 
	val = (1 << 28);	
	if(!pdat->timing.h_sync_active)	val |= (1 << 25);	
	if(!pdat->timing.v_sync_active)	val |= (1 << 24);	
	if(!pdat->timing.den_active)		val |= (1 << 27);		
	if(!pdat->timing.clk_active)		val |= (1 << 26);	
	write32((virtual_addr_t)&tcon->tcon0_io_polarity, val);
	//触发控制关
	write32((virtual_addr_t)&tcon->tcon0_io_tristate, 0);
}

void Debe_Init(struct fb_f1c100s_pdata_t * pdat)
{
	struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)pdat->virtdebe;
	u32_t val;
	//-----------------------------------------DEBE时钟与复位-------------------------------------------------
	//使能DEBE时钟
	write32(CCU_BUS_CLK_GATING_REG1,read32(CCU_BUS_CLK_GATING_REG1)|(1)<<12);
	//使能DEBE复位
	write32(CCU_BUS_SOFT_RST_REG1,read32(CCU_BUS_SOFT_RST_REG1)|((1)<<12));
	delay_ms(1);

//使能DEBE
	val = read32((virtual_addr_t)&debe->mode);
	val |= (1 << 0);
	write32((virtual_addr_t)&debe->mode, val);
//设置层0参数
	write32((virtual_addr_t)&debe->disp_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
	write32((virtual_addr_t)&debe->layer0_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
	write32((virtual_addr_t)&debe->layer0_stride, ((pdat->width) << 4));
	write32((virtual_addr_t)&debe->layer0_addr_low32b, (u32)(pdat->vram[0]) << 3);
	write32((virtual_addr_t)&debe->layer0_addr_high4b, (u32)(pdat->vram[0]) >> 29);
	
	write32((virtual_addr_t)&debe->layer0_attr1_ctrl, (0x05 << 8));//层0属性 5=RGB565
//层0使能
	val = read32((virtual_addr_t)&debe->mode);
	val |= (1 << 8);
	write32((virtual_addr_t)&debe->mode, val);
//加载
	val = read32((virtual_addr_t)&debe->reg_ctrl);
	val |= (1 << 0);
	write32((virtual_addr_t)&debe->reg_ctrl, val);
//DEBE 开始
	val = read32((virtual_addr_t)&debe->mode);
	val |= (1 << 1);
	write32((virtual_addr_t)&debe->mode, val);
}

void f1c100s_debe_set_address(struct fb_f1c100s_pdata_t * pdat, void * vram)
{
	struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)pdat->virtdebe;

	write32((virtual_addr_t)&debe->layer0_addr_low32b, (u32_t)vram << 3);
	write32((virtual_addr_t)&debe->layer0_addr_high4b, (u32_t)vram >> 29);
}


void f1c100s_tcon_enable(struct fb_f1c100s_pdata_t * pdat)
{
	struct f1c100s_tcon_reg_t * tcon = (struct f1c100s_tcon_reg_t *)pdat->virttcon;
	u32_t val;

	val = read32((virtual_addr_t)&tcon->ctrl);
	val |= ((u64_t)1 << 31);
	write32((virtual_addr_t)&tcon->ctrl, val); 	
}

void f1c100s_tcon_disable(struct fb_f1c100s_pdata_t * pdat)
{
	struct f1c100s_tcon_reg_t * tcon = (struct f1c100s_tcon_reg_t *)pdat->virttcon;
	u32_t val;

	write32((virtual_addr_t)&tcon->ctrl, 0);
	write32((virtual_addr_t)&tcon->int0, 0);

	val = read32((virtual_addr_t)&tcon->tcon0_dclk);
	val &= ~((u64_t)0xf << 28);
	write32((virtual_addr_t)&tcon->tcon0_dclk, val);

	write32((virtual_addr_t)&tcon->tcon0_io_tristate, 0xffffffff);
	write32((virtual_addr_t)&tcon->tcon1_io_tristate, 0xffffffff);
}

//全局结构体
struct fb_f1c100s_pdata_t * lcd_pdat;
struct fb_f1c100s_pdata_t f1;
//
void F1C100S_LCD_Init(int width,int height,unsigned int *buff1,unsigned int *buff2)
{
	int i;
	lcd_pdat=&f1;
//	memset(&f1,0,sizeof(struct fb_f1c100s_pdata_t));
	//
	lcd_pdat->virtdebe = F1C100S_DEBE_BASE;
	lcd_pdat->virttcon = F1C100S_TCON_BASE;
	/*********************************TCON SET**************************/
	//LCD模式 0-cpu 1-rgb
	lcd_pdat->mode = 1;
	//LCD宽高
	lcd_pdat->width = width;
	lcd_pdat->height = height;
	//像素位宽
	lcd_pdat->bits_per_pixel = 18;

	
#ifdef LCD_TYPE_RGB_480_272
  //时序
	lcd_pdat->timing.h_front_porch = 40;//
	lcd_pdat->timing.h_back_porch = 87;//
	lcd_pdat->timing.h_sync_len = 1;//HSPW
	lcd_pdat->timing.v_front_porch = 13;//
	lcd_pdat->timing.v_back_porch = 31;//
	lcd_pdat->timing.v_sync_len = 1;//VSPW
#endif

#ifdef LCD_TYPE_RGB_800_480
  //时序
	lcd_pdat->timing.h_front_porch = 40;//
	lcd_pdat->timing.h_back_porch = 87;//
	lcd_pdat->timing.h_sync_len = 1;//HSPW
	lcd_pdat->timing.v_front_porch = 13;//
	lcd_pdat->timing.v_back_porch = 31;//
	lcd_pdat->timing.v_sync_len = 1;//VSPW
#endif

#ifdef LCD_TYPE_RGB_1024_600
  //时序
	lcd_pdat->timing.h_front_porch = 160;//
	lcd_pdat->timing.h_back_porch = 160;//
	lcd_pdat->timing.h_sync_len = 1;//HSPW
	lcd_pdat->timing.v_front_porch = 2;//
	lcd_pdat->timing.v_back_porch = 23;//
	lcd_pdat->timing.v_sync_len = 12;//VSPW
#endif
	
	//极性设置 0反相
	lcd_pdat->timing.h_sync_active = 0;
	lcd_pdat->timing.v_sync_active = 0;
	lcd_pdat->timing.den_active = 1; //反相
	lcd_pdat->timing.clk_active = 0;
	/*********************************DEBE SET**************************/
	lcd_pdat->index=0;//FB索引
	lcd_pdat->bytes_per_pixel = 4;//buff 色位宽	
	lcd_pdat->vram[0] = buff1;
	lcd_pdat->vram[1] = buff2;
	
	/*********************************初始化**************************/
	for(i = 0x0800; i < 0x1000; i += 4)
	write32(F1C100S_DEBE_BASE + i, 0);
	//
	f1c100s_tcon_disable(lcd_pdat);	
  Debe_Init(lcd_pdat);	
	Tcon_Init(lcd_pdat);	

  f1c100s_tcon_enable(lcd_pdat);

}

//LCD IO初始化
void LCD_IO_Init(void)
{
int i=0;
  for(i=0;i<=21;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);	
}	

/*
LCD初始化
*/

void Sys_LCD_Init(int width,int height,unsigned int *buff1,unsigned int *buff2)
{

	LCD_IO_Init();//IO初始化
	F1C100S_LCD_Init(width,height,buff1,buff2);//TCON DEBE 初始化
	
}

