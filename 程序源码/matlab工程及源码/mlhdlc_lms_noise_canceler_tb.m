
clear('mlhdlc_lms_fcn');

fss=20000;
[data,fs]=audioread('sp01.wav');
% redata=resample(data,fss,fs);
% left=redata(1:65536,1);
to1= round(data*2^14);
hfilt2 = dsp.FIRFilter(...
        'Numerator', fir1(10, [.5, .75]));
rng('default'); % always default to known state  
% % % x = randn(1000,1);                              % Noise
x1 = round(randn(length(to1),1)*2^14/8);  
% % % d = step(hfilt2, x) + sin(0:.05:49.95)';         % Noise + Signal
d = to1+x1;
% [y, e] = mlhdlc_lms_fcn(x, d,  stepSize, reset_weights);
hSrc = dsp.SignalSource(x1);
hDesiredSrc = dsp.SignalSource(d);

hOut = dsp.SignalSink;
hErr = dsp.SignalSink;

while (~isDone(hSrc))
    [y, e] = mlhdlc_lms_fcn(step(hSrc), step(hDesiredSrc));
    step(hOut, y);
    step(hErr, e);
end

figure('Name', [mfilename, '_signal_plot']);
subplot(3,1,1),plot(to1), title('Signal');
subplot(3,1,2), plot(d), title('Noise + Signal');
subplot(3,1,3),plot(hErr.Buffer), title('Signal');
 % test = hErr.Buffer;
 % test2 = hOut.Buffer;
N= 16;
 fid=fopen('D:\qq\li\hdl_coder\lms2.0\quartus\prj\simulation\modelsim\noise.txt','w');    %把数据写入sin_data.txt文件中，如果没有就创建该文件  
 for k=1:length(x1)
     B_s=dec2bin(x1(k)+((x1(k))<0)*2^N,N);
     for j=1:N
         if B_s(j)=='1'
             tb=1;
         else
             tb=0;
         end
         fprintf(fid,'%d',tb);
     end
     fprintf(fid,'\r\n');
 end
 
 % fprintf(fid,';');
 fclose(fid);
 fid=fopen('D:\qq\li\hdl_coder\lms2.0\quartus\prj\simulation\modelsim\music.txt','w');    %把数据写入sin_data.txt文件中，如果没有就创建该文件  
 for k=1:length(to1)
     B_s=dec2bin(to1(k)+((to1(k))<0)*2^N,N);
     for j=1:N
         if B_s(j)=='1'
             tb=1;
         else
             tb=0;
         end
         fprintf(fid,'%d',tb);
     end
     fprintf(fid,'\r\n');
 end
 
 % fprintf(fid,';');
 fclose(fid);
%  noise=(randn(65536,1));
%  sound(n,44100);