
cp -f D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_rtl_core_u0_m0_wo0_cm0_lutmem.hex ./

vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/dspba_library_package.vhd"                      
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/dspba_library.vhd"                              
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_math_pkg_hpfir.vhd"                   
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_lib_pkg_hpfir.vhd"                    
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_avalon_streaming_controller_hpfir.vhd"
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_avalon_streaming_sink_hpfir.vhd"      
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_avalon_streaming_source_hpfir.vhd"    
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/auk_dspip_roundsat_hpfir.vhd"                   
vlogan +v2k  "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/altera_avalon_sc_fifo.v"                        
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_rtl_core.vhd"                               
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_ast.vhd"                                    
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir.vhd"                                        
vhdlan -xlrm "D:/qq/fpga_learning/li/fpga/6.0++/au_filter/prj/ipcore/fir/fir_sim/fir_tb.vhd"                                     
