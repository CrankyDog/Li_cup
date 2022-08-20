import sensor, image, lcd, time, utime,network,usocket
import KPU as kpu
from machine import UART,Timer
from fpioa_manager import fm
from Maix import GPIO
import network,usocket,time
from machine import UART,Timer
from Maix import GPIO
from fpioa_manager import fm

#映射串口引脚
fm.register(19, fm.fpioa.UART1_RX, force=True)
fm.register(20, fm.fpioa.UART1_TX, force=True)
fm.register(6, fm.fpioa.UART2_RX, force=True)
fm.register(7, fm.fpioa.UART2_TX, force=True)

SSID='ATK-ESP8266' # WiFi 账号
KEY='12345678'  # WiFi 密码

data=None

#socket数据接收中断标志位
socket_node = 0

###### WiFi模块初始化 ######
#使能引脚初始化
fm.register(8, fm.fpioa.GPIOHS0, force=True)
wifi_en=GPIO(GPIO.GPIOHS0, GPIO.OUT)

#初始化串口
uart1 = UART(UART.UART1, 9600, read_buf_len=4096)
##uart_32 = UART(UART.UART2, 115200, read_buf_len=4096)
uart = UART(UART.UART2,115200,timeout=1000,read_buf_len=4096)

color_R = (255, 0, 0)
color_G = (0, 255, 0)
color_B = (0, 0, 255)


class_IDs = ['no_mask', 'mask']


def drawConfidenceText(image, rol, classid, value):
    text = ""
    _confidence = int(value * 100)

    if classid == 1:
        text = 'mask: ' + str(_confidence) + '%'
        color_text=color_G
    else:
        text = 'no_mask: ' + str(_confidence) + '%'
        color_text=color_R
    image.draw_string(rol[0], rol[1], text, color=color_text, scale=2.5)

#WiFi使能函数
def wifi_enable(en):
    global wifi_en
    wifi_en.value(en)

#WiFi对象初始化，波特率需要修改
def wifi_init():
    global uart
    wifi_enable(0)
    time.sleep_ms(200)
    wifi_enable(1)
    time.sleep(2)
    uart = UART(UART.UART2,115200,timeout=1000, read_buf_len=4096)
    tmp = uart.read()
    uart.write("AT+UART_CUR=921600,8,1,0,0\r\n")
    print(uart.read())
    uart = UART(UART.UART2,921600,timeout=1000, read_buf_len=10240) #实测模块波特率太低或者缓存长度太短会导致数据丢失。
    uart.write("AT\r\n")
    tmp = uart.read()
    print(tmp)
    if not tmp.endswith("OK\r\n"):
        print("reset fail")
        return None
    try:
        nic = network.ESP8285(uart)
    except Exception:
        return None
    return nic

lcd.init()
sensor.reset(dual_buff=True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(0)
sensor.run(1)
sensor.set_vflip(1)    #设置摄像头后置

#标志变量
unmasked_waiting=0
masked_waiting=0
unmask_confirm=0
mask_confirm=0

#定时器回调函数
def fun0(tim0):
    global unmasked_waiting
    unmasked_waiting = 0
    print(unmasked_waiting)

#定时器0初始化
tim0 = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_ONE_SHOT, period=4000, callback=fun0, start=False)

#定时器回调函数
def fun1(tim1):
    global masked_waiting
    masked_waiting = 0
    print(masked_waiting+1)

#定时器1初始化
tim1 = Timer(Timer.TIMER1, Timer.CHANNEL0, mode=Timer.MODE_ONE_SHOT, period=4000, callback=fun1, start=False)

###定时器回调函数
##def fun2(tim):
##    global socket_node
##    socket_node = 1 #改变 socket 标志位
##    if socket_node:
##        try:
##            client.connect(addr)
##            print("接收?")
##            data = client.recv(10)
##            print(data)
##        except OSError:
##            data = None
##            print("无")
##        if data:
##            print("收到！")
##            print("rcv:", len(data),data)
##        socket_node = 0
##    else: pass
##
###定时器2初始化
##tim2 = Timer(Timer.TIMER2, Timer.CHANNEL0, mode=Timer.MODE_PERIODIC, period=100, callback=fun2)

task = kpu.load(0x300000)


anchor = (0.1606, 0.3562, 0.4712, 0.9568, 0.9877, 1.9108, 1.8761, 3.5310, 3.4423, 5.6823)
_ = kpu.init_yolo2(task, 0.5, 0.3, 5, anchor)
img_lcd = image.Image()
unmasked_cmd = [b'\x7e',b'\x05',b'\x41',b'\x00',b'\x01',b'\x45',b'\xef']
masked_cmd = [b'\x7e',b'\x05',b'\x41',b'\x00',b'\x02',b'\x46',b'\xef']
#stm32_cmd =b'\x66'
stm32_cmd =[b'\x31',b'\x0D',b'\x0A']
ismasked = 0
isunmasked = 0

#构建WiFi对象并使能
wlan = wifi_init()

#正在连接印提示
print("Trying to connect... (may take a while)...")

#连接网络
wlan.connect(SSID,KEY)

#打印IP相关信息
print(wlan.ifconfig())

#创建socket连接，连接成功后发送“Hello 01Studio！”给服务器。
client=usocket.socket()

addr=('192.168.4.1',8080) #服务器IP和端口

starttesting=0
clock = time.clock()
while (True):
    clock.tick()
    img = sensor.snapshot()
    img.rotation_corr(1,1,90,1,1,1)
    img.pix_to_ai()
    code = kpu.run_yolo2(task, img)
    if code:
        totalRes = len(code)

        for item in code:
            confidence = float(item.value())
            itemROL = item.rect()
            classID = int(item.classid())

            if confidence < 0.65:        #0.52
                _ = img.draw_rectangle(itemROL, color=color_B, tickness=5)
                continue

            if classID == 1 and confidence > 0.65:
                _ = img.draw_rectangle(itemROL, color_G, tickness=5)
                if totalRes == 1:
                    drawConfidenceText(img, (80, 0), 1, confidence)
                    mask_confirm=mask_confirm+1
                    if mask_confirm == 7:
                        ismasked=1
                        mask_confirm=0
                        unmask_confirm=0
            else:
                _ = img.draw_rectangle(itemROL, color=color_R, tickness=5)
                if totalRes == 1:
                    drawConfidenceText(img, (80, 0), 0, confidence)
                    unmask_confirm=unmask_confirm+1
                    if unmask_confirm == 7:
                        isunmasked=1
                        unmask_confirm=0
                        mask_confirm=0
    lcd.rotation(3)
    _ = lcd.display(img)

    '''print(clock.fps())'''
    if isunmasked == 1:
        if unmasked_waiting == 0:
            for i in unmasked_cmd:
                uart1.write(i)
                utime.sleep_ms(5)
            unmasked_waiting = 1
            tim0.start()
        isunmasked = 0
    if ismasked == 1:
        if masked_waiting == 0:
#            for i in masked_cmd:
#                uart1.write(i)
#                utime.sleep_ms(5)
##            for i in stm32_cmd:
##                uart_32.write(i)
##                ##utime.sleep_ms(5)
            if starttesting == 0:
                try:
                    client.connect(addr)
                    client.send('1')
                    client.settimeout(0.1)
                    client.close()
                    print(2)
                except:
                    print(32)
                ##starttesting=1
            masked_waiting = 1
            tim1.start()
        ismasked = 0



_ = kpu.deinit(task)
