module i2s_tx(
	input clk_in,
	input [15:0] data_in,
	input rstn,
	input enable,
	output DATA,
	input WS,
	output send_over
    );
	 
assign clk = clk_in;

localparam IDLE=2'b00;
localparam LEFT=2'b01;
localparam RIGHT=2'b11;
reg [4:0] cnt;
reg [1:0] state;
reg [1:0] next_state;

reg [17:0] data_send;

always @(posedge clk or negedge rstn) begin
	if(!rstn) state <= IDLE;
	else if(enable) state <= next_state;
	else state <= IDLE;
end

// 状态
always @(*) begin
	case(state) 
		IDLE: next_state = LEFT;
		LEFT: 
			begin
				if(cnt == 'd17) next_state = RIGHT;
				else next_state = LEFT;
			end
		RIGHT:
			begin
				if(cnt == 'd17) next_state = LEFT;
				else next_state = RIGHT;
			end
		default: next_state = IDLE;
	endcase
end

// 发送比特计数
always @(posedge clk or negedge rstn) begin
	if(!rstn) cnt <= 'd0;
	else if((state != IDLE) && (cnt != 'd17)) cnt <= cnt + 1'b1;
	else cnt <= 'd0;
end


always @(posedge clk or negedge rstn) begin
	if(!rstn) data_send <= 'd0;
	else if((state == IDLE) || (cnt == 'd17)) data_send <= {data_in,2'b00}; // 装载数据
	else data_send <= {data_send[16:0],1'b0};  // 在发送状态下对data_send进行左移位
end

assign WS = (state == LEFT)? 1'b1 : 1'b0;  
assign DATA = data_send[17];  // 数据线为data_send寄存器的最高位
assign send_over = (cnt == 'd15)? 1'b1 : 1'b0;

endmodule