#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

/*===========================================================================*/
struct complex_t {
    double real;
    double imag;
    complex_t(double real_ = 0.0, double imag_ = 0.0) :
        real(real_), imag(imag_) {}
};
/*===========================================================================*/
#define SQUARE(x) ((x)*(x))
double snr(size_t n, const complex_t *x, const complex_t *y)
{
    double _snr = 0.0;
    double sig  = 0.0;
    double noi  = 0.0;

    complex_t *err = new complex_t[n];

    for (size_t idx = 0; idx < n; idx++) {
        err[idx].real = std::abs(x[idx].real - y[idx].real);
        err[idx].imag = std::abs(x[idx].imag - y[idx].imag);
    }

    for (size_t idx = 0; idx < n; idx++) {
        sig += SQUARE(  x[idx].real)+SQUARE(  x[idx].imag);
        noi += SQUARE(err[idx].real)+SQUARE(err[idx].imag);

    }

    _snr = 10*log10(sig/noi);

    delete [] err;

    return _snr;
}
#undef SQUARE
/*===========================================================================*/
typedef unsigned long int UINT64;
static UINT64 clearUpperBits(UINT64 value, UINT64 n)
{
    UINT64 mask = 0;
    for (UINT64 idx = 0; idx < n ; idx++) {
        mask |= (0x1lu << idx);
    }

    return (value&mask);
}
/*===========================================================================*/
double fix2double(UINT64 x, size_t word_length, size_t fraction_length)
{
    double value = 0.0;
    double fraction = pow(1.0/2.0, static_cast<double>(fraction_length));
    int sign = ((x>>(word_length-1))&0x1);

    if (sign==1) {
        x=~x+1;
    }

    x = clearUpperBits(x, word_length);

    value = static_cast<double>(x)*fraction;

    if (sign==1) {
        value=-value;
    }

    return value;
}
/*===========================================================================*/
int main(int argc, const char *argv[])
{
#if 0
    FILE *fp = NULL;
    const size_t        n = atoi(argv[1]);
    const char *file_name =      argv[2];

    complex_t *x = new complex_t[n];
    complex_t *y = new complex_t[n];

    srand(time(NULL));

    if ((fp = fopen(file_name, "r")) != NULL) {
        for (size_t idx = 0; idx < n; idx++) {
            fscanf(fp, "%le %le\n", &(x[idx].real), &(x[idx].imag));
        }
    } else {
        fprintf(stderr, "can't open file ");
        perror(file_name);
        exit(-1);
    }

    fclose(fp);

    for (int i = 0; i < 30; i++) {
        for (size_t idx = 0; idx < 8192; idx++) {
            y[idx].real =
                x[idx].real+static_cast<double>(rand()%2)/pow(2.0, static_cast<double>(i));
            y[idx].imag =
                x[idx].imag+static_cast<double>(rand()%2)/pow(2.0, static_cast<double>(i));
        }
        printf("snr = %g\n", snr(n, x, y));
    }

    UINT64 x = 0;

    for (int i = 16; i >= -16; i--) {
        printf("%g\n", fix2double((x+i)&0x1f, 5, 1));
    }
#else
    const size_t               n = atoi(argv[1]);
    const size_t     word_length = atoi(argv[2]);
    const size_t fraction_length = atoi(argv[3]);
    const char      *c_filename  =      argv[4];
    const char *matlab_filename  =      argv[5];
    const size_t truncate_bits   = atoi(argv[6]);
    complex_t         *c_result  = new complex_t[n];
    complex_t    *matlab_result  = new complex_t[n];

    FILE *fp = NULL;
    if ((fp = fopen(matlab_filename, "r")) != NULL) {
        for (size_t idx = 0; idx < n; idx++) {
            fscanf(fp, "%le %le\n", &(matlab_result[idx].real), &(matlab_result[idx].imag));
        }
    } else {
        fprintf(stderr, "can't open file ");
        perror(matlab_filename);
        exit(-1);
    }
    fclose(fp);

    fp = NULL;

    if ((fp = fopen(c_filename, "r")) != NULL) {
        for (size_t idx = 0; idx < n; idx++) {
            size_t nouse[2] = {0, 0};
            UINT64 real = 0x0;
            UINT64 imag = 0x0;
            fscanf(fp, "%lx (%lu) %lx (%lu)\n", &real, &nouse[0], &imag, &nouse[1]);
            real &= ((-1)>>truncate_bits)<<truncate_bits;
            imag &= ((-1)>>truncate_bits)<<truncate_bits;
            c_result[idx].real = fix2double(real, word_length, fraction_length);
            c_result[idx].imag = fix2double(imag, word_length, fraction_length);
            // fprintf(stdout, "%d, %.16f %.16f |  %.16f %.16f|  %.16f %.16f\n", idx,
            // std::abs(std::abs(c_result[idx].real)-std::abs(matlab_result[idx].real)),
            // std::abs(std::abs(c_result[idx].imag)-std::abs(matlab_result[idx].imag)),
            // std::abs(c_result[idx].real),
            // std::abs(c_result[idx].imag),
            // std::abs(matlab_result[idx].real),
            // std::abs(matlab_result[idx].imag));

        }
    } else {
        fprintf(stderr, "can't open file ");
        perror(matlab_filename);
        exit(-1);
    }
    fclose(fp);
    fp = NULL;

    printf("%11lu %13lu     %.4f(%2lu) \n", word_length, truncate_bits, snr(n, c_result, matlab_result), word_length-truncate_bits);
#endif
//    UINT64 x = 0;
//
//    printf("%g\n", fix2double(0xfef235d, 30, 17));

    return 0;
}
/*===========================================================================*/
