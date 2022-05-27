//B站ID 亢亢kk 欢迎关注//

/*麦克风阵列开发日志
V0.01：
新增：实现SPI协议的收发功能（主机发送从机接收）。该测试功能已移除。
下一步打算：通过SPI接口实现灯珠SK9822序列的驱动开发。
V0.03：
新增：初步通过SPI2接口实现灯珠SK9822控制。
下一步打算：目前灯珠控制不太理想，稳定性很差，可能是SPI时序问题，需要继续尝试改进。
V0.04：
新增：
1、上次稳定性差是硬件接线问题，重新接线后一切正常。
2、重写了灯珠SK9822驱动，接口参数规范统一化，方便后续驱动移植。
下一步打算：将从stm32f103rb改为stm32f103ze，移植SK9822驱动，因为之后涉及I2S接口的开发，
小容量103系列不支持。
V0.045：
新增：
1、成功移植灯珠SK9822驱动。从SPI2改为使用SPI1。
2、初步实现I2S2控制时序，逻辑分析仪观察正常。
下一步打算：尝试进行麦克风芯片MSM261S4030H0驱动开发。
V0.60：
新增：实现中断接收I2S数据。
下一步打算：获取的数据严重异常，且收集数据时对MCU负荷很大（48k采样和8k采样时连串口都
发不出去）。
而且24bit数据只支持接收16bit，推测因为接收24bit等于中断间隔再减少一半，MCU处理不过来。
也可能是原板子上SD接口的FLASH芯片使能端导致的，之后打算把这个芯片焊下来再试试。
V0.90：
新增：
1、排除了FLASH芯片影响SD接口数据的原因，焊下来后无变化。
2、发现数据接收异常的原因是因为逻辑分析仪导致的，更换为示波器后数据正常，可从示波器
残影看出声音对麦克风编码的影响。（但是还是需要一台好点的逻辑分析仪，人肉分析我要死了QAQ）
下一步打算：等新的逻辑分析仪到了先看看数据是否真的正确，想着尝试导出数据先用MATLAB
绘图看看。
V0.92：
新增：新逻辑分析仪一切正常，通过导出数据再用MATLAB绘图发现数据正常。
下一步打算：尝试初步实现双麦克风的数据获取与数据处理。
V0.95：
新增：
1、成功实现了MIC0和MIC1的数据获取和左右声道数据分离（8k采样，16bit精度）。
下一步打算：发现SPI1和I2S2无法同时工作，导致麦克风和灯珠无法同时初始化，下一步解决这个问题。
V1.00：
新增：
1、解决了SPI1和I2S2同时初始化冲突的问题，可以同时正常初始化。
2、实现了双MIC（MIC0和MIC1）的粗略数据比较，通过SK9822指示方位。
下一步打算：
1、尝试解决只能控制一对MIC的缺陷。
2、尝试使用DMA接收数据，减轻MCU负担。（存疑，不知道stm32f103系列能否DMA接收后区分左右声道）
3、尝试增大存储空间。（受制于堆栈影响，最大申请数组大小只有2048）
V1.05：
新增：
1、初步实现了DMA接收数据。
2、针对I2S接口不足的致命缺陷，想到两个解决方案：（1）加入其它带有I2S接口的控制芯片(SPI接口
也能凑合用)。（2）使用一个复用芯片，时分复用完成全麦克风采样。初步决定采样方案2，采用模拟
2路四选一开关芯片74LV4052,已经绘制PCB，正在打样。等到货了焊接后验证是否可以实现。
下一步打算：DMA接收数据仍存在较大问题，体现在多次进入中断异常以及数据错位。
V1.08：
新增：
1、解决了DMA中断和错位问题。
2、完美的实现了DMA对麦克风数据的接收，采样率8k~48k，精度16bit/24bit均可正常实现。
下一步打算：重构主体代码，这段时间打了太多补丁，代码没法看了，还没写注释。
V1.10：
新增：
1、完成了main函数代码重构，优化了比较算法，加入了稳定机制，使得方位显示不再出现“频闪”现象。
2、板载4对麦克风均完成测试，均可单独实现双麦克风定位。
下一步打算：
1、优化获取数据最大长度只有2048的问题。
2、之前考虑不周，4052芯片是模拟信号复用芯片，对于MEMS麦克风应该采用数字信号复用芯片，遂重新
换了芯片为74HC153,重新绘制了PCB打样，等两个板子都到了比对一下。
V1.12：
新增：突破了最大长度2048的限制，不再采用全局长数组而是改为内存管理申请空间，最大可申请
40k空间。
下一步打算：之前打样的板子预计明天到，尝试实现全MIC时分复用采集。
V1.20：
新增：
1、扩展板功能验证通过，成功使用74HS4052实现了同一引脚多路I2S信号采集。
2、开发了74LV4052驱动，为下一步多路时分复用采集做好准备。
下一步打算：之前打样的板子预计明天到，尝试实现全MIC时分复用采集。
V1.50：
新增：
1、完全实现多路复用循环DMA采集，共采样6枚(3组)麦克风数据，每组麦克风采样间隔约35us，每轮麦克风
采样间隔约8.5ms。中间的第7枚麦克风暂时没用到，但在代码中保留了其定义，方便程序扩展。
2、根据新功能重构了代码，更进一步优化了算法，现在程序支持的最小扫描角度减小到原来的一倍，
即360°/12最小分辨角度。
3、采用LCD显示各个判决参数。暂时取消了灯珠显示功能。
下一步打算：终于成功了！在V1.50终于初步实现了最初的目的。之后打算等74HC153板子到了看看效果比对一下，
另外对于一些细节部分之后考虑再作出优化，以及尝试扩展一些功能等。灯珠显示功能将在下个版本马上回归。
V2.00：
新增：
1、灯珠显示功能已重新加入。
2、大幅度整合代码，整体优化了代码量。着重整合了模式控制变量以及LCD内容排版等。
3、将按键由查询方式改为了中断方式，按键响应现在更快了。
4、新增了声音到达麦克风大小粗判断，按KEY_UP来打开/关闭。
5、新增了模式选择功能。有三种模式：高速模式，中速精准模式以及最大采样低速模式。
各个模式的区别直观体现在响应速度和精度上。在硬件中体现为一轮采集数据大小、稳态
移位数据表大小、静默态判决门限来决定。
各模式数据(50MHz采样)：
高速模式：一轮采样间隔：36.280ms；一组采样间隔：36.08us；一组采样有效时间：2.545ms
中速精准模式：一轮采样间隔：16.586ms；一组采样间隔：36.08us；一组采样有效时间：10.212ms
最大采样低速模式：未观测到；一组采样间隔：36.28us；一组采样有效时间：未观测到
*/

