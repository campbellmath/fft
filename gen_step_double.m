n_point   = str2num(getenv('N_POINT'));
data_bits = str2num(getenv('DATA_BITS'));
fp = fopen('step_double.txt', 'w');
t=1:1:n_point;
data=t/(n_point);
fprintf(fp, '%e 0.000000e+00\n', data);
fclose(fp);

data_fixed = double2fixed(data, data_bits);
name = sprintf('step_fixed_%d_bits.txt', data_bits);
fp = fopen(name, 'w');
for i = 1:n_point
    fprintf(fp, '%s %s\n', hex(real(data_fixed(i))), hex(imag(data_fixed(i))));
end
fclose(fp);

name = sprintf('step_double_result_matlab_%d.txt', n_point);
fp = fopen(name, 'w');
data=ifft(t)*n_point;
for i=1:1:n_point
    fprintf(fp, '%e %e\n', real(data(i)), imag(data(i)));
end
fclose(fp);

quit
