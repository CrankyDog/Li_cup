#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "sys.h" 

#define USART3_MAX_RECV_LEN  			200
#define USART3_MAX_SEND_LEN  			200

extern vu16 USART3_RX_STA;
extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN];


void usart3_init(u32 bound);
void u3_printf(char* fmt,...);
#endif