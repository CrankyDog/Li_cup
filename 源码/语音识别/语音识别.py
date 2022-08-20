import time
from Maix import GPIO, I2S
from fpioa_manager import fm
import os, Maix, lcd, image
img = image.Image(size=(320, 240))
io_led_red = 13
fm.register(io_led_red, fm.fpioa.GPIO0)
led_r=GPIO(GPIO.GPIO0, GPIO.OUT)
led_r.value(1)
#采样频率
sample_rate   = 16000
record_time   = 4  #s
#配置一个 I2S.DEVICE_0 设备
fm.register(20,fm.fpioa.I2S0_IN_D0, force=True)
fm.register(18,fm.fpioa.I2S0_SCLK, force=True) # bit
fm.register(19,fm.fpioa.I2S0_WS, force=True) # bit
#设置 CHANNEL_0 通道到录音输入
rx = I2S(I2S.DEVICE_0)
rx.channel_config(rx.CHANNEL_0, rx.RECEIVER, align_mode=I2S.STANDARD_MODE)
rx.set_sample_rate(sample_rate)
print(rx)

from speech_recognizer import asr
#创建 maix_asr 的辅助类
class maix_asr(asr):

  asr_vocab = ["lv", "shi", "yang", "chun", "yan", "jing", "da", "kuai", "wen", "zhang", "de", "di", "se", "si", "yue", "lin", "luan", "geng", "xian", "huo", "xiu", "mei", "yi", "ang", "ran", "ta", "jin", "ping", "yao", "bu", "li", "liang", "zai", "yong", "dao", "shang", "xia", "fan", "teng", "dong", "she", "xing", "zhuang", "ru", "hai", "tun", "zhi", "tou", "you", "ling", "pao", "hao", "le", "zha", "zen", "me", "zheng", "cai", "ya", "shu", "tuo", "qu", "fu", "guang", "bang", "zi", "chong", "shui", "cuan", "ke", "shei", "wan", "hou", "zhao", "jian", "zuo", "cu", "hei", "yu", "ce", "ming", "dui", "cheng", "men", "wo", "bei", "dai", "zhe", "hu", "jiao", "pang", "ji", "lao", "nong", "kang", "yuan", "chao", "hui", "xiang", "bing", "qi", "chang", "nian", "jia", "tu", "bi", "pin", "xi", "zou", "chu", "cun", "wang", "na", "ge", "an", "ning", "tian", "xiao", "zhong", "shen", "nan", "er", "ri", "zhu", "xin", "wai", "luo", "gang", "qing", "xun", "te", "cong", "gan", "lai", "he", "dan", "wei", "die", "kai", "ci", "gu", "neng", "ba", "bao", "xue", "shuai", "dou", "cao", "mao", "bo", "zhou", "lie", "qie", "ju", "chuan", "guo", "lan", "ni", "tang", "ban", "su", "quan", "huan", "ying", "a", "min", "meng", "wu", "tai", "hua", "xie", "pai", "huang", "gua", "jiang", "pian", "ma", "jie", "wa", "san", "ka", "zong", "nv", "gao", "ye", "biao", "bie", "zui", "ren", "jun", "duo", "ze", "tan", "mu", "gui", "qiu", "bai", "sang", "jiu", "yin", "huai", "rang", "zan", "shuo", "sha", "ben", "yun", "la", "cuo", "hang", "ha", "tuan", "gong", "shan", "ai", "kou", "zhen", "qiong", "ding", "dang", "que", "weng", "qian", "feng", "jue", "zhuan", "ceng", "zu", "bian", "nei", "sheng", "chan", "zao", "fang", "qin", "e", "lian", "fa", "lu", "sun", "xu", "deng", "guan", "shou", "mo", "zhan", "po", "pi", "gun", "shuang", "qiang", "kao", "hong", "kan", "dian", "kong", "pei", "tong", "ting", "zang", "kuang", "reng", "ti", "pan", "heng", "chi", "lun", "kun", "han", "lei", "zuan", "man", "sen", "duan", "leng", "sui", "gai", "ga", "fou", "kuo", "ou", "suo", "sou", "nu", "du", "mian", "chou", "hen", "kua", "shao", "rou", "xuan", "can", "sai", "dun", "niao", "chui", "chen", "hun", "peng", "fen", "cang", "gen", "shua", "chuo", "shun", "cha", "gou", "mai", "liu", "diao", "tao", "niu", "mi", "chai", "long", "guai", "xiong", "mou", "rong", "ku", "song", "che", "sao", "piao", "pu", "tui", "lang", "chuang", "keng", "liao", "miao", "zhui", "nai", "lou", "bin", "juan", "zhua", "run", "zeng", "ao", "re", "pa", "qun", "lia", "cou", "tie", "zhai", "kuan", "kui", "cui", "mie", "fei", "tiao", "nuo", "gei", "ca", "zhun", "nie", "mang", "zhuo", "pen", "zun", "niang", "suan", "nao", "ruan", "qiao", "fo", "rui", "rao", "ruo", "zei", "en", "za", "diu", "nve", "sa", "nin", "shai", "nen", "ken", "chuai", "shuan", "beng", "ne", "lve", "qia", "jiong", "pie", "seng", "nuan", "nang", "miu", "pou", "cen", "dia", "o", "zhuai", "yo", "dei", "n", "ei", "nou", "bia", "eng", "den", "_"]

  def get_asr_list(string='xiao-luo'):
    return [__class__.asr_vocab.index(t) for t in string.split('-') if t in __class__.asr_vocab]

  def get_asr_string(listobj=[117, 214, 257, 144]):
    return '-'.join([__class__.asr_vocab[t] for t in listobj if t < len(__class__.asr_vocab)])

  def unit_test():
    print(__class__.get_asr_list('xiao-luo'))
    print(__class__.get_asr_string(__class__.get_asr_list('xiao-ai-fas-tong-xue')))

  def config(self, sets):
    self.set([(sets[key], __class__.get_asr_list(key)) for key in sets])

  def recognize(self):
    res = self.result()
    if res != None:
      sets = {}
      for tmp in res:
        sets[__class__.get_asr_string(tmp[1])] = tmp[0]
      return sets
    return None

