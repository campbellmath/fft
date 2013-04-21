#ifndef __FFT_H__
#define __FFT_H__

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <typeinfo>
#include <algorithm>

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
#include <iostream>
static unsigned long int signedExtend(
        unsigned long int x,
        unsigned int original_bit_length,
        unsigned int final_bit_length)
{
    unsigned long int mask = 0;
    for (unsigned int i = 0; i < final_bit_length; i++) {
        mask |= 0x1<<i;
    }
    //// set high bits to 0 ( > final_bit_length)
    x &= mask;
    for (unsigned int i = original_bit_length; i < final_bit_length; i++) {
        //                            -1 for 2's complement MSB
        x |= ((x>>(original_bit_length-1))&0x1l)<<i;
    }

    return x;
}
template<typename T>
void radix2Butterfly(
        T *a_r, T *a_i,
        T *b_r, T *b_i,
        const T *w_r, const T *w_i)
{
#if DOUBLE
    T tmp_r = ((*b_r)*(*w_r)) - ((*b_i)*(*w_i));
    T tmp_i = ((*b_r)*(*w_i)) + ((*b_i)*(*w_r));

    *b_r = (*a_r) - tmp_r;
    *b_i = (*a_i) - tmp_i;

    *a_r = (*a_r) + tmp_r;
    *a_i = (*a_i) + tmp_i;
#else
    //printf("=======================================\n");
    T tmp_w_r = *w_r;
    T tmp_w_i = *w_i;

    T tmp_r = ((*b_r)*(tmp_w_r)) - ((*b_i)*(tmp_w_i));
    T tmp_i = ((*b_r)*(tmp_w_i)) + ((*b_i)*(tmp_w_r));

    //printf("w = %5lx(%2d-bit) + %5lx(%2d-bit)\n", w_r->getValue(), w_r->getBitLength(), w_i->getValue(), w_i->getBitLength());
    //printf("a = %5lx(%2d-bit) + %5lx(%2d-bit)\n", a_r->getValue(), a_r->getBitLength(), a_i->getValue(), a_i->getBitLength());
    //printf("b = %5lx(%2d-bit) + %5lx(%2d-bit)\n", b_r->getValue(), b_r->getBitLength(), b_i->getValue(), b_i->getBitLength());
    //printf("tmp   = %5lx(%2d-bit) + %5lx(%2d-bit)\n", tmp_r.getValue(), tmp_r.getBitLength(), tmp_i.getValue(), tmp_i.getBitLength());

    unsigned int current_bit = a_r->getBitLength();

    T tmp0a_r(a_r->getValue()<<(w_r->getBitLength()-1), tmp_r.getBitLength());
    T tmp0a_i(a_i->getValue()<<(w_i->getBitLength()-1), tmp_i.getBitLength());

    //printf("tmp0a = %5lx(%2d-bit) + %5lx(%2d-bit)\n", tmp0a_r.getValue(), tmp0a_r.getBitLength(), tmp0a_i.getValue(), tmp0a_i.getBitLength());

    T tmp1b_r = tmp0a_r - tmp_r;
    T tmp1b_i = tmp0a_i - tmp_i;

    //printf("tmp1b = %5lx(%2d-bit) + %5lx(%2d-bit)\n", tmp1b_r.getValue(), tmp1b_r.getBitLength(), tmp1b_i.getValue(), tmp1b_i.getBitLength());
    T tmp1a_r = tmp0a_r + tmp_r;
    T tmp1a_i = tmp0a_i + tmp_i;
    //printf("tmp1a = %5lx(%2d-bit) + %5lx(%2d-bit)\n", tmp1a_r.getValue(), tmp1a_r.getBitLength(), tmp1a_i.getValue(), tmp1a_i.getBitLength());

    // get MSB part
    T tmp2b_r(signedExtend((tmp1b_r.getValue()>>(w_r->getBitLength())-1), current_bit, current_bit+1), current_bit+1);
    T tmp2b_i(signedExtend((tmp1b_i.getValue()>>(w_r->getBitLength())-1), current_bit, current_bit+1), current_bit+1);

    T tmp2a_r(signedExtend((tmp1a_r.getValue()>>(w_r->getBitLength())-1), current_bit, current_bit+1), current_bit+1);
    T tmp2a_i(signedExtend((tmp1a_i.getValue()>>(w_r->getBitLength())-1), current_bit, current_bit+1), current_bit+1);

    *b_r = tmp2b_r;
    *b_i = tmp2b_i;

    *a_r = tmp2a_r;
    *a_i = tmp2a_i;
    //printf("=======================================\n");
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

    /* bit reverse */
    bitReverse2(n_point, data_r, data_i);

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
            fprintf(stdout, "%11lx %11lx ", data_r[butterfly_idx_a].getValue(),    data_i[butterfly_idx_a].getValue());
            fprintf(stdout, "%11lx %11lx ", data_r[butterfly_idx_a+n1].getValue(), data_i[butterfly_idx_a+n1].getValue());
            fprintf(stdout, "%11lx %11lx ", w_r[w_idx].getValue(), w_i[w_idx].getValue());
#endif
            radix2Butterfly(
                    data_r+butterfly_idx_a   , data_i+butterfly_idx_a,
                    data_r+butterfly_idx_a+n1, data_i+butterfly_idx_a+n1,
                    &c, &s);
#if DOUBLE
            fprintf(stdout, "%12.4f %12.4f ",  data_r[butterfly_idx_a],    data_i[butterfly_idx_a]);
            fprintf(stdout, "%12.4f %12.4f\n", data_r[butterfly_idx_a+n1], data_i[butterfly_idx_a+n1]);
#else
            fprintf(stdout, "%11lx %11lx ",  data_r[butterfly_idx_a].getValue(),    data_i[butterfly_idx_a].getValue());
            fprintf(stdout, "%11lx %11lx\n", data_r[butterfly_idx_a+n1].getValue(), data_i[butterfly_idx_a+n1].getValue());
#endif
        }
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
