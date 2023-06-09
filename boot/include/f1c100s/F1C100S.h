#ifndef __F1C100S_H_
#define __F1C100S_H_

#include <stddef.h>

// typedef signed char         int8_t;
// typedef signed short        int16_t;
// typedef signed int          int32_t;
// typedef signed long long    int64_t;
// typedef unsigned char       uint8_t;
// typedef unsigned short      uint16_t;
// typedef unsigned int        uint32_t;
// typedef unsigned long long  uint64_t;

typedef volatile unsigned char vuint8_t;
typedef volatile unsigned short vuint16_t;
typedef volatile unsigned int vuint32_t;
typedef volatile unsigned long long vuint64_t;

// typedef signed char s8;
// typedef signed short s16;
// typedef signed int s32;
// typedef signed long long s64;
// typedef unsigned char u8;
// typedef unsigned short u16;
// typedef unsigned int u32;
// typedef unsigned long long u64;

#define CCU_BASE 0x01C20000
#define INTC_BASE 0x01C20400
#define TIMER_BASE 0x01C20C00
#define PWM_BASE 0x01C21000
#define DMA_BASE 0x01C02000
#define PIO_BASE 0x01C20800
#define KEYADC_BASE 0x01C23400
#define TP_BASE 0x01C24800
#define AUDIOC_BASE 0x01C23C00
#define TCON_BASE 0x01C0C000
#define DEFE_BASE 0x01E00000
#define DEBE_BASE 0x01E60000
#define DI_BASE 0x01E70000
#define CSI_BASE 0x01CB0000
#define SDC_BASE 0x01C0F000
#define TWI_BASE 0x01C27000
#define SPI_BASE 0x01C05000
#define UART_BASE 0x01C25000
#define RSB_BASE 0x08003400
#define CIR_BASE 0x01C22C00
#define DA_BASE 0x01C22000
#define OWA_BASE 0x01C21400

