#ifndef _DRV_UART_H_
#define _DRV_UART_H_
#include "sysconfig.h"

void DrvUart0Init(uint32_t baudrate);
void DrvUart0SendBuf(u8 *data, u8 len);
void DrvUart1Init(uint32_t baudrate);
void DrvUart1SendBuf(u8 *data, u8 len);
void DrvUart2Init(uint32_t baudrate);
void DrvUart2SendBuf(u8 *data, u8 len);
void DrvUart3Init(uint32_t baudrate);
void DrvUart3SendBuf(u8 *data, u8 len);
void DrvUart4Init(uint32_t baudrate);
void DrvUart4SendBuf(u8 *data, u8 len);
void DrvUart5Init(uint32_t baudrate);
void DrvUart5SendBuf(u8 *data, u8 len);
void DrvUart6Init(uint32_t baudrate);
void DrvUart6SendBuf(u8 *data, u8 len);
void DrvUart7Init(uint32_t baudrate);
void DrvUart7SendBuf(u8 *data, u8 len);

void DrvUartDataCheck(void);

/*-------------------÷ÿ∂®“Âprintf------------------*/

#define UartSendLXIMU	DrvUart5SendBuf


#define print_u1 Printf_uart1
#define print_F4 Printf_uart2
#define print_u3 Printf_uart3
#define print_u4 Printf_uart4
#define print_u5 Printf_uart5

void Printf_uart0(char *format, ...);
void Printf_uart1(char *format, ...);
void Printf_uart2(char *format, ...);
void Printf_uart3(char *format, ...);
void Printf_uart4(char *format, ...);
void Printf_uart5(char *format, ...);
void Printf_uart6(char *format, ...);
void Printf_uart7(char *format, ...);

#endif
