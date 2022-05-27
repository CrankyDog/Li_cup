module equalizer(
        input                   clk_40k,
		input					clk_240k,
        input                   sys_rst,
        input [4:0]             coe_ctrl,
        input   signed [23:0]   fir_vld_data,
        
        output signed [28:0]    eq_data
        
);
localparam                  IDLE =  0;
localparam                  CTRL_SIG =  1;
localparam                  STA_CTRL_ST =  2;
localparam                  STA_CTRL_WK =  3;
localparam                  SEND =  4;


reg [3:0]       state_100;
reg [3:0]       state_200;
//reg [3:0]       state_400;
reg [3:0]       state_600;
reg [3:0]       state_1000;
reg [3:0]       state_2000;
reg [3:0]       state_3000;
reg [3:0]       state_5000;
reg [3:0]       state_8000;

wire signed [28:0]  iir_dout_100;
wire signed [28:0]  iir_dout_200;
//wire signed [28:0]  iir_dout_400;
wire signed [28:0]  iir_dout_600;
wire signed [28:0]  iir_dout_1000;
wire signed [28:0]  iir_dout_2000;
wire signed [28:0]  iir_dout_3000;
wire signed [28:0]  iir_dout_5000;





wire signed [16:0] coe_vld_100 ;
wire signed [16:0] coe_vld_200 ;
//wire signed [16:0] coe_vld_400 ;
wire signed [16:0] coe_vld_600 ;
wire signed [16:0] coe_vld_1000 ;
wire signed [16:0] coe_vld_2000 ;
wire signed [16:0] coe_vld_3000 ;
wire signed [16:0] coe_vld_5000 ;
wire signed [16:0] coe_vld_8000 ;

wire signed [16:0] coe_100 ;
wire signed [16:0] coe_200 ;
//wire signed [16:0] coe_400 ;
wire signed [16:0] coe_600 ;
wire signed [16:0] coe_1000 ;
wire signed [16:0] coe_2000 ;
wire signed [16:0] coe_3000 ;
wire signed [16:0] coe_5000 ;
wire signed [16:0] coe_8000 ;

reg signed [16:0] coe_buf_100 ;
reg signed [16:0] coe_buf_200 ;
//reg signed [16:0] coe_buf_400 ;
reg signed [16:0] coe_buf_600 ;
reg signed [16:0] coe_buf_1000 ;
reg signed [16:0] coe_buf_2000 ;
reg signed [16:0] coe_buf_3000 ;
reg signed [16:0] coe_buf_5000 ;
reg signed [16:0] coe_buf_8000 ;

reg              coe_en_100 ;
reg              coe_en_200 ;
//reg              coe_en_400 ;
reg              coe_en_600 ;
reg              coe_en_1000 ;
reg              coe_en_2000 ;
reg              coe_en_3000 ;
reg              coe_en_5000 ;
reg              coe_en_8000 ;
 

reg [6:0]   iir100_rom_addr;
reg [6:0]   iir200_rom_addr;
//reg [6:0]   iir400_rom_addr;
reg [6:0]   iir600_rom_addr;
reg [6:0]   iir1000_rom_addr;
reg [6:0]   iir2000_rom_addr;
reg [6:0]   iir3000_rom_addr;
reg [6:0]   iir5000_rom_addr;
reg [6:0]   iir8000_rom_addr;


assign  coe_vld_100  =  coe_buf_100 ;
assign  coe_vld_200  =  coe_buf_200 ;
//assign  coe_vld_400  =  coe_buf_400 ;
assign  coe_vld_600  =  coe_buf_600 ;
assign  coe_vld_1000 =  coe_buf_1000;
assign  coe_vld_2000 =  coe_buf_2000;
assign  coe_vld_3000 =  coe_buf_3000;
assign  coe_vld_5000 =  coe_buf_5000;
assign  coe_vld_8000 =  coe_buf_8000;

