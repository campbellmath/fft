function snr = SNR(x,y)
    tmp_N=size(x);
    N=tmp_N(1);
    errors=x-y;
    Sig=sum(abs(x).^2)/N;
    Noise=sum(abs(errors).^2)/N;
    snr=10*log10(Sig/Noise);
