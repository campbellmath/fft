#!/usr/bin/env bash


function runTest ()
{
    program_name="$1"
    input_data_file="$2"
    input_twiddle_file="$3"
    output_file="$4"
    n_point="$5"
    # Generate Double Data

    ./$1 ${n_point} ${input_data_file} > result

#    cat result | head -n `perl -e "print ${n_point}+1 "`                   | tail -n ${n_point}                 > input_data_"${n_point}".txt
#    cat result | head -n `perl -e "print 2*(${n_point}+1) "`               | tail -n ${n_point}                 > input_twiddle_"${n_point}".txt

    for (( i = 0; i < `perl -e "print log(${n_point})/log(2)"`; i++ )); do
        cat result | head -n `perl -e "print 2*(${n_point}+1)+(${i}+1)*(${n_point}/2+1)"` | tail -n `perl -e "print ${n_point}/2+1"` > "${output_file}"_stage_"${i}"_"${n_point}".txt
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
}

function execIFFT ()
{
    n_point=$1
    genData
    runTest ifft   sin_double.txt xxx_twiddle.txt   sin_double_result ${n_point}
    runTest ifft  step_double.txt xxx_twiddle.txt  step_double_result ${n_point}
    runTest ifft const_double.txt xxx_twiddle.txt const_double_result ${n_point}
    runTest ifft rand_double.txt  xxx_twiddle.txt  rand_double_result ${n_point}

}

make clean_all
make

# for $n in "2 4 8 16 32 512 1024 2048 4096 8192 16384 32768 65536" ; do
for n in 4 8 ; do
    export N_POINT=${n}
    execIFFT ${N_POINT}
done
