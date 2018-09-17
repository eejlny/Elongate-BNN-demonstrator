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
 *     (c) Copyright 2008-2016 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

/**
 * @file hls_basic_math.h
 * This file contains basic comparison-related functions.
 */

template <typename T>
int generic_signbit(T x) {
    fp_struct<T> s(x);
    return s.__signbit();
}
static int __signbit(float x)
{
    return generic_signbit(x);
}

static int __signbit(double x)
{
    return generic_signbit(x);
}

static int __signbit(half x)
{
    return generic_signbit(x);
}

template <typename T>
T generic_nan(const char *tagp) {
    fp_struct<T> nan;
    nan.sig = -1; // all ones
    nan.exp = -1; // all ones
    nan.sign = 0;
    return nan.to_ieee();
}
static double nan(const char *tagp) {
    return generic_nan<double>(tagp);
}

static float nanf(const char *tagp) {
    return generic_nan<double>(tagp);
}

static half half_nan(const char *tagp) {
    return generic_nan<half>(tagp);
}


/**
 * Determines if given floating point number has a finite value.
 * i.e. normal, subnormal or zero, but not infinite or NAN
 *
 * @param x floating point vlaue.
 * @return nonzero value if arg has finite value, 0 otherwise.
 */
template <typename T>
int generic_isfinite(T x) {
    fp_struct<T> fs = x;
    int ret = (fs.exp == fp_struct<T>::EXP_INFNAN) ? 0 : 1;
    return ret;
}
static int __isfinite(float x)
{
    return generic_isfinite(x);
}

static int __isfinite(double x)
{
    return generic_isfinite(x);
}

static int __isfinite(half x)
{
    return generic_isfinite(x);
}


/**
 * Determines if given floating point number is positive or negative infinity.
 *
 * @param x floating point vlaue.
 * @return nonzero value if arg is infinite, 0 otherwise.
 */
template <typename T>
int generic_isinf(T x) {
    fp_struct<T> fs = x;
    int ret = ((fs.exp == fp_struct<T>::EXP_INFNAN) && (fs.sig == 0x0)) ? 1 : 0;
    return ret;
}
static int __isinf(float x)
{
    return generic_isinf(x);
}

static int __isinf(double x)
{
    return generic_isinf(x);
}

static int __isinf(half x)
{
    return generic_isinf(x);
}

/**
 * Determines if given floating point number is not-a-number (NaN)
 *
 * @param x floating point vlaue.
 * @return nonzero value if arg is not-a-number (NaN), 0 otherwise.
 */
template <typename T>
int generic_isnan(T x) {
    fp_struct<T> fs = x;
    int ret = ((fs.exp == fp_struct<T>::EXP_INFNAN) && (fs.sig != 0x0)) ? 1 : 0;
    return ret;
}
static int __isnan(float x)
{
    return generic_isnan(x);
}

static int __isnan(double x)
{
    return generic_isnan(x);
}

static int __isnan(half x)
{
    return generic_isnan(x);
}

/**
 * Determines if given floating point number is normal
 * i.e. is neither zero, subnormal, infinite or NaN.
 *
 * @param x floating point vlaue.
 * @return nonzero value if arg is normal, 0 otherwise.
 */

template <typename T>
int generic_isnormal(T x) {
    fp_struct<T> fs = x;
    int ret = ((fs.exp != fp_struct<T>::EXP_INFNAN) && (fs.exp != 0x0)) ? 1 : 0;
    return ret;
}
static int __isnormal(float x)
{
    return generic_isnormal(x);
}

static int __isnormal(double x)
{
    return generic_isnormal(x);
}

static int __isnormal(half x)
{
    return generic_isnormal(x);
}

/**
 * Categorizes floating point value into the following categories:
 * zero, subnormal, normal, infinite, NaN, or implementation-defined category.
 *
 * @param x floating point vlaue.
 * @return FP_INFINITE, FP_NAN, FP_NORMAL, FP_SUBNORMAL, FP_ZERO
 */
#ifndef FP_NORMAL
#define FP_NORMAL    4
#endif

#ifndef FP_SUBNORMAL
#define FP_SUBNORMAL 3
#endif

#ifndef FP_ZERO
#define FP_ZERO      2
#endif

#ifndef FP_INFINITE
#define FP_INFINITE  1
#endif

#ifndef FP_NAN
#define FP_NAN       0
#endif

template <typename T>
int generic_fpclassify(T x) {
    fp_struct<T> fs = x;
    int ret;
    if(fs.exp == 0x0) {
        ret = (fs.sig == 0x0) ? FP_ZERO : FP_SUBNORMAL;
    } else if(fs.exp == fp_struct<T>::EXP_INFNAN) {
        ret = (fs.sig == 0x0) ? FP_INFINITE : FP_NAN;
    } else {
        ret = FP_NORMAL;
    }
    return ret;
}
static int __fpclassifyf(float x)
{
    return generic_fpclassify(x);
}

static int __fpclassify(double x)
{
    return generic_fpclassify(x);
}

static int __fpclassify(half x)
{
    return generic_fpclassify(x);
}

static
double copysign(double x, double y)
{
    fp_struct<double> xs(x), ys(y);
    xs.sign = ys.sign;
    return xs.to_ieee();
}

static
float copysignf(float x, float y)
{
    fp_struct<float> xs(x), ys(y);
    xs.sign = ys.sign;
    return xs.to_ieee();
}
static
half half_copysign(half x, half y)
{
    fp_struct<half> xs(x), ys(y);
    xs.sign = ys.sign;
    return xs.to_ieee();
}

