`timescale 1ns/1ps
//-------------------------------------------------------
//   IIR�˲�������ϵ��ģ�飬�ṹ��FIR�˲�����ͬ
//-------------------------------------------------------
module pole
#(parameter p0  =   17'd32768,
            p1  =   -17'd63005,
            p2  =   17'd30677
)
(
    input rst,                 //�ߵ�ƽ��Ч��λ�ź�
    input clk,                 //ϵͳʱ��2kHz
    input signed [28:0] Yin,   //������������2kHz
    output signed [47:0] Yout  //����ϵ��ģ�����
);

//-------------------------------------------------------
//   ���ݴ�����λ�Ĵ�����
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
//   �˲���ϵ�����壬17bit����
//-------------------------------------------------------
wire signed [16:0] coe [2:0]; 
//assign coe[0] = p0;  //y(n)ϵ�����˴�����ʹ��
assign coe[1] = p1;
assign coe[2] =  p2;


//-------------------------------------------------------
//   ���Ӧϵ�����˷���ʹ�ó˷���IP��
//-------------------------------------------------------
wire signed [45:0] Mult_reg [1:0];  //�����ܳ������ֵ��23bit����



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
//   �˷�����ۼ��������λ������λ
//-------------------------------------------------------	
assign Yout = {{2{Mult_reg[0][45]}},Mult_reg[0]} 
            + {{2{Mult_reg[1][45]}},Mult_reg[1]};
       
endmodule





