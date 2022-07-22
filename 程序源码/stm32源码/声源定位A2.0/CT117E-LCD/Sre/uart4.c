#include "sys.h"
#include "uart4.h"


u8 UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
vu16 UART4_RX_STA=0;       //接收状态标记

void uart4_init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能USART1，GPIOA时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

    //USART1_TX   GPIOC.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PC.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //USART1_RX	  GPIOC.3初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PC.3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(UART4, &USART_InitStructure); //初始化串口1
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(UART4, ENABLE);                    //使能串口1

}


/**
  * 函数功能: COM发送1个字节
  * 输入参数: data 发送1个字节数据
  * 返 回 值: 无
  * 说    明：无
  */
void UART4_SendByte(u8 data)
{ 	
		/*发送数据*/
	USART_SendData(UART4,(u8)data);
	/*等待发送完毕，在发送完成中断清零忙标志*/
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==0);
}

/**
  * 函数功能: COM发送字符串
  * 输入参数: data 发送字符串首地址
  * 返 回 值: 无
  * 说    明：无
  */
void UART4_SendStr(u8* s)
{ 
	while(*s)
	{
	  UART4_SendByte(*s);
    s++;		
	}
}
/**
  * 函数功能: COM发送多个字符
  * 输入参数: data 发送字符串首地址
  * 返 回 值: 无
  * 说    明：无
  */
void UART4_SendBytes(u8* data,int32_t len)
{ 
	while(len)
	{
	  UART4_SendByte(*data);
		data++;
    len--;		
	}
}

void UART4_IRQHandler(void)                	//串口1中断服务程序
{
    u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
    OSIntEnter();
#endif
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res =USART_ReceiveData(UART4);	//读取接收到的数据

        if((UART4_RX_STA&0x8000)==0)//接收未完成
        {
            if(UART4_RX_STA&0x4000)//接收到了0x0d
            {
                if(Res!=0x0a)UART4_RX_STA=0;//接收错误,重新开始
                else UART4_RX_STA|=0x8000;	//接收完成了
            }
            else //还没收到0X0D
            {
                if(Res==0x0d)UART4_RX_STA|=0x4000;
                else
                {
                    UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
                    UART4_RX_STA++;
                    if(UART4_RX_STA>(UART4_REC_LEN-1))UART4_RX_STA=0;//接收数据错误,重新开始接收
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
    OSIntExit();
#endif
}







