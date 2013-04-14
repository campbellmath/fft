n_point = str2num(getenv('N_POINT'));
fp = fopen('rand_double.txt', 'w');
t=rand(1, n_point)*(2^16);
fprintf(fp, '%e 0.000000e+00\n', t);
fclose(fp);

name = sprintf('rand_double_result_matlab_%d.txt', n_point);
fp = fopen(name, 'w');
data=ifft(t)*n_point;
for i=1:1:n_point
    fprintf(fp, '%e %e\n', real(data(i)), imag(data(i)));
end
fclose(fp);

quit
