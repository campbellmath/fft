#include <cstdio>
#include <cstdlib>
#include "fft.h"
#include "FixedPoint.h"

#include <iostream>
#include <vector>

#define IFFT               (0)
#define BUFFTERFLY_DOUBLE  (1)
#define BUFFTERFLY_FIX     (2)

#if (DOUBLE==1)
#define TEST BUFFTERFLY_DOUBLE
#else
// #define TEST BUFFTERFLY_FIX
#define TEST IFFT
#endif
/*===========================================================================*/
#if (TEST==IFFT)
// ./ifft n_poiunt input_file.txt twiddle_file.txt data_bit twiddle_bit
int main(int argc, const char *argv[])
{
    size_t n_point           = atoi(argv[1]);
    const char *input_file   =      argv[2] ;
    const char *twiddle_file =      argv[3] ;
    int data_bit             = atoi(argv[4]);
    int twiddle_bit          = atoi(argv[5]);
#if DOUBLE
    double *data_r = new double[n_point];
    double *data_i = new double[n_point];
    double *w_r    = new double[n_point/2];
    double *w_i    = new double[n_point/2];
#else


    FixedPoint *data_r = new FixedPoint[n_point];
    FixedPoint *data_i = new FixedPoint[n_point];
    FixedPoint *w_r    = new FixedPoint[n_point/2];
    FixedPoint *w_i    = new FixedPoint[n_point/2];
#endif

    /*----------------------------------------------------------------*/
    // read data
    FILE *fp = NULL;

    if ((fp = fopen(input_file, "r")) != NULL) {
        for (size_t idx = 0; idx < n_point; idx++) {
#if DOUBLE
            fscanf(fp, "%le %le\n", &data_r[idx], &data_i[idx]);
#else
            unsigned long int r = 0L;
            unsigned long int i = 0L;
            fscanf(fp, "%lX %lX\n", &r, &i);
            data_r[idx].setBitLength(data_bit).setValue(r);
            data_i[idx].setBitLength(data_bit).setValue(i);
            // std::cout<<data_r[idx]<<" "<<data_i[idx]<<std::endl;
#endif
        }
    } else {
        fprintf(stderr, "can't open file ");
        perror(input_file);
        exit(-1);
    }

    fclose(fp);
    fp=NULL;
    // read data
    /*----------------------------------------------------------------*/
    // read twiddle
    if ((fp = fopen(twiddle_file, "r")) != NULL) {
#if DOUBLE
        double useless_w_r;
        double useless_w_i;
        for (size_t idx = 0; idx < n_point/2; idx++) {
            fscanf(fp, "%le %le\n", &w_r[idx], &w_i[idx]);
            for (size_t idj = 0; idj < (8192/n_point)-1; idj++) {
                fscanf(fp, "%le %le\n", &useless_w_r, &useless_w_i);
            }
        }
#else
        unsigned long int useless_w_r;
        unsigned long int useless_w_i;
        for (size_t idx = 0; idx < n_point/2; idx++) {
            unsigned long int r;
            unsigned long int i;
            fscanf(fp, "%lx %lx\n", &r, &i);
            w_r[idx].setBitLength(twiddle_bit).setValue(r>>(20-twiddle_bit));
            w_i[idx].setBitLength(twiddle_bit).setValue(i>>(20-twiddle_bit));
            for (size_t idj = 0; idj < (8192/n_point)-1; idj++) {
                fscanf(fp, "%lx %lx\n", &useless_w_r, &useless_w_i);
            }
        }
#endif
    } else {
        fprintf(stderr, "can't open file ");
        perror(twiddle_file);
        exit(-1);
    }

    fclose(fp);
    fp=NULL;
    /*----------------------------------------------------------------*/

    // fft(n_point, data_r, data_i, w_r, w_i);
    ifft(n_point, data_r, data_i, w_r, w_i);

#if DOUBLE
    for (size_t idx = 0; idx < n_point; idx++) { printf("%e %e\n", data_r[idx], data_i[idx]); }
#else
    for (size_t idx = 0; idx < n_point; idx++) { printf("%6lx (%d) %6lx (%d)\n", data_r[idx].getValue(), data_r[idx].getBitLength(), data_i[idx].getValue(), data_i[idx].getBitLength()); }
#endif

    delete [] data_r;
    delete [] data_i;
    delete [] w_r;
    delete [] w_i;

    return 0;
}
/*===========================================================================*/
#elif (TEST==BUFFTERFLY_DOUBLE)

int main(int argc, const char *argv[])
{
    double a_r = 30875.0/32768.0; // 30857.0/2**15
    double a_i =     0.0;
    double b_r = -5926.0/32768.0; // -5926.0/2**15
    double b_i =     0.0;
    double w_r =     1.0;
    double w_i =     0.0;

    std::cout<<"w = "<<w_r<<" + "<<w_i<<std::endl;
    std::cout<<"a = "<<a_r<<" + "<<a_i<<std::endl;
    std::cout<<"b = "<<b_r<<" + "<<b_i<<std::endl;

    radix2Butterfly(
            &a_r, &a_i,
            &b_r, &b_i,
            &w_r, &w_i);
    std::cout<<"A = "<<a_r<<" + "<<a_i<<"    "<<std::endl;
    std::cout<<"B = "<<b_r<<" + "<<b_i<<"    "<<std::endl;

    // w = 1 + 0
    // a = 0.94223 + 0
    // b = -0.180847 + 0
    // A = 0.761383 + 0
    // B = 1.12308 + 0

    return 0;
}
#elif (TEST==BUFFTERFLY_FIX)
int main(int argc, const char *argv[])
{
    FixedPoint a_r(0x07889, 17);
    FixedPoint a_i(0x00000, 17);
    FixedPoint b_r(0x1e8da, 17);
    FixedPoint b_i(0x00000, 17);
    FixedPoint w_r( 0x7fff, 16);
    FixedPoint w_i( 0x0000, 16);
///   FixedPoint a_r(0x0040, 16);
///   FixedPoint a_i(0x0000, 16);
///   FixedPoint b_r(0x0040, 16);
///   FixedPoint b_i(0x0000, 16);
///   FixedPoint w_r(0x7fff, 16);
///   FixedPoint w_i(0x0000, 16);

    std::cout<<"w = "<<w_r<<" + "<<w_i<<std::endl;
    std::cout<<"a = "<<a_r<<" + "<<a_i<<std::endl;
    std::cout<<"b = "<<b_r<<" + "<<b_i<<std::endl;

    radix2Butterfly(
            &a_r, &a_i,
            &b_r, &b_i,
            &w_r, &w_i);
    std::cout<<"A = "<<a_r<<" + "<<a_i<<"    "<<a_r.getBitLength()<<std::endl;
    std::cout<<"B = "<<b_r<<" + "<<b_i<<"    "<<b_r.getBitLength()<<std::endl;

    return 0;
}
#else
int main(int argc, const char *argv[])
{
    FixedPoint x(0xe, 4);
    FixedPoint y(0xd, 4);

    FixedPoint z=x*y;
    std::cout<<"x  "<<x<<std::endl;
    std::cout<<"y  "<<y<<std::endl;
    std::cout<<"z  "<<z<<std::endl;

    printf("0x%x\n", signedExtend(0x2f, 3, 5));
    return 0;
}
#endif
