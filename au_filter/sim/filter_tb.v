`timescale 1ns/1ns

module filter_tb();


reg    signed[23:0] au_data    ;
reg    sys_clk    ;
reg    sys_rst    ;
wire    dac_clka   ;
wire   [9:0] dac_dat_a  ;
wire        ws;
wire        bclk;

reg   [23:0]sin_data;
reg    [7:0]addr = 0;


   



integer i=0;

initial                                               
begin
        sys_rst = 0;   
		sys_clk = 0;
        #100;
		sys_rst = 1;
  
end

always@(negedge ws)begin
        addr = addr+4;    
        au_data =   sin_data-8388608;
    if(addr == 128)
        addr = 0;
end


always #10 sys_clk = ~sys_clk;


filter_test filter_test_inst
(
   .au_data       (au_data)     ,
   . sys_clk      (sys_clk) ,
   . sys_rst      (sys_rst) ,
   .  ws            (ws),
                  
   .      dac_clka(dac_clka) ,
   .     dac_dat_a(dac_dat_a)

);

pll	pll_inst (
	.inclk0 ( sys_clk ),
	.c0 ( bclk ),
	.c1 ( ws )
	);


/*always @(*)begin
    case(addr)
        0: sin_data =   24'hffffff;
        1: sin_data =   24'hffffff;
        2: sin_data =   24'hffffff;
        3: sin_data =   24'hffffff;
        4: sin_data =   24'hffffff;
        5: sin_data =   24'hffffff;
        6: sin_data =   24'hffffff;
        7: sin_data =   24'hffffff;
        8: sin_data =   24'hffffff;
        9: sin_data =   24'hffffff;
        10: sin_data =  24'hffffff;
        11: sin_data =  24'hffffff;
        12: sin_data =  24'hffffff;
        13: sin_data =  24'hffffff;
        14: sin_data =  24'hffffff;
        15: sin_data =  24'hffffff;
        16: sin_data =  24'hffffff;
        17: sin_data =  24'hffffff;
        18: sin_data =  24'hffffff;
        19: sin_data =  24'hffffff;
        20: sin_data =  24'hffffff;
        21: sin_data =  24'hffffff;
        22: sin_data =  24'hffffff;
        23: sin_data =  24'hffffff;
        24: sin_data =  24'hffffff;
        25: sin_data =  24'hffffff;
        26: sin_data =  24'hffffff;
        27: sin_data =  24'hffffff;
        28: sin_data =  24'hffffff;
        29: sin_data =  24'hffffff;
        30: sin_data =  24'hffffff;
        31: sin_data =  24'hffffff;
        32: sin_data =  24'hffffff;
        33: sin_data =  24'hffffff;
        34: sin_data =  24'hffffff;
        35: sin_data =  24'hffffff;
        36: sin_data =  24'hffffff;
        37: sin_data =  24'hffffff;
        38: sin_data =  24'hffffff;
        39: sin_data =  24'hffffff;
        40: sin_data =  24'hffffff;
        41: sin_data =  24'hffffff;
        42: sin_data =  24'hffffff;
        43: sin_data =  24'hffffff;
        44: sin_data =  24'hffffff;
        45: sin_data =  24'hffffff;
        46: sin_data =  24'hffffff;
        47: sin_data =  24'hffffff;
        48: sin_data =  24'hffffff;
        49: sin_data =  24'hffffff;
        50: sin_data =  24'hffffff;
        51: sin_data =  24'hffffff;
        52: sin_data =  24'hffffff;
        53: sin_data =  24'hffffff;
        54: sin_data =  24'hffffff;
        55: sin_data =  24'hffffff;
        56: sin_data =  24'hffffff;
        57: sin_data =  24'hffffff;
        58: sin_data =  24'hffffff;
        59: sin_data =  24'hffffff;
        60: sin_data =  24'hffffff;
        61: sin_data =  24'hffffff;
        62: sin_data =  24'hffffff;
        63: sin_data =  24'hffffff;
        64: sin_data =  24'hffffff;
        65: sin_data =  24'h000000;
        66: sin_data =  24'h000000;
        67: sin_data =  24'h000000;
        68: sin_data =  24'h000000;
        69: sin_data =  24'h000000;
        70: sin_data =  24'h000000;
        71: sin_data =  24'h000000;
        72: sin_data =  24'h000000;
        73: sin_data =  24'h000000;
        74: sin_data =  24'h000000;
        75: sin_data =  24'h000000;
        76: sin_data =  24'h000000;
        77: sin_data =  24'h000000;
        78: sin_data =  24'h000000;
        79: sin_data =  24'h000000;
        80: sin_data =  24'h000000;
        81: sin_data =  24'h000000;
        82: sin_data =  24'h000000;
        83: sin_data =  24'h000000;
        84: sin_data =  24'h000000;
        85: sin_data =  24'h000000;
        86: sin_data =  24'h000000;
        87: sin_data =  24'h000000;
        88: sin_data =  24'h000000;
        89: sin_data =  24'h000000;
        90: sin_data =  24'h000000;
        91: sin_data =  24'h000000;
        92: sin_data =  24'h000000;
        93: sin_data =  24'h000000;
        94: sin_data =  24'h000000;
        95: sin_data =  24'h000000;
        96: sin_data =  24'h000000;
        97: sin_data =  24'h000000;
        98: sin_data =  24'h000000;
        99: sin_data =  24'h000000;
        100: sin_data = 24'h000000;
        101: sin_data = 24'h000000;
        102: sin_data = 24'h000000;
        103: sin_data = 24'h000000;
        104: sin_data = 24'h000000;
        105: sin_data = 24'h000000;
        106: sin_data = 24'h000000;
        107: sin_data = 24'h000000;
        108: sin_data = 24'h000000;
        109: sin_data = 24'h000000;
        110: sin_data = 24'h000000;
        111: sin_data = 24'h000000;
        112: sin_data = 24'h000000;
        113: sin_data = 24'h000000;
        114: sin_data = 24'h000000;
        115: sin_data = 24'h000000;
        116: sin_data = 24'h000000;
        117: sin_data = 24'h000000;
        118: sin_data = 24'h000000;
        119: sin_data = 24'h000000;
        120: sin_data = 24'h000000;
        121: sin_data = 24'h000000;
        122: sin_data = 24'h000000;
        123: sin_data = 24'h000000;
        124: sin_data = 24'h000000;
        125: sin_data = 24'h000000;
        126: sin_data = 24'h000000;
        127: sin_data = 24'h000000;
    endcase
end*/


always @(*)begin
    case(addr)
        0: sin_data =   24'h7F0000;
        1: sin_data =   24'h850000;
        2: sin_data =   24'h8C0000;
        3: sin_data =   24'h920000;
        4: sin_data =   24'h980000;
        5: sin_data =   24'h9E0000;
        6: sin_data =   24'hA40000;
        7: sin_data =   24'hAA0000;
        8: sin_data =   24'hB00000;
        9: sin_data =   24'hB60000;
        10: sin_data =  24'hBC0000;
        11: sin_data =  24'hC10000;
        12: sin_data =  24'hC60000;
        13: sin_data =  24'hCB0000;
        14: sin_data =  24'hD00000;
        15: sin_data =  24'hD50000;
        16: sin_data =  24'hDA0000;
        17: sin_data =  24'hDE0000;
        18: sin_data =  24'hE20000;
        19: sin_data =  24'hE60000;
        20: sin_data =  24'hEA0000;
        21: sin_data =  24'hED0000;
        22: sin_data =  24'hF00000;
        23: sin_data =  24'hF30000;
        24: sin_data =  24'hF50000;
        25: sin_data =  24'hF70000;
        26: sin_data =  24'hF90000;
        27: sin_data =  24'hFB0000;
        28: sin_data =  24'hFC0000;
        29: sin_data =  24'hFD0000;
        30: sin_data =  24'hFE0000;
        31: sin_data =  24'hFE0000;
        32: sin_data =  24'hFE0000;
        33: sin_data =  24'hFE0000;
        34: sin_data =  24'hFE0000;
        35: sin_data =  24'hFD0000;
        36: sin_data =  24'hFC0000;
        37: sin_data =  24'hFA0000;
        38: sin_data =  24'hF80000;
        39: sin_data =  24'hF60000;
        40: sin_data =  24'hF40000;
        41: sin_data =  24'hF10000;
        42: sin_data =  24'hEF0000;
        43: sin_data =  24'hEB0000;
        44: sin_data =  24'hE80000;
        45: sin_data =  24'hE40000;
        46: sin_data =  24'hE00000;
        47: sin_data =  24'hDC0000;
        48: sin_data =  24'hD80000;
        49: sin_data =  24'hD30000;
        50: sin_data =  24'hCE0000;
        51: sin_data =  24'hC90000;
        52: sin_data =  24'hC40000;
        53: sin_data =  24'hBE0000;
        54: sin_data =  24'hB90000;
        55: sin_data =  24'hB30000;
        56: sin_data =  24'hAD0000;
        57: sin_data =  24'hA70000;
        58: sin_data =  24'hA10000;
        59: sin_data =  24'h9B0000;
        60: sin_data =  24'h950000;
        61: sin_data =  24'h8F0000;
        62: sin_data =  24'h890000;
        63: sin_data =  24'h820000;
        64: sin_data =  24'h7D0000;
        65: sin_data =  24'h770000;
        66: sin_data =  24'h700000;
        67: sin_data =  24'h6A0000;
        68: sin_data =  24'h640000;
        69: sin_data =  24'h5E0000;
        70: sin_data =  24'h580000;
        71: sin_data =  24'h520000;
        72: sin_data =  24'h4C0000;
        73: sin_data =  24'h460000;
        74: sin_data =  24'h410000;
        75: sin_data =  24'h3C0000;
        76: sin_data =  24'h360000;
        77: sin_data =  24'h310000;
        78: sin_data =  24'h2C0000;
        79: sin_data =  24'h280000;
        80: sin_data =  24'h230000;
        81: sin_data =  24'h1F0000;
        82: sin_data =  24'h1B0000;
        83: sin_data =  24'h170000;
        84: sin_data =  24'h140000;
        85: sin_data =  24'h110000;
        86: sin_data =  24'hE0000;
        87: sin_data =  24'hB0000;
        88: sin_data =  24'h90000;
        89: sin_data =  24'h70000;
        90: sin_data =  24'h50000;
        91: sin_data =  24'h30000;
        92: sin_data =  24'h20000;
        93: sin_data =  24'h10000;
        94: sin_data =  24'h10000;
        95: sin_data =  24'h10000;
        96: sin_data =  24'h10000;
        97: sin_data =  24'h10000;
        98: sin_data =  24'h20000;
        99: sin_data =  24'h30000;
        100: sin_data = 24'h40000;
        101: sin_data = 24'h60000;
        102: sin_data = 24'h70000;
        103: sin_data = 24'hA0000;
        104: sin_data = 24'hC0000;
        105: sin_data = 24'hF0000 ;
        106: sin_data = 24'h120000;
        107: sin_data = 24'h150000;
        108: sin_data = 24'h190000;
        109: sin_data = 24'h1D0000;
        110: sin_data = 24'h210000;
        111: sin_data = 24'h250000;
        112: sin_data = 24'h2A0000;
        113: sin_data = 24'h2E0000;
        114: sin_data = 24'h330000;
        115: sin_data = 24'h380000;
        116: sin_data = 24'h3E0000;
        117: sin_data = 24'h430000;
        118: sin_data = 24'h490000;
        119: sin_data = 24'h4E0000;
        120: sin_data = 24'h540000;
        121: sin_data = 24'h5A0000;
        122: sin_data = 24'h600000;
        123: sin_data = 24'h670000;
        124: sin_data = 24'h6D0000;
        125: sin_data = 24'h730000;
        126: sin_data = 24'h790000;
        127: sin_data = 24'h7F0000;
    endcase
end



endmodule