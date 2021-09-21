#ifndef __SYS_UART_H_
#define __SYS_UART_H_

// #define LOG_ENABLE

void sys_uart_init(void);
void sys_uart_exit(void);
void sys_uart_putc(char c);

#endif