typedef struct
{
	vuint32_t PLL_CPU_CTRL_REG; //PLL_CPU Control Register
	vuint32_t rsv1;
	vuint32_t PLL_AUDIO_CTRL_REG; //PLL_AUDIO Control Register
	vuint32_t rsv2;
	vuint32_t PLL_VIDEO_CTRL_REG; //PLL_VIDEO Control Register
	vuint32_t rsv3;
	vuint32_t PLL_VE_CTRL_REG; //PLL_VE Control Register
	vuint32_t rsv4;
	vuint32_t PLL_DDR_CTRL_REG; //PLL_DDR Control Register
	vuint32_t rsv5;
	vuint32_t PLL_PERIPH_CTRL_REG; //PLL_PERIPH Control Register
	vuint32_t rsv6[9];

	vuint32_t CPU_CLK_SRC_REG;		 //CPU Clock Source Register
	vuint32_t AHB_APB_HCLKC_CFG_REG; //AHB/APB/HCLKC Configuration Register
	vuint32_t rsv7[2];
	vuint32_t BUS_CLK_GATING_REG0; //Bus Clock Gating Register 0
	vuint32_t BUS_CLK_GATING_REG1; //Bus Clock Gating Register 1
	vuint32_t BUS_CLK_GATING_REG2; //Bus Clock Gating Register 2
	vuint32_t rsv8[7];
	vuint32_t SDMMC0_CLK_REG; //SDMMC0 Clock Register
	vuint32_t SDMMC1_CLK_REG; //SDMMC1 Clock Register
	vuint32_t rsv9[8];
	vuint32_t DAUDIO_CLK_REG; //DAUDIO Clock Register
	vuint32_t OWA_CLK_REG;	  //OWA Clock Register
	vuint32_t CIR_CLK_REG;	  //CIR Clock Register
	vuint32_t rsv10[4];
	vuint32_t USBPHY_CLK_REG; //USBPHY Clock Register
	vuint32_t rsv11[12];
	vuint32_t DRAM_GATING_REG; //DRAM GATING Register
	vuint32_t BE_CLK_REG;	   //BE Clock Register
	vuint32_t rsv12;
	vuint32_t FE_CLK_REG; //FE Clock Register
	vuint32_t rsv13[2];
	vuint32_t TCON_CLK_REG; //TCON Clock Register
	vuint32_t DI_CLK_REG;	//De-interlacer Clock Register
	vuint32_t TVE_CLK_REG;	//TVE Clock Register
	vuint32_t TVD_CLK_REG;	//TVD Clock Register
	vuint32_t rsv14[3];
	vuint32_t CSI_CLK_REG; //CSI Clock Register
	vuint32_t rsv15;
	vuint32_t VE_CLK_REG;		   //VE Clock Register
	vuint32_t AUDIO_CODEC_CLK_REG; //Audio Codec Clock Register
	vuint32_t AVS_CLK_REG;		   //AVS Clock Register
	vuint32_t rsv16[46];
	vuint32_t PLL_STABLE_TIME_REG0; //PLL Stable Time Register 0
	vuint32_t PLL_STABLE_TIME_REG1; //PLL Stable Time Register 1
	vuint32_t rsv17[6];
	vuint32_t PLL_CPU_BIAS_REG;	   //PLL_CPU Bias Register
	vuint32_t PLL_AUDIO_BIAS_REG;  //PLL_AUDIO Bias Register
	vuint32_t PLL_VIDEO_BIAS_REG;  //PLL_VIDEO Bias Register
	vuint32_t PLL_VE_BIAS_REG;	   //PLL_VE Bias Register
	vuint32_t PLL_DDR_BIAS_REG;	   //PLL_DDR Bias Register
	vuint32_t PLL_PERIPH_BIAS_REG; //PLL_PERIPH Bias Register
	vuint32_t rsv18[6];
	vuint32_t PLL_CPU_TUN_REG; //PLL_CPU Tuning Register
	vuint32_t rsv19[3];
	vuint32_t PLL_DDR_TUN_REG; //PLL_DDR Tuning Register
	vuint32_t rsv20[8];
	vuint32_t PLL_AUDIO_PAT_CTRL_REG; //PLL_AUDIO Pattern Control Register
	vuint32_t PLL_VIDEO_PAT_CTRL_REG; //PLL_VIDEO Pattern Control Register
	vuint32_t rsv21;
	vuint32_t PLL_DDR_PAT_CTRL_REG; //PLL_DDR Pattern Control Register
	vuint32_t rsv22[11];
	vuint32_t BUS_SOFT_RST_REG0; //Bus Software Reset Register 0
	vuint32_t BUS_SOFT_RST_REG1; //Bus Software Reset Register 1
	vuint32_t rsv23[2];
	vuint32_t BUS_SOFT_RST_REG2; //Bus Software Reset Register 2
} CCU_Type;

typedef struct
{
	vuint32_t CFG0; //Configure Register 0
	vuint32_t CFG1; //Configure Register 1
	vuint32_t CFG2; //Configure Register 2
	vuint32_t CFG3; //Configure Register 3
	vuint32_t DATA; //Data Register
	vuint32_t DRV0; //Multi-Driving Register 0
	vuint32_t DRV1; //Multi-Driving Register 1
	vuint32_t PUL0; //Pull Register 0
	vuint32_t PUL1; //Pull Register 1
} GPIO_Type;

typedef struct
{
	vuint32_t PIO_INT_CFG0; //PIO Interrupt Configure Register 0
	vuint32_t PIO_INT_CFG1; //PIO Interrupt Configure Register 1
	vuint32_t PIO_INT_CFG2; //PIO Interrupt Configure Register 2
	vuint32_t PIO_INT_CFG3; //PIO Interrupt Configure Register 3
	vuint32_t PIO_INT_CTRL; //PIO Interrupt Control Register
	vuint32_t PIO_INT_STA;	//PIO Interrupt Status Register
	vuint32_t PIO_INT_DEB;	//PIO Interrupt Debounce Register
} GPIO_INT_Type;

typedef struct
{
	vuint32_t MR; //Multi-Driving Register
	vuint32_t PR; //Pull Register
} GPIO_DDR_Type;

