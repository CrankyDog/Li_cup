
file copy -force D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_rtl_core_u0_m0_wo0_cm0_lutmem.hex ./

vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/dspba_library_package.vhd"                      
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/dspba_library.vhd"                              
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_math_pkg_hpfir.vhd"                   
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_lib_pkg_hpfir.vhd"                    
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_avalon_streaming_controller_hpfir.vhd"
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_avalon_streaming_sink_hpfir.vhd"      
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_avalon_streaming_source_hpfir.vhd"    
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_roundsat_hpfir.vhd"                   
vlog -v2k5 "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/altera_avalon_sc_fifo.v"                        
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_rtl_core.vhd"                               
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_ast.vhd"                                    
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir.vhd"                                        
vcom       "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_tb.vhd"                                     
