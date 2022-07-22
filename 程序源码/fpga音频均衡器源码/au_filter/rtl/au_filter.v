module au_filter
(
    input  wire  au_data,
    input  wire  sys_clk,
    input  wire  sys_rst,
    input  wire[4:0]coe_ctrl,
	input 	wire[3:0] key,
           
    input wire      bclk,
    input wire      ws,
	input  wire[7:0]adc_dat_a,
	output reg	[3:0]led,
    output wire      adc_clka,
    output wire      dac_clkb,
    output  reg[7:0]dac_dat_b

    

);

wire    signed [28:0]     eq_data;         
      
wire    signed[23:0]  receive_left_data;

wire    signed[23:0]  fir_dout;
wire                     fir_dout_vld;
reg    signed[23:0]       fir_vld_data;

reg    signed[23:0]       fir_all_vld_data;
wire                    fir_all_dout_vld;
wire    signed[23:0]    fir_all_dout;

wire    signed[7:0]      iir_music_dout2;
wire    signed[7:0]      iir_music_dout3;


wire    signed[7:0]      iir_speak_dout2    ;
wire    signed[7:0]      iir_speak_dout3;

wire               data_value     ;
reg     [10:0]      cnt0         ;

wire	signed [28:0]	iir_dout_250;
wire	signed [28:0]	iir_dout_1000;
wire	signed [28:0]	iir_dout_2250;

reg signed[28:0] eq_data_buf;
wire  clk_40k;
wire  clk_2560k;
wire  clk_240k;
reg signed[23:0] receive_left_data_buf;
reg signed[23:0] adc_data_exp_buf;
reg signed[23:0] eq_din;
//wire     [9:0]      i2s_dout_buf;


//wire      [9:0]       i2s_dout_buf2;

localparam		  STABLE	= 	5'd0  ;
localparam        EQ250_ST   = 	5'd10 ;
localparam        EQ250_WK  = 	5'd9 ;
localparam        EQ1000_ST = 	5'd14  ;
localparam        EQ1000_WK   = 5'd13 ;
localparam        EQ2250_ST = 	5'd16  ;
localparam        EQ2250_WK  = 	5'd17 ;





//工程的工作时钟clk是50Mhz的时钟，而设置matlab生成fir系数的时候，设置的采样频率是0.04Mhz
//即1250个clk时钟周期才送一个数据给FIR滤波器。
//cnt0:时钟分频计数器
//加1条件:一直要分频，一直计数，固定为1

always @(posedge sys_clk or negedge sys_rst)begin
    if(!sys_rst)begin
        cnt0 <= 11'd0;
    end
    else   if(cnt0 == 11'd1249)
            cnt0 <= 11'd0;
    else
            cnt0 <= cnt0 + 11'd1;
end

always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        receive_left_data_buf <= 24'd0;
    else   
	receive_left_data_buf <= receive_left_data;
end

always @(posedge clk_240k or negedge sys_rst)begin
    if(!sys_rst)
        adc_data_exp_buf <= 24'd0;
    else   
	adc_data_exp_buf <= adc_data_exp;
end

reg mode = 0;

always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        mode <= 1;
    else   if(key[1] == 0)
		mode <= 1;
	else   if(key[2] == 0)
		mode <= 0;
	else 
		mode <= mode;
end



always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        eq_din <= receive_left_data_buf;
    else   if(mode == 1)
		eq_din <= adc_data_exp_buf;
	else   if(mode == 0)
		eq_din <= receive_left_data_buf;
end



assign data_value = cnt0 == 11'd1250-11'd1; 

pll	pll_inst (
	.inclk0 ( sys_clk ),
	.c0 ( clk_40k ),
	.c1 ( clk_2560k ),
	.c2 ( clk_240k )
	);
	
i2s i2s_inst(
   .sys_clk     (sys_clk),
   .bclk         (bclk) ,            
   .ws               (ws)  ,             
   .sdata             (au_data)  ,
   
   .receive_valid     () ,     
   .receive_left_data (receive_left_data),
   .receive_right_data()
);

//reg [4:0]coe_ctrl_test;
//always @(posedge sys_clk or negedge sys_rst)begin
//    if(!sys_rst)begin
//        led <= 4'b0000;
//    end
//    else   if(coe_ctrl ==9)
//            led <= 4'b0011;
//	else   if(coe_ctrl_buf == 9)
//            led <= 4'b1111;
//	else   if(key[2] == 0)
//            led <= 4'b0110;
//	else   if(key[3] == 0)
//            led <= 4'b0101;
//end

//always @(posedge sys_clk or negedge sys_rst)begin
//    if(!sys_rst)begin
//        coe_ctrl_test <= 3'd0;
//    end
//    else   if(key[0] == 0)
//            coe_ctrl_test <= 0;
//	else   if(key[1] == 0)
//            coe_ctrl_test <= 9;
//	else   if(key[2] == 0)
//            coe_ctrl_test <= 10;
//	else   if(key[3] == 0)
//            coe_ctrl_test <= 11;
//end



always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        coe_ctrl_buf <= 5'd0;
    else   
	coe_ctrl_buf <= coe_ctrl;
end
reg [4:0]	coe_ctrl_buf;
 
equalizer equalizer_inst(
       .clk_40k          (clk_40k)  ,
	   .clk_240k		(clk_240k),
       .sys_rst      (sys_rst)  ,
       .coe_ctrl     (coe_ctrl_buf)  ,
       .fir_vld_data (eq_din)  ,
                    
       .eq_data      (eq_data)
        
);






always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        eq_data_buf <= 29'd0;
    else   
		eq_data_buf <= eq_data;
end




FirIP FirIP_inst
(       
   .datain_valid    (data_value)   ,
   .reset_n         (sys_rst)   ,              //复位信号，低电平有效
   .clk             (sys_clk)   ,                  //FPGA系统时钟/数据速率：50MHz
   .din             (eq_data_buf[23:0])   ,    //数据输入频率为40kHZ
   .dout            (fir_dout)   ,   //滤波后的输出数据
   .source_valid    (fir_dout_vld)
);



//
//assign dac_clka  = ~sys_clk     ;

//dac_dat_b是直接输出滤波后的信号(fir_dout)。
//MP801和MP802板的DA电压，是与数值相反的，即给0，电压最高;255时，输出电压是最低
//因此需要(255-fir_dout2)来调整
//最终实现效果：fir_dout2=0时，DA电压最低；fir_dout2=255时，DA电压最高。
always  @(posedge sys_clk or negedge sys_rst)begin
    if(sys_rst==1'b0)begin
        dac_dat_b <= 8'd0;
        fir_vld_data<= 24'd0;
    end
    else if(fir_dout_vld)begin
        dac_dat_b <= 8'd255 -iir_speak_dout3;
        fir_vld_data <= fir_dout;
    end
end

//DAC输出是原码的
//IIR滤波器输出是补码的
//iir_dout是补码，所以要转为原码
assign iir_speak_dout2 = fir_vld_data[14:7]+8'd127;
assign iir_speak_dout3 = iir_speak_dout2;

assign dac_clkb  = ~sys_clk;

//adc 采样数据输入40kHz 8位无符号数 0-255
wire signed[23:0]adc_data_exp;

wire signed[7:0] adc_dat_buf = adc_dat_a-8'd128;
assign adc_data_exp = {adc_dat_buf[7],adc_dat_buf,15'd0};
assign adc_clka  = ~clk_240k;



endmodule