#ifdef __cplusplus
static
float copysign(float x, float y)
{
    return copysignf(x, y);
}
static
half copysign(half x, half y)
{
    return ::hls::half_copysign(x, y);
}
#endif


static
double fabs(double x)
{
    return copysign(x, 0.0);
}

static
float fabsf(float x)
{
    return copysignf(x, 0.0f);
}

static
half half_fabs(half x)
{
    return ::hls::half_copysign(x, (half)0.0);
}

#ifdef __cplusplus
static
float fabs(float x)
{
    return fabsf(x);
}
static
half fabs(half x)
{
    return ::hls::half_fabs(x);
}
#endif

template <typename T>
T isequal(T x, T y) {
    return x == y;
}

template <typename T>
T isgreater(T x, T y) {
    return x > y;
}

template <typename T>
T isgreaterequal(T x, T y) {
    return x >= y;
}

template <typename T>
T isless(T x, T y) {
    return x < y;
}

template <typename T>
T islessequal(T x, T y) {
    return x <= y;
}

template <typename T>
T isnotequal(T x, T y) {
    return x != y;
}

template <typename T>
T isordered(T x, T y) {
    return (((!__isnan(x))&&(!__isnan(y))) ? 1 : 0); // &&(in0==in0)&&(in1==in1));
}

template <typename T>
T isunordered(T x, T y) {
    return ((  __isnan(x) ||  __isnan(y) ) ? 1 : 0);
}

/**
 * fmax returns the maximum of two arguments. 
 * If both arguments are 0/ -0, fmax always returns the 2nd argument
 * If both argumetns are nan, fmax always returns the 1st argument but casted
 * to be of type QNaN meaning bit 23 is a 1.
 * If only one argument is nan, fmax always returns the other argument.
 */
template <typename T>
T generic_fmax(T x, T y)
{
	fp_struct<T> x_fp(x);
	fp_struct<T> y_fp(y);
    ap_uint<fp_struct<T>::EXP_BITS> max_exponent = -1;
	T res;
	if(x_fp.exp == 0 && x_fp.sig == 0 && y_fp.exp == 0 && y_fp.sig == 0) { // 0, -0: always return 2nd arg
		res = y;
	} else if(__isnan(x)) { // 1st arg: nan
		if(__isnan(y)) { // 1st and 2nd arg: nan, cast to QNaN and choose 1st arg
			x_fp.sig[fp_struct<T>::SIG_BITS-1] = 1;
			res = x_fp.to_ieee();
		} else { // 1st arg: nan, 2nd: ~nan
			res = y;
		}
	} else if(__isnan(y)) { // 2nd arg: nan, 1st arg: ~nan
		res = x;
	} else {
		//res = (x > y) ? x : y;
        bool ymaggreater = x_fp.to_int() < y_fp.to_int();
        if(y_fp.sign && x_fp.sign) ymaggreater = !ymaggreater;
        res = ymaggreater ? y : x;
	}
	return res;
}
static
double fmax(double x, double y)
{
    return generic_fmax<double>(x,y);
}
static
float fmaxf(float x, float y)
{
    return generic_fmax<float>(x,y);
}
static
half half_fmax(half x, half y)
{
    return generic_fmax<half>(x,y);
}

#ifdef __cplusplus
static
float fmax(float x, float y)
{
    return fmaxf(x,y);
}
static
half fmax(half x, half y)
{
    return generic_fmax<half>(x,y);
}
#endif

/**
 * fmax returns the maximum of two arguments.
 * If both arguments are 0/ -0, fmax always returns the 2nd argument
 * If both argumetns are nan, fmax always returns the 1st argument but casted
 * to be of type QNaN meaning bit 23 is a 1.
 * If only one argument is nan, fmax always returns the other argument.
 */
template<typename T>
T generic_fmin(T x, T y)
{
	fp_struct<T> x_fp(x);
	fp_struct<T> y_fp(y);
    ap_uint<fp_struct<T>::EXP_BITS> max_exponent = -1;
	T res;
	if(x_fp.exp == 0 && x_fp.sig == 0 && y_fp.exp == 0 && y_fp.sig == 0) { // 0, -0: always return 2nd arg
		res = y;
	} else if(__isnan(x)) { // 1st arg: nan
		if(__isnan(y)) { // 1st and 2nd arg: nan, cast to QNaN and choose 1st arg
			x_fp.sig[fp_struct<T>::SIG_BITS-1] = 1;
			res = x_fp.to_ieee();
		} else { // 1st arg: nan, 2nd: ~nan
			res = y;
		}
	} else if(__isnan(y)) { // 2nd arg: nan, 1st arg: ~nan
		res = x;
	} else {
		//res = (x < y) ? x : y;
        bool ymaggreater = x_fp.to_int() < y_fp.to_int();
        if(y_fp.sign && x_fp.sign) ymaggreater = !ymaggreater;
        res = ymaggreater ? x : y;
    }
	return res;
}

static
double fmin(double x, double y)
{
    return generic_fmin<double>(x,y);
}

static
float fminf(float x, float y)
{
    return generic_fmin<float>(x,y);
}
static
half half_fmin(half x, half y)
{
    return generic_fmin<half>(x,y);
}

#ifdef __cplusplus
static
float fmin(float x, float y)
{
    return fminf(x,y);
}
static
half fmin(half x, half y)
{
    return generic_fmin<half>(x,y);
}
#endif