reg [2:0] coe_ful_cnt; 

        
always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)begin
        coe_ful_cnt <= 3'd0;
    end
    else   if(coe_ful_cnt == 3'd7)begin
            coe_ful_cnt <= 3'd0;
    end
    else    if(coe_en_100 ||coe_en_200||coe_en_600||coe_en_1000||coe_en_2000||coe_en_3000
				||coe_en_5000||coe_en_8000)
            coe_ful_cnt <= coe_ful_cnt + 3'd1;
    else    
            coe_ful_cnt <= coe_ful_cnt;
end        
        

//100hz 系数控制 状态机
always@(posedge clk_40k or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_100 <= IDLE;
        coe_en_100  <=1'd0;
        iir100_rom_addr   <= 7'd30;
    end
    else
    case(state_100)
        IDLE:
            state_100 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 5'd1)
                state_100 <= STA_CTRL_ST;
            else if(coe_ctrl == 5'd2)
                state_100 <= STA_CTRL_WK;
            else
                state_100 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir100_rom_addr>=7'd60)begin
                    iir100_rom_addr   <=    iir100_rom_addr;
                end
                else begin
                    iir100_rom_addr   <=    iir100_rom_addr +7'd6;
                end
                coe_en_100 <= 1'd1;
                state_100 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir100_rom_addr<=7'd0)begin
                    iir100_rom_addr   <=    iir100_rom_addr;
                end
                else begin
                    iir100_rom_addr   <=    iir100_rom_addr -7'd6;
                end
                coe_en_100 <= 1'd1;
                state_100 <= SEND;                     
            end    
        end
        SEND:
        begin
            if(coe_en_100 && coe_ful_cnt>=0 && coe_ful_cnt<5)
                iir100_rom_addr <= iir100_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_100 <= 1'd0;
                iir100_rom_addr <= iir100_rom_addr-7'd5;
                state_100 <= CTRL_SIG;
            end
        end
        default :
            state_100 <= IDLE;
    endcase
end

    
always@(posedge clk_40k or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_100 <= 17'd0;
    else if(coe_en_100&& coe_ful_cnt>=1 && coe_ful_cnt<7)
        coe_buf_100 <= coe_100;
    else
        coe_buf_100 <=  coe_buf_100;
end


//200hz 系数控制 状态机
always@(posedge clk_40k or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_200 <= IDLE;
        coe_en_200  <=1'd0;
        iir200_rom_addr   <= 7'd30;
    end
    else
    case(state_200)
        IDLE:
            state_200 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 5'd3)
                state_200 <= STA_CTRL_ST;
            else if(coe_ctrl == 5'd4)
                state_200 <= STA_CTRL_WK;
            else
                state_200 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir200_rom_addr>=7'd60)begin
                    iir200_rom_addr   <=    iir200_rom_addr;
                end
                else begin
                    iir200_rom_addr   <=    iir200_rom_addr +7'd6;
                end
                coe_en_200 <= 1'd1;
                state_200 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir200_rom_addr<=7'd0)begin
                    iir200_rom_addr   <=    iir200_rom_addr;
                end
                else begin
                    iir200_rom_addr   <=    iir200_rom_addr -7'd6;
                end
                coe_en_200 <= 1'd1;
                state_200 <= SEND;                     
            end    
        end
        SEND:
        begin
            if(coe_en_200 && coe_ful_cnt>=0 && coe_ful_cnt<5)
                iir200_rom_addr <= iir200_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_200 <= 1'd0;
                iir200_rom_addr <= iir200_rom_addr-7'd5;
                state_200 <= CTRL_SIG;
            end
        end
        default :
            state_200 <= IDLE;
    endcase
end

    
always@(posedge clk_40k or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_200 <= 17'd0;
    else if(coe_en_200&& coe_ful_cnt>=1 && coe_ful_cnt<7)
        coe_buf_200 <= coe_200;
    else
        coe_buf_200 <=  coe_buf_200;
end


////400hz 系数控制 状态机
//always@(posedge clk_40k or negedge sys_rst)
//begin
//	if(sys_rst == 1'b0)begin
//        state_400 <= IDLE;
//        coe_en_400  <=1'd0;
//        iir400_rom_addr   <= 7'd30;
//    end
//    else
//    case(state_400)
//        IDLE:
//            state_400 <= CTRL_SIG;
//        CTRL_SIG:
//        begin
//            if(coe_ctrl == 5'd5)
//                state_400 <= STA_CTRL_ST;
//            else if(coe_ctrl == 5'd6)
//                state_400 <= STA_CTRL_WK;
//            else
//                state_400 <= CTRL_SIG;
//        end
//        STA_CTRL_ST:
//        begin
//            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
//                if(iir400_rom_addr>=7'd60)begin
//                    iir400_rom_addr   <=    iir400_rom_addr;
//                end
//                else begin
//                    iir400_rom_addr   <=    iir400_rom_addr +7'd6;
//                end
//                coe_en_400 <= 1'd1;
//                state_400 <= SEND;               
//            end    
//        end
//        STA_CTRL_WK:
//        begin
//            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
//                if(iir400_rom_addr<=7'd0)begin
//                    iir400_rom_addr   <=    iir400_rom_addr;
//                end
//                else begin
//                    iir400_rom_addr   <=    iir400_rom_addr -7'd6;
//                end
//                coe_en_400 <= 1'd1;
//                state_400 <= SEND;                     
//            end    
//        end
//        SEND:
//        begin
//            if(coe_en_400 && coe_ful_cnt>=0 && coe_ful_cnt<5)
//                iir400_rom_addr <= iir400_rom_addr + 7'd1;
//            else   if(coe_ful_cnt == 3'd7)begin
//                coe_en_400 <= 1'd0;
//                iir400_rom_addr <= iir400_rom_addr-7'd5;
//                state_400 <= CTRL_SIG;
//            end
//        end
//        default :
//            state_400 <= IDLE;
//    endcase
//end
//
//    
//always@(posedge clk_40k or  negedge sys_rst)begin
//    if(sys_rst == 1'b0)
//        coe_buf_400 <= 17'd0;
//    else if(coe_en_400&& coe_ful_cnt>=1 && coe_ful_cnt<7)
//        coe_buf_400 <= coe_400;
//    else
//        coe_buf_400 <=  coe_buf_400;
//end


//600hz 系数控制 状态机
always@(posedge clk_40k or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_600 <= IDLE;
        coe_en_600  <=1'd0;
        iir600_rom_addr   <= 7'd30;
    end
    else
    case(state_600)
        IDLE:
            state_600 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 5'd7)
                state_600 <= STA_CTRL_ST;
            else if(coe_ctrl == 5'd8)
                state_600 <= STA_CTRL_WK;
            else
                state_600 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir600_rom_addr>=7'd60)begin
                    iir600_rom_addr   <=    iir600_rom_addr;
                end
                else begin
                    iir600_rom_addr   <=    iir600_rom_addr +7'd6;
                end
                coe_en_600 <= 1'd1;
                state_600 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir600_rom_addr<=7'd0)begin
                    iir600_rom_addr   <=    iir600_rom_addr;
                end
                else begin
                    iir600_rom_addr   <=    iir600_rom_addr -7'd6;
                end
                coe_en_600 <= 1'd1;
                state_600 <= SEND;                     
            end    
        end
        SEND:
        begin
            if(coe_en_600 && coe_ful_cnt>=0 && coe_ful_cnt<5)
                iir600_rom_addr <= iir600_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_600 <= 1'd0;
                iir600_rom_addr <= iir600_rom_addr-7'd5;
                state_600 <= CTRL_SIG;
            end
        end
        default :
            state_600 <= IDLE;
    endcase
end

    
always@(posedge clk_40k or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_600 <= 17'd0;
    else if(coe_en_600&& coe_ful_cnt>=1 && coe_ful_cnt<7)
        coe_buf_600 <= coe_600;
    else
        coe_buf_600 <=  coe_buf_600;
end



//1000hz 系数控制 状态机
always@(posedge clk_40k or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_1000 <= IDLE;
        coe_en_1000  <=1'd0;
        iir1000_rom_addr   <= 7'd30;
    end
    else
    case(state_1000)
        IDLE:
            state_1000 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 5'd9)
                state_1000 <= STA_CTRL_ST;
            else if(coe_ctrl == 5'd10)
                state_1000 <= STA_CTRL_WK;
            else
                state_1000 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_100)&&(!coe_en_2000))begin
                if(iir1000_rom_addr>=7'd60)begin
                    iir1000_rom_addr   <=    iir1000_rom_addr;
                end
                else begin
                    iir1000_rom_addr   <=    iir1000_rom_addr +7'd6;
                end
                coe_en_1000 <= 1'd1;
                state_1000 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_100)&&(!coe_en_2000))begin
                if(iir1000_rom_addr<=7'd0)begin
                    iir1000_rom_addr   <=    iir1000_rom_addr;
                end
                else begin
                    iir1000_rom_addr   <=    iir1000_rom_addr -7'd6;
                end
                coe_en_1000 <= 1'd1;
                state_1000 <= SEND;               
            end    
        end
        SEND:
        begin
            if(coe_en_1000 && coe_ful_cnt>=0 && coe_ful_cnt<5)
                iir1000_rom_addr <= iir1000_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_1000 <= 1'd0;
                iir1000_rom_addr <= iir1000_rom_addr-7'd5;
                state_1000 <= CTRL_SIG;
            end
        end
        default :
            state_1000 <= IDLE;
    endcase
end

    
always@(posedge clk_40k or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_1000 <= 17'd0;
    else if(coe_en_1000&& coe_ful_cnt>=1 && coe_ful_cnt<7)
        coe_buf_1000 <= coe_1000;
    else
        coe_buf_1000 <=  coe_buf_1000;
end



//2000hz 系数控制 状态机
always@(posedge clk_40k or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_2000 <= IDLE;
        coe_en_2000  <=1'd0;
        iir2000_rom_addr   <= 7'd30;
    end
    else
    case(state_2000)
        IDLE:
            state_2000 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 5'd11)
                state_2000 <= STA_CTRL_ST;
            else if(coe_ctrl == 5'd12)
                state_2000 <= STA_CTRL_WK;
            else
                state_2000 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_100)&&(!coe_en_1000))begin
                if(iir2000_rom_addr>=7'd60)begin
                    iir2000_rom_addr   <=    iir2000_rom_addr;
                end
                else begin
                    iir2000_rom_addr   <=    iir2000_rom_addr +7'd6;
                end
                coe_en_2000 <= 1'd1;
                state_2000 <= SEND;               
            end 
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_100)&&(!coe_en_1000))begin
                if(iir2000_rom_addr<=7'd0)begin
                    iir2000_rom_addr   <=    iir2000_rom_addr;
                end
                else begin
                    iir2000_rom_addr   <=    iir2000_rom_addr -7'd6;
                end
                coe_en_2000 <= 1'd1;
                state_2000 <= SEND;               
            end  
        end
        SEND:
        begin
            if(coe_en_2000 && coe_ful_cnt>=0 && coe_ful_cnt<5)
                iir2000_rom_addr <= iir2000_rom_addr + 7'd1;                
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_2000 <= 1'd0;
                iir2000_rom_addr <= iir2000_rom_addr-7'd5;
                state_2000 <= CTRL_SIG;
            end
        end
        default :
            state_2000 <= IDLE;
    endcase
end

  
always@(posedge clk_40k or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_2000 <= 17'd0;
    else if(coe_en_2000&& coe_ful_cnt>=1 && coe_ful_cnt<7)
        coe_buf_2000 <= coe_2000;
    else
        coe_buf_2000 <=  coe_buf_2000;
end


//3000hz 系数控制 状态机
always@(posedge clk_40k or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_3000 <= IDLE;
        coe_en_3000  <=1'd0;
        iir3000_rom_addr   <= 7'd30;
    end
    else
    case(state_3000)
        IDLE:
            state_3000 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 5'd13)
                state_3000 <= STA_CTRL_ST;
            else if(coe_ctrl == 5'd14)
                state_3000 <= STA_CTRL_WK;
            else
                state_3000 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir3000_rom_addr>=7'd60)begin
                    iir3000_rom_addr   <=    iir3000_rom_addr;
                end
                else begin
                    iir3000_rom_addr   <=    iir3000_rom_addr +7'd6;
                end
                coe_en_3000 <= 1'd1;
                state_3000 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir3000_rom_addr<=7'd0)begin
                    iir3000_rom_addr   <=    iir3000_rom_addr;
                end
                else begin
                    iir3000_rom_addr   <=    iir3000_rom_addr -7'd6;
                end
                coe_en_3000 <= 1'd1;
                state_3000 <= SEND;                     
            end    
        end
        SEND:
        begin
            if(coe_en_3000 && coe_ful_cnt>=0 && coe_ful_cnt<5)
                iir3000_rom_addr <= iir3000_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_3000 <= 1'd0;
                iir3000_rom_addr <= iir3000_rom_addr-7'd5;
                state_3000 <= CTRL_SIG;
            end
        end
        default :
            state_3000 <= IDLE;
    endcase
end

    
always@(posedge clk_40k or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_3000 <= 17'd0;
    else if(coe_en_3000&& coe_ful_cnt>=1 && coe_ful_cnt<7)
        coe_buf_3000 <= coe_3000;
    else
        coe_buf_3000 <=  coe_buf_3000;
end


//5000hz 系数控制 状态机
always@(posedge clk_40k or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_5000 <= IDLE;
        coe_en_5000  <=1'd0;
        iir5000_rom_addr   <= 7'd30;
    end
    else
    case(state_5000)
        IDLE:
            state_5000 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 5'd15)
                state_5000 <= STA_CTRL_ST;
            else if(coe_ctrl == 5'd16)
                state_5000 <= STA_CTRL_WK;
            else
                state_5000 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir5000_rom_addr>=7'd60)begin
                    iir5000_rom_addr   <=    iir5000_rom_addr;
                end
                else begin
                    iir5000_rom_addr   <=    iir5000_rom_addr +7'd6;
                end
                coe_en_5000 <= 1'd1;
                state_5000 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir5000_rom_addr<=7'd0)begin
                    iir5000_rom_addr   <=    iir5000_rom_addr;
                end
                else begin
                    iir5000_rom_addr   <=    iir5000_rom_addr -7'd6;
                end
                coe_en_5000 <= 1'd1;
                state_5000 <= SEND;                     
            end    
        end
        SEND:
        begin
            if(coe_en_5000 && coe_ful_cnt>=0 && coe_ful_cnt<5)
                iir5000_rom_addr <= iir5000_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_5000 <= 1'd0;
                iir5000_rom_addr <= iir5000_rom_addr-7'd5;
                state_5000 <= CTRL_SIG;
            end
        end
        default :
            state_5000 <= IDLE;
    endcase
end

    
always@(posedge clk_40k or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_5000 <= 17'd0;
    else if(coe_en_5000&& coe_ful_cnt>=1 && coe_ful_cnt<7)
        coe_buf_5000 <= coe_5000;
    else
        coe_buf_5000 <=  coe_buf_5000;
end


//8000hz 系数控制 状态机
always@(posedge clk_40k or negedge sys_rst)
begin
	if(sys_rst == 1'b0)begin
        state_8000 <= IDLE;
        coe_en_8000  <=1'd0;
        iir8000_rom_addr   <= 7'd30;
    end
    else
    case(state_8000)
        IDLE:
            state_8000 <= CTRL_SIG;
        CTRL_SIG:
        begin
            if(coe_ctrl == 5'd17)
                state_8000 <= STA_CTRL_ST;
            else if(coe_ctrl == 5'd18)
                state_8000 <= STA_CTRL_WK;
            else
                state_8000 <= CTRL_SIG;
        end
        STA_CTRL_ST:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir8000_rom_addr>=7'd60)begin
                    iir8000_rom_addr   <=    iir8000_rom_addr;
                end
                else begin
                    iir8000_rom_addr   <=    iir8000_rom_addr +7'd6;
                end
                coe_en_8000 <= 1'd1;
                state_8000 <= SEND;               
            end    
        end
        STA_CTRL_WK:
        begin
            if(coe_ctrl == 5'd0 && (!coe_en_1000)&&(!coe_en_2000))begin
                if(iir8000_rom_addr<=7'd0)begin
                    iir8000_rom_addr   <=    iir8000_rom_addr;
                end
                else begin
                    iir8000_rom_addr   <=    iir8000_rom_addr -7'd6;
                end
                coe_en_8000 <= 1'd1;
                state_8000 <= SEND;                     
            end    
        end
        SEND:
        begin
            if(coe_en_8000 && coe_ful_cnt>=0 && coe_ful_cnt<5)
                iir8000_rom_addr <= iir8000_rom_addr + 7'd1;
            else   if(coe_ful_cnt == 3'd7)begin
                coe_en_8000 <= 1'd0;
                iir8000_rom_addr <= iir8000_rom_addr-7'd5;
                state_8000 <= CTRL_SIG;
            end
        end
        default :
            state_8000 <= IDLE;
    endcase
end

    
always@(posedge clk_40k or  negedge sys_rst)begin
    if(sys_rst == 1'b0)
        coe_buf_8000 <= 17'd0;
    else if(coe_en_8000&& coe_ful_cnt>=1 && coe_ful_cnt<7)
        coe_buf_8000 <= coe_8000;
    else
        coe_buf_8000 <=  coe_buf_8000;
end

//buf
reg [28:0] iir_dout_100_buf;
reg [28:0] iir_dout_200_buf;
reg [28:0] iir_dout_400_buf;
reg [28:0] iir_dout_600_buf;
reg [28:0] iir_dout_1000_buf;
reg [28:0] iir_dout_2000_buf;
reg [28:0] iir_dout_3000_buf;
reg [28:0] iir_dout_5000_buf;


always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        iir_dout_100_buf <= 29'd0;
    else   
	iir_dout_100_buf <= iir_dout_100;
end


always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        iir_dout_1000_buf <= 29'd0;
    else   
	iir_dout_1000_buf <= iir_dout_1000;
end

always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        iir_dout_2000_buf <= 29'd0;
    else   
	iir_dout_2000_buf <= iir_dout_2000;
end

always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        iir_dout_200_buf <= 29'd0;
    else   
	iir_dout_200_buf <= iir_dout_200;
end

//always @(posedge clk_40k or negedge sys_rst)begin
//    if(!sys_rst)
//        iir_dout_400_buf <= 29'd0;
//    else   
//	iir_dout_400_buf <= iir_dout_400;
//end

always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        iir_dout_600_buf <= 29'd0;
    else   
	iir_dout_600_buf <= iir_dout_600;
end

always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        iir_dout_3000_buf <= 29'd0;
    else   
	iir_dout_3000_buf <= iir_dout_3000;
end

always @(posedge clk_40k or negedge sys_rst)begin
    if(!sys_rst)
        iir_dout_5000_buf <= 29'd0;
    else   
	iir_dout_5000_buf <= iir_dout_5000;
end




//fc 100hz
iir iir_100 ( 
	.rst(sys_rst),
	.clk_40k(clk_40k),
	.clk_240k(clk_240k),
	.Din(fir_vld_data),
    .coe     (coe_vld_100)   ,
    .coe_en   (coe_en_100)  ,
    
	.Dout(iir_dout_100)
);

rom_iir_100	rom_iir_100_inst (
	.address ( iir100_rom_addr ),
	.clock ( clk_40k ),
	.q ( coe_100 )
	);


//fc 200hz
iir iir_200 ( 
	.rst(sys_rst),
	.clk_40k(clk_40k),
	.clk_240k(clk_240k),
	.Din(iir_dout_100_buf[23:0]),
    .coe     (coe_vld_200)   ,
    .coe_en   (coe_en_200)  ,
    
	.Dout(iir_dout_200)
);

rom_iir_200	rom_iir_200_inst (
	.address ( iir200_rom_addr ),
	.clock ( clk_40k ),
	.q ( coe_200 )
	);
	
	
////fc 400hz
//iir iir_400 ( 
//	.rst(sys_rst),
//	.clk_40k(clk_40k),
//	.clk_240k(clk_240k),
//	.Din(iir_dout_200_buf[23:0]),
//    .coe     (coe_vld_400)   ,
//    .coe_en   (coe_en_400)  ,
//    
//	.Dout(iir_dout_400)
//);
//
//rom_iir_400	rom_iir_400_inst (
//	.address ( iir400_rom_addr ),
//	.clock ( clk_40k ),
//	.q ( coe_400 )
//	);
	
//fc 600hz
iir iir_600 ( 
	.rst(sys_rst),
	.clk_40k(clk_40k),
	.clk_240k(clk_240k),
	.Din(iir_dout_200_buf[23:0]),
    .coe     (coe_vld_600)   ,
    .coe_en   (coe_en_600)  ,
    
	.Dout(iir_dout_600)
);

rom_iir_600	rom_iir_600_inst (
	.address ( iir600_rom_addr ),
	.clock ( clk_40k ),
	.q ( coe_600 )
	);
	
//fc 1000hz
iir iir_1000 (
    .rst(sys_rst),
	.clk_40k(clk_40k),
	.clk_240k(clk_240k),
	.Din(iir_dout_600_buf[23:0]),
    .coe     (coe_vld_1000)   ,
    .coe_en   (coe_en_1000)  ,
	.Dout(iir_dout_1000)


);

rom_iir_1000	rom_iir_1000_inst (
	.address ( iir1000_rom_addr ),
	.clock ( clk_40k ),
	.q ( coe_1000 )
	);
	
//fc 2000hz
iir iir_2000 ( 
    .rst(sys_rst),
	.clk_40k(clk_40k),
	.clk_240k(clk_240k),
	.Din(iir_dout_1000_buf[23:0]),
    .coe     (coe_vld_2000)   ,
    .coe_en  (coe_en_2000)  ,
    
	.Dout(iir_dout_2000)
);


rom_iir_2000	rom_iir_2000_inst (
	.address ( iir2000_rom_addr ),
	.clock ( clk_40k ),
	.q ( coe_2000 )
	);

//fc 3000hz
iir iir_3000 ( 
	.rst(sys_rst),
	.clk_40k(clk_40k),
	.clk_240k(clk_240k),
	.Din(iir_dout_2000_buf[23:0]),
    .coe     (coe_vld_3000)   ,
    .coe_en   (coe_en_3000)  ,
    
	.Dout(iir_dout_3000)
);

rom_iir_3000	rom_iir_3000_inst (
	.address ( iir3000_rom_addr ),
	.clock ( clk_40k ),
	.q ( coe_3000 )
	);
	
//fc 5000hz
iir iir_5000 ( 
	.rst(sys_rst),
	.clk_40k(clk_40k),
	.clk_240k(clk_240k),
	.Din(iir_dout_3000_buf[23:0]),
    .coe     (coe_vld_5000)   ,
    .coe_en   (coe_en_5000)  ,
    
	.Dout(iir_dout_5000)
);

rom_iir_5000	rom_iir_5000_inst (
	.address ( iir5000_rom_addr ),
	.clock ( clk_40k ),
	.q ( coe_5000 )
	);
	
//fc 8000hz
iir iir_8000 ( 
	.rst(sys_rst),
	.clk_40k(clk_40k),
	.clk_240k(clk_240k),
	.Din(iir_dout_5000_buf[23:0]),
    .coe     (coe_vld_8000)   ,
    .coe_en   (coe_en_8000)  ,
    
	.Dout(eq_data)
);

rom_iir_8000	rom_iir_8000_inst (
	.address ( iir8000_rom_addr ),
	.clock ( clk_40k ),
	.q ( coe_8000 )
	);
	

endmodule