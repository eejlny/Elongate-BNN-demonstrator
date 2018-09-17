/*****************************************************************************
 *
 *     Author: Xilinx, Inc.
 *
 *     This text contains proprietary, confidential information of
 *     Xilinx, Inc. , is distributed by under license from Xilinx,
 *     Inc., and may be used, copied and/or disclosed only pursuant to
 *     the terms of a valid license agreement with Xilinx, Inc.
 *
 *     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
 *     AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
 *     SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
 *     OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
 *     APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
 *     THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
 *     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
 *     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
 *     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
 *     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
 *     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
 *     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE.
 *
 *     Xilinx products are not intended for use in life support appliances,
 *     devices, or systems. Use in such applications is expressly prohibited.
 *
 *     (c) Copyright 2008-2012 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/


/**
 * @file hls_math.h
 * This file contains math.h related functions such as sin, cos, sqrt, ln, etc.
 */

#ifndef X_HLS_MATH_H
#define X_HLS_MATH_H

#ifndef __cplusplus
#error C++ is required to include this header file
#endif

#include <assert.h>
#include <stdint.h>
#include "ap_int.h"
#include "hls/utils/x_hls_defines.h"
#include "hls/utils/x_hls_utils.h"
#include "hls/utils/x_hls_traits.h"
#include "hls_fpo.h"

namespace hls
{

/**
 * The Xilinx math.h library implements standard math.h functions
 * optimized for FPGA implementation.
 *
 */

/* @{ */

#include "hls/hls_basic_math.h"

template <typename T>
T generic_frexp (T x, int* exp) {
    // The maximum exponent possible for type T.
    ap_uint<fp_struct<T>::EXP_BITS> max_exponent = -1;
    fp_struct<T> xs = x;
    bool zero = (xs.exp == 0x0);
    bool nan = (xs.exp == max_exponent);
    *exp = (zero || nan) ? 0 : xs.expv()+1;
    if(zero) {
        // zero or subnormal
        return (T)0.0;
    } else if(nan) {
        // nan or infinity, return input.
        return x;
    } else {
        xs.exp = fp_struct<T>::EXP_BIAS-1;
        return xs.to_ieee();
    }
}
static double frexp (double x, int* exp) {
    return generic_frexp<double>(x, exp);
}
static float frexpf (float x, int* exp) {
    return generic_frexp<float>(x, exp);
}
static half half_frexp (half x, int* exp) {
    return generic_frexp<half>(x, exp);
}


template <typename T>
T generic_ldexp (T x, int exp) {
    // The maximum exponent possible for type T.
    ap_uint<fp_struct<T>::EXP_BITS> max_exponent = -1;
    fp_struct<T> xs = x;
    exp += xs.exp;
    bool ininf = (xs.exp == max_exponent);
    bool zero = (xs.exp == 0) || (exp <= 0);
    bool inf = ininf || (exp >= max_exponent);
    if(__isnan(x)) {
        return x;
    } else if(zero && !ininf) {
        return ::hls::copysign((T)0.0,x);
    } else if(inf) {
        // infinity, return input.
        xs.sig = 0x0;
        xs.exp = max_exponent;
    } else {
        xs.exp = exp;
    }
    return xs.to_ieee();
}
static double ldexp (double x, int exp) {
    return generic_ldexp<double>(x, exp);
}

static float ldexpf (float x, int exp) {
    return generic_ldexp<float>(x, exp);
}

static half half_ldexp (half x, int exp) {
    return generic_ldexp<half>(x, exp);
}

#include "hls/hls_normalize.h"
#include "hls/hls_round.h"
#include "hls/hls_log.h"

// Trig functions have 2 different possible implementations.
#include "hls/hls_cordic.h"
#include "hls/hls_hotbm.h"

// sin
static float sinf(float t_in)
{
    return hotbm::sinf(t_in);
}

static float sin(float t_in)
{
    return sinf(t_in);
}

static double sin(double t_in)
{
    return cordic::sin(t_in);
}
static half half_sin(half t_in)
{
    return hotbm::half_sin(t_in);
}
static half sin(half t_in)
{
    return ::hls::half_sin(t_in);
}

// cos
static float cosf(float t_in)
{
    return hotbm::cosf(t_in);
}

static float cos(float t_in)
{
    return cosf(t_in);
}

static half half_cos(half t_in)
{
    return hotbm::half_cos(t_in);
}
static half cos(half t_in)
{
    return ::hls::half_cos(t_in);
}

static double cos(double t_in)
{
    return cordic::cos(t_in);
}

//sinpi
static float sinpif(float t_in)
{
    return hotbm::sinpif(t_in);
}

static half half_sinpi(half t_in)
{
    return hotbm::half_sinpi(t_in);
}
//cospi
static float cospif(float t_in)
{
    return hotbm::cospif(t_in);
}

static half half_cospi(half t_in)
{
    return hotbm::half_cospi(t_in);
}

template<int W, int I>
ap_fixed<W, 2> cosf(ap_fixed<W, I> t_in)
{
    return hotbm::cosf(t_in);
}
template<int W, int I>
ap_fixed<W, 2> sinf(ap_fixed<W, I> t_in)
{
    return hotbm::sinf(t_in);
}

template<int W, int I>
ap_fixed<W, 2> cospif(ap_fixed<W, I> t_in)
{
    return hotbm::cospif(t_in);
}
template<int W, int I>
ap_fixed<W, 2> sinpif(ap_fixed<W, I> t_in)
{
    return hotbm::sinpif(t_in);
}

template<int W, int I>
ap_fixed<W, 2> cos(ap_fixed<W, I> t_in)
{
    return hotbm::cosf(t_in);
}
template<int W, int I>
ap_fixed<W, 2> sin(ap_fixed<W, I> t_in)
{
    return hotbm::sinf(t_in);
}

template<int W, int I>
ap_fixed<W, 2> cospi(ap_fixed<W, I> t_in)
{
    return hotbm::cospif(t_in);
}
template<int W, int I>
ap_fixed<W, 2> sinpi(ap_fixed<W, I> t_in)
{
    return hotbm::sinpif(t_in);
}


static
void sincos(double x, double *sin, double *cos)
{
    cordic::sincos(x, sin, cos);
}

static
void sincosf(float x, float *sin, float *cos)
{
    hotbm::sincosf(x, sin, cos);
}

static
double
atan(double t_in)
{
	return cordic::atan(t_in);
}

static
float
atanf(float t_in)
{
	return cordic::atanf(t_in);
}

static
half
half_atan(half t_in)
{
	return cordic::half_atan(t_in);
}

static
double
atan2(double y_in, double x_in)
{
	return cordic::atan2(y_in, x_in);
}

static
float
atan2f(float y_in, float x_in)
{
	return cordic::atan2f(y_in, x_in);
}

static
half
half_atan2(half y_in, half x_in)
{
	return cordic::half_atan2(y_in, x_in);
}

static
float
sinhf(float t_in)
{
	return cordic::sinhf(t_in);
}

static
float
coshf(float t_in)
{
	return cordic::coshf(t_in);
}

}

