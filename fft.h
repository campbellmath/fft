#ifndef __FFT_H__
#define __FFT_H__

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <typeinfo>
#include <algorithm>
#include <iostream>

#define DOUBLE (0)
/*===========================================================================*/
template<typename T>
inline T numBitsOne(T n)
{
    T x = 0;;

    for (T i = 0; i < n; i++) {
        x |= 0x1<<i;
    }

    return x;
}
/*===========================================================================*/
/**
 *            ////////////////////
 *            // 0  00 -> 00  0 //
 *            // 1  01 -> 10  2 //
 *            // 2  10 -> 01  1 //
 *            // 3  11 -> 11  3 //
 *           //////////////////////
 *           // 0  000 -> 000  0 //
 *           // 1  001 -> 100  4 //
 *           // 2  010 -> 010  2 //
 *           // 3  011 -> 110  6 //
 *           // 4  100 -> 001  1 //
 *           // 5  101 -> 101  5 //
 *           // 6  110 -> 011  3 //
 *           // 7  111 -> 111  7 //
 *          ////////////////////////
 *          //  0 0000 ->  0 0000 //
 *          //  1 0001 ->  8 1000 //
 *          //  2 0010 ->  4 0100 //
 *          //  3 0011 -> 12 1100 //
 *          //  4 0100 ->  2 0010 //
 *          //  5 0101 -> 10 1010 //
 *          //  6 0110 ->  6 0110 //
 *          //  7 0111 -> 14 1110 //
 *          //  8 1000 ->  1 0001 //
 *          //  9 1001 ->  9 1001 //
 *          // 10 1010 ->  5 0101 //
 *          // 11 1011 -> 13 1101 //
 *          // 12 1100 ->  3 0011 //
 *          // 13 1101 -> 11 1011 //
 *          // 14 1110 ->  7 0111 //
 *          // 15 1111 -> 15 1111 //
 *          ////////////////////////
 **/
template<typename T>
void bitReverse2(int n, T *data_r, T *data_i)
{
    int i, j, k;

    j = 0;
    for (i = 1; i < n-1; i++) {
        k = n >> 1;
        while (k <= j) {
            j -= k;
            k >>= 1;
        }
        j += k;
        if (i < j) {
            std::swap(data_r[i], data_r[j]);
            std::swap(data_i[i], data_i[j]);
        }
    }

    return;
}
template<typename T>
void bitReverse1(int n, T *data_)
{
    int i, j, k;

    j = 0;
    for (i = 1; i < n-1; i++) {
        k = n >> 1;
        while (k <= j) {
            j -= k;
            k >>= 1;
        }
        j += k;
        if (i < j) {
            std::swap(data_[i], data_[j]);
        }
    }

    return;
}
/*===========================================================================*/
/**
 * Radix-2 Butterfly
 *
 *             a o------->o A
 *                \      /
 *                 \    / w     a = a_r + j*a_i
 *                  \  /        b = b_r + j*b_i
 *                   \/         w = w_r + j*w_i
 *                   /\         A = A_r + j*A_i
 *                  /  \        B = B_r + j*B_i
 *                 /    \-w
 *                /      \
 *             b o------->o B
 *
 *   A = a+w*b    A_r = a_r + ( b_r*w_r - b_i*w_i )
 *   B = a-w*b => B_r = a_r - ( b_r*w_r - b_i*w_i )
 *                A_i = a_i + ( b_r*w_i + b_i*w_r )
 *                B_i = a_i - ( b_r*w_i + b_i*w_r )
 *
 *
 **/
