library verilog;
use verilog.vl_types.all;
entity i2s is
    generic(
        WORD_LENGTH     : integer := 24
    );
    port(
        sys_clk         : in     vl_logic;
        sdata           : in     vl_logic;
        ws              : out    vl_logic;
        bclk            : out    vl_logic;
        receive_valid   : out    vl_logic;
        receive_left_data: out    vl_logic_vector;
        receive_right_data: out    vl_logic_vector
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of WORD_LENGTH : constant is 1;
end i2s;