#include "hls_fpo.h"

#ifdef AESL_SYN
#ifdef __cplusplus
extern "C" {
#endif
    double _ssdm_op_DRecip(double) __attribute__ ((nothrow));
    float  _ssdm_op_FRecip(float)  __attribute__ ((nothrow));
    double _ssdm_op_DRSqrt(double) __attribute__ ((nothrow));
    float  _ssdm_op_FRSqrt(float)  __attribute__ ((nothrow));
#ifdef __cplusplus
}
#endif
#endif

namespace hls {

static
float
tanf(float t_in)
{
	float s_out, c_out;
    fp_struct<float> fs = t_in;
    if(fs.exp == 0x0) {
        //subnormal or zero
        return t_in;
    } else {
        sincosf(t_in, &s_out, &c_out);
        return HLS_FPO_DIVF(s_out, c_out);
    }
}

static
double
tan(double t_in)
{
	double s_out, c_out;
    fp_struct<double> fs = t_in;
    if(fs.exp == 0x0) {
        //subnormal or zero
        return t_in;
    } else {
        sincos(t_in, &s_out, &c_out);
        return HLS_FPO_DIV(s_out, c_out);
    }
}

static
float logf(float x)
{
    return ::HLS_FPO_LOGF(x);
}


static
float log10f(float x)
{
    return HLS_FPO_MULF(logf(x), 0.434294481903251827651128918916605082294397005f);
}

static
double log(double x)
{
    return ::HLS_FPO_LOG(x);
}

static
double log10(double x)
{
    return HLS_FPO_MUL(log(x), 0.434294481903251827651128918916605082294397005);
}

static half half_log(half x) {
    return hls::log_reduce::half_log(x);
}
static half half_log10(half x) {
    return hls::log_reduce::half_log10(x);
}
static half half_log2(half x) {
    return hls::log_reduce::half_log2(x);
}

#include "hls/hls_sqrt.h"
#include "hls/hls_rsr.h"
static double sqrt(double x) {
    return ::HLS_FPO_SQRT(x);
}
static float sqrtf(float x) {
    return ::HLS_FPO_SQRTF(x);
}
static half half_sqrt(half x) {
    return hls::hls_rsr::half_sqrt(x);
}


static half half_recip(half x) {
    return hls::hls_rsr::half_recip(x);
}
static half half_rsqrt(half x) {
    return hls::hls_rsr::half_rsqrt(x);
}

#ifdef AESL_SYN
static
double recip(double x)
{
    return _ssdm_op_DRecip(x);
}

static
float recipf(float x)
{
    return _ssdm_op_FRecip(x);
}

static
double rsqrt(double x)
{
    return _ssdm_op_DRSqrt(x);
}

static
float rsqrtf(float x)
{
    return _ssdm_op_FRSqrt(x);
}


#else
static
double recip(double x)
{
    return HLS_FPO_RECIP(x);
}

static
float recipf(float x)
{
    return HLS_FPO_RECIPF(x);
}

static
double rsqrt(double x)
{
    return HLS_FPO_RSQRT(x);
}

static
float rsqrtf(float x)
{
    return HLS_FPO_RSQRTF(x);
}
#endif


static
double exp(double x)
{
    return ::HLS_FPO_EXP(x);
}

static
float expf(float x)
{
    return ::HLS_FPO_EXPF(x);
}
#include "hls/hls_exp.h"
static half half_exp(half x) {
    return hls::exp_reduce::half_exp(x);
}
static half half_exp10(half x) {
    return hls::exp_reduce::half_exp10(x);
}
static half half_exp2(half x) {
    return hls::exp_reduce::half_exp2(x);
}

// Do nothing with the result of the filter.
template <typename T>
struct hls_filter_traits {
    typedef T T_data;
    typedef T T_accum;
    static T_data shift(T_accum in) {
        return in;
    }
};

struct hls_filter_traits_Q15 {
    typedef short T_data;
    typedef int T_accum;
    static T_data shift(T_accum in) {
        return (T_data)(in >> 15);
    }
};

/** Compute the dot product of x and y, returning the result.
 * @param x the first vector, with length n
 * @param y the second vector, with length n
 * @param n the size of the vectors.
 * @param T the type of the data
 * @param T_accum the type used for the accumulation
 */
template <typename T_accum, typename T>
T_accum dot(T *x, T *y, int n)
{
    // This volatile prevents the first tap from being constant-optimized.
    volatile T_accum r = 0;
    T_accum result = r;
dot_foreach_tap:
    for(int i = 0; i < n; i++) {
        result += x[i] * y[i];
    }
    return result;
}

/**
 * Block FIR filter, with data type T.  This function supports traits
 * which can be used to control the type used to perform the filter
 * accumultation.
 *
 * @param in the input data, with size incout.
 * @param state the filter state (size = numtaps), with the newest
 * sample in state[0].
 * @param out the filtered data, with size incout.
 * @param taps the filter taps (size = numtaps), stored in 'normal' order.
 * in and out are size incount.
 * @param incount
 * @param numtaps
 */
template <typename T, typename TRAITS>
void fir_blocksrl(T *in, T *taps, T *out, T *state, int incount, int numtaps)
{
#pragma HLS inline self
fir_blocksrl_foreach_data:
    for(int n = 0; n < incount; n++) {
#pragma HLS expression_balance off
        for(int i = numtaps-1; i > 0; i--) {
            state[i] = state[i-1];
        }
        state[0] = in[n];
        out[n] = TRAITS::shift( dot<typename TRAITS::T_accum>(state, taps, numtaps));
    }
}

/**
 * Block FIR filter, with data type T.  This function should be used
 * when the type used for accumulation is the same as the type of the
 * output (e.g. 'float').
 *
 * @param in the input data, with size incout.
 * @param state the filter state (size = numtaps), with the newest
 * sample in state[0].
 * @param out the filtered data, with size incout.
 * @param taps the filter taps (size = numtaps), stored in 'normal' order.
 * in and out are size incount.
 * @param incount
 * @param numtaps
 */
template <typename T>
void fir_blocksrl(T *in, T *taps, T *out, T *state, int incount, int numtaps)
{
    fir_blocksrl<T, hls_filter_traits<T> >(in, taps, out, state, incount, numtaps);
}

/**
 * Block FIR filter, with Q15 datatypes modeled as 'short'.  The
 * result always includes the required right shift by 15.
 *
 * @param in the input data, with size incout.
 * @param state the filter state (size = numtaps), with the newest
 * sample in state[0].
 * @param out the filtered data, with size incout.
 * @param taps the filter taps (size = numtaps), stored in 'normal' order.
 * in and out are size incount.
 * @param incount
 * @param numtaps
 */
static void fir_blocksrl_Q15(short *in, short *taps, short *out, short *state, int incount, int numtaps)
{
    fir_blocksrl<short, hls_filter_traits_Q15>(in, taps, out, state, incount, numtaps);
}

} // namespace hls

#endif

// XSIP watermark, do not delete 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
