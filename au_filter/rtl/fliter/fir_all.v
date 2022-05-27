module FirIP_all 
(
    input   datain_valid,
    input reset_n,              //复位信号，低电平有效
    input clk,                  //FPGA系统时钟/数据速率：40kHz
    input signed [23:0] din,    //数据输入频率为40kHZ
    output signed [23:0] dout,   //滤波后的输出数据
    output      source_valid
);


wire [1:0] source_error;

fir_all fir_all_inst
(
    .clk                (clk),             //输入，时钟信号
    .reset_n            (reset_n),         //输入，低电平有效复位
    .ast_sink_data      (din),             //输入，采样输入数据
    .ast_sink_valid     (datain_valid),      //输入，置1时向FIR滤波器输入数据
    .ast_sink_error     (2'd0),            //输入，标识信宿端出现的错误
    .ast_source_data    (dout),            //输出，滤波器输出，位宽与设计的滤波器参数有关
    .ast_source_valid   (source_valid),    //输出，FIR输出数据有效时该信号置位
    .ast_source_error   (source_error)     //输出，标识信源端出现的错误
);

endmodule