#include "stm32f10x.h"
#include "SK9822.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "usart.h"
#include "MicArray.h"
#include "malloc.h"
#include "74LV4052.h"
#include <stdio.h>

/*公共全局变量*/
u16 i=0,j=0;
u8 s1[100];
u8 buff_len=2;//可用于控制精确定位精准程度与响应时间，模式三变量之一
u8 *buff;
u8 buff_repeat[2],buff_repeat_num[2],buff_mov;
int tmp;
u8 mode = 0;//0:高速模式，1:中速精准模式，2：最大采样低速模式。默认高速模式
/*74LV4052全局变量*/
u8 select_channel = 0;
/*MIC全局变量*/
u16 len = 1500;//一轮dma扫描数据总量(short为单位),可用于控制采集数据长短，模式三变量之一
short *dma_data;
u8 DMA_FLAG_END = 0,MIC_MAX_DIR,MIC_S_DIR,LAST_MIC_S_DIR=13;//13为静默态，12为保留态
u16 MIC_GROUP_MAX_DIR;
int max_voice;//左右声道数据(24b)
int threshold = 30000;//用于控制静默态门限值，模式三变量之一
/*LED全局变量*/
u8 led_data[12][4]= {0x1f,0x00,0x00,0x00,	//led阵列设置：亮度(最大0x1f)，蓝，绿，红
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00
                    };
u8 light_level = 1;
int level_set = 200000;
u8 LED_FLAG_LEVEL_ENABLE = 0;

