%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File Name   : mytwiddle.m
% Author      : Wen-Chi Guo
% E-mail      : campbellmath@gmail.com
% Created     : Sun 14 Apr 2013 07:49:39 PM CST
% Modified    : Sun 14 Apr 2013 09:11:29 PM CST
% Description : Description
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [twiddle, twiddle_fixed] = mytwiddle(transform_lenghth, twiddle_precision)

idx = (0:transform_lenghth/2-1);

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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
twiddle = exp(-1i*2*pi*idx/transform_lenghth);
% twiddle_fixed = fi(twiddle,...
%                    1, twiddle_precision, twiddle_precision-1,...
%                    'roundmode', roundmode);
twiddle_fixed = double2fixed(twiddle, twiddle_precision);