template<typename T>
void radix2Butterfly(
        T *a_r, T *a_i,
        T *b_r, T *b_i,
        const T *w_r, const T *w_i)
{
#if DOUBLE
    T bw_r = ((*b_r)*(*w_r)) - ((*b_i)*(*w_i));
    T bw_i = ((*b_r)*(*w_i)) + ((*b_i)*(*w_r));

    *b_r = (*a_r) - bw_r;
    *b_i = (*a_i) - bw_i;

    *a_r = (*a_r) + bw_r;
    *a_i = (*a_i) + bw_i;
#else
    unsigned int current_bit = a_r->getBitLength();

    // printf("=======================================\n");
    T tmp_w_r = *w_r;
    T tmp_w_i = *w_i;

    T bw_r = ((*b_r)*(tmp_w_r)) - ((*b_i)*(tmp_w_i));
    T bw_i = ((*b_r)*(tmp_w_i)) + ((*b_i)*(tmp_w_r));

    // printf("w     = %16lx(%2d-bit) + %16lx(%2d-bit)\n", w_r->getValue(), w_r->getBitLength(), w_i->getValue(), w_i->getBitLength());
    // printf("a     = %16lx(%2d-bit) + %16lx(%2d-bit)\n", a_r->getValue(), a_r->getBitLength(), a_i->getValue(), a_i->getBitLength());
    // printf("b     = %16lx(%2d-bit) + %16lx(%2d-bit)\n", b_r->getValue(), b_r->getBitLength(), b_i->getValue(), b_i->getBitLength());
    // printf("bw    = %16lx(%2d-bit) + %16lx(%2d-bit)\n", bw_r.getValue(), bw_r.getBitLength(), bw_i.getValue(), bw_i.getBitLength());

    bw_r = bw_r>>(w_r->getBitLength()-1);
    bw_i = bw_i>>(w_i->getBitLength()-1);
    // printf("bw    = %16lx(%2d-bit) + %16lx(%2d-bit)\n", bw_r.getValue(), bw_r.getBitLength(), bw_i.getValue(), bw_i.getBitLength());


    T tmp_b_r = *a_r - bw_r;
    T tmp_b_i = *a_i - bw_i;
    // printf("tmp_b = %16lx(%2d-bit) + %16lx(%2d-bit)\n", tmp_b_r.getValue(), tmp_b_r.getBitLength(), tmp_b_i.getValue(), tmp_b_i.getBitLength());

    T tmp_a_r = *a_r + bw_r;
    T tmp_a_i = *a_i + bw_i;
    // printf("tmp_a = %16lx(%2d-bit) + %16lx(%2d-bit)\n", tmp_a_r.getValue(), tmp_a_r.getBitLength(), tmp_a_i.getValue(), tmp_a_i.getBitLength());

    *b_r = T(tmp_b_r.getValue(), current_bit+1);
    *b_i = T(tmp_b_i.getValue(), current_bit+1);

    *a_r = T(tmp_a_r.getValue(), current_bit+1);
    *a_i = T(tmp_a_i.getValue(), current_bit+1);
    // printf("=======================================\n");
#endif

    return;
}
/*===========================================================================*\
 *    Euler's formula                                                        *
 *                                                                           *
 *    exp(ix) = cos(x) + j sin(x)                                            *
 *                                                                           *
 *    FFT:                                                                   *
 *                                                                           *
 *           N-1                                                             *
 *           ----                                                            *
 *           \__             -j*2pi*nk                                       *
 *    X[k] = /     x(n)*exp(-----------), where k=0, 1, 2, ..., N-1          *
 *           ----                N                                           *
 *           n=0                                                             *
 *    IFFT:                                                                  *
 *                                                                           *
 *                 N-1                                                       *
 *                 ----                                                      *
 *             1   \__             j*2pi*nk                                  *
 *    x(k) = ----- /     X[n]*exp(----------), where n=0, 1, 2, ..., N-1     *
 *             N   ----               N                                      *
 *                 k=0                                                       *
 \*===========================================================================*/
