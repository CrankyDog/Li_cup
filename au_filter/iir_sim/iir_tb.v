
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
//  .rst    (rst) ,                 //�ߵ�ƽ��Ч��λ�ź�
//  .clk    (clk) ,                 //ϵͳʱ��40kHz
//  .Din    (din) ,   //������������40kHz
//  . coe   (17'd0) ,
//  . coe_en(1'd0)     ,
//  . Dout  (dout1)     //IIR�˲����
//);

equalizer equalizer_inst(
        .ws          (clk)   ,
        .sys_rst     (rst)  ,
        .coe_ctrl    (coe_ctrl)  ,
        .fir_vld_data(din)  ,
                     
        .eq_data     (dout1)
        
);

parameter clk_period=626; //����ʱ���ź����ڣ�Ƶ�ʣ�:1.6MHz
parameter clk_half_period=clk_period/2;
parameter data_num=40000;  //�������ݳ���
parameter time_sim=data_num*clk_period/2; //����ʱ��

initial
begin
	//����ʱ���źų�ֵ
	clk=1;
	//���ø�λ�ź�
	rst=0;
	#10000 rst=1;
	//���������źų�ֵ
	din=24'd0;
    coe_ctrl = 3'b001;
    #50000
    coe_ctrl = 3'b000;

end

//����ʱ���ź�
always                                                 
	#12500 clk=~clk;

//���ⲿTX�ļ�����������Ϊ���Լ���
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
			#25000;//��������Ϊʱ�����ڵ�8��
		end
end
	
endmodule
