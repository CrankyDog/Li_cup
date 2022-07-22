`timescale 1ns / 1ps

module i2s_tb;

	// Inputs
	reg clk_in;
	reg data_in;
	reg rstn;
	reg enable;

	// Outputs
	wire ws;
	wire bclk;


	
	wire signed [23:0] L_DATA;
   //wire [23:0] R_DATA;
	//wire recv_over;

	// Instantiate the Unit Under Test (UUT)
i2s i2s_inst(

   .sys_clk       (clk_in) ,
   .sdata         (data_in) ,
                     
   .ws                (ws)  ,             
   .bclk              (bclk)  ,
   .receive_valid     () ,     
   .receive_left_data (L_DATA),
   .receive_right_data()

);

	initial begin
		// Initialize Inputs
		clk_in = 1;
		data_in = 0;
		rstn = 1;
		enable = 0;
	end	

		// Wait 100 ns for global reset to finish
		always@(negedge bclk)begin
        data_in <= ~data_in;
        end



	
	
	always #20 clk_in = ~clk_in;

      
endmodule