typedef struct
{
	vuint32_t rsv1;
	vuint32_t SPI_GCR_REG; //Global Control Register
	vuint32_t SPI_TCR_REG; //Transfer Control Register
	vuint32_t rsv2;
	vuint32_t SPI_IER_REG; //Interrupt Control Register
	vuint32_t SPI_ISR_REG; //Interrupt Status Registe
	vuint32_t SPI_FCR_REG; //FIFO Control Register
	vuint32_t SPI_FSR_REG; //FIFO Status Register
	vuint32_t SPI_WCR_REG; //Wait Clock Counter Register
	vuint32_t SPI_CCR_REG; //Clock Rate Control Register
	vuint32_t rsv3[2];
	vuint32_t SPI_MBC_REG; //Burst Counter Register
	vuint32_t SPI_MTC_REG; //Transmit Counter Register
	vuint32_t SPI_BCC_REG; //Burst Control Counter Register
	vuint32_t rsv4[113];
	vuint8_t SPI_TXD_REG; //TX Data Register
	vuint8_t rsv5[3];
	vuint32_t rsv6[63];
	vuint8_t SPI_RXD_REG; //RX Data Register
} SPI_Type;

typedef struct
{
	vuint32_t UART_RBR_THR_DLL_REG; /* Receiver Buffer Register
                                         * Transmit Holding Register
                                         * Divisor Latch Low Register
                                         */
	vuint32_t UART_DLH_IER_REG;		/* Divisor Latch High Register
                                         * Interrupt Enable Register
                                         */
	vuint32_t UART_IIR_FCR_REG;		/* Interrupt Identity Register
                                         * FIFO Control Register
                                         */
	vuint32_t UART_LCR_REG;			//Line Control Register
	vuint32_t UART_MCR_REG;			//Modem Control Register
	vuint32_t UART_LSR_REG;			//Line Status Register
	vuint32_t UART_MSR_REG;			//Modem Status Register
	vuint32_t UART_SCH_REG;			//Scratch Register
	vuint32_t rsv1[23];
	vuint32_t UART_USR_REG; //Status Register
	vuint32_t UART_TFL_REG; //Transmit FIFO Level Register
	vuint32_t UART_RFL_REG; //Receive FIFO Level Register
	vuint32_t UART_HSK_REG; //DMA Handshake Config Register
	vuint32_t rsv2[6];
	vuint32_t UART_HALT_REG; //Halt TX Register
	vuint32_t rsv3[2];
	vuint32_t UART_DBG_DLL_REG; //Debug DLL Register
	vuint32_t UART_DBG_DLH_REG; //Debug DLH Register
} UART_Type;

typedef struct
{
	vuint32_t TMR_IRQ_EN_REG;	   //0x00
	vuint32_t TMR_IRQ_STA_REG;	   //0x04
	vuint32_t rsv1[2];			   //
	vuint32_t TMR0_CTRL_REG;	   //0x10
	vuint32_t TMR0_INTV_VALUE_REG; //0x14
	vuint32_t TMR0_CUR_VALUE_REG;  //0x18
	vuint32_t rsv2[1];			   //
	vuint32_t TMR1_CTRL_REG;	   //0x20
	vuint32_t TMR1_INTV_VALUE_REG; //0x24
	vuint32_t TMR1_CUR_VALUE_REG;  //0x28
	vuint32_t rsv3[1];			   //
	vuint32_t TMR2_CTRL_REG;	   //0x30
	vuint32_t TMR2_INTV_VALUE_REG; //0x34
	vuint32_t TMR2_CUR_VALUE_REG;  //0x38
	vuint32_t rsv4[17];			   //
	vuint32_t AVS_CNT_CTL_REG;	   //0x80
	vuint32_t AVS_CNT0_REG;		   //0x84
	vuint32_t AVS_CNT1_REG;		   //0x88
	vuint32_t AVS_CNT_DIV_REG;	   //0x8C
	vuint32_t rsv5[4];			   //
	vuint32_t WDOG_IRQ_EN_REG;	   //0xA0
	vuint32_t WDOG_IRQ_STA_REG;	   //0xA4
	vuint32_t rsv6[2];			   //
	vuint32_t WDOG_CTRL_REG;	   //0xB0
	vuint32_t WDOG_CFG_REG;		   //0xB4
	vuint32_t WDOG_MODE_REG;	   //0xB8
} Timer_Type;