void find_max_voice(void);
void data_proc(void);
void buff_proc(void);
void disp_proc(void);
void start_dma_get(u8 channel);

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    key_init();
    usart1_init();
    delay_init();
    LCD_Init();
    my_mem_init(SRAMIN);		//初始化内部内存池
    dma_data = mymalloc(SRAMIN,19500*2);//申请存储空间，接近最大len*2
    buff = mymalloc(SRAMIN,10);//申请存储空间，接近最大buff_len
    LCD_ShowString(8,10,32*18,24,24,"MicArray Test v2.00");

    POINT_COLOR=WHITE;
    BACK_COLOR=BLUE;
    LCD_Fill(0,50,239,109,BLUE);
    LCD_ShowString(8,50,240,16,16,"KEY0:   Off ->             ");
    LCD_ShowString(8,70,240,16,16,"KEY1:   0   -> MODE        ");
    LCD_ShowString(8,90,240,16,16,"KEY_UP: Off -> LEVEL_ENABLE");
    POINT_COLOR=BLUE;
    BACK_COLOR=WHITE;

    _74LV4052_init();//复用功能初始化
    SK9822_init();//LED阵列初始化
    MicArray_init(_74LV4052_Channel_0);//麦克风阵列初始化,从Channel0开始
    while(1)
    {
        if(DMA_FLAG_END)//DMA处理完成标志
        {
            DMA_FLAG_END = 0;//标志位清零


            start_dma_get(0);//直接开始下一轮DMA数据采集

            find_max_voice();	//确定最大声源方位范围以及大小
            data_proc();			//数据定位
            disp_proc();			//显示结果和参数

            LAST_MIC_S_DIR = MIC_S_DIR;//更新上一稳定态
            max_voice=0;//清零最大值，预备下一轮数据处理
        }
    }
}

void start_dma_get(u8 channel)
{
    _74LV4052_SelectChannel(channel);//重选Channel
    MicArray_init(channel);//开始下一组MIC数据采集
}

void find_max_voice(void)
{
    for(i=0; i<len; i+=2)
    {
        tmp = ((int)(dma_data[i])<<8)+((unsigned short)dma_data[i+1]>>8);
        if(tmp > max_voice)
        {
            MIC_GROUP_MAX_DIR=i;
            max_voice = tmp;
        }
    }
}

void data_proc(void)
{
    /*step1:粗判断组位置*/
    if(max_voice < threshold)//判断是否为静默态
        MIC_MAX_DIR = 13;//
    else if(MIC_GROUP_MAX_DIR<len/3)//根据有效最大值位置，粗略定位声源位置
        MIC_MAX_DIR = 0;
    else if(MIC_GROUP_MAX_DIR<len*2/3)
        MIC_MAX_DIR = 4;
    else if(MIC_GROUP_MAX_DIR<len)
        MIC_MAX_DIR = 8;

    /*step2:粗判断麦克风位置*/
    if(MIC_GROUP_MAX_DIR%4 && MIC_MAX_DIR != 13)//非静默态时，判断左右声道
        MIC_MAX_DIR+=2;

    /*step3:精确定位*/
    buff_proc();	//历史数据更新
    MIC_S_DIR = MIC_MAX_DIR;//假定当前最大值为稳定态

    buff_repeat[0]=buff[0];
    for(i=1,buff_mov=0; i<buff_len; i++) //遍历buff，寻找是否可能是中间情况
    {
        for(j=0; j<=buff_mov; j++)	//从已记录的元素中遍历
        {
            if(buff[i]==buff_repeat[j])//如果重复
            {
                //buff_repeat_num[j]++;
                break;
            }
        }
        if(j>buff_mov)//未找到该元素
        {
            buff_mov++;
            if(buff_mov>1)
            {
                MIC_S_DIR = LAST_MIC_S_DIR;//非稳态情况，保持上一次稳定状态
                break;
            }
            else
                buff_repeat[buff_mov] = buff[i];//更新buff_repeat表
        }
    }
    //buff_repeat_num[0] = 0;
    //buff_repeat_num[1] = 0;

    if(i == buff_len)//只有两种元素情况
    {
        if(buff_mov)
        {
            MIC_S_DIR = ((buff_repeat[0]>buff_repeat[1]) ? (buff_repeat[0]-buff_repeat[1]):(buff_repeat[1]-buff_repeat[0]));
            if(MIC_S_DIR == 2)
                MIC_S_DIR = ((buff_repeat[0]>buff_repeat[1]) ? buff_repeat[1]:buff_repeat[0])+1;
            else if(MIC_S_DIR == 10)
                MIC_S_DIR = 11;
            else
                MIC_S_DIR = LAST_MIC_S_DIR;
        }
    }
}

