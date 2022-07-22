module equalizer(
        input                   ws,
        input                   sys_rst,
        input [2:0]             coe_ctrl,
        input   signed [23:0]   fir_vld_data,
        
        output signed [28:0]    eq_data
        
);
localparam                  IDLE =  0;
localparam                  CTRL_SIG =  1;
localparam                  STA_CTRL_ST =  2;
localparam                  STA_CTRL_WK =  3;
localparam                  SEND =  4;


reg [3:0]       state_250;
reg [3:0]       state_1000;
reg [3:0]       state_2250;

wire signed [28:0]  iir_dout_250;
wire signed [28:0]  iir_dout_1000;

wire signed [16:0] coe_vld_250 ;
wire signed [16:0] coe_vld_1000 ;
wire signed [16:0] coe_vld_2250 ;

wire signed [16:0] coe_250 ;
wire signed [16:0] coe_1000 ;
wire signed [16:0] coe_2250 ;

reg signed [16:0] coe_buf_250 ;
reg signed [16:0] coe_buf_1000 ;
reg signed [16:0] coe_buf_2250 ;

reg              coe_en_250 ;
reg              coe_en_1000 ;
reg              coe_en_2250 ;
 

reg [6:0]   iir250_rom_addr;
reg [6:0]   iir1000_rom_addr;
reg [6:0]   iir2250_rom_addr;


assign  coe_vld_250  =  coe_buf_250 ;
assign  coe_vld_1000 =  coe_buf_1000;
assign  coe_vld_2250 =  coe_buf_2250;

