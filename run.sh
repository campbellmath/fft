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
        cat result | head -n `perl -e "print ((${i}+1)*(${n_point}/2+1+2*(${n_point}+1)))"` | tail -n `perl -e "print (${n_point}/2+1+2*(${n_point}+1))"` | head -n  `perl -e "print (${n_point}/2+1)"`> "${output_file}"_"${n_point}"_points_databits_"${data_bits}"_twiddiebits_"${twiddle_bits}"_index_"${i}".txt
    done

    for (( i = 0; i < `perl -e "print log(${n_point})/log(2)"`; i++ )); do
        cat result | head -n `perl -e "print ((${i}+1)*(${n_point}/2+1+2*(${n_point}+1)))"` | tail -n `perl -e "print (${n_point}/2+1+2*(${n_point}+1))"` | tail -n  `perl -e "print (${n_point})"` > "${output_file}"_"${n_point}"_points_databits_"${data_bits}"_twiddiebits_"${twiddle_bits}"_`echo ${i}+1 | bc`_imag.txt
    done

    for (( i = 0; i < `perl -e "print log(${n_point})/log(2)"`; i++ )); do
        cat result | head -n `perl -e "print ((${i}+1)*(${n_point}/2+1+2*(${n_point}+1)))"` | tail -n `perl -e "print (${n_point}/2+1+2*(${n_point}+1))"` | tail -n  `perl -e "print 2*(${n_point}+1)"` | head -n  `perl -e "print (${n_point}+1)" ` | tail -n  `perl -e "print (${n_point})" `> "${output_file}"_"${n_point}"_points_databits_"${data_bits}"_twiddiebits_"${twiddle_bits}"_`echo ${i}+1 | bc`_real.txt
    done

    cat result | tail -n ${n_point} > "${output_file}"_result_C_"${n_point}".txt

    rm -vfr result
}

function genData ()
{
    matlab -nosplash -nodesktop -nojvm -r gen_sin_double;
    # matlab -nosplash -nodesktop -nojvm -r gen_step_double;
    # matlab -nosplash -nodesktop -nojvm -r gen_const_double;
    # matlab -nosplash -nodesktop -nojvm -r gen_rand_double;
}

function execIFFT ()
{
    n_points=$1
    data_bits=$2
    twiddle_bits=$3
    genData
    # runTest ifft   sin_double.txt twiddle_double.txt   sin_double_result ${n_point}
    # runTest ifft  step_double.txt twiddle_double.txt  step_double_result ${n_point}
    # runTest ifft const_double.txt twiddle_double.txt const_double_result ${n_point}
    # runTest ifft  rand_double.txt twiddle_double.txt  rand_double_result ${n_point}

    runTest ifft ${n_points}   sin_"${n_points}"_points_"${data_bits}"_bits_hex.txt twiddle_20_bits_hex.txt   sin ${data_bits} ${twiddle_bits}
    # runTest ifft ${n_points}  step_"${n_points}"_points_"${data_bits}"_bits_hex.txt twiddle_20_bits_hex.txt  step ${data_bits} ${twiddle_bits}
    # runTest ifft ${n_points} const_"${n_points}"_points_"${data_bits}"_bits_hex.txt twiddle_20_bits_hex.txt const ${data_bits} ${twiddle_bits}
    # runTest ifft ${n_point}  rand_"${n_points}"_points_"${data_bits}"_bits_hex.txt twiddle_20_bits_hex.txt   rand ${data_bits} ${twiddle_bits}
}

make clean_all
make
# matlab -nosplash -nodesktop -nojvm -r gen_twiddle;

# for $n in "2 4 8 16 32 512 1024 2048 4096 8192 16384 32768 65536" ; do
for n in 2048; do
    export N_POINT=${n}
    for data_bits in 20; do
        export DATA_BITS=${data_bits}
        for twiddle_bits in 20 ; do
            export TWIDDLE_BITS=${twiddle_}
            execIFFT ${N_POINT} ${DATA_BITS} ${twiddle_bits}
        done
    done
done
