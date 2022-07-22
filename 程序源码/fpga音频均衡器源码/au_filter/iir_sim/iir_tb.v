
`timescale 1 ns/ 1 ns
module iir_tb();

reg clk;
reg [23:0] din;
reg rst;
// wires                                               
wire signed[28:0]  dout1;
reg [2:0] coe_ctrl;


//iir iir_inst
//(
//  .rst    (rst) ,                 //高电平有效复位信号
//  .clk    (clk) ,                 //系统时钟40kHz
//  .Din    (din) ,   //采样数据输入40kHz
//  . coe   (17'd0) ,
//  . coe_en(1'd0)     ,
//  . Dout  (dout1)     //IIR滤波输出
//);

equalizer equalizer_inst(
        .ws          (clk)   ,
        .sys_rst     (rst)  ,
        .coe_ctrl    (coe_ctrl)  ,
        .fir_vld_data(din)  ,
                     
        .eq_data     (dout1)
        
);

parameter clk_period=626; //设置时钟信号周期（频率）:1.6MHz
parameter clk_half_period=clk_period/2;
parameter data_num=40000;  //仿真数据长度
parameter time_sim=data_num*clk_period/2; //仿真时间

initial
begin
	//设置时钟信号初值
	clk=1;
	//设置复位信号
	rst=0;
	#10000 rst=1;
	//设置输入信号初值
	din=24'd0;
    coe_ctrl = 3'b001;
    #50000
    coe_ctrl = 3'b000;

end

//产生时钟信号
always                                                 
	#12500 clk=~clk;

//从外部TX文件读入数据作为测试激励
integer Pattern;
reg [23:0] stimulus[1:data_num];
initial
begin
	$readmemb("D:/qq/fpga_learning/li/fpga/au_filter/sim/SinIn.txt",stimulus);
	Pattern=0;
	repeat(data_num)
		begin
			Pattern=Pattern+1;
			din=stimulus[Pattern];
			#25000;//数据周期为时钟周期的8倍
		end
end
	
endmodule
