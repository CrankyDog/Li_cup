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
  .rst    (sys_rst) ,                 //�ߵ�ƽ��Ч��λ�ź�
  .clk_40k	(clk_40k),                 //����ʱ��40kHz
  .clk_240k	(clk_240k),				//ϵͳʱ��120kHz
  .Din    (din) ,   //������������40kHz
  . Dout  (dout)     //IIR�˲����
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
////���̵Ĺ���ʱ��clk��50Mhz��ʱ�ӣ�������matlab����firϵ����ʱ�����õĲ���Ƶ����0.04Mhz
////��1250��clkʱ�����ڲ���һ�����ݸ�FIR�˲�����
////cnt0:ʱ�ӷ�Ƶ������
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
//   .reset_n         (sys_rst)   ,              //��λ�źţ��͵�ƽ��Ч
//   .clk             (sys_clk)   ,                  //FPGAϵͳʱ��/�������ʣ�50MHz
//   .din             (din)   ,    //��������Ƶ��Ϊ40kHZ
//   .dout            (fir_dout)   ,   //�˲�����������
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

parameter clk_period=626; //����ʱ���ź����ڣ�Ƶ�ʣ�:1.6MHz
parameter clk_half_period=clk_period/2;
parameter data_num=40000;  //�������ݳ���
parameter time_sim=data_num*clk_period/2; //����ʱ��

initial
begin
	//����ʱ���źų�ֵ
	sys_clk = 1;
	clk_40k=1;
    clk_240k = 1;
    sys_clk = 1;
	//���ø�λ�ź�
	sys_rst=0;
	#10000 sys_rst=1;
	//���������źų�ֵ
	din=24'd0;

end

//����ʱ���ź�
always                                                 
	#3750 clk_40k=~clk_40k;
    

	    
always                                                 
	#625 clk_240k=~clk_240k;
	
always                                                 
	#3 sys_clk=~sys_clk;
    
//always                                                 
//	#10 sys_clk=~sys_clk;

//���ⲿTX�ļ�����������Ϊ���Լ���
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
                #7500;//��������Ϊʱ�����ڵ�8��
            end
end

	
endmodule