void buff_proc(void)
{
    for(i=0; i<buff_len-1; i++)
        buff[i]=buff[i+1];
    buff[buff_len-1]=MIC_MAX_DIR;
}

void disp_proc(void)
{
    /*led_data数据处理*/
    for(i=0; i<12; i++)
    {
        if(MIC_S_DIR != i)
        {
            led_data[i][1] = 0x00;
            led_data[i][3] = 0x00;
        }
        else
        {
            if(LED_FLAG_LEVEL_ENABLE)
            {
                if(max_voice < level_set)
                    light_level = 1;
                else
                    light_level = 2;
            }
            else
                light_level = 1;
            if(light_level == 1)//蓝色
            {
                led_data[i][1] = 0xff;
                led_data[i][3] = 0x00;
            }
            else if(light_level == 2)//红色
            {
                led_data[i][1] = 0x00;
                led_data[i][3] = 0xff;
            }
        }
    }
    /*SK9822显示*/
    SK9822_disp(12,led_data);
    /*LCD参数显示*/
    sprintf((char*)s1,"max_voice = %08d     ",max_voice);
    LCD_ShowString(30,110,200,16,16,s1);
    sprintf((char*)s1,"MIC_GROUP_MAX_DIR = %04u    ",MIC_GROUP_MAX_DIR);
    LCD_ShowString(30,130,200,16,16,s1);
    sprintf((char*)s1,"MIC_MAX_DIR = %02u    ",MIC_MAX_DIR);
    LCD_ShowString(30,150,200,16,16,s1);
    sprintf((char*)s1,"MIC_S_DIR = %02u    ",MIC_S_DIR);
    LCD_ShowString(30,170,200,16,16,s1);
}

void EXTI0_IRQHandler(void)
{
    EXTI_ClearFlag(EXTI_Line0);
    delay_ms(5);
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
        LED_FLAG_LEVEL_ENABLE = !LED_FLAG_LEVEL_ENABLE;
        if(LED_FLAG_LEVEL_ENABLE)
            LCD_ShowString(8,90,240,16,16,"KEY_UP: On  -> LEVEL_ENABLE");
        else
            LCD_ShowString(8,90,240,16,16,"KEY_UP: Off -> LEVEL_ENABLE");
    }
}

void EXTI3_IRQHandler(void)//设置模式
{
    EXTI_ClearFlag(EXTI_Line3);
    delay_ms(5);
    if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3))
    {
        mode++;
        if(mode>2)
            mode = 0;
        if(mode == 0)//高速模式
        {
            len = 1500;
            buff_len = 2;
            threshold = 30000;
        }
        else if(mode == 1)//中速精确模式
        {
            len = 6000;
            buff_len = 4;
            threshold = 15000;
        }
        else//最大采样低速模式
        {
            len = 19500;
            buff_len = 10;
            threshold = 15000;
        }
        sprintf((char*)s1,"KEY1:   %u   -> MODE        ",mode);
        LCD_ShowString(8,70,240,16,16,s1);
    }
}

void DMA1_Channel4_IRQHandler(void)//DMA传输完成中断函数
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
    {
        SPI_I2S_DeInit(SPI2);//如没有重配置SPI2，会出现DMA接收错位
        DMA_DeInit(DMA1_Channel4);//如没有DMA中断重配置，DMA将无法进入中断

        select_channel++;//选择下一通道
        if(select_channel>_74LV4052_Channel_2)//扫描完成一轮
        {
            select_channel = _74LV4052_Channel_0;
            DMA_FLAG_END = 1;
        }
        else
        {
            start_dma_get(select_channel);
        }
    }
}