typedef struct
{
	vuint32_t INTC_VECTOR_REG;	  //0x00
	vuint32_t INTC_BASE_ADDR_REG; //0x04
	vuint32_t rsv1[1];			  //
	vuint32_t NMI_INT_CTRL_REG;	  //0x0C
	vuint32_t INTC_PEND_REG0;	  //0x10
	vuint32_t INTC_PEND_REG1;	  //0x14
	vuint32_t rsv2[2];			  //
	vuint32_t INTC_EN_REG0;		  //0x20
	vuint32_t INTC_EN_REG1;		  //0x24
	vuint32_t rsv3[2];			  //
	vuint32_t INTC_MASK_REG0;	  //0x30
	vuint32_t INTC_MASK_REG1;	  //0x34
	vuint32_t rsv4[2];			  //
	vuint32_t INTC_RESP_REG0;	  //0x40
	vuint32_t INTC_RESP_REG1;	  //44
	vuint32_t rsv5[2];			  //
	vuint32_t INTC_FF_REG0;		  //0x50
	vuint32_t INTC_FF_REG1;		  //0x54
	vuint32_t rsv6[2];			  //
	vuint32_t INTC_PRIO_REG0;	  //0x60
	vuint32_t INTC_PRIO_REG1;	  //0x64
	vuint32_t INTC_PRIO_REG2;	  //0x68
	vuint32_t INTC_PRIO_REG3;	  //0x6C
} INTC_Type;

typedef struct
{
	vuint32_t TWI_ADDR_REG;	 //0x00
	vuint32_t TWI_XADDR_REG; //0x04
	vuint32_t TWI_DATA_REG;	 //0x08
	vuint32_t TWI_CNTR_REG;	 //0x0C
	vuint32_t TWI_STAT_REG;	 //0x10
	vuint32_t TWI_CCR_REG;	 //0x14
	vuint32_t TWI_SRST_REG;	 //0x18
	vuint32_t TWI_EFR_REG;	 //0x1C
	vuint32_t TWI_LCR_REG;	 //0x20
} I2C_Type;

typedef struct
{
	vuint32_t CSI_EN_REG;		  //0x00
	vuint32_t CSI_CFG_REG;		  //0x04
	vuint32_t CSI_CAP_REG;		  //0x08
	vuint32_t CSI_SCALE_REG;	  //0x0C
	vuint32_t CSI_FIFO0_BUFA_REG; //0x10
	vuint32_t CSI_FIFO0_BUFB_REG; //0x14
	vuint32_t CSI_FIFO1_BUFA_REG; //0x18
	vuint32_t CSI_FIFO1_BUFB_REG; //0x1C
	vuint32_t CSI_FIFO2_BUFA_REG; //0x20
	vuint32_t CSI_FIFO2_BUFB_REG; //0x24
	vuint32_t CSI_BUF_CTL_REG;	  //0x28
	vuint32_t CSI_BUF_STA_REG;	  //0x2C
	vuint32_t CSI_INT_EN_REG;	  //0x30
	vuint32_t CSI_INT_STA_REG;	  //0x34
	vuint32_t rsv1[2];			  //
	vuint32_t CSI_HSIZE_REG;	  //0x40
	vuint32_t CSI_VSIZE_REG;	  //0x44
	vuint32_t CSI_BUF_LEN_REG;	  //0x48
} CSI_Type;

