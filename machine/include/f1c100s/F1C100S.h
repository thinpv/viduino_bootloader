#ifndef __F1C100S_H_
#define __F1C100S_H_

// typedef signed char         int8_t;
// typedef signed short        int16_t;
// typedef signed int          int32_t;
// typedef signed long long    int64_t;
// typedef unsigned char       uint8_t;
// typedef unsigned short      uint16_t;
// typedef unsigned int        uint32_t;
// typedef unsigned long long  uint64_t;

typedef volatile unsigned char      vuint8_t;
typedef volatile unsigned short     vuint16_t;
typedef volatile unsigned int       vuint32_t;
typedef volatile unsigned long long vuint64_t;

// typedef signed char         s8;
// typedef signed short        s16;
// typedef signed int          s32;
// typedef signed long long    s64;
// typedef unsigned char       u8;
// typedef unsigned short      u16;
// typedef unsigned int        u32;
// typedef unsigned long long  u64;

#define CCU_BASE    0x01C20000
#define INTC_BASE   0x01C20400
#define TIMER_BASE  0x01C20C00
#define PWM_BASE    0x01C21000
#define DMA_BASE    0x01C02000
#define PIO_BASE    0x01C20800
#define KEYADC_BASE 0x01C23400
#define TP_BASE     0x01C24800
#define AUDIOC_BASE 0x01C23C00
#define TCON_BASE   0x01C0C000
#define DEFE_BASE   0x01E00000
#define DEBE_BASE   0x01E60000
#define DI_BASE     0x01E70000
#define CSI_BASE    0x01CB0000
#define SDC_BASE    0x01C0F000
#define TWI_BASE    0x01C27000
#define SPI_BASE    0x01C05000
#define UART_BASE   0x01C25000
#define RSB_BASE    0x08003400
#define CIR_BASE    0x01C22C00
#define DA_BASE     0x01C22000
#define OWA_BASE    0x01C21400

typedef struct 
{
    vuint32_t   PLL_CPU_CR;     //PLL_CPU Control Register
    vuint32_t   rsv1;
    vuint32_t   PLL_AUDIO_CR;   //PLL_AUDIO Control Register
    vuint32_t   rsv2;
    vuint32_t   PLL_VIDEO_CR;   //PLL_VIDEO Control Register
    vuint32_t   rsv3;
    vuint32_t   PLL_VE_CR;      //PLL_VE Control Register
    vuint32_t   rsv4;
    vuint32_t   PLL_DDR_CR;     //PLL_DDR Control Register
    vuint32_t   rsv5;
    vuint32_t   PLL_PERIPH_CR;  //PLL_PERIPH Control Register
    vuint32_t   rsv6[9];

    vuint32_t   CPU_CSR;        //CPU Clock Source Register
    vuint32_t   AHB_APB_CFG;    //AHB/APB/HCLKC Configuration Register
    vuint32_t   rsv7[2];
    vuint32_t   GR0;            //Bus Clock Gating Register 0
    vuint32_t   GR1;            //Bus Clock Gating Register 1
    vuint32_t   GR2;            //Bus Clock Gating Register 2
    vuint32_t   rsv8[7];
    vuint32_t   SDIO0_CLK;      //SDMMC0 Clock Register
    vuint32_t   SDIO1_CLK;      //SDMMC1 Clock Register
    vuint32_t   rsv9[8];
    vuint32_t   DAUDIO_CLK;     //DAUDIO Clock Register
    vuint32_t   OWA_CLK;        //OWA Clock Register
    vuint32_t   CIR_CLK;        //CIR Clock Register
    vuint32_t   rsv10[4];
    vuint32_t   USB_CLK;        //USBPHY Clock Register
    vuint32_t   rsv11[12];
    vuint32_t   DDR_GR;         //DRAM GATING Register
    vuint32_t   BE_CLK;         //BE Clock Register
    vuint32_t   rsv12;
    vuint32_t   FE_CLK;         //FE Clock Register
    vuint32_t   rsv13[2];
    vuint32_t   TCON_CLK;       //TCON Clock Register
    vuint32_t   DI_CLK;         //De-interlacer Clock Register
    vuint32_t   TVE_CLK;        //TVE Clock Register
    vuint32_t   TVD_CLK;        //TVD Clock Register
    vuint32_t   rsv14[3];
    vuint32_t   CSI_CLK;        //CSI Clock Register
    vuint32_t   rsv15;
    vuint32_t   VE_CLK;         //VE Clock Register
    vuint32_t   AUDIOC_CLK;     //Audio Codec Clock Register
    vuint32_t   AVS_CLK;        //AVS Clock Register
    vuint32_t   rsv16[46];

    vuint32_t   PLL_STR0;       //PLL Stable Time Register 0
    vuint32_t   PLL_STR1;       //PLL Stable Time Register 1
    vuint32_t   rsv17[6];
    vuint32_t   PLL_CPU_BR;     //PLL_CPU Bias Register
    vuint32_t   PLL_AUDIO_BR;   //PLL_AUDIO Bias Register
    vuint32_t   PLL_VIDEO_BR;   //PLL_VIDEO Bias Register
    vuint32_t   PLL_VE_BR;      //PLL_VE Bias Register
    vuint32_t   PLL_DDR_BR;     //PLL_DDR Bias Register
    vuint32_t   PLL_PERIPH_BR;  //PLL_PERIPH Bias Register
    vuint32_t   rsv18[6];
    vuint32_t   PLL_CPU_TR;     //PLL_CPU Tuning Register
    vuint32_t   rsv19[3];
    vuint32_t   PLL_DDR_TR;     //PLL_DDR Tuning Register
    vuint32_t   rsv20[8];
    vuint32_t   PLL_AUDIO_PCR;  //PLL_AUDIO Pattern Control Register
    vuint32_t   PLL_VIDEO_PCR;  //PLL_VIDEO Pattern Control Register
    vuint32_t   rsv21;
    vuint32_t   PLL_DDR_PCR;    //PLL_DDR Pattern Control Register
    vuint32_t   rsv22[11];

    vuint32_t   SOFT_RST0;      //Bus Software Reset Register 0
    vuint32_t   SOFT_RST1;      //Bus Software Reset Register 1
    vuint32_t   rsv23[2];
    vuint32_t   SOFT_RST2;      //Bus Software Reset Register 2
}CCU_Type;

