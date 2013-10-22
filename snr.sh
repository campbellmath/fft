#!/usr/bin/env bash

# echo "rand"
# # echo "======================================================================"
# ./snr 2048 27 15 rand_result_C_2048_points_databits_16.txt rand_double_result_matlab_2048.txt
# ./snr 4096 28 15 rand_result_C_4096_points_databits_16.txt rand_double_result_matlab_4096.txt
# ./snr 8192 29 15 rand_result_C_8192_points_databits_16.txt rand_double_result_matlab_8192.txt
# echo "======================================================================"
# ./snr 2048 29 17 rand_result_C_2048_points_databits_18.txt rand_double_result_matlab_2048.txt
# ./snr 4096 30 17 rand_result_C_4096_points_databits_18.txt rand_double_result_matlab_4096.txt
# ./snr 8192 31 17 rand_result_C_8192_points_databits_18.txt rand_double_result_matlab_8192.txt
# echo "======================================================================"
# ./snr 2048 31 19 rand_result_C_2048_points_databits_20.txt rand_double_result_matlab_2048.txt
# ./snr 4096 32 19 rand_result_C_4096_points_databits_20.txt rand_double_result_matlab_4096.txt
# ./snr 8192 33 19 rand_result_C_8192_points_databits_20.txt rand_double_result_matlab_8192.txt
# echo "======================================================================"
# echo "sin"
# echo "======================================================================"
# ./snr 2048 27 15 sin_result_C_2048_points_databits_16.txt sin_double_result_matlab_2048.txt
# ./snr 4096 28 15 sin_result_C_4096_points_databits_16.txt sin_double_result_matlab_4096.txt
# ./snr 8192 29 15 sin_result_C_8192_points_databits_16.txt sin_double_result_matlab_8192.txt
# echo "======================================================================"
# ./snr 2048 29 17 sin_result_C_2048_points_databits_18.txt sin_double_result_matlab_2048.txt
# ./snr 4096 30 17 sin_result_C_4096_points_databits_18.txt sin_double_result_matlab_4096.txt
# ./snr 8192 31 17 sin_result_C_8192_points_databits_18.txt sin_double_result_matlab_8192.txt
# echo "======================================================================"
# ./snr 2048 31 19 sin_result_C_2048_points_databits_20.txt sin_double_result_matlab_2048.txt
# ./snr 4096 32 19 sin_result_C_4096_points_databits_20.txt sin_double_result_matlab_4096.txt
# ./snr 8192 33 19 sin_result_C_8192_points_databits_20.txt sin_double_result_matlab_8192.txt
# echo "======================================================================"

echo sin
echo "2048 word_length truncate_bits     snr"

for data_bits in `seq 16 20`; do
    echo ""
    for truncate_bits in `seq 0 11` ; do
        ./snr 2048 `expr $data_bits + 11` `expr $data_bits - 1` "sin_result_C_2048_points_databits_${data_bits}.txt" sin_double_result_matlab_2048.txt  ${truncate_bits}
    done
done

echo sin
echo "4096 word_length truncate_bits     snr"
for data_bits in `seq 16 20`; do
    echo ""
    for truncate_bits in `seq 0 12` ; do
        ./snr 4096 `expr $data_bits + 12` `expr $data_bits - 1` "sin_result_C_4096_points_databits_${data_bits}.txt" sin_double_result_matlab_4096.txt  ${truncate_bits}
    done
done

echo sin
echo "8192 word_length truncate_bits     snr"
for data_bits in `seq 16 20`; do
    echo ""
    for truncate_bits in `seq 0 13` ; do
        ./snr 8192 `expr $data_bits + 13` `expr $data_bits - 1` "sin_result_C_8192_points_databits_${data_bits}.txt" sin_double_result_matlab_8192.txt  ${truncate_bits}
    done
done
