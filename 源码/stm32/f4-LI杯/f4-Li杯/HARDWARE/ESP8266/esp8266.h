#ifndef _ESP8266_H
#define _ESP8266_H
#include "sys.h"
#include "stdio.h"


u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* esp8266_check_cmd(u8 *str);
u8* esp8266_send_data(u8 *cmd,u16 waittime);
u8 esp8266_quit_trans(void);
void esp8266_start_trans(void);
void esp8266_start_AP(void);


#endif

