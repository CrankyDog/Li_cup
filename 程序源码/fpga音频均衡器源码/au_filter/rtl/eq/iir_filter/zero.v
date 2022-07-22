`timescale 1ns/1ps
//-------------------------------------------------------
//   IIR滤波器零点系数模块，结构与FIR滤波器相同
//-------------------------------------------------------
module zero
#(parameter z0  =  17'd33199, 
            z1  =  -17'd63005,
            z2  =  17'd30245
)
(
    input rst,                 //高电平有效复位信号
    input clk,                 //系统时钟2kHz
    input signed [23:0] Xin,   //采样数据输入2kHz
    input     [3:0]coe_ctrl,
    output signed [42:0] Xout  //零点系数模块输出
);

//-------------------------------------------------------
//   数据存入移位寄存器中
//-------------------------------------------------------
reg signed [23:0] Xin_reg [3:0];
reg [3:0] i,j;

always @ (posedge clk or negedge rst)
    if (!rst) begin
        for (i=0; i<3; i=i+1'b1)
            Xin_reg[i] <= 'd0;
    end
    else begin
        for (j=0; j<2; j=j+1'b1)
            Xin_reg[j+1] <= Xin_reg[j];
        Xin_reg[0] <= Xin;
    end



wire signed [16:0] coe [2:0]; 

assign coe[0] = z0  ;  
assign coe[1] = z1;
assign coe[2] = z2 ;


//-------------------------------------------------------
//   与对应系数做乘法；使用乘法器IP核
//-------------------------------------------------------
wire signed [40:0] Mult_reg [2:0];  //不可能出现最大负值，23bit即可



my_lpm_mult	my_lpm_mult_zm0 (
	.dataa (  coe[0]      ),
	.datab (  Xin_reg[0]     ),
	.result ( Mult_reg[0] )
	);
    
my_lpm_mult	my_lpm_mult_zm1 (
	.dataa (  coe[1]      ),
	.datab (  Xin_reg[1]     ),
	.result ( Mult_reg[1] )
	);
    
my_lpm_mult	my_lpm_mult_zm2 (
	.dataa (  coe[2]      ),
	.datab (  Xin_reg[2]     ),
	.result ( Mult_reg[2] )
	);
//-------------------------------------------------------
//   乘法结果累加输出
//-------------------------------------------------------	
assign Xout = {{2{Mult_reg[0][40]}},Mult_reg[0]}
                +{{2{Mult_reg[1][40]}},Mult_reg[1]}
                +{{2{Mult_reg[2][40]}},Mult_reg[2]};

 
endmodule






