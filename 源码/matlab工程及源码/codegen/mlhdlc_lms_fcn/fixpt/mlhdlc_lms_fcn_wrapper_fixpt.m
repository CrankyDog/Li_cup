%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                          %
%          Generated by MATLAB 9.12 and Fixed-Point Designer 7.4           %
%                                                                          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [filtered_signal,y] = mlhdlc_lms_fcn_wrapper_fixpt(input,desired)
    fm = get_fimath();
    input_in = fi( input, 1, 16, 0, fm );
    desired_in = fi( desired, 1, 16, 0, fm );
    [filtered_signal_out,y_out] = mlhdlc_lms_fcn_fixpt( input_in, desired_in );
    filtered_signal = double( filtered_signal_out );
    y = double( y_out );
end

function fm = get_fimath()
	fm = fimath('RoundingMethod', 'Floor',...
	     'OverflowAction', 'Wrap',...
	     'ProductMode','FullPrecision',...
	     'SumMode','FullPrecision');
end