typedef struct
{
	vuint32_t SD_GCTL_REG;	  //0x00
	vuint32_t SD_CKCR_REG;	  //0x04
	vuint32_t SD_TMOR_REG;	  //0x08
	vuint32_t SD_BWDR_REG;	  //0x0C
	vuint32_t SD_BKSR_REG;	  //0x10
	vuint32_t SD_BYCR_REG;	  //0x14
	vuint32_t SD_CMDR_REG;	  //0x18
	vuint32_t SD_CAGR_REG;	  //0x1C
	vuint32_t SD_RESP0_REG;	  //0x20
	vuint32_t SD_RESP1_REG;	  //0x24
	vuint32_t SD_RESP2_REG;	  //0x28
	vuint32_t SD_RESP3_REG;	  //0x2C
	vuint32_t SD_IMKR_REG;	  //0x30
	vuint32_t SD_MISR_REG;	  //0x34
	vuint32_t SD_RISR_REG;	  //0x38
	vuint32_t SD_STAR_REG;	  //0x3C
	vuint32_t SD_FWLR_REG;	  //0x40
	vuint32_t SD_FUNS_REG;	  //0x44
	vuint32_t SD_CBCR_REG;	  //0x48
	vuint32_t SD_BBCR_REG;	  //0x4C
	vuint32_t SD_DBGC_REG;	  //0x50
	vuint32_t rsv1[1];		  //
	vuint32_t SD_A12A_REG;	  //0x58
	vuint32_t rsv2[7];		  //
	vuint32_t SD_HWRST_REG;	  //0x78
	vuint32_t rsv3[1];		  //
	vuint32_t SD_DMAC_REG;	  //0x80
	vuint32_t SD_DLBA_REG;	  //0x84
	vuint32_t SD_IDST_REG;	  //0x88
	vuint32_t SD_IDIE_REG;	  //0x8C
	vuint32_t SD_CHDA_REG;	  //0x90
	vuint32_t SD_CBDA_REG;	  //0x94
	vuint32_t rsv4[26];		  //
	vuint32_t CARD_THLDC_REG; //0x100
	vuint32_t rsv5[2];		  //
	vuint32_t EMMC_DSBD_REG;  //0x10C
	vuint32_t rsv6[60];		  //
	vuint32_t SD_FIFO_REG;	  //0x200
} SDC_Type;

typedef struct
{
	vuint32_t AC_DAC_DPC_REG;	 //0x00
	vuint32_t AC_DAC_FIFOC_REG;	 //0x04
	vuint32_t AC_DAC_FIFOS_REG;	 //0x08
	vuint32_t AC_DAC_TXDATA_REG; //0x0C
	vuint32_t AC_ADC_FIFOC_REG;	 //0x10
	vuint32_t AC_ADC_FIFOS_REG;	 //0x14
	vuint32_t AC_ADC_RXDATA_REG; //0x18
	vuint32_t rsv1[1];
	vuint32_t DAC_MIXER_CTRL_REG;	   //0x20
	vuint32_t ADC_MIXER_CTRL_REG;	   //0x24
	vuint32_t ADDA_TUNE_REG;		   //0x28
	vuint32_t BIAS_DA16_CAL_CTRL_REG0; //0x2C
	vuint32_t rsv2[1];
	vuint32_t BIAS_DA16_CAL_CTRL_REG1; //0x34
	vuint32_t rsv3[2];
	vuint32_t AC_DAC_CNT_REG; //0x40
	vuint32_t AC_ADC_CNT_REG; //0x44
	vuint32_t AC_DAC_DG_REG;  //0x48
	vuint32_t AC_ADC_DG_REG;  //0x4C
	vuint32_t rsv4[8];
	vuint32_t AC_ADC_DAP_CTR_REG;  //0x70
	vuint32_t AC_ADC_DAP_LCTR_REG; //0x74
	vuint32_t AC_ADC_DAP_RCTR_REG; //0x78
	vuint32_t AC_ADC_DAP_PARA_REG; //0x7C
	vuint32_t AC_ADC_DAP_LAC_REG;  //0x80
	vuint32_t AC_ADC_DAP_LDAT_REG; //0x84
	vuint32_t AC_ADC_DAP_RAC_REG;  //0x88
	vuint32_t AC_ADC_DAP_RDAT_REG; //0x8C
	vuint32_t ADC_DAP_HPFC_REG;	   //0x90
	vuint32_t ADC_DAP_LINAC_REG;   //0x94
	vuint32_t ADC_DAP_RINAC_REG;   //0x98
	vuint32_t ADC_DAP_ORT_REG;	   //0x9C
} Audio_Type;

