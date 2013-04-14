/*===========================================================================*/
/**
 * @file main.cpp
 * @author Campbell Wen-Chi Guo, CCU/CSIE <campbellmath@gmail.com>
 * @date Created  : Tue 21 Aug 2012 09:12:20 PM
 * @brief
 */
/*===========================================================================*/
#include <cstdio>
#include <cstdlib>
#include "fft.h"
#include "FixedPoint.h"

#if 0
/*===========================================================================*/
#if TEST_DOUBLE
void fftTestDouble(const char *data_file_name, const char *twiddle_file_name, const char *result_file_name)
{
    double *data_r = new double[n_g];// (double *) malloc(sizeof(double)*n_g);
    double *data_i = new double[n_g];// (double *) malloc(sizeof(double)*n_g);

    double *w_r = new double[n_g/2];
    double *w_i = new double[n_g/2];

//-----------------------------------------------------------
    FILE *data_fd = fopen(data_file_name, "r");

    for (int i= 0; i < n_g; i++) {
        fscanf(data_fd, "%lf %lf\n", data_r+i, data_i+i);
    }
    fclose(data_fd);
//-----------------------------------------------------------

    FILE *w_fd = fopen(twiddle_file_name, "r");

    for (int i = 0; i < n_g/2; i++) {
        fscanf(w_fd, "%lf %lf\n", w_r+i, w_i+i);
    }
    fclose(w_fd);
//-----------------------------------------------------------

    // FFT
    // fft(n_g, data_r, data_i, w_r, w_i, 1);

    // IFFT
    fft(n_g, data_r, data_i, w_r, w_i, -1);

    FILE *result_fd = fopen(result_file_name, "w");
    for (int i = 0; i < n_g; i++) {
        fprintf(result_fd, "%.8lf %.8lf\n", data_r[i], data_i[i]);
    }
    fclose(result_fd);

    delete [] data_r;
    delete [] data_i;
    delete [] w_r;
    delete [] w_i;

    return;
}
#endif
/*===========================================================================*/
#if TEST_FIX
void fftTestFix(const char *data_file_name, const char *twiddle_file_name, const char *result_file_name)
{
    unsigned long int tmp_r, tmp_i;
    FixedPoint *data_r = new FixedPoint[n_g];
    FixedPoint *data_i = new FixedPoint[n_g];

    FixedPoint *w_r = new FixedPoint[n_g/2];
    FixedPoint *w_i = new FixedPoint[n_g/2];

//-----------------------------------------------------------
    FILE *data_fd = fopen(data_file_name, "r");

    for (int i= 0; i < n_g; i++) {
        fscanf(data_fd, "%lx %lx\n", &tmp_r, &tmp_i);
        data_r[i].setValue(tmp_r).setBitLength(data_bits_g);
        data_i[i].setValue(tmp_i).setBitLength(data_bits_g);
    }

    fclose(data_fd);
//-----------------------------------------------------------

    FILE *w_fd = fopen(twiddle_file_name, "r");

    for (int i = 0; i < n_g/2; i++) {
        fscanf(w_fd, "%lx %lx\n", &tmp_r, &tmp_i);
        w_r[i].setValue(tmp_r>>(20-twiddle_bits_g)).setBitLength(twiddle_bits_g);
        w_i[i].setValue(tmp_i>>(20-twiddle_bits_g)).setBitLength(twiddle_bits_g);
    }

    fclose(w_fd);
//-----------------------------------------------------------

    // FFT
    // fft(n_g, data_r, data_i, w_r, w_i, 1);

    // IFFT
    fft(n_g, data_r, data_i, w_r, w_i, -1);

    FILE *result_fd = fopen(result_file_name, "w");
    for (int i = 0; i < n_g; i++) {
        fprintf(result_fd, "%lu %lu\n", data_r[i].getValue(), data_i[i].getValue());
    }
    fclose(result_fd);

    delete [] data_r;
    delete [] data_i;
    delete [] w_r;
    delete [] w_i;

    return;
}
/*===========================================================================*/
#endif
/*===========================================================================*/

#define BUFFER_SIZE (1024)

int n_g=2048;
int data_bits_g=10;
int twiddle_bits_g=10;

