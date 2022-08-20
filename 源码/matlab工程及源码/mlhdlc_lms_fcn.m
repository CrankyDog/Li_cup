function [filtered_signal, y] = mlhdlc_lms_fcn(input, ...
                                        desired)

persistent filter_coeff;
if isempty(filter_coeff)
    filter_coeff = zeros(1, 40);
end

delayed_signal = mtapped_delay_fcn(input);


weight_applied = delayed_signal .* filter_coeff;

filtered_signal = mtreesum_fcn(weight_applied);

td = desired;
tf = filtered_signal;
esig = td - tf;
y = esig;

updated_weight = update_weight_fcn( esig, delayed_signal, ...
                                   filter_coeff);

filter_coeff = updated_weight;


function y = mtreesum_fcn(u)


level1 = vsum(u);
level2 = vsum(level1);
level3 = vsum(level2);
level4 = vsum(level3);
level5 = vsum(level4);
level6 = vsum(level5);
y = level6;

function output = vsum(input)

coder.inline('always');

vt = input(1:2:end);
    
for i = int32(1:numel(input)/2)
    k = int32(i*2);
    vt(i) = vt(i) + input(k);
end

output = vt;

function tap_delay = mtapped_delay_fcn(input)


persistent u_d;
if isempty(u_d)
    u_d = zeros(1,40);
end


u_d = [u_d(2:40), input];

tap_delay = u_d;

function weights = update_weight_fcn(err_sig, ... 
            delayed_signal, filter_coeff)

step_sig = 0.000000001 .* err_sig;
correction_factor = delayed_signal .* step_sig;
updated_weight = correction_factor + filter_coeff;

  
weights = updated_weight;