typedef struct
{
	vuint32_t NDMA_CFG_REG;		 //0x00
	vuint32_t NDMA_SRC_ADR_REG;	 //0x04
	vuint32_t NDMA_DES_ADR_REG;	 //0x08
	vuint32_t NDMA_BYTE_CNT_REG; //0x0C
	vuint32_t rsv[4];
} NDMA_Type;

typedef struct
{
	vuint32_t DDMA_CFG_REG;		 //0x00
	vuint32_t DDMA_SRC_ADR_REG;	 //0x04
	vuint32_t DDMA_DES_ADR_REG;	 //0x08
	vuint32_t DDMA_BYTE_CNT_REG; //0x0C
	vuint32_t rsv[2];
	vuint32_t DDMA_PAR_REG;	 //0x18
	vuint32_t DDMA_GEN_DATA; //0x1C
} DDMA_Type;

typedef struct
{
	vuint32_t DMA_INT_CTRL_REG; //0x00
	vuint32_t DMA_INT_STA_REG;	//0x04
	vuint32_t DMA_PTY_CFG_REG;	//0x08
	vuint32_t rsv[61];
	NDMA_Type nDMA[4];
	DDMA_Type dDMA[4];
} DMA_Type;

typedef struct
{
	vuint32_t KEYADC_CTRL_REG; //0x00
	vuint32_t KEYADC_INTC_REG; //0x04
	vuint32_t KEYADC_INTS_REG; //0x08
	vuint32_t KEYADC_DATA_REG; //0x0C
} KEYADC_Type;

typedef struct
{

} IRQn_Type;

typedef struct
{
	vuint8_t res0[0x800];			   /* 0x000 */
	vuint32_t DEBE_MODE_CTRL_REG;	   /* 0x800 */
	vuint32_t DEBE_COLOR_CTRL_REG;	   /* 0x804 */
	vuint32_t disp_size;			   /* 0x808 */
	vuint8_t res1[0x4];				   /* 0x80c */
	vuint32_t DEBE_LAY0_SIZE_REG;	   /* 0x810 */
	vuint32_t DEBE_LAY1_SIZE_REG;	   /* 0x814 */
	vuint32_t DEBE_LAY2_SIZE_REG;	   /* 0x818 */
	vuint32_t DEBE_LAY3_SIZE_REG;	   /* 0x81c */
	vuint32_t DEBE_LAY0_CODNT_REG;	   /* 0x820 */
	vuint32_t DEBE_LAY1_CODNT_REG;	   /* 0x824 */
	vuint32_t DEBE_LAY2_CODNT_REG;	   /* 0x828 */
	vuint32_t DEBE_LAY3_CODNT_REG;	   /* 0x82c */
	vuint8_t res2[0x10];			   /* 0x830 */
	vuint32_t DEBE_LAY0_LINEWIDTH_REG; /* 0x840 */
	vuint32_t DEBE_LAY1_LINEWIDTH_REG; /* 0x844 */
	vuint32_t DEBE_LAY2_LINEWIDTH_REG; /* 0x848 */
	vuint32_t DEBE_LAY3_LINEWIDTH_REG; /* 0x84c */
	vuint32_t DEBE_LAY0_FB_ADDR_REG;   /* 0x850 */
	vuint32_t DEBE_LAY1_FB_ADDR_REG;   /* 0x854 */
	vuint32_t DEBE_LAY2_FB_ADDR_REG;   /* 0x858 */
	vuint32_t DEBE_LAY3_FB_ADDR_REG;   /* 0x85c */
	vuint32_t layer0_addr_high4b;	   /* 0x860 */
	vuint32_t layer1_addr_high4b;	   /* 0x864 */
	vuint32_t layer2_addr_high4b;	   /* 0x868 */
	vuint32_t layer3_addr_high4b;	   /* 0x86c */
	vuint32_t DEBE_REGBUFF_CTRL_REG;   /* 0x870 */
	vuint8_t res3[0xc];				   /* 0x874 */
	vuint32_t DEBE_CK_MAX_REG;		   /* 0x880 */
	vuint32_t DEBE_CK_MIN_REG;		   /* 0x884 */
	vuint32_t DEBE_CK_CFG_REG;		   /* 0x888 */
	vuint8_t res4[0x4];				   /* 0x88c */
	vuint32_t DEBE_LAY0_ATT_CTRL_REG0; /* 0x890 */
	vuint32_t DEBE_LAY1_ATT_CTRL_REG0; /* 0x894 */
	vuint32_t DEBE_LAY2_ATT_CTRL_REG0; /* 0x898 */
	vuint32_t DEBE_LAY3_ATT_CTRL_REG0; /* 0x89c */
	vuint32_t DEBE_LAY0_ATT_CTRL_REG1; /* 0x8a0 */
	vuint32_t DEBE_LAY1_ATT_CTRL_REG1; /* 0x8a4 */
	vuint32_t DEBE_LAY2_ATT_CTRL_REG1; /* 0x8a8 */
	vuint32_t DEBE_LAY3_ATT_CTRL_REG1; /* 0x8ac */
	vuint8_t res5[0x110];			   /* 0x8b0 */
	vuint32_t output_color_ctrl;	   /* 0x9c0 */
	vuint8_t res6[0xc];				   /* 0x9c4 */
	vuint32_t output_color_coef[12];   /* 0x9d0 */
} DEBE_Type;