int main(int argc, char **argv)
{
    char data_file_name[BUFFER_SIZE];
    char twiddle_file_name[BUFFER_SIZE];
    char result_file_name[BUFFER_SIZE];

    int n[3] = {2048, 4096, 8192};
//    int twiddle_bits[11] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
//    int data_bits[11] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    int twiddle_bits[3] = {16, 18, 20};
    int data_bits[3] = {16, 18, 20};

    for (int i = 0; i < 3; i++) {
        n_g = n[i];
        for (int j = 0; j < 3; j++) {
            data_bits_g = data_bits[j];
            for (int k = 0; k < 3; k++) {
                twiddle_bits_g = twiddle_bits[k];
#if TEST_FIX
                sprintf(data_file_name, "../FFT_MATLAB/data_%d_points_%dbits_hex", n_g, data_bits_g);
                // sprintf(twiddle_file_name, "../FFT_MATLAB/twiddle_%d_points_%dbits_hex", n_g, twiddle_bits_g);
                sprintf(twiddle_file_name, "../FFT_MATLAB/twiddle_%d_points_%dbits_hex", n_g, 20);
                sprintf(result_file_name, "result_%d_points_databits_%d_twiddlebits_%d_fixed", n_g, data_bits_g, twiddle_bits_g);
//                printf("n_g = %d, data bits = %d, twiddle bits = %d\n", n_g, data_bits_g, twiddle_bits_g);
                fftTestFix(data_file_name, twiddle_file_name, result_file_name);
#endif

#if TEST_DOUBLE
                sprintf(data_file_name, "../FFT_MATLAB/data_%d_points_%dbits", n_g, data_bits_g);
                // sprintf(twiddle_file_name, "../FFT_MATLAB/twiddle_%d_points_%dbits", n_g, twiddle_bits_g);
                sprintf(twiddle_file_name, "../FFT_MATLAB/twiddle_%d_points_%dbits", n_g, 20);
                sprintf(result_file_name, "result_%d_points_databits_%d_twiddlebits_%d_double", n_g, data_bits_g, twiddle_bits_g);
                printf("n_g = %d, data bits = %d, twiddle bits = %d\n", n_g, data_bits_g, twiddle_bits_g);
                fftTestDouble(data_file_name, twiddle_file_name, result_file_name);
#endif
            }
        }
    }

    return 0;
}
/*===========================================================================*/
#else
#include <iostream>
#include <vector>

/*===========================================================================*/
// ./ifft n_poiunt input_file.txt twiddle_double.txt
int main(int argc, const char *argv[])
{
    size_t n_point = atoi(argv[1]);
    double *data_r = new double[n_point];
    double *data_i = new double[n_point];
    double *w_r    = new double[n_point/2];
    double *w_i    = new double[n_point/2];

    /*----------------------------------------------------------------*/
    // read data
    FILE *fp = NULL;

    if ((fp = fopen(argv[2], "r")) != NULL) {
        for (size_t idx = 0; idx < n_point; idx++) {
            fscanf(fp, "%le %le\n", &data_r[idx], &data_i[idx]);
        }
    } else {
        fprintf(stderr, "can't open file ");
        perror(argv[2]);
        exit(-1);
    }

    fclose(fp);
    fp=NULL;
    /*----------------------------------------------------------------*/
    // read twiddle
    if ((fp = fopen(argv[3], "r")) != NULL) {
        double useless_w_r;
        double useless_w_i;
        for (size_t idx = 0; idx < n_point/2; idx++) {
            fscanf(fp, "%le %le\n", &w_r[idx], &w_i[idx]);
            for (size_t idj = 0; idj < (8192/n_point)-1; idj++) {
                fscanf(fp, "%le %le\n", &useless_w_r, &useless_w_i);
            }
        }
    } else {
        fprintf(stderr, "can't open file ");
        perror(argv[2]);
        exit(-1);
    }

    fclose(fp);
    fp=NULL;
    /*----------------------------------------------------------------*/
    
    printf("data\n");
    for (size_t idx = 0; idx < n_point; idx++) { printf("%le %le\n", data_r[idx], data_i[idx]); }
    printf("twiddle\n");
    for (size_t idx = 0; idx < n_point; idx++) { printf("%le %le\n", w_r[idx], w_i[idx]); }

    //  fft(n_point, data_r, data_i, w_r, w_i);
    ifft(n_point, data_r, data_i, w_r, w_i);

    for (size_t idx = 0; idx < n_point; idx++) { printf("%le %le\n", data_r[idx], data_i[idx]); }

    delete [] data_r;
    delete [] data_i;
    delete [] w_r;
    delete [] w_i;

    return 0;
}
/*===========================================================================*/
#endif
