import sensor, image, lcd, time, utime
import KPU as kpu
from machine import UART,Timer
from fpioa_manager import fm

#映射串口引脚
fm.register(6, fm.fpioa.UART1_RX, force=True)
fm.register(7, fm.fpioa.UART1_TX, force=True)

#初始化串口
uart = UART(UART.UART1, 9600, read_buf_len=4096)

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
tim0 = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_ONE_SHOT, period=5000, callback=fun0, start=False)

#定时器回调函数
def fun1(tim1):
    global masked_waiting
    masked_waiting = 0
    print(masked_waiting+1)

#定时器1初始化
tim1 = Timer(Timer.TIMER1, Timer.CHANNEL0, mode=Timer.MODE_ONE_SHOT, period=5000, callback=fun1, start=False)

task = kpu.load(0x300000)


anchor = (0.1606, 0.3562, 0.4712, 0.9568, 0.9877, 1.9108, 1.8761, 3.5310, 3.4423, 5.6823)
_ = kpu.init_yolo2(task, 0.5, 0.3, 5, anchor)
img_lcd = image.Image()
unmasked_cmd = [b'\x7e',b'\x05',b'\x41',b'\x00',b'\x01',b'\x45',b'\xef']
masked_cmd = [b'\x7e',b'\x05',b'\x41',b'\x00',b'\x02',b'\x46',b'\xef']
ismasked = 0
isunmasked = 0

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
                    if mask_confirm == 10:
                        ismasked=1
                        mask_confirm=0
                        unmask_confirm=0
            else:
                _ = img.draw_rectangle(itemROL, color=color_R, tickness=5)
                if totalRes == 1:
                    drawConfidenceText(img, (80, 0), 0, confidence)
                    unmask_confirm=unmask_confirm+1
                    if unmask_confirm == 10:
                        isunmasked=1
                        unmask_confirm=0
                        mask_confirm=0
    lcd.rotation(3)
    _ = lcd.display(img)

    '''print(clock.fps())'''
    if isunmasked == 1:
        if unmasked_waiting == 0:
            for i in unmasked_cmd:
                uart.write(i)
                utime.sleep_ms(5)
            unmasked_waiting = 1
            tim0.start()
        isunmasked = 0
    if ismasked == 1:
        if masked_waiting == 0:
            for i in masked_cmd:
                uart.write(i)
                utime.sleep_ms(5)
            masked_waiting = 1
            tim1.start()
        ismasked = 0

_ = kpu.deinit(task)