typedef struct 
{
    vuint32_t   CR0;            //Configure Register 0
    vuint32_t   CR1;            //Configure Register 1
    vuint32_t   CR2;            //Configure Register 2
    vuint32_t   CR3;            //Configure Register 3
    vuint32_t   DR;             //Data Register
    vuint32_t   MR0;            //Multi-Driving Register 0
    vuint32_t   MR1;            //Multi-Driving Register 1
    vuint32_t   PR0;            //Pull Register 0
    vuint32_t   PR1;            //Pull Register 1
}GPIO_Type;

typedef struct 
{
    vuint32_t   PIO_INT_CFG0;
    vuint32_t   PIO_INT_CFG1;
    vuint32_t   PIO_INT_CFG2;
    vuint32_t   PIO_INT_CFG3;
    vuint32_t   PIO_INT_CTRL;
    vuint32_t   PIO_INT_STA;
    vuint32_t   PIO_INT_DEB;
}GPIO_INT_Type;

typedef struct 
{
    vuint32_t   MR;             //Multi-Driving Register
    vuint32_t   PR;             //Pull Register
}GPIO_DDR_Type;

typedef struct
{
    vuint32_t   rsv1;
    vuint32_t   GCR;            //Global Control Register
    vuint32_t   TCR;            //Transfer Control Register
    vuint32_t   rsv2;
    vuint32_t   ICR;            //Interrupt Control Register
    vuint32_t   ISR;            //Interrupt Status Registe
    vuint32_t   FCR;            //FIFO Control Register
    vuint32_t   FSR;            //FIFO Status Register
    vuint32_t   WCR;            //Wait Clock Counter Register
    vuint32_t   CCR;            //Clock Rate Control Register
    vuint32_t   rsv3[2];
    vuint32_t   MBC;          //Burst Counter Register
    vuint32_t   MTC;          //Transmit Counter Register
    vuint32_t   BCC;         //Burst Control Counter Register
    vuint32_t   rsv4[113];
    vuint8_t    TXD;           //TX Data Register
    vuint8_t    rsv5[3];
    vuint32_t   rsv6[63];
    vuint8_t    RXD;           //RX Data Register
}SPI_Type;

