module filter_test
(
    input  wire  [23:0]au_data,
    input  wire  sys_clk,
    input  wire  sys_rst,
    input  wire  ws,
    input  wire[2:0]coe_ctrl,
           
    output wire      dac_clka,
    output  reg[9:0]dac_dat_a

);

wire    [23:0]  fir_dout;
wire            fir_dout_vld;
wire    [17:0]      fir_dout2    ;
wire               data_value     ;
reg     [10:0]      cnt0         ;
wire    [17:0]      fir_dout3;
wire    [9:0]      fir_dout4;
reg     [23:0]   fir_vld_data;
reg     [28:0]      eq_data;

//工程的工作时钟clk是50Mhz的时钟，而设置matlab生成fir系数的时候，设置的采样频率是0.04Mhz
//即1250个clk时钟周期才送一个数据给FIR滤波器。
//cnt0:时钟分频计数器

always @(posedge sys_clk or negedge sys_rst)begin
    if(!sys_rst)begin
        cnt0 <= 11'd0;
    end
    else   if(cnt0 == 11'd1249)
            cnt0 <= 11'd0;
    else
            cnt0 <= cnt0 + 11'd1;
end


assign data_value = cnt0 == 11'd1250-11'd1; 

FirIP FirIP_inst
(       
   .datain_valid    (data_value)   ,
   .reset_n         (sys_rst)   ,              //复位信号，低电平有效
   .clk             (sys_clk)   ,                  //FPGA系统时钟/数据速率：50MHz
   .din             (au_data)   ,    //数据输入频率为40kHZ
   .dout            (fir_dout)   ,   //滤波后的输出数据
   .source_valid    (fir_dout_vld)
);

equalizer equalizer_inst(
       .ws           (ws)  ,
       .sys_rst      (sys_rst)  ,
       .coe_ctrl     (coe_ctrl)  ,
       .fir_vld_data (fir_vld_data)  ,
                    
       .eq_data      (eq_data)
        
);
//dac_dat_b:最终输出给DA芯片的数据值
//输出最大值255时应正好对应最大电压幅值
//输出电压与输出数字量数值成反比，因此dac_dat_a = 255 - sin_data
/*always  @(posedge clk or negedge rst_n)begin
    if(rst_n==1'b0)begin
        dac_dat_b <= 0;
    end
    else begin
        dac_dat_b <= 255 - sin_data;
         
    end
end

assign dac_clka  = ~sys_clk     ;*/

//dac_dat_b是直接输出滤波后的信号(fir_dout)。
//MP801和MP802板的DA电压，是与数值相反的，即给0，电压最高;255时，输出电压是最低
//因此需要(255-fir_dout2)来调整
//最终实现效果：fir_dout2=0时，DA电压最低；fir_dout2=255时，DA电压最高。
always  @(posedge sys_clk or negedge sys_rst)begin
    if(sys_rst==1'b0)begin
        dac_dat_a <= 10'd0;
    end
    else if(fir_dout_vld)begin
        dac_dat_a <= 10'd1023 -fir_dout4;
        fir_vld_data <= fir_dout;
    end
end

//DAC输出是原码的（即最小是8'h0，最大是8'hff）
//FIR滤波器输出是补码的（最小是8'h80,最大是8'h7f）
//fir_dout是补码（范围是-128~127），所以要转为原码（0~255）
assign fir_dout2 = eq_data[28:11];
assign fir_dout3 = fir_dout2+ 131072;
assign fir_dout4 = fir_dout3[17:8];

assign dac_clka  = ~sys_clk;



endmodule