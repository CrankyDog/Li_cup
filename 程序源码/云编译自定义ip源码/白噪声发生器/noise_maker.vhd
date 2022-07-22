library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
USE IEEE.numeric_std.ALL;

entity noise_maker is
  port(clk : in std_logic;
       rst : in std_logic;
	   sregout : out signed(15 downto 0));
end;
                   
architecture rt1 of noise_maker is

signal cnt0: integer range 0 to 1000;
signal din: std_logic;
signal memory :signed(15 downto 0) := x"8000";
signal noise_buf:signed(11 downto 0);
begin
din<=memory(15) xor memory(14) xor memory(12) xor memory(3);
sregout<= memory/8;
PROCESS(clk,rst)
	 begin
	 if rst='1' then
	    cnt0 <= 0;
	 elsif rising_edge(clk) then
       	if cnt0=90 then
          cnt0 <= 0;
       else
    	  cnt0 <=cnt0 + 1;
		end if;
	 end if;
end process;
       
PROCESS(cnt0,rst)
	begin
    if rst='1' then
	    memory <= x"8000";
	elsif cnt0=90 then
		memory<= memory(14 downto 0) & din;
	end if;
end process;

end;