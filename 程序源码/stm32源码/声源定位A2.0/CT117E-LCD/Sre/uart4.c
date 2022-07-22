#include "sys.h"
#include "uart4.h"


u8 UART4_RX_BUF[UART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
vu16 UART4_RX_STA=0;       //����״̬���

void uart4_init(u32 bound) {
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��USART1��GPIOAʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

    //USART1_TX   GPIOC.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PC.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //USART1_RX	  GPIOC.3��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PC.3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure); //��ʼ������1
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ���1

}


/**
  * ��������: COM����1���ֽ�
  * �������: data ����1���ֽ�����
  * �� �� ֵ: ��
  * ˵    ������
  */
void UART4_SendByte(u8 data)
{ 	
		/*��������*/
	USART_SendData(UART4,(u8)data);
	/*�ȴ�������ϣ��ڷ�������ж�����æ��־*/
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==0);
}

/**
  * ��������: COM�����ַ���
  * �������: data �����ַ����׵�ַ
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: COM���Ͷ���ַ�
  * �������: data �����ַ����׵�ַ
  * �� �� ֵ: ��
  * ˵    ������
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

void UART4_IRQHandler(void)                	//����1�жϷ������
{
    u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntEnter();
#endif
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res =USART_ReceiveData(UART4);	//��ȡ���յ�������

        if((UART4_RX_STA&0x8000)==0)//����δ���
        {
            if(UART4_RX_STA&0x4000)//���յ���0x0d
            {
                if(Res!=0x0a)UART4_RX_STA=0;//���մ���,���¿�ʼ
                else UART4_RX_STA|=0x8000;	//���������
            }
            else //��û�յ�0X0D
            {
                if(Res==0x0d)UART4_RX_STA|=0x4000;
                else
                {
                    UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
                    UART4_RX_STA++;
                    if(UART4_RX_STA>(UART4_REC_LEN-1))UART4_RX_STA=0;//�������ݴ���,���¿�ʼ����
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntExit();
#endif
}







