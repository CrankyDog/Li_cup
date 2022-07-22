
cp -f D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_rtl_core_u0_m0_wo0_cm0_lutmem.hex ./

ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/dspba_library_package.vhd"                      
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/dspba_library.vhd"                              
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_math_pkg_hpfir.vhd"                   
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_lib_pkg_hpfir.vhd"                    
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_avalon_streaming_controller_hpfir.vhd"
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_avalon_streaming_sink_hpfir.vhd"      
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_avalon_streaming_source_hpfir.vhd"    
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_roundsat_hpfir.vhd"                   
ncvlog      "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/altera_avalon_sc_fifo.v"                        
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_rtl_core.vhd"                               
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_ast.vhd"                                    
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir.vhd"                                        
ncvhdl -v93 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_tb.vhd"                                     