reg [2:0] coe_ful_cnt; 



        
always @(posedge ws or negedge sys_rst)begin
    if(!sys_rst)begin
        coe_ful_cnt <= 3'd0;
    end
    else   if(coe_ful_cnt == 3'd7)begin
            coe_ful_cnt <= 3'd0;
    end
    else    if(coe_en_250 ||coe_en_1000||coe_en_2250)
            coe_ful_cnt <= coe_ful_cnt + 3'd1;
    else    
            coe_ful_cnt <= coe_ful_cnt;
end        
        

//250hz 系数控制 状态机
always@(posedge ws or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_250 <= IDLE;
        coe_en_250  <=1'd0;
        iir250_rom_addr   <= 7'd60;
    end
    else
    case(state_250)
        IDLE:
            state_250 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 3'b001)
                state_250 <= STA_CTRL_ST;
            else if(coe_ctrl == 3'b010)
                state_250 <= STA_CTRL_WK;
            else
                state_250 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 3'b000 && (!coe_en_1000)&&(!coe_en_2250))begin
                iir250_rom_addr   <=    iir250_rom_addr +7'd6;
                coe_en_250 <= 1'd1;
                state_250 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 3'b000)begin
                iir250_rom_addr   <=    iir250_rom_addr -7'd6;
                coe_en_250 <= 1'd1;
                state_250 <= SEND;               
            end    
        end
        SEND:
        begin
            if(coe_en_250 && coe_ful_cnt>0 && coe_ful_cnt<7)
                iir250_rom_addr <= iir250_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_250 <= 1'd0;
                state_250 <= CTRL_SIG;
            end
        end
        default :
            state_250 <= IDLE;
    endcase
end

    
always@(posedge ws or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_250 <= 17'd0;
    else if(coe_en_250&& coe_ful_cnt>0 && coe_ful_cnt<7)
        coe_buf_250 <= coe_250;
    else
        coe_buf_250 <=  coe_buf_250;
end


//1000hz 系数控制 状态机
always@(posedge ws or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_1000 <= IDLE;
        coe_en_1000  <=1'd0;
        iir1000_rom_addr   <= 7'd60;
    end
    else
    case(state_1000)
        IDLE:
            state_1000 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 3'b011)
                state_1000 <= STA_CTRL_ST;
            else if(coe_ctrl == 3'b100)
                state_1000 <= STA_CTRL_WK;
            else
                state_1000 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 3'b000 && (!coe_en_250)&&(!coe_en_2250))begin
                iir1000_rom_addr   <=    iir1000_rom_addr +7'd6;
                coe_en_1000 <= 1'd1;
                state_1000 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 3'b000)begin
                iir1000_rom_addr   <=    iir1000_rom_addr -7'd6;
                coe_en_1000 <= 1'd1;
                state_1000 <= SEND;               
            end    
        end
        SEND:
        begin
            if(coe_en_1000 && coe_ful_cnt>0 && coe_ful_cnt<7)
                iir1000_rom_addr <= iir1000_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_1000 <= 1'd0;
                state_1000 <= CTRL_SIG;
            end
        end
        default :
            state_1000 <= IDLE;
    endcase
end

    
always@(posedge ws or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_1000 <= 17'd0;
    else if(coe_en_1000&& coe_ful_cnt>0 && coe_ful_cnt<7)
        coe_buf_1000 <= coe_1000;
    else
        coe_buf_1000 <=  coe_buf_1000;
end



//2250hz 系数控制 状态机
always@(posedge ws or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_2250 <= IDLE;
        coe_en_2250  <=1'd0;
        iir2250_rom_addr   <= 7'd60;
    end
    else
    case(state_2250)
        IDLE:
            state_2250 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 3'b101)
                state_2250 <= STA_CTRL_ST;
            else if(coe_ctrl == 3'b110)
                state_2250 <= STA_CTRL_WK;
            else
                state_2250 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 3'b000 && (!coe_en_250)&&(!coe_en_1000))begin
                iir2250_rom_addr   <=    iir2250_rom_addr +7'd6;
                coe_en_2250 <= 1'd1;
                state_2250 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 3'b000)begin
                iir2250_rom_addr   <=    iir2250_rom_addr -7'd6;
                coe_en_2250 <= 1'd1;
                state_2250 <= SEND;               
            end    
        end
        SEND:
        begin
            if(coe_en_2250 && coe_ful_cnt>0 && coe_ful_cnt<7)
                iir2250_rom_addr <= iir2250_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_2250 <= 1'd0;
                state_2250 <= CTRL_SIG;
            end
        end
        default :
            state_2250 <= IDLE;
    endcase
end

    
always@(posedge ws or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_2250 <= 17'd0;
    else if(coe_en_2250&& coe_ful_cnt>0 && coe_ful_cnt<7)
        coe_buf_2250 <= coe_2250;
    else
        coe_buf_2250 <=  coe_buf_2250;
end

//fc 250hz
iir iir_250 ( 
	.rst(sys_rst),
	.clk(ws),
	.Din(fir_vld_data),
    .coe     (coe_vld_250)   ,
    .coe_en   (coe_en_250)  ,
    
	.Dout(iir_dout_250)
);

rom_iir_250	rom_iir_250_inst (
	.address ( iir250_rom_addr ),
	.clock ( ws ),
	.q ( coe_250 )
	);

//fc 1000hz
iir iir_1000 (
    .rst(sys_rst),
    .clk(ws),
	.Din(iir_dout_250[28:5]),
    .coe     (coe_vld_1000)   ,
    .coe_en   (coe_en_1000)  ,
	.Dout(iir_dout_1000)


);

rom_iir_1000	rom_iir_1000_inst (
	.address ( iir1000_rom_addr ),
	.clock ( ws ),
	.q ( coe_1000 )
	);

//fc 2250hz
iir iir_2250 ( 
    .rst(sys_rst),
    .clk(ws),
	.Din(iir_dout_1000[25:2]),
    .coe     (coe_vld_2250)   ,
    .coe_en  (coe_en_2250)  ,
    
	.Dout(eq_data)
);

rom_iir_2250	rom_iir_2250_inst (
	.address ( iir2250_rom_addr ),
	.clock ( ws ),
	.q ( coe_2250 )
	);

endmodule