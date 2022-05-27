`timescale 1ns/1ps
//-------------------------------------------------------
//   IIR滤波器极点系数模块，结构与FIR滤波器相同
//-------------------------------------------------------
module pole
#(parameter p0  =   17'd32768,
            p1  =   -17'd63005,
            p2  =   17'd30677
)
(
    input rst,                 //高电平有效复位信号
    input clk,                 //系统时钟2kHz
    input signed [28:0] Yin,   //采样数据输入2kHz
    output signed [47:0] Yout  //极点系数模块输出
);

//-------------------------------------------------------
//   数据存入移位寄存器中
//-------------------------------------------------------
reg signed [28:0] Yin_reg [1:0];
reg [3:0] i,j;

always @ (posedge clk or negedge rst)
    if (!rst) begin
        for (i=0; i<2; i=i+1'b1)
            Yin_reg[i] <= 'd0;
    end
    else begin
        for (j=0; j<1; j=j+1'b1)
            Yin_reg[j+1] <= Yin_reg[j];
        Yin_reg[0] <= Yin;
    end

//-------------------------------------------------------
//   滤波器系数定义，17bit量化
//-------------------------------------------------------
wire signed [16:0] coe [2:0]; 
//assign coe[0] = p0;  //y(n)系数，此处无需使用
assign coe[1] = p1;
assign coe[2] =  p2;


//-------------------------------------------------------
//   与对应系数做乘法；使用乘法器IP核
//-------------------------------------------------------
wire signed [45:0] Mult_reg [1:0];  //不可能出现最大负值，23bit即可



my_lpm_mult28in	my_lpm_mult_pm0 (
	.dataa (  coe[1]      ),
	.datab (  Yin_reg[0]     ),
	.result ( Mult_reg[0] )
	);
    
my_lpm_mult28in	my_lpm_mult_pm1 (
	.dataa (  coe[2]      ),
	.datab (  Yin_reg[1]     ),
	.result ( Mult_reg[1] )
	);
    

	
//-------------------------------------------------------
//   乘法结果累加输出，高位补符号位
//-------------------------------------------------------	
assign Yout = {{2{Mult_reg[0][45]}},Mult_reg[0]} 
            + {{2{Mult_reg[1][45]}},Mult_reg[1]};
       
endmodule






