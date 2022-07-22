-- lms.vhdl

library IEEE;
use IEEE.Std_Logic_1164.all;
use IEEE.Numeric_Std.all;




architecture Behavioural of CustomWrapper is
  signal noise:signed(15 downto 0);
  signal noise_buf:signed(15 downto 0);
  signal InputA_buf:signed(15 downto 0);

  signal ori_music:signed(15 downto 0);
  signal eq_music:signed(15 downto 0);
  signal filtered_music:signed(15 downto 0);
  signal noise_music:signed(15 downto 0);
  signal sound_out:signed(15 downto 0);
  signal ctrl_buf:signed(15 downto 0);

  signal s:std_logic_vector(1 downto 0);
begin
    ori_music <= InputA_buf;
    eq_music <= InputA_buf;
    noise_music <= noise_buf + InputA_buf;
    ctrl_buf <= InputB;
    OutputA <= sound_out;
    s <= ctrl_buf(1) & ctrl_buf(0);

    OutputB <= eq_music;
    OutputC <= noise_music;
  lms:entity work.mlhdlc_lms_fcn_fixpt
  port map 
  (
	clk 			=> Clk,           
    reset           => Reset,
    input           => noise_buf,
    desired         => noise_buf+InputA_buf,
    filtered_signal => open,
    y              	=> filtered_music
  
  );

  noise_maker:entity work.noise_maker
  port map 
  (
	clk 			=> Clk,           
    rst           => Reset,
    sregout         => noise
  
  );    
PROCESS(clk,Reset)
	 begin
	 if Reset='1' then
	    noise_buf <= x"0000";
	 elsif rising_edge(clk) then
       	noise_buf <= noise;
	 end if;
end process;

PROCESS(clk,Reset)
	 begin
	 if Reset='1' then
	    InputA_buf <= x"0000";
	 elsif rising_edge(clk) then
       	InputA_buf <= InputA+InputA+InputA+InputA;
	 end if;
end process;

PROCESS(s,Clk) begin
  case (s) is
    when "00" =>   sound_out <= x"0000";
    when "01" =>   sound_out <= eq_music;
    when "10" =>   sound_out <= noise_music;
    when "11" =>   sound_out <= filtered_music+filtered_music;
    when others => null;
  end case;
end process;
    


end architecture;