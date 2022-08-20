from Maix import MIC_ARRAY as mic
import lcd,time
import math
from machine import UART,Timer
from fpioa_manager import fm
from Maix import GPIO
mic.init()#可自定义配置 IO

#映射串口引脚
fm.register(9, fm.fpioa.UART1_RX, force=True)
fm.register(10, fm.fpioa.UART1_TX, force=True)
#初始化串口
uart = UART(UART.UART1, 115200, read_buf_len=4096)

send_buf = '0'
send_permission = 0
dis_value=0
#定时器回调函数
def fun0(tim0):
    global send_permission
    send_permission = 1

#定时器0初始化
tim0 = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PERIODIC, period=10, callback=fun0)

def get_mic_dir():
    global dis_value
    AngleX=0
    AngleY=0
    AngleR=0
    Angle=0
    AngleAddPi=0
    mic_list=[]
    shreshold=2
    imga = mic.get_map()    # 获取声音源分布图像
    b = mic.get_dir(imga)   # 计算、获取声源方向
    c = list(b)
    for i in range(len(b)):
        if b[i]>=shreshold:
            AngleX+= b[i]*math.sin(i*math.pi/6)
            AngleY+= b[i]*math.cos(i*math.pi/6)
        else:
            c[i]=0
    d = tuple(c)
    AngleX=round(AngleX,6) #计算坐标转换值
    AngleY=round(AngleY,6)
    if AngleY<0:AngleAddPi=180
    if AngleX<0 and AngleY > 0:AngleAddPi=360
    if AngleX!=0 or AngleY!=0: #参数修正
        if AngleY==0:
            Angle=90 if AngleX>0 else 270 #填补X轴角度
            dis_value=Angle
        else:
            Angle=AngleAddPi+round(math.degrees(math.atan(AngleX/AngleY)),4) #计算角度
            dis_value=Angle
        AngleR=round(math.sqrt(AngleY*AngleY+AngleX*AngleX),4) #计算强度
        #print(AngleR)
    #if AngleR<shreshold:Angle=0
    mic_list.append(AngleX)
    mic_list.append(AngleY)
    mic_list.append(AngleR)
    mic_list.append(Angle)
    a = mic.set_led(d,(0,0,255))# 配置 RGB LED 颜色值
    return mic_list #返回列表，X坐标，Y坐标，强度，角度

cnt = 0
col_angle_buf = 0
col_angle = 0
angle = 0
sum_anl = 0
sta_cnt = 0
stable_angle = 0
avg_angle = 0
avg_angle_buf = 0
stg_angle = 0
stg_angle_buf = 0
flag1 = 0
flag2 = 0
sta_angle_buf = 0
def get_filter_angle():
    global cnt
    global angle
    global sum_anl
    global col_angle
    global col_angle_buf
    global avg_angle
    global flag1
    global flag2
    global dis_value
    global sta_angle_buf
    col_angle = col_angle_buf
    a = get_mic_dir()
    sta_angle_buf = kalman_filter.kalman(dis_value)
    if a[3]:
        col_angle_buf = a[3]
        if abs(col_angle-col_angle_buf)< 10 and flag1== 0 and flag2 == 0:
            sum_anl = sum_anl + col_angle_buf
            cnt = cnt + 1
        elif (col_angle-col_angle_buf > 180 or flag1 != 0) and flag2 == 0:
            if flag1 == 0:
                cnt = 0
                sum_anl = 0
            flag1 = flag1 + 1
            if(col_angle_buf < 60):
                sum_anl = sum_anl + col_angle_buf +360
                cnt = cnt+1
            else:
                sum_anl = sum_anl + col_angle_buf
                cnt = cnt+1
        elif (col_angle_buf-col_angle > 330 or flag2 != 0) and flag1 == 0:
            if flag2 == 0:
                cnt = 0
                sum_anl = 0
            flag2 = flag2 + 1
            if(col_angle_buf > 300):
                sum_anl = sum_anl + col_angle_buf -360
                cnt = cnt+1
            else:
                sum_anl = sum_anl + col_angle_buf
                cnt = cnt+1
        else :
            cnt =0
            sum_anl = 0

    if cnt == 3:
        if flag1 != 0 or flag2!= 0 or col_angle_buf >= 325 or col_angle_buf < 10:
            avg_angle = (sum_anl/cnt)%360
            #print("pinghua:",avg_angle)
        else :
            avg_angle = sta_angle_buf
            #print("kaerman:",avg_angle,"col_angle_buf:",col_angle_buf)

        sum_anl = 0
        cnt = 0
        flag1 = 0
        flag2 = 0
    #print("return:",avg_angle)
    return avg_angle

class kalman_filter:
    def __init__(self,Q,R):
        self.Q = Q
        self.R = R

        self.P_k_k1 = 1
        self.Kg = 0
        self.P_k1_k1 = 1
        self.x_k_k1 = 0
        self.ADC_OLD_Value = 0
        self.Z_k = 0
        self.kalman_adc_old=0

    def kalman(self,ADC_Value):

        self.Z_k = ADC_Value

        if (abs(self.kalman_adc_old-ADC_Value)>=60 and abs(self.kalman_adc_old-ADC_Value)<=330):
            self.x_k1_k1= ADC_Value*0.382 + self.kalman_adc_old*0.618
        else:
            self.x_k1_k1 = self.kalman_adc_old;

        self.x_k_k1 = self.x_k1_k1
        self.P_k_k1 = self.P_k1_k1 + self.Q

        self.Kg = self.P_k_k1/(self.P_k_k1 + self.R)

        kalman_adc = self.x_k_k1 + self.Kg * (self.Z_k - self.kalman_adc_old)
        self.P_k1_k1 = (1 - self.Kg)*self.P_k_k1
        self.P_k_k1 = self.P_k1_k1

        self.kalman_adc_old = kalman_adc

        return kalman_adc

kalman_filter =  kalman_filter(0.001,0.1)


#ef get_sta_angle():
#   global stg_angle_buf
#   global stg_angle
#   global sta_cnt
#   global stable_angle
#   stg_angle = stg_angle_buf
#   stg_angle_buf = get_filter_angle()
#   if abs(stg_angle_buf-stg_angle)< 3:
#       sta_cnt = sta_cnt + 1
#   else:
#       sta_cnt = 0
#   if sta_cnt == 5:
#       sta_cnt = 0
#       stable_angle = stg_angle_buf
#   return stable_angle



while True:
    sta_angle = 0
    sta_angle = get_filter_angle()
    #imf=get_mic_dir()
    #if dis_value<330 and  dis_value>15:
    #    sta_angle = kalman_filter.kalman(dis_value)
    #else:
    #    sta_angle = dis_value
    if sta_angle:
        if sta_angle < 344 and sta_angle > 18:
            sta_angle=sta_angle+15
    print(sta_angle)
    send_buf = str(int(round(sta_angle, 0)))#将浮点型转为字符串
    if send_permission==1:
        uart.write(send_buf)
        uart.write(b'\x0d')
        uart.write(b'\x0a')
        send_permission=0
