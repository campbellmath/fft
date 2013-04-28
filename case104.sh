#!/usr/bin/env bash

case="case104"
n_points="2048"
databits="18"
twiddlebits="16"

rm -vfr ${case}
mkdir ${case}

cat sin_"${n_points}"_points_"${databits}"_bits_hex.txt | awk '{print $1}' > 00_real.txt
cat sin_"${n_points}"_points_"${databits}"_bits_hex.txt | awk '{print $2}' > 00_imag.txt

mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_1_real.txt  01_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_2_real.txt  02_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_3_real.txt  03_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_4_real.txt  04_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_5_real.txt  05_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_6_real.txt  06_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_7_real.txt  07_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_8_real.txt  08_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_9_real.txt  09_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_10_real.txt 10_real.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_11_real.txt 11_real.txt

mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_1_imag.txt  01_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_2_imag.txt  02_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_3_imag.txt  03_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_4_imag.txt  04_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_5_imag.txt  05_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_6_imag.txt  06_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_7_imag.txt  07_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_8_imag.txt  08_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_9_imag.txt  09_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_10_imag.txt 10_imag.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_11_imag.txt 11_imag.txt

mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_0.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_00.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_1.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_01.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_2.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_02.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_3.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_03.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_4.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_04.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_5.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_05.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_6.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_06.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_7.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_07.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_8.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_08.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_9.txt  "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_09.txt
mv sin_"${n_points}"_points_databits_"${databits}"_twiddlebits_"${twiddlebits}"_index_10.txt "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_10.txt

for stage in `seq -f "%02g" 0 11` ; do
    mv "${stage}"_real.txt ${case}
    mv "${stage}"_imag.txt ${case}
done
# 
for stage in `seq -f "%02g" 0 10` ; do
    mv "${n_points}"_points_databits_"${databits}"_twiddiebits_"${twiddlebits}"_index_"${stage}".txt ${case}
done
