#!/usr/bin/env bash

function runTest ()
{
    program_name="$1"
    n_point="$2"
    input_data_file="$3"
    input_twiddle_file="$4"
    output_file="$5"
    data_bits="$6"
    twiddle_bits="$7"
    # Generate Double Data

##  ./ifft n_poiunt input_file.txt     twiddle_double.txt    data_bit     twiddle_bit
    echo "./$1 ${n_point} ${input_data_file} ${input_twiddle_file} ${data_bits} ${twiddle_bits} > result"
    ./$1 ${n_point} ${input_data_file} ${input_twiddle_file} ${data_bits} ${twiddle_bits} > result

#    cat result | head -n `perl -e "print ${n_point}+1 "`                   | tail -n ${n_point}                 > input_data_"${n_point}".txt
#    cat result | head -n `perl -e "print 2*(${n_point}+1) "`               | tail -n ${n_point}                 > input_twiddle_"${n_point}".txt

    for (( i = 0; i < `perl -e "print log(${n_point})/log(2)"`; i++ )); do
        cat result | head -n `perl -e "print ((${i}+1)*(${n_point}/2+1+2*(${n_point}+1))+2*(${n_point}+1))"` | tail -n `perl -e "print (${n_point}/2+1+2*(${n_point}+1))"` | head -n  `perl -e "print (${n_point}/2+1)"`> "${output_file}"_"${n_point}"_points_databits_"${data_bits}"_twiddlebits_"${twiddle_bits}"_index_"${i}".txt
    done

    for (( i = 0; i < `perl -e "print log(${n_point})/log(2)"`; i++ )); do
        cat result | head -n `perl -e "print ((${i}+1)*(${n_point}/2+1+2*(${n_point}+1))+2*(${n_point}+1))"` | tail -n `perl -e "print (${n_point}/2+1+2*(${n_point}+1))"` | tail -n  `perl -e "print (${n_point})"` > "${output_file}"_"${n_point}"_points_databits_"${data_bits}"_twiddlebits_"${twiddle_bits}"_`echo ${i}+1 | bc`_imag.txt
    done

    for (( i = 0; i < `perl -e "print log(${n_point})/log(2)"`; i++ )); do
        cat result | head -n `perl -e "print ((${i}+1)*(${n_point}/2+1+2*(${n_point}+1))+2*(${n_point}+1))"` | tail -n `perl -e "print (${n_point}/2+1+2*(${n_point}+1))"` | tail -n  `perl -e "print 2*(${n_point}+1)"` | head -n  `perl -e "print (${n_point}+1)" ` | tail -n  `perl -e "print (${n_point})" `> "${output_file}"_"${n_point}"_points_databits_"${data_bits}"_twiddlebits_"${twiddle_bits}"_`echo ${i}+1 | bc`_real.txt
    done

    cat result | head -n `perl -e "print 2*(${n_point}+1)"` | head -n `perl -e "print (${n_point}+1)"` | tail -n `perl -e "print ${n_point}"` > "${output_file}"_"${n_point}"_points_databits_"${data_bits}"_twiddlebits_"${twiddle_bits}"_0_real.txt
    cat result | head -n `perl -e "print 2*(${n_point}+1)"` | tail -n `perl -e "print (${n_point}+1)"` | tail -n `perl -e "print ${n_point}"` > "${output_file}"_"${n_point}"_points_databits_"${data_bits}"_twiddlebits_"${twiddle_bits}"_0_imag.txt

    cat result | tail -n ${n_point} > "${output_file}"_result_C_"${n_point}"_points_databits_"${data_bits}".txt

    rm -vfr result
}

function genData ()
{
    matlab -nosplash -nodesktop -nojvm -r gen_sin_double;
    # matlab -nosplash -nodesktop -nojvm -r gen_step_double;
    # matlab -nosplash -nodesktop -nojvm -r gen_const_double;
    matlab -nosplash -nodesktop -nojvm -r gen_rand_double;
}

function execIFFT ()
{
    n_points=$1
    data_bits=$2
    twiddle_bits=$3
    genData
    # ./ifft ${n_points}   sin_double.txt twiddle_double.txt | tail -n ${n_points} >   sin_double_result_C_"${n_points}".txt
    # ./ifft ${n_points}  step_double.txt twiddle_double.txt | tail -n ${n_points} >  step_double_result_C_"${n_points}".txt
    # ./ifft ${n_points} const_double.txt twiddle_double.txt | tail -n ${n_points} > const_double_result_C_"${n_points}".txt
    # ./ifft ${n_points}  rand_double.txt twiddle_double.txt | tail -n ${n_points} >  rand_double_result_C_"${n_points}".txt

    runTest ifft ${n_points}   sin_"${n_points}"_points_"${data_bits}"_bits_hex.txt twiddle_20_bits_hex.txt   sin ${data_bits} ${twiddle_bits}
    # runTest ifft ${n_points}  step_"${n_points}"_points_"${data_bits}"_bits_hex.txt twiddle_20_bits_hex.txt  step ${data_bits} ${twiddle_bits}
    # runTest ifft ${n_points} const_"${n_points}"_points_"${data_bits}"_bits_hex.txt twiddle_20_bits_hex.txt const ${data_bits} ${twiddle_bits}
    runTest ifft ${n_points}  rand_"${n_points}"_points_"${data_bits}"_bits_hex.txt twiddle_20_bits_hex.txt  rand ${data_bits} ${twiddle_bits}
}

make clean_all
make
matlab -nosplash -nodesktop -nojvm -r gen_twiddle;

# for $n in "2 4 8 16 32 512 1024 2048 4096 8192 16384 32768 65536" ; do
for n in 2048 4096 8192; do
    export N_POINT=${n}
    for data_bits in 16 18 20; do
        export DATA_BITS=${data_bits}
        for twiddle_bits in 20; do
            export TWIDDLE_BITS=${twiddle_}
            execIFFT ${N_POINT} ${DATA_BITS} ${twiddle_bits}
        done
    done
done

# sh case001.sh
# sh case004.sh
# sh case007.sh
# sh case010.sh
# sh case013.sh
# sh case016.sh
# sh case019.sh
# sh case022.sh
# sh case025.sh
# 
# sh case101.sh
# sh case104.sh
# sh case107.sh
# sh case110.sh
# sh case113.sh
# sh case116.sh
# sh case119.sh
# sh case122.sh
# sh case125.sh
