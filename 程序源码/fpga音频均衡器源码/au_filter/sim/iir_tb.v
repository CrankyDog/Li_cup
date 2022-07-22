`timescale 1 ns/ 1 ns
module iir_tb();


reg clk;
reg [23:0] din;
reg sys_rst;
// wires                                               
reg sys_clk;
reg clk_240k;
reg clk_40k;
reg [2:0] coe_ctrl;
wire dac_clka;
reg [9:0]dac_dat_a;
wire     signed[28:0]      dout;

iir_static iir_static_notiching
(
  .rst    (sys_rst) ,                 //高电平有效复位信号
  .clk_40k	(clk_40k),                 //数据时钟40kHz
  .clk_240k	(clk_240k),				//系统时钟120kHz
  .Din    (din) ,   //采样数据输入40kHz
  . Dout  (dout)     //IIR滤波输出
);

//equalizer equalizer_inst(
//        .ws          (clk)   ,
//        .sys_rst     (rst)  ,
//        .coe_ctrl    (coe_ctrl)  ,
//        .fir_vld_data(din)  ,
//                     
//        .eq_data     (dout1)
//        
//);


//filter_test filter_test_my
//(
//   .au_data       (din)     ,
//   . sys_clk      (sys_clk) ,
//   . sys_rst      (rst) ,
//   .  ws            (clk),
//   .coe_ctrl    (coe_ctrl)  ,
//                  
//   .      dac_clka(dac_clka) ,
//   .     dac_dat_a(dac_dat_a)
//
//);

//wire    signed[23:0]  fir_dout;
//wire            fir_dout_vld;
//wire    signed[17:0]      fir_dout2    ;
//wire               data_value     ;
//reg     [10:0]      cnt0         ;
//wire    signed[17:0]      fir_dout3;
//wire    signed[9:0]      fir_dout4;
//reg     signed[23:0]   fir_vld_data;
//wire     signed[28:0]      eq_data;
////
////工程的工作时钟clk是50Mhz的时钟，而设置matlab生成fir系数的时候，设置的采样频率是0.04Mhz
////即1250个clk时钟周期才送一个数据给FIR滤波器。
////cnt0:时钟分频计数器
//
//always @(posedge sys_clk or negedge sys_rst)begin
//    if(!sys_rst)begin
//        cnt0 <= 11'd0;
//    end
//    else   if(cnt0 == 11'd1249)
//            cnt0 <= 11'd0;
//    else
//            cnt0 <= cnt0 + 11'd1;
//end
//
//
//assign data_value = cnt0 == 11'd1250-11'd1; 
//
//FirIP FirIP_inst
//(       
//   .datain_valid    (data_value)   ,
//   .reset_n         (sys_rst)   ,              //复位信号，低电平有效
//   .clk             (sys_clk)   ,                  //FPGA系统时钟/数据速率：50MHz
//   .din             (din)   ,    //数据输入频率为40kHZ
//   .dout            (fir_dout)   ,   //滤波后的输出数据
//   .source_valid    (fir_dout_vld)
//);
//
//equalizer equalizer_inst(
//       .ws           (clk)  ,
//       .sys_rst      (sys_rst)  ,
//       .coe_ctrl     (coe_ctrl)  ,
//       .fir_vld_data (fir_vld_data)  ,
//                    
//       .eq_data      (eq_data)
//        
//);
//

parameter clk_period=626; //设置时钟信号周期（频率）:1.6MHz
parameter clk_half_period=clk_period/2;
parameter data_num=40000;  //仿真数据长度
parameter time_sim=data_num*clk_period/2; //仿真时间

initial
begin
	//设置时钟信号初值
	sys_clk = 1;
	clk_40k=1;
    clk_240k = 1;
    sys_clk = 1;
	//设置复位信号
	sys_rst=0;
	#10000 sys_rst=1;
	//设置输入信号初值
	din=24'd0;

end

//产生时钟信号
always                                                 
	#3750 clk_40k=~clk_40k;
    

	    
always                                                 
	#625 clk_240k=~clk_240k;
	
always                                                 
	#3 sys_clk=~sys_clk;
    
//always                                                 
//	#10 sys_clk=~sys_clk;

//从外部TX文件读入数据作为测试激励
integer Pattern ,j;
reg [23:0] stimulus[1:data_num];
initial
begin
//for(j = 0;j<10;j=j+1)begin
//        #50000
//        coe_ctrl = 3'b001;
//        #50000
//        coe_ctrl = 3'b000;
//        #50000
//        coe_ctrl = 3'b011;
//        #50000
//        coe_ctrl = 3'b000;
//        #50000
//        coe_ctrl = 3'b101;
//        #50000
//        coe_ctrl = 3'b000;
//end
        $readmemb("D:/qq/fpga_learning/li/fpga/au_filter/sim/SinIn.txt",stimulus);
        Pattern=0;
        repeat(data_num)
            begin
                Pattern=Pattern+1;
                din=stimulus[Pattern];
                #7500;//数据周期为时钟周期的8倍
            end
end

	
endmodule
