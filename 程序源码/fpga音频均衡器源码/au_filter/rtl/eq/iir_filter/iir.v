//-------------------------------------------------------
//   IIR滤波器顶层模块
//-------------------------------------------------------
module iir
(
    input rst,                 //高电平有效复位信号
    input clk_40k,                 //数据时钟40kHz
	input clk_240k,				//系统时钟120kHz
    input signed [23:0] Din,   //采样数据输入40kHz
    input signed [16:0] coe,
    input               coe_en,
    output signed [28:0] Dout  //IIR滤波输出
);

reg signed [16:0] coe_reg [0:5];
reg signed [16:0]  coe_buf[0:5];
reg [3:0] i,j;
wire coe_ful_flag;
reg [2:0] coe_ful_cnt;
reg coe_en_delay1;

always @ (posedge clk_40k or negedge rst)
    if (!rst)  
        coe_en_delay1 <= 1'd0;
    else 
        coe_en_delay1 <= coe_en;


always @ (posedge clk_40k or negedge rst)
    if (!rst) begin
            coe_reg[0] <= 17'd32768;
            coe_reg[1] <= -17'd58935;
            coe_reg[2] <= 17'd30050;
            coe_reg[3] <= 17'd32768;
            coe_reg[4] <= -17'd58935;
            coe_reg[5] <= 17'd30050;
    end
    else begin
        if(coe_en_delay1 && coe_ful_cnt>0 && coe_ful_cnt<7)begin
            for (j=0; j<5; j=j+1'b1)
                coe_reg[j] <= coe_reg[j+1];
            coe_reg[5] <= coe;
        end
    end

always @ (posedge clk_40k or negedge rst)
    if (!rst) begin
        coe_buf[0] <= 17'd32768;
        coe_buf[1] <= -17'd58935;
        coe_buf[2] <= 17'd30050;
        coe_buf[3] <= 17'd32768;
        coe_buf[4] <= -17'd58935;
        coe_buf[5] <= 17'd30050;   
    end
    else if(coe_ful_flag)begin
        coe_buf[0] <= coe_reg[0];
        coe_buf[1] <= coe_reg[1];
        coe_buf[2] <= coe_reg[2];
        coe_buf[3] <= coe_reg[3];
        coe_buf[4] <= coe_reg[4];
        coe_buf[5] <= coe_reg[5];
    end
  
assign coe_ful_flag = (coe_ful_cnt == 3'd7)?1'd1:1'd0;
  
always @(posedge clk_40k or negedge rst)begin
    if(!rst)begin
        coe_ful_cnt <= 3'd0;
    end
    else   if(coe_ful_cnt == 3'd7)
            coe_ful_cnt <= 3'd0;
    else    if(coe_en_delay1)
            coe_ful_cnt <= coe_ful_cnt + 3'd1;
    else    
            coe_ful_cnt <= coe_ful_cnt;
end
 

 
//-------------------------------------------------------
//   零点系数
//-------------------------------------------------------
reg signed [42:0] Xout;
wire signed [28:0] Yin;
reg signed [47:0] Yout;

reg signed [23:0] Xin_reg [2:0];


always @ (posedge clk_40k or negedge rst)
    if (!rst) begin
        for (i=0; i<3; i=i+1'b1)
            Xin_reg[i] <= 'd0;
    end
    else begin
        for (j=0; j<2; j=j+1'b1)
            Xin_reg[j+1] <= Xin_reg[j];
        Xin_reg[0] <= Din;
    end



wire signed [16:0] zero_coe [2:0]; 

assign zero_coe[0] = coe_buf[0] ;  
assign zero_coe[1] = coe_buf[1];
assign zero_coe[2] = coe_buf[2] ;


//-------------------------------------------------------
//   与对应系数做乘法；使用乘法器IP核
//-------------------------------------------------------
reg signed [40:0] zero_mult_reg [2:0];  //不可能出现最大负值，23bit即可

reg [2:0] coe_mul_cnt;




always @(posedge clk_240k or negedge rst)begin
    if(!rst)begin
        coe_mul_cnt <= 3'd0;
    end
    else   if(coe_mul_cnt == 3'd2)
            coe_mul_cnt <= 3'd0;
    else
            coe_mul_cnt <= coe_mul_cnt + 3'd1;
end

reg [2:0] clk240k_cnt;

always @(posedge clk_240k or negedge rst)begin
    if(!rst)begin
        clk240k_cnt <= 3'd0;
    end
    else   if(clk240k_cnt == 3'd5)
            clk240k_cnt <= 3'd0;
    else
            clk240k_cnt <= clk240k_cnt + 3'd1;
end

//always @(posedge clk_240k or negedge rst)begin
//    if(!rst)
//        coe_mul_cnt_delay <= 2'd0;
//    else 
//        coe_mul_cnt_delay <= coe_mul_cnt;
//
//end


//always @(posedge clk_240k or negedge rst)begin
//    if(!rst)
//       coe_mul_flag <= 1'd0;
//	else   if(coe_mul_cnt == 2'd2)
//       coe_mul_flag <= 1'd1;
//	else
//	   coe_mul_flag <= 1'd0;
//end	

wire	signed[40:0] zero_mult_reg_buf;

my_lpm_mult	my_lpm_mult_inst (
	.dataa (  zero_coe[coe_mul_cnt]      ),
	.datab (  Xin_reg[coe_mul_cnt]     ),
	.result ( zero_mult_reg_buf )
	);
 

always @(posedge clk_240k or negedge rst)begin
    if(!rst)begin
        zero_mult_reg[0] <= 41'd0;
        zero_mult_reg[1] <= 41'd0;
        zero_mult_reg[2] <= 41'd0;
    end
    else   if(coe_mul_cnt == 3'd0)
            zero_mult_reg[0] <= zero_mult_reg_buf;
	else   if(coe_mul_cnt == 3'd1)
            zero_mult_reg[1] <= zero_mult_reg_buf;
	else   if(coe_mul_cnt == 3'd2)
            zero_mult_reg[2] <= zero_mult_reg_buf;
end	

//my_lpm_mult	my_lpm_mult_zm1 (
//	.dataa (  zero_coe[1]      ),
//	.datab (  Xin_reg[1]     ),
//	.result ( zero_mult_reg[1] )
//	);
//    
//my_lpm_mult	my_lpm_mult_zm2 (
//	.dataa (  zero_coe[2]      ),
//	.datab (  Xin_reg[2]     ),
//	.result ( zero_mult_reg[2] )
//	);
//-------------------------------------------------------
//   乘法结果累加输出
//-------------------------------------------------------	
//assign Xout = coe_mul_flag?{{{2{zero_mult_reg[0][40]}},zero_mult_reg[0]}
//                +{{2{zero_mult_reg[1][40]}},zero_mult_reg[1]}
//                +{{2{zero_mult_reg[2][40]}},zero_mult_reg[2]}}:Xout;

always @ (posedge clk_240k or negedge rst)begin
    if (!rst) begin
		Xout <= 43'd0;
    end
    else if(clk240k_cnt == 3'd2)begin
        Xout <= {{2{zero_mult_reg[0][40]}},zero_mult_reg[0]}
                +{{2{zero_mult_reg[1][40]}},zero_mult_reg[1]}
                +{{2{zero_mult_reg[2][40]}},zero_mult_reg[2]};
	end
    else
		Xout <= Xout;
end	 

 

//-------------------------------------------------------
//   极点系数
//-------------------------------------------------------
//-------------------------------------------------------
//   数据存入移位寄存器中
//-------------------------------------------------------
reg signed [28:0] Yin_reg [2:0];


always @ (posedge clk_40k or negedge rst)
    if (!rst) begin
        for (i=0; i<3; i=i+1'b1)
            Yin_reg[i] <= 'd0;
    end
    else begin
        for (j=0; j<2; j=j+1'b1)
            Yin_reg[j+1] <= Yin_reg[j];
        Yin_reg[0] <= Yin;
    end

//-------------------------------------------------------
//   滤波器系数定义，17bit量化
//-------------------------------------------------------
wire signed [16:0] pole_coe [3:0]; 
assign pole_coe[0] =coe_buf[3] ; //y(n)系数，此处无需使用
assign pole_coe[1] =coe_buf[4] ;
assign pole_coe[2] =coe_buf[5] ;
assign pole_coe[3] =17'd0 ;



//-------------------------------------------------------
//   与对应系数做乘法；使用乘法器IP核
//-------------------------------------------------------
reg signed [45:0] pole_mult_reg [2:0];  //不可能出现最大负值，23bit即可

wire signed	[45:0] pole_mult_reg_buf;

my_lpm_mult28in	my_lpm_mult28in_inst (
	.dataa (  pole_coe[coe_mul_cnt+1]      ),
	.datab (  Yin_reg[coe_mul_cnt]     ),
	.result ( pole_mult_reg_buf)
	);
  

always @(posedge clk_240k or negedge rst)begin
    if(!rst)begin
        pole_mult_reg[0] <= 46'd0;
        pole_mult_reg[1] <= 46'd0;
        pole_mult_reg[2] <= 46'd0;
    end
    else   if(coe_mul_cnt == 3'd0)
            pole_mult_reg[0] <= pole_mult_reg_buf;
	else   if(coe_mul_cnt == 3'd1)
            pole_mult_reg[1] <= pole_mult_reg_buf;
	else   if(coe_mul_cnt == 3'd2)
            pole_mult_reg[2] <= pole_mult_reg_buf;
end	  
//my_lpm_mult28in	my_lpm_mult_pm1 (
//	.dataa (  pole_coe[2]      ),
//	.datab (  Yin_reg[1]     ),
//	.result ( pole_mult_reg[1] )
//	);
    

	
//-------------------------------------------------------
//   乘法结果累加输出，高位补符号位
//-------------------------------------------------------	
//assign Yout = coe_mul_flag?{{{2{pole_mult_reg[0][45]}},pole_mult_reg[0]} 
//            + {{2{pole_mult_reg[1][45]}},pole_mult_reg[1]}}:Yout;
			
always @ (posedge clk_240k or negedge rst)begin
    if (!rst) begin
		Yout <= 48'd0;
    end
    else if(coe_mul_cnt == 3'd2)
        Yout <= {{2{pole_mult_reg[0][45]}},pole_mult_reg[0]}+ {{2{pole_mult_reg[1][45]}},pole_mult_reg[1]};
    else
		Yout <= Yout;
end		
//-------------------------------------------------------
//   反馈结构，右移实现除法
//-------------------------------------------------------

//reg [49:0] Ysum;
//always @ (posedge clk_240k or negedge rst)begin
//    if (!rst) begin
//		Ysum <= 50'd0;
//    end
//    else if(coe_mul_cnt_delay == 2'd2)
//        Ysum <= {{7{Xout[42]}},Xout} - {{2{Yout[47]}},Yout};
//    else
//		Ysum <= Ysum;
//end	
//wire signed [49:0] Ysum = (coe_mul_cnt_delay == 2'd2)?{{7{Xout[42]}},Xout} - {{2{Yout[47]}},Yout}:Ysum; //减法器
wire signed [49:0] Ysum ={{7{Xout[42]}},Xout} - {{2{Yout[47]}},Yout}; //减法器




wire signed [49:0] Ydiv = {{15{Ysum[49]}},Ysum[49:15]};  //除法器
assign Yin = (!rst) ? 28'd0 : Ydiv[28:0];   //反馈

reg [28:0] Dout_buf;
assign Dout = Dout_buf;

always @ (posedge clk_240k or negedge rst)begin
    if (!rst) begin
		Dout_buf <= 29'd0;
    end
    else if(clk240k_cnt == 3'd2)
        Dout_buf <= Yin;
    else
		Dout_buf <= Dout_buf;
end	




endmodule
