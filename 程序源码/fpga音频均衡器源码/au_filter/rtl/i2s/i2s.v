`timescale 1ns/1ps
/****************************************************************
*   Auther : Abner
*   Mail   : hn.cy@foxmail.com
*   Time   : 2019.03.01
*   Design :
*   Description :
****************************************************************/

module i2s#(
// Input Parameter Definitions
    parameter WORD_LENGTH = 24
    )(
// Hardware Port Definitions
    input      sys_clk,
    input      bclk,
    input      ws,              // 0 = left  1 = right
    input      sdata,
// Software Port Definitions
    output reg                   receive_valid      = 1'd0,
    output reg signed[WORD_LENGTH-1:0] receive_left_data  = {WORD_LENGTH{1'd0}}, // Left  channel
    output reg signed[WORD_LENGTH-1:0] receive_right_data = {WORD_LENGTH{1'd0}} // right channel
);

// Local Parameter Definitions
localparam WORD_LENGTH_DW = CLOG2(WORD_LENGTH);
localparam WS_LEFT  = 1'd0;
localparam WS_RIGHT = 1'd1;

// Wires/Reg Definitions
reg  ws_d1 = 1'd0;
reg  [WORD_LENGTH_DW-1:0] left_cnt = {WORD_LENGTH_DW{1'd0}};
reg  [WORD_LENGTH_DW-1:0] right_cnt= {WORD_LENGTH_DW{1'd0}};
reg  [WORD_LENGTH-1:0] left_data = {WORD_LENGTH{1'd0}};
reg  [WORD_LENGTH-1:0] right_data = {WORD_LENGTH{1'd0}};

////////////////////////////////////////////////////////////////////////////////
// RTL Begin

// delay one clk perior
always @ (posedge bclk) begin
    ws_d1 <= ws;
end

always @ (posedge bclk) begin
    if (ws_d1 == WS_LEFT) begin
        if (left_cnt < WORD_LENGTH) begin
            left_data[WORD_LENGTH-1:0] <= {left_data[WORD_LENGTH-2:0], sdata};
        end
    end
    else if (ws_d1 == WS_RIGHT) begin
        if (right_cnt < WORD_LENGTH) begin
            right_data[WORD_LENGTH-1:0] <= {right_data[WORD_LENGTH-2:0], sdata};
        end
    end
end

always @ (posedge bclk) begin
    if (ws_d1 == WS_LEFT) begin
        left_cnt <= left_cnt + 1'd1;
    end
    else begin
        left_cnt <= {WORD_LENGTH_DW{1'd0}};
    end
end

always @ (posedge bclk) begin
    if (ws_d1 == WS_RIGHT) begin
        right_cnt <= right_cnt + 1'd1;
    end
    else begin
        right_cnt <= {WORD_LENGTH_DW{1'd0}};
    end
end

always @ (posedge bclk) begin
    if (right_cnt == WORD_LENGTH) begin
        receive_valid      <= 1'd1;
        receive_left_data  <= left_data;
        receive_right_data <= right_data;
    end
    else begin
        receive_valid <= 1'd0;
    end
end


// RTL End
////////////////////////////////////////////////////////////////////////////////

/*pll_i2s	pll_i2s_inst (
	.inclk0 ( sys_clk ),
	.c0 ( bclk ),
	.c1 ( ws )
	);*/



////////////////////////////////////////////////////////////////////////////////
// Function Begin
function integer CLOG2;
  input integer value;
  begin
    for (CLOG2=0; value>0; CLOG2=CLOG2+1)
      value = value>>1;
  end
endfunction
// Function End
////////////////////////////////////////////////////////////////////////////////


/*--------------------------------------------------------------------------------------

Multiple_of_Fs    Fs
              I2S

           32 kHz      44.1 kHz       48 kHz       88.2 kHz       96 kHz      176.4 kHz       192 kHz
128     4.096  MHz   5.645  MHz    6.144  MHz    11.290 MHz    12.288 MHz    22.579 MHz     24.576 MHz
192     6.144  MHz   8.467  MHz    9.216  MHz    16.934 MHz    18.432 MHz    33.868 MHz     36.864 MHz
256     8.192  MHz   11.290 MHz    12.288 MHz    22.579 MHz    24.576 MHz    45.158 MHz     49.152 MHz
384     12.288 MHz   16.934 MHz    18.432 MHz    33.864 MHz    36.864 MHz    67.737 MHz     73.728 MHz
512     16.384 MHz   22.579 MHz    24.576 MHz    45.158 MHz    49.152 MHz
768     24.576 MHz   33.869 MHz    36.864 MHz    67.738 MHz    73.728 MHz
1024    32.768 MHz   45.158 MHz    49.152 MHz
1152    36.864 MHz   50.803 MHz    55.296 MHz
--------------------------------------------------------------------------------------*/
endmodule
