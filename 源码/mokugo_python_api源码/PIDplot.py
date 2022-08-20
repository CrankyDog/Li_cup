# moku example: Basic PID Controller
#
# This script demonstrates how to configure one of the two PID Controllers
# in the PID Controller instrument. Configuration is done by specifying
# frequency response characteristics of the controller.
#
# (c) 2021 Liquid Instruments Pty. Ltd.
#

import matplotlib.pyplot as plt
from moku.instruments import PIDController

# Connect to your Moku by its ip address using PIDController('192.168.###.###')
# or by its serial number using PIDController(serial=123)
i = PIDController('192.168.73.1', force_connect=False)

try:
    # Configures the control matrix:
    # Channel 1: input 1 gain = 1 dB, input 2 gain = 0 dB
    # Channel 2: input 2 gain = 0 dB, input 2 gain = 1 dB
    i.set_control_matrix(channel=1, input_gain1=1, input_gain2=0)
    i.set_control_matrix(channel=2, input_gain1=0, input_gain2=1)

    # Configure PID Control loop 1 using frequency response characteristics
    #   P = -10dB
    #   I Crossover = 100Hz
    #   D Crossover = 10kHz
    #   I Saturation = 10dB
    #   D Saturation = 10dB
    #   Double-I = OFF
    i.set_by_frequency(channel=2, prop_gain=-8.0, int_crossover=0.3125,
                       diff_crossover=312500, int_saturation=-8.0,
                       diff_saturation=-8.0)

    #  Configure PID Control loop 2 using gain
    #   Proportional gain = 10
    #   Differentiator gain = -5
    #   Differentiator gain corner = 5 kHz

    # Enable the inputs and outputs of the PID controller
    i.enable_input(2, True)
    i.enable_output(2, signal=True, output=True)

    i.set_monitor(1, 'Output2')
    i.set_monitor(2, 'Input2')
    i.set_timebase(0, 8)

    data = i.get_data()
    data_buf = data
    plt.ion()
    plt.show()
    plt.grid(visible=True)
    plt.ylim([-5, 5])
    plt.xlim([0, 8])
    plt.title('PID坐标图')
    plt.xlabel("时间/s")
    plt.ylabel("电压/V")
    # 解决中文显示问题
    plt.rcParams['font.sans-serif'] = ['SimHei']
    plt.rcParams['axes.unicode_minus'] = False
    line1, = plt.plot([], label='输出')
    line2, = plt.plot([], label='输入')
    plt.legend()
    ax = plt.gca()
    while True:
        # Get new data
        data = i.get_data()


        # Update the plot
        line1.set_ydata(data['ch1'])
        line2.set_ydata(data['ch2'])
        line1.set_xdata(data['time'])
        line2.set_xdata(data['time'])

        plt.pause(0.05)

except Exception as e:
    print(f'Exception occurred: {e}')
finally:
    # Close the connection to the Moku device
    # This ensures network resources and released correctly
    i.relinquish_ownership()
