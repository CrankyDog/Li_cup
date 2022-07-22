单片机A2	串口2发送指令控制声源定位的开启和均衡器的控制，串口1接收单片机A1发送来的数据并转到指定角度

USART	usart1	PA9（TX），PA10（RX）		串口屏
		usart2	PA2（TX），PA3（TX）	与声源定位单片机连接
		usart3	PB10（TX），PB11（RX）	WiFi

GPIO	均衡器调整  位数从高往低	PE4，PE3，PE2，PE1，PE0 
	蜂鸣器控制	PD0，PD1，PD2，PD3，PD4，PD5 
	Moku：go多位选择控制线	PC0，PC1
	舵机信号pwm输出	PA0
	舵机数据反馈线AD1	PA1
	舵机moku：go PID输入 DA	PA4
	moku：go输出采样AD2	PA5
	



