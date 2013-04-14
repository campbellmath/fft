function [ output_fixed ] = double2fixed(input_double, precision_bit)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Round Mode
% ceil
%     — Round toward positive infinity.
%
% convergent
%     — Round toward nearest. Ties round to the nearest even stored integer.
%     This is the least biased rounding method provided by Fixed-Point Toolbox
%     software.
%
% fix
%     — Round toward zero.
%
% floor
%     — Round toward negative infinity.
%
% nearest
%     — Round toward nearest. Ties round toward positive infinity.
%
% round
%     — Round toward nearest. Ties round toward negative infinity for negative
%     numbers, and toward positive infinity for positive numbers.
%
roundmode = 'convergent';
output_fixed = fi(input_double,...
                   1, precision_bit, precision_bit-1,...
                   'roundmode', roundmode);
