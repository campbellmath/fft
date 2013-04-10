n_point = str2num(getenv('N_POINT'));
fp = fopen('sin_double.txt', 'w');
t=1:1:n_point;
t=t/10000;
data = sin(2*pi*100*t);
fprintf(fp, '%e 0.000000e+00\n', data);
fclose(fp);

name = sprintf('sin_double_result_matlab_%d.txt', n_point);
fp = fopen(name, 'w');
data=ifft(data)*n_point;

for i=1:1:n_point
    fprintf(fp, '%e %e\n', real(data(i)), imag(data(i)));
end
fclose(fp);

quit