typedef struct
{
    vuint32_t   RBR_THR_DLL;    /* Receiver Buffer Register
                                 * Transmit Holding Register
                                 * Divisor Latch Low Register
                                 */
    vuint32_t   DLH_IER;        /* Divisor Latch High Register
                                 * Interrupt Enable Register
                                 */
    vuint32_t   IIR_FCR;        /* Interrupt Identity Register
                                 * FIFO Control Register
                                 */
    vuint32_t   LCR;            //Line Control Register
    vuint32_t   MCR;            //Modem Control Register
    vuint32_t   LSR;            //Line Status Register
    vuint32_t   MSR;            //Modem Status Register
    vuint32_t   SCH;            //Scratch Register
    vuint32_t   rsv1[23];
    vuint32_t   SR;             //Status Register
    vuint32_t   TFL;            //Transmit FIFO Level Register
    vuint32_t   RFL;            //Receive FIFO Level Register
    vuint32_t   HSK;            //DMA Handshake Config Register
    vuint32_t   rsv2[6];
    vuint32_t   HALT;           //Halt TX Register
    vuint32_t   rsv3[2];
    vuint32_t   DBG_DLL;        //Debug DLL Register
    vuint32_t   DBG_DLH;        //Debug DLH Register
}UART_Type;

typedef struct
{
    vuint32_t   TMR_IRQ_EN_REG; //0x00
    vuint32_t   TMR_IRQ_STA_REG; //0x04
    vuint32_t   rsv1[2];//
    vuint32_t   TMR0_CTRL_REG; //0x10
    vuint32_t   TMR0_INTV_VALUE_REG;//0x14
    vuint32_t   TMR0_CUR_VALUE_REG;//0x18
    vuint32_t   rsv2[1];//
    vuint32_t   TMR1_CTRL_REG;//0x20
    vuint32_t   TMR1_INTV_VALUE_REG;//0x24
    vuint32_t   TMR1_CUR_VALUE_REG;//0x28
    vuint32_t   rsv3[1];//
    vuint32_t   TMR2_CTRL_REG;//0x30
    vuint32_t   TMR2_INTV_VALUE_REG;//0x34
    vuint32_t   TMR2_CUR_VALUE_REG;//0x38
    vuint32_t   rsv4[17];//
    vuint32_t   AVS_CNT_CTL_REG;//0x80
    vuint32_t   AVS_CNT0_REG;//0x84
    vuint32_t   AVS_CNT1_REG;//0x88
    vuint32_t   AVS_CNT_DIV_REG;//0x8C
    vuint32_t   rsv5[4];//
    vuint32_t   WDOG_IRQ_EN_REG;//0xA0
    vuint32_t   WDOG_IRQ_STA_REG;//0xA4
    vuint32_t   rsv6[2];//
    vuint32_t   WDOG_CTRL_REG;//0xB0
    vuint32_t   WDOG_CFG_REG;//0xB4
    vuint32_t   WDOG_MODE_REG;//0xB8
}Timer_Type;

typedef struct
{
    vuint32_t   INTC_VECTOR_REG;//0x00
    vuint32_t   INTC_BASE_ADDR_REG;//0x04
    vuint32_t   rsv1[1];//
    vuint32_t   NMI_INT_CTRL_REG;//0x0C
    vuint32_t   INTC_PEND_REG0;//0x10
    vuint32_t   INTC_PEND_REG1;//0x14
    vuint32_t   rsv2[2];//
    vuint32_t   INTC_EN_REG0;//0x20
    vuint32_t   INTC_EN_REG1;//0x24
    vuint32_t   rsv3[2];//
    vuint32_t   INTC_MASK_REG0;//0x30
    vuint32_t   INTC_MASK_REG1;//0x34
    vuint32_t   rsv4[2];//
    vuint32_t   INTC_RESP_REG0;//0x40
    vuint32_t   INTC_RESP_REG1;//44
    vuint32_t   rsv5[2];//
    vuint32_t   INTC_FF_REG0;//0x50
    vuint32_t   INTC_FF_REG1;//0x54
    vuint32_t   rsv6[2];//
    vuint32_t   INTC_PRIO_REG0;//0x60
    vuint32_t   INTC_PRIO_REG1;//0x64
    vuint32_t   INTC_PRIO_REG2;//0x68
    vuint32_t   INTC_PRIO_REG3;//0x6C
}INTC_Type;