#define CCU ((CCU_Type *)(CCU_BASE + 0x0))

#define GPIOA ((GPIO_Type *)(PIO_BASE + 0 * 0x24))
#define GPIOB ((GPIO_Type *)(PIO_BASE + 1 * 0x24))
#define GPIOC ((GPIO_Type *)(PIO_BASE + 2 * 0x24))
#define GPIOD ((GPIO_Type *)(PIO_BASE + 3 * 0x24))
#define GPIOE ((GPIO_Type *)(PIO_BASE + 4 * 0x24))
#define GPIOF ((GPIO_Type *)(PIO_BASE + 5 * 0x24))

// #define GPIOA_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 0*0x20))
// #define GPIOB_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 1*0x20))
// #define GPIOC_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 2*0x20))
#define GPIOD_INT ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 0 * 0x20))
#define GPIOE_INT ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 1 * 0x20))
#define GPIOF_INT ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 2 * 0x20))

#define GPIO_DDR ((GPIO_DDR_Type *)(PIO_BASE + 0x2C0))

#define SPI0 ((SPI_Type *)(SPI_BASE + 0x0))
#define SPI1 ((SPI_Type *)(SPI_BASE + 0x1000))

#define UART0 ((UART_Type *)(UART_BASE + 0x0))
#define UART1 ((UART_Type *)(UART_BASE + 0x400))
#define UART2 ((UART_Type *)(UART_BASE + 0x800))

#define TIMER ((Timer_Type *)TIMER_BASE)

#define INTC ((INTC_Type *)INTC_BASE)

#define I2C0 ((I2C_Type *)(TWI_BASE + 0x0))
#define I2C1 ((I2C_Type *)(TWI_BASE + 0x400))
#define I2C2 ((I2C_Type *)(TWI_BASE + 0x800))

#define CSI ((CSI_Type *)CSI_BASE)

#define SDC0 ((SDC_Type *)SDC_BASE)
#define SDC1 ((SDC_Type *)SDC_BASE + 0x1000)

#define AUDIO ((Audio_Type *)AUDIOC_BASE)

#define DMA ((DMA_Type *)DMA_BASE)

#define KEYADC ((KEYADC_Type *)KEYADC_BASE)

#define DEBE ((DEBE_Type *)DEBE_BASE)

#include <gpio.h>
#include <uart.h>
#include <irq.h>

#endif // !__F1C100S_H_
