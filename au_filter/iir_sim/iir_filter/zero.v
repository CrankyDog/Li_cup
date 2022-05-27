`timescale 1ns/1ps
//-------------------------------------------------------
//   IIR�˲������ϵ��ģ�飬�ṹ��FIR�˲�����ͬ
//-------------------------------------------------------
module zero
#(parameter z0  =  17'd33199, 
            z1  =  -17'd63005,
            z2  =  17'd30245
)
(
    input rst,                 //�ߵ�ƽ��Ч��λ�ź�
    input clk,                 //ϵͳʱ��2kHz
    input signed [23:0] Xin,   //������������2kHz
    input     [3:0]coe_ctrl,
    output signed [42:0] Xout  //���ϵ��ģ�����
);

//-------------------------------------------------------
//   ���ݴ�����λ�Ĵ�����
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
//   ���Ӧϵ�����˷���ʹ�ó˷���IP��
//-------------------------------------------------------
wire signed [40:0] Mult_reg [2:0];  //�����ܳ������ֵ��23bit����



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
//   �˷�����ۼ����
//-------------------------------------------------------	
assign Xout = {{2{Mult_reg[0][40]}},Mult_reg[0]}
                +{{2{Mult_reg[1][40]}},Mult_reg[1]}
                +{{2{Mult_reg[2][40]}},Mult_reg[2]};

 
endmodule






