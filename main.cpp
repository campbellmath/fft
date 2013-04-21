#include <cstdio>
#include <cstdlib>
#include "fft.h"
#include "FixedPoint.h"

#define DOUBLE (0)

#include <iostream>
#include <vector>

/*===========================================================================*/
#if 1
// ./ifft n_poiunt input_file.txt twiddle_double.txt
int main(int argc, const char *argv[])
{
    size_t n_point = atoi(argv[1]);
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

    if ((fp = fopen(argv[2], "r")) != NULL) {
        for (size_t idx = 0; idx < n_point; idx++) {
#if DOUBLE
            fscanf(fp, "%le %le\n", &data_r[idx], &data_i[idx]);
#else
            unsigned long int r = 0L;
            unsigned long int i = 0L;
            fscanf(fp, "%lX %lX\n", &r, &i);
            data_r[idx].setBitLength(16).setValue(r);
            data_i[idx].setBitLength(16).setValue(i);
            // std::cout<<data_r[idx]<<" "<<data_i[idx]<<std::endl;
#endif
        }
    } else {
        fprintf(stderr, "can't open file ");
        perror(argv[2]);
        exit(-1);
    }

    fclose(fp);
    fp=NULL;
    // read data
    /*----------------------------------------------------------------*/
    // read twiddle
    if ((fp = fopen(argv[3], "r")) != NULL) {
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
            w_r[idx].setBitLength(16).setValue(r>>(20-16));
            w_i[idx].setBitLength(16).setValue(i>>(20-16));
            for (size_t idj = 0; idj < (8192/n_point)-1; idj++) {
                fscanf(fp, "%lx %lx\n", &useless_w_r, &useless_w_i);
            }
        }
#endif
    } else {
        fprintf(stderr, "can't open file ");
        perror(argv[2]);
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
#else

int main(int argc, const char *argv[])
{
#if 0
    FixedPoint a_r(0x07889,17); // 6.279052e-02 + j*0.000000e+00
    FixedPoint a_i(0x00000,17);
    FixedPoint b_r(0x1e8da,17); // 1.253332e-01 + j*0.000000e+00
    FixedPoint b_i(0x00000,17);
    FixedPoint w_r(0x7fff,16);// 1.000000e+00 + j*0.000000e+00
    FixedPoint w_i(0x0000,16);

    std::cout<<"w = "<<w_r<<" + "<<w_i<<std::endl;
    std::cout<<"a = "<<a_r<<" + "<<a_i<<std::endl;
    std::cout<<"b = "<<b_r<<" + "<<b_i<<std::endl;

    radix2Butterfly(
            &a_r, &a_i,
            &b_r, &b_i,
            &w_r, &w_i);
    std::cout<<"A = "<<a_r<<" + "<<a_i<<"    "<<a_r.getBitLength()<<std::endl;
    std::cout<<"B = "<<b_r<<" + "<<b_i<<"    "<<b_r.getBitLength()<<std::endl;
#else
    FixedPoint x(0xe, 4);
    FixedPoint y(0xd, 4);


    FixedPoint z=x*y;
    std::cout<<"x  "<<x<<std::endl;
    std::cout<<"y  "<<y<<std::endl;
    std::cout<<"z  "<<z<<std::endl;

    printf("0x%x\n", signedExtend(0x2f, 3, 5));
#endif

    return 0;
}

#endif