template<typename T>
void fft_(int n_point, T *data_r, T *data_i, const T *w_r, const T *w_i, int fft_inv_sign)
{
    int stages = log2(n_point);
#if !(DOUBLE)
        printf("real\n");
        for (int idx = 0; idx < n_point; idx++) {
            fprintf(stdout, "%lx\n", data_r[idx].getValue());
        }
        printf("imag\n");
        for (int idx = 0; idx < n_point; idx++) {
            fprintf(stdout, "%lx\n", data_i[idx].getValue());
        }
#endif
    /* FFT/IFFT */
    /* note:  n-point FFT need to compute n_point*log(n_point)/2 butterflies */
    int *data = new int[n_point];
    for (int idx = 0; idx < n_point; idx++) { data[idx] = idx; }
    int stage_idx = 0;
    int n1 = 0, n2 = 1;
    for (stage_idx = 0, n1 = 0, n2 = 1; stage_idx < stages; stage_idx++) {
        n1 = n2;
        n2 = n2 + n2;
        for (int i = 0; i < n_point; i++) { data[i]=i; }
        if (stage_idx!=0) {
            for (int i = 0; i < n_point/n2; i++) {
                bitReverse1(n2, data+n2*i);
            }
        }
        fprintf(stdout, " a_idx b_idx twiddle_idx       a_r          a_i          b_r          b_i      twiddle_r    twiddle_i        A_r          A_i          B_r          B_i\n");
        for (int i = 0; i < n_point; i+=2) {
            int w_idx = (1 << (stages-stage_idx-1))*data[i%n2];
            int butterfly_idx_a=data[i];
            n1 = n2/2;
            T c = w_r[w_idx];
            T s = w_i[w_idx];
#if DOUBLE
            fprintf(stdout, "%5d %5d %7d    ", butterfly_idx_a, butterfly_idx_a+n1, w_idx);
            fprintf(stdout, "%12.4f %12.4f ", data_r[butterfly_idx_a],    data_i[butterfly_idx_a]);
            fprintf(stdout, "%12.4f %12.4f ", data_r[butterfly_idx_a+n1], data_i[butterfly_idx_a+n1]);
            fprintf(stdout, "%12.4f %12.4f ", w_r[w_idx], w_i[w_idx]);
#else
            fprintf(stdout, "%5d %5d %7d    ", butterfly_idx_a, butterfly_idx_a+n1, w_idx);
            fprintf(stdout, "%12lx %12lx ", data_r[butterfly_idx_a].getValue(),    data_i[butterfly_idx_a].getValue());
            fprintf(stdout, "%12lx %12lx ", data_r[butterfly_idx_a+n1].getValue(), data_i[butterfly_idx_a+n1].getValue());
            fprintf(stdout, "%12lx %12lx ", w_r[w_idx].getValue(), w_i[w_idx].getValue());
#endif
            radix2Butterfly(
                    data_r+butterfly_idx_a   , data_i+butterfly_idx_a,
                    data_r+butterfly_idx_a+n1, data_i+butterfly_idx_a+n1,
                    &c, &s);
#if DOUBLE
            fprintf(stdout, "%12.4f %12.4f ",  data_r[butterfly_idx_a],    data_i[butterfly_idx_a]);
            fprintf(stdout, "%12.4f %12.4f\n", data_r[butterfly_idx_a+n1], data_i[butterfly_idx_a+n1]);
#else
            fprintf(stdout, "%12lx %12lx ",  data_r[butterfly_idx_a].getValue(),    data_i[butterfly_idx_a].getValue());
            fprintf(stdout, "%12lx %12lx\n", data_r[butterfly_idx_a+n1].getValue(), data_i[butterfly_idx_a+n1].getValue());
#endif
        }
#if !(DOUBLE)
        printf("real\n");
        for (int idx = 0; idx < n_point; idx++) {
            fprintf(stdout, "%lx\n", data_r[idx].getValue());
        }
        printf("imag\n");
        for (int idx = 0; idx < n_point; idx++) {
            fprintf(stdout, "%lx\n", data_i[idx].getValue());
        }
#endif
    }

    delete [] data;

    return;
}
/*===========================================================================*/
template<typename T>
void fft(int n_point, T *data_r, T *data_i, const T *w_r, const T *w_i)
{
    fft_(n_point, data_r, data_i, w_r, w_i, -1);
}
template<typename T>
/*===========================================================================*/
void ifft(int n_point, T *data_r, T *data_i, const T *w_r, const T *w_i)
{
    fft_(n_point, data_r, data_i, w_r, w_i, 1);
}
/*===========================================================================*/
#endif /* end of __FFT_H__ */
