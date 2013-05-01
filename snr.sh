#!/usr/bin/env bash

echo "rand"
echo "======================================================================"
./a.out 2048 27 15 rand_result_C_2048_points_databits_16.txt rand_double_result_matlab_2048.txt
./a.out 4096 28 15 rand_result_C_4096_points_databits_16.txt rand_double_result_matlab_4096.txt
./a.out 8192 29 15 rand_result_C_8192_points_databits_16.txt rand_double_result_matlab_8192.txt
echo "======================================================================"
./a.out 2048 29 17 rand_result_C_2048_points_databits_18.txt rand_double_result_matlab_2048.txt
./a.out 4096 30 17 rand_result_C_4096_points_databits_18.txt rand_double_result_matlab_4096.txt
./a.out 8192 31 17 rand_result_C_8192_points_databits_18.txt rand_double_result_matlab_8192.txt
echo "======================================================================"
./a.out 2048 31 19 rand_result_C_2048_points_databits_20.txt rand_double_result_matlab_2048.txt
./a.out 4096 32 19 rand_result_C_4096_points_databits_20.txt rand_double_result_matlab_4096.txt
./a.out 8192 33 19 rand_result_C_8192_points_databits_20.txt rand_double_result_matlab_8192.txt
echo "======================================================================"
echo "sin"
echo "======================================================================"
./a.out 2048 27 15 sin_result_C_2048_points_databits_16.txt sin_double_result_matlab_2048.txt
./a.out 4096 28 15 sin_result_C_4096_points_databits_16.txt sin_double_result_matlab_4096.txt
./a.out 8192 29 15 sin_result_C_8192_points_databits_16.txt sin_double_result_matlab_8192.txt
echo "======================================================================"
./a.out 2048 29 17 sin_result_C_2048_points_databits_18.txt sin_double_result_matlab_2048.txt
./a.out 4096 30 17 sin_result_C_4096_points_databits_18.txt sin_double_result_matlab_4096.txt
./a.out 8192 31 17 sin_result_C_8192_points_databits_18.txt sin_double_result_matlab_8192.txt
echo "======================================================================"
./a.out 2048 31 19 sin_result_C_2048_points_databits_20.txt sin_double_result_matlab_2048.txt
./a.out 4096 32 19 sin_result_C_4096_points_databits_20.txt sin_double_result_matlab_4096.txt
./a.out 8192 33 19 sin_result_C_8192_points_databits_20.txt sin_double_result_matlab_8192.txt
echo "======================================================================"
