
enum eUart
{
  UART0=0,
	UART1,
	UART2
};

void Sys_Uart_Init(enum eUart uart,unsigned int cpu_frequency,unsigned int Baudrate,int Interrupt);
void Sys_Uart_Put(enum eUart uart,char c);
void sysprintf(char * pcStr,...);
void Sys_SET_UART_DBG(enum eUart uart);