typedef struct
{
    vuint32_t   TWI_ADDR_REG;//0x00
    vuint32_t   TWI_XADDR_REG; //0x04
    vuint32_t   TWI_DATA_REG; //0x08
    vuint32_t   TWI_CNTR_REG; //0x0C
    vuint32_t   TWI_STAT_REG; //0x10
    vuint32_t   TWI_CCR_REG;//0x14
    vuint32_t   TWI_SRST_REG;//0x18
    vuint32_t   TWI_EFR_REG;//0x1C
    vuint32_t   TWI_LCR_REG;//0x20
}I2C_Type;

typedef struct
{
    vuint32_t   CSI_EN_REG;//0x00
    vuint32_t   CSI_CFG_REG; //0x04
    vuint32_t   CSI_CAP_REG; //0x08
    vuint32_t   CSI_SCALE_REG; //0x0C
    vuint32_t   CSI_FIFO0_BUFA_REG; //0x10
    vuint32_t   CSI_FIFO0_BUFB_REG;//0x14
    vuint32_t   CSI_FIFO1_BUFA_REG;//0x18
    vuint32_t   CSI_FIFO1_BUFB_REG;//0x1C
    vuint32_t   CSI_FIFO2_BUFA_REG;//0x20
    vuint32_t   CSI_FIFO2_BUFB_REG;//0x24
    vuint32_t   CSI_BUF_CTL_REG;//0x28
    vuint32_t   CSI_BUF_STA_REG;//0x2C
    vuint32_t   CSI_INT_EN_REG;//0x30
    vuint32_t   CSI_INT_STA_REG;//0x34
    vuint32_t   rsv1[2];//
    vuint32_t   CSI_HSIZE_REG;//0x40
    vuint32_t   CSI_VSIZE_REG;//0x44
    vuint32_t   CSI_BUF_LEN_REG;//0x48
}CSI_Type;

typedef struct {
    
}IRQn_Type;

#define CCU         ((CCU_Type *)(CCU_BASE + 0x0))

#define GPIOA       ((GPIO_Type *)(PIO_BASE + 0*0x24))
#define GPIOB       ((GPIO_Type *)(PIO_BASE + 1*0x24))
#define GPIOC       ((GPIO_Type *)(PIO_BASE + 2*0x24))
#define GPIOD       ((GPIO_Type *)(PIO_BASE + 3*0x24))
#define GPIOE       ((GPIO_Type *)(PIO_BASE + 4*0x24))
#define GPIOF       ((GPIO_Type *)(PIO_BASE + 5*0x24))

// #define GPIOA_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 0*0x20))
// #define GPIOB_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 1*0x20))
// #define GPIOC_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 2*0x20))
#define GPIOD_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 0*0x20))
#define GPIOE_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 1*0x20))
#define GPIOF_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 2*0x20))

#define GPIO_DDR    ((GPIO_DDR_Type *)(PIO_BASE + 0x2C0))

#define SPI0        ((SPI_Type *)(SPI_BASE + 0x0))
#define SPI1        ((SPI_Type *)(SPI_BASE + 0x1000))

#define UART0       ((UART_Type *)(UART_BASE + 0x0))
#define UART1       ((UART_Type *)(UART_BASE + 0x400))
#define UART2       ((UART_Type *)(UART_BASE + 0x800))

#define TIMER       ((Timer_Type *)TIMER_BASE)

#define INTC        ((INTC_Type *)INTC_BASE)

#define I2C0       ((I2C_Type *)(TWI_BASE + 0x0))
#define I2C1       ((I2C_Type *)(TWI_BASE + 0x400))
#define I2C2       ((I2C_Type *)(TWI_BASE + 0x800))

#define CSI        ((CSI_Type *)CSI_BASE)


#include <gpio.h>
#include <uart.h>

#endif // !__F1C100S_H_
