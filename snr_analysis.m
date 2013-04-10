
matlab_data_fd = fopen(matlab_data_name);
matlab_data = fscanf(matlab_data_fd, '%lg %lg', [2 inf]);
x = matlab_data(1,:)+1i*matlab_data(2,:);
fclose(matlab_data_fd);
x = ifft(x)*transform_lenghth(i);

c_data_fd = fopen(c_data_name);
c_data = fscanf(c_data_fd, '%lg %lg', [2 inf]);
y = c_data(1,:)+1i*c_data(2,:);
fclose(c_data_fd);

data_snr = SNR(x, y);
