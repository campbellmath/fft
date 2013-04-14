#!/usr/bin/env bash


function runTest ()
{
    program_name="$1"
    input_data_file="$2"
    input_twiddle_file="$3"
    output_file="$4"
    n_point="$5"
    # Generate Double Data

    ./$1 ${n_point} ${input_data_file}  ${input_twiddle_file} > result

#    cat result | head -n `perl -e "print ${n_point}+1 "`                   | tail -n ${n_point}                 > input_data_"${n_point}".txt
#    cat result | head -n `perl -e "print 2*(${n_point}+1) "`               | tail -n ${n_point}                 > input_twiddle_"${n_point}".txt

    for (( i = 0; i < `perl -e "print log(${n_point})/log(2)"`; i++ )); do
        cat result | head -n `perl -e "print ((${i}+1)*(${n_point}/2+1))"` | tail -n `perl -e "print ${n_point}/2+1"` > "${output_file}"_stage_"${i}"_"${n_point}".txt
    done

    cat result | tail -n ${n_point} > "${output_file}"_"${n_point}".txt

    rm result

}

function genData ()
{
    matlab -nosplash -nodesktop -nojvm -r gen_sin_double;
    matlab -nosplash -nodesktop -nojvm -r gen_step_double;
    matlab -nosplash -nodesktop -nojvm -r gen_const_double;
    matlab -nosplash -nodesktop -nojvm -r gen_rand_double;
    matlab -nosplash -nodesktop -nojvm -r gen_twiddle;
}

function execIFFT ()
{
    n_point=$1
    genData
    runTest ifft   sin_double.txt twiddle_double.txt   sin_double_result ${n_point}
    runTest ifft  step_double.txt twiddle_double.txt  step_double_result ${n_point}
    runTest ifft const_double.txt twiddle_double.txt const_double_result ${n_point}
    runTest ifft  rand_double.txt twiddle_double.txt  rand_double_result ${n_point}
}

make clean_all
make

# for $n in "2 4 8 16 32 512 1024 2048 4096 8192 16384 32768 65536" ; do
for n in 4; do
    export DATA_BITS=16
    export N_POINT=${n}
    execIFFT ${N_POINT}
    export MATLAB_DATA_NAME=sin_double_result_${n}.txt
    export C_DATA_NAME=sin_double_result_matlab_${n}.txt
    matlab -nosplash -nodesktop -nojvm -r snr_analysis;
done