from machine import Timer

def on_timer(timer):
  timer.callback_arg().state()

try:
  # default: maix dock / maix duino set shift=0
  t = maix_asr(0x500000, I2S.DEVICE_0, 3, shift=1) # maix bit set shift=1
  tim = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PERIODIC, period=64, callback=on_timer, arg=t)
  tim.start()

 t.config({
    'xiao-luo' : 0.3, 'kai-shi-ce-shi' : 0.3,
    'da-kai-ding-wei' : 0.2, 'guan-bi-ding-wei' : 0.2,
    'ti-liang-yin-se' : 0.3, 'hui-fu-yin-se' : 0.3,
    'da-kai-qu-zao' : 0.3, 'guan-bi-qu-zao' : 0.3,
  })


  print(t.get())
  img.draw_rectangle((0, 0, 320, 240), fill=True, color=(255, 255, 255))
  img.draw_string(10,80, "Please speak", color=(255, 0, 0), scale=2, mono_space=0)
  lcd.display(img)

  while True:
    tmp = t.recognize()
    if tmp != None:
      print(tmp)
      string = str(tmp)
      a= string.split("'")
      print(a[1])
      img.draw_rectangle((0, 0, 320, 240), fill=True, color=(255, 255, 255))
      img.draw_string(10,80, "%s"%(a[1]), color=(255, 0, 0), scale=3, mono_space=0)
      lcd.display(img)
      if a[1]=='kai-deng':
        led_r.value(0)
      if a[1]=='guan-deng':
        led_r.value(1)


except Exception as e:
  print(e)
finally:
  tim.stop()
  t.__del__()
  del t

