n_point       = 8192;
precision_bit = 20;

[twiddle, twiddle_fixed] = mytwiddle(n_point, precision_bit);

fp = fopen('twiddle_double.txt', 'w');
for i = 1:n_point/2
    fprintf(fp, '%e %e\n', real(twiddle(i)), imag(twiddle(i)));
end
fclose(fp);

fp = fopen('twiddle_fixed_20bits.txt', 'w');
for i = 1:n_point/2
    fprintf(fp, '%s %s\n', hex(real(twiddle_fixed(i))), hex(imag(twiddle_fixed(i))));
end
fclose(fp);
quit
