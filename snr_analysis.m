n_point = str2num(getenv('N_POINT'));
matlab_data_name = getenv('MATLAB_DATA_NAME');
c_data_name = getenv('C_DATA_NAME');

matlab_data_fd = fopen(matlab_data_name);
matlab_data = fscanf(matlab_data_fd, '%e %e', [2 inf]);
x = matlab_data(1,:)+1i*matlab_data(2,:);
fclose(matlab_data_fd);

c_data_fd = fopen(c_data_name);
c_data = fscanf(c_data_fd, '%e %e', [2 inf]);
y = c_data(1,:)+1i*c_data(2,:);
fclose(c_data_fd);

snr_fd = fopen('snr_result.txt', 'a');
data_snr = SNR(x, y)
fprintf(snr_fd, '%d: %f\n', n_point, data_snr);
fclose(snr_fd);

quit
