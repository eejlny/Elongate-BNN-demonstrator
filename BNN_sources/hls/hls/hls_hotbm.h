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
 *     (c) Copyright 2012-2013 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

/**
 * @file hls_hotbm.h
 * An implementation of sin/cos, based on the HOTBM method published by
 * Detrey and deDinechin, "Floating-point Trigonometric Functions for FPGAs"
 */ 

#ifndef X_HLS_HOTBM_H
#define X_HLS_HOTBM_H

#ifndef M_PI
// Some compilers are stingy about giving us all these constants that are usually in the C library.
#define M_PI            3.14159265358979323846
#endif

#include "utils/x_hls_defines.h"
#include "utils/x_hls_utils.h"
#include "utils/x_hls_float_utils.h"
//#include "hls_range_redux.h"

namespace hotbm
{

#define parts ((W+7)/8)
#define rem (W - (parts-1)*8)
template <int W, int I>
void my_to_ieee(ap_fixed<W, I> in, half &result, int prescale)
{
    //#pragma HLS inline self off
    fp_struct<half> out;
    out.sign[0] = in[in.wl()-1];

    // Note that when we chop off the sign bit, we don't loose any
    // bits to allow for -MAX to be converted correctly.
    ap_ufixed<W,I> ain = in;

    if(out.__signbit()) {
        ain = -in;
    }

    //const int parts = (W+15)/16;
    //const int rem = W - (parts-1)*16;
    assert(rem > 0);
    assert(rem <= 8);

    // Chop into 16 bit segments. 
    int c[parts];	
    ap_uint<16> t1;
    t1(15,8) = ain(ain.wl()-1,ain.wl()-8);
    t1[7] = 1;
	
    ap_uint<16> t2;
    t2(15,9) = ain(ain.wl()-9,ain.wl()-15);
    t2[8] = 1; 
    c[0]=t1.countLeadingZeros();
    c[1]=t2.countLeadingZeros();

    int shift = 0;
    ap_ufixed<W, I> in_shift = ain;

    for(int i = 0; i < parts; i++) {
        shift += c[i];
        in_shift = in_shift << c[i];
        if(c[i] != 15) {
            break;
        }
    }

    //    assert(in_shift[W-1] == 1);
    ap_ufixed<W,1> result_mantissa; // Reinterpret the bits as a mantissa.
    result_mantissa(W-1,0) = in_shift(W-1,0);

    // Truncate the implicit one and zero extend, if necessary.
    out.sig = result_mantissa; // FIXME: Round here?
    assert(((int)fp_struct<half>::EXP_BIAS+in_shift.iwl()-prescale-shift)<31);
    assert(((int)fp_struct<half>::EXP_BIAS+in_shift.iwl()-prescale-shift)>0);
    out.exp = (in == 0) ? 0 : (fp_struct<half>::EXP_BIAS+in_shift.iwl())-prescale-shift;

    result = out.to_ieee();
}

template <int W, int I>
void my_to_ieee(ap_ufixed<W, I> in, half &result, int prescale)
{
    //#pragma HLS inline self off
    fp_struct<half> out;
    out.sign[0] = 0;
    ap_ufixed<W,I> ain = in;
    assert(rem > 0);
    assert(rem <= 8);

    // Count the leading zeros in each segment.
    int c[parts];	
    ap_uint<16> t1;
    t1(15,8) = ain(ain.wl()-1,ain.wl()-8);
    t1[7] = 1;
	
    ap_uint<16> t2;
    t2(15,9) = ain(ain.wl()-9,ain.wl()-15);
    t2[8] = 1; 
    c[0]=t1.countLeadingZeros();
    c[1]=t2.countLeadingZeros();
	
    int shift = 0;
    ap_fixed<W, I> in_shift = ain;

    for(int i = 0; i < parts; i++) {
        shift += c[i];
        in_shift = in_shift << c[i];
        if(c[i] != 8) {
            break;
        }
    }
    //    assert(in_shift[W-1] == 1);
    ap_ufixed<W,1> result_mantissa; // Reinterpret the bits as a mantissa.

    int newexp = (int)fp_struct<half>::EXP_BIAS+in_shift.iwl()-1-prescale-shift;
    if(newexp < 0 || in == 0) {
        result_mantissa = 0;
        newexp = 0;
    } else {
        result_mantissa(W-1,0) = in_shift(W-1,0);
        assert(newexp<31);
        assert(newexp>=0);
    }

    // Truncate the implicit one and zero extend, if necessary.
    out.set_mantissa(result_mantissa);
    out.exp = newexp;

    result = out.to_ieee();
}
#undef parts
#undef rem

#define parts ((W+15)/16)
#define rem (W - (parts-1)*16)
template <int W, int I>
void my_to_ieee(ap_fixed<W, I> in, float &result, int prescale)
{
    //#pragma HLS inline self off
    fp_struct<float> out;
    out.sign[0] = in[in.wl()-1];

    // Note that when we chop off the sign bit, we don't loose any
    // bits to allow for -MAX to be converted correctly.
    ap_ufixed<W,I> ain = in;

    if(out.__signbit()) {
        ain = -in;
    }

    //const int parts = (W+15)/16;
    //const int rem = W - (parts-1)*16;
    assert(rem > 0);
    assert(rem <= 16);

    // Chop into 16 bit segments. 
    ap_uint<32> out_bits[parts] = {};
    for(int i = 0; i < parts-1; i++) {
        out_bits[i](31, 16) = ain(ain.wl()-16*i-1,ain.wl()-16*i-16);
        out_bits[i][15] = 1;
    }
    out_bits[parts-1](31,31-rem) = ain(ain.wl()-16*(parts-1)-rem);
    out_bits[parts-1][31-rem-1] = 1;

    // Count the leading zeros in each segment.
    int c[parts];
    for(int i = 0; i < parts; i++) {
        c[i] = clz(out_bits[i].to_uint());
    }

    int shift = 0;
    ap_ufixed<W, I> in_shift = ain;

    for(int i = 0; i < parts; i++) {
        shift += c[i];
        in_shift = in_shift << c[i];
        if(c[i] != 16) {
            break;
        }
    }

    //    assert(in_shift[W-1] == 1);
    ap_ufixed<W,1> result_mantissa; // Reinterpret the bits as a mantissa.
    result_mantissa(W-1,0) = in_shift(W-1,0);

    // Truncate the implicit one and zero extend, if necessary.
    out.sig = result_mantissa; // FIXME: Round here?
    assert(((int)fp_struct<float>::EXP_BIAS+in_shift.iwl()-prescale-shift)<255);
    assert(((int)fp_struct<float>::EXP_BIAS+in_shift.iwl()-prescale-shift)>0);
    out.exp = (in == 0) ? 0 : (fp_struct<float>::EXP_BIAS+in_shift.iwl())-prescale-shift;

    result = out.to_ieee();
}


template <int W, int I>
void my_to_ieee(ap_ufixed<W, I> in, float &result, int prescale)
{
    //#pragma HLS inline self off
	
    fp_struct<float> out;
    out.sign[0] = 0;
    ap_ufixed<W,I> ain = in;

    assert(rem > 0);
    assert(rem <= 16);

    // Chop into 16 bit segments. out_bits[0] includes the MSBs.
    ap_uint<32> out_bits[parts] = {};
    for(int i = 0; i < parts-1; i++) {
        out_bits[i](31, 16) = ain(ain.wl()-16*i-1,
                                  ain.wl()-16*i-16);
        out_bits[i][15] = 1;
    }
    out_bits[parts-1](32-1,32-rem) = ain(ain.wl()-16*(parts-1)-1,
                                         ain.wl()-16*(parts-1)-rem);
    out_bits[parts-1][32-rem-1] = 1;

    // Count the leading zeros in each segment.
    int c[parts];
    for(int i = 0; i < parts; i++) {
        c[i] = clz(out_bits[i].to_uint());
    }

    int shift = 0;
    ap_fixed<W, I> in_shift = ain;

    for(int i = 0; i < parts; i++) {
        shift += c[i];
        in_shift = in_shift << c[i];
        if(c[i] != 16) {
            break;
        }
    }

    //    assert(in_shift[W-1] == 1);
    ap_ufixed<W,1> result_mantissa; // Reinterpret the bits as a mantissa.

    int newexp = (int)fp_struct<float>::EXP_BIAS+in_shift.iwl()-1-prescale-shift;
#ifdef DEBUG
    std::cout << "in_shift = " << in_shift << "\n";
    std::cout << "shift = " << shift << "\n";
    std::cout << "newexp = " << newexp << "\n";
#endif
    if(newexp < 0 || in == 0) {
        result_mantissa = 0;
        newexp = 0;
    } else {
        result_mantissa(W-1,0) = in_shift(W-1,0);
        assert(newexp<255);
        assert(newexp>=0);
    }

    // Truncate the implicit one and zero extend, if necessary.
    out.set_mantissa(result_mantissa);
    out.exp = newexp;

    result = out.to_ieee();
}

#undef parts
#undef rem

#define parts ((W+31)/32)
#define rem (W - (parts-1)*32)
template <int W, int I>
void my_to_ieee(ap_fixed<W, I> in, double &result, int prescale)
{
    //#pragma HLS inline self off
    fp_struct<double> out;
    out.sign[0] = in[in.wl()-1];

    // Note that when we chop off the sign bit, we don't loose any
    // bits to allow for -MAX to be converted correctly.
    ap_ufixed<W,I> ain = in;

    if(out.__signbit()) {
        ain = -in;
    }

    //const int parts = (W+31)/32;
    //const int rem = W - (parts-1)*32;
    assert(rem > 0);
    assert(rem <= 32);

    // Chop into 16 bit segments. 
    int c[parts];
	
    ap_uint<64> t1;
    t1(63,32) = ain(ain.wl()-1,ain.wl()-32);
    t1[31] = 1;
	
    ap_uint<64> t2;
    t2(63,33) = ain(ain.wl()-33,ain.wl()-63);
    t2[32] = 1;
 
    c[0]=t1.countLeadingZeros();
    c[1]=t2.countLeadingZeros();
	
    int shift = 0;
    ap_ufixed<W, I> in_shift = ain;

    for(int i = 0; i < parts; i++) {
        shift += c[i];
        in_shift = in_shift << c[i];
        if(c[i] != 32) {
            break;
        }
    }

    //    assert(in_shift[W-1] == 1);
    ap_ufixed<W,1> result_mantissa; // Reinterpret the bits as a mantissa.
    result_mantissa(W-1,0) = in_shift(W-1,0);

    // Truncate the implicit one and zero extend, if necessary.
    out.sig = result_mantissa; // FIXME: Round here?
    assert(((int)fp_struct<double>::EXP_BIAS+in_shift.iwl()-prescale-shift)<2047);
    assert(((int)fp_struct<double>::EXP_BIAS+in_shift.iwl()-prescale-shift)>0);
    out.exp = (in == 0) ? 0 : (fp_struct<double>::EXP_BIAS+in_shift.iwl())-prescale-shift;
    result = out.to_ieee();
}

template <int W, int I>
void my_to_ieee(ap_ufixed<W, I> in, double &result, int prescale)
{
    //#pragma HLS inline self off
    fp_struct<double> out;
    out.sign[0] = 0;
    ap_ufixed<W,I> ain = in;


    assert(rem > 0);
    assert(rem <= 32);

    // Chop into 16 bit segments.
    int c[parts];
	
    ap_uint<64> t1;
    t1(63,32) = ain(ain.wl()-1,ain.wl()-32);
    t1[31] = 1;
	
    ap_uint<64> t2;
    t2(63,33) = ain(ain.wl()-33,ain.wl()-63);
    t2[32] = 1;
 
    c[0]=t1.countLeadingZeros();
    c[1]=t2.countLeadingZeros();
	
    int shift = 0;
    ap_fixed<W, I> in_shift = ain;

    for(int i = 0; i < parts; i++) {
        shift += c[i];
        in_shift = in_shift << c[i];
        if(c[i] != 32) {
            break;
        }
    }

    //    assert(in_shift[W-1] == 1);
    ap_ufixed<W,1> result_mantissa; // Reinterpret the bits as a mantissa.

    int newexp = (int)fp_struct<double>::EXP_BIAS+in_shift.iwl()-1-prescale-shift;
    if(newexp < 0 || in == 0) {
        result_mantissa = 0;
        newexp = 0;
    } else {
        result_mantissa(W-1,0) = in_shift(W-1,0);
        assert(newexp<2047);
        assert(newexp>=0);
    }

    // Truncate the implicit one and zero extend, if necessary.
    out.set_mantissa(result_mantissa);
    out.exp = newexp;

    result = out.to_ieee();
}

#include "hls_hotbm_tables.h"

template<class T>
struct value_list{
};

template<>
struct value_list<double>{
    typedef double T;
	static const int g = 10;  // guard bits during core approx
	static const int p1 = 61; // padding bits during range reduction
	static const int alpha = 7;
    // The number of bits of B that are not used in the K1 computation
	static const int beta1 = 0;
    // The number of bits of B that are not used in the K2 computation
	static const int beta2 = alpha;

    static const int W_dout = fp_struct<T>::SIG_BITS+1+g;
    static const int W_Mx = fp_struct<T>::SIG_BITS+1+g;

    template <int Wx, int W_Mx>
    static ap_ufixed<Wx, 1> sin_or_cos_approximation(bool do_cos,
                                               ap_uint<3> &k,
                                               ap_ufixed<Wx, 0> &x,
                                               ap_ufixed<W_Mx, 0> &Mx,
                                               ap_int<fp_struct<double>::EXP_BITS> &Ex){

        const bool swap_table[8] = {0,1,1,0,0,1,1,0};
#pragma HLS array_partition variable=swap_table complete
#pragma HLS RESOURCE variable=fourth_order_double::sin_cos_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=fourth_order_double::sin_cos_K1 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=fourth_order_double::sin_cos_K2 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=fourth_order_double::sin_cos_K3 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=fourth_order_double::sin_cos_K4 core=ROM_1P_LUTRAM

        ap_uint<alpha+1> A = x(x.wl()-x.iwl()-1,x.wl()-x.iwl()-alpha);
        ap_ufixed<Wx-alpha-beta1, -alpha> B = x;
        ap_ufixed<Wx-alpha-beta2, -alpha> B_trunc = B;
        ap_ufixed<Wx-2*alpha, -2*alpha> B_squared = B_trunc*B_trunc;
        ap_ufixed<Wx-3*alpha, -3*alpha> B_third_power = B_trunc*B_squared;
        ap_ufixed<Wx-4*alpha, -4*alpha> B_fourth_power = B_squared*B_squared;

        bool sin_basis = do_cos? swap_table[k]:!swap_table[k];
        A[alpha] = sin_basis; // cos and sin are concatentated.

        ap_fixed<Wx, 1> t1 = fourth_order_double::sin_cos_K0[A];
        ap_fixed<Wx, 1> t2 = B*fourth_order_double::sin_cos_K1[A];
        ap_fixed<Wx, 1> t3 = B_squared*fourth_order_double::sin_cos_K2[A];
        ap_fixed<Wx, 1> t4 = B_third_power*fourth_order_double::sin_cos_K3[A];
        ap_fixed<Wx, 1> t5 = B_fourth_power*fourth_order_double::sin_cos_K4[A];
        ap_ufixed<Wx, 1> result = Mx*(t1+t2+t3+t4+t5);

#ifdef DEBUG
        std::cout << "flipped_x      =" << x << " " << x.to_string(2) << "\n";
        std::cout << "A              = " << A.to_string(2) << "\n";
        std::cout << "B              = " << B.to_string(2) << "\n";
        std::cout << "B_trunc        = " << B_trunc.to_string(2) << "\n";
        std::cout << "B*B            = " << (B*B).to_string(2) << "\n";
        std::cout << "B_squared      = " << B_squared.to_string(2) << "\n";
        std::cout << "B_third_power  = " << B_third_power.to_string(2) << "\n";
        std::cout << "B_fourth_power = " << B_fourth_power.to_string(2) << "\n";
        std::cout << "t1 = " << t1.to_string(2) << "\n";
        std::cout << "t2 = " << t2.to_string(2) << "\n";
        std::cout << "t3 = " << t3.to_string(2) << "\n";
        std::cout << "t4 = " << t4.to_string(2) << "\n";
        std::cout << "t5 = " << t5.to_string(2) << "\n";
        std::cout << "Mx = " << Mx.to_string(2) << "\n";
        std::cout << "result = " << result.to_string(2) << " " << result << "\n";
#endif
        return result;
    }
};


template<>
struct value_list<float> {
    typedef float T;
	static const int g = 5;
	static const int p1 = 29;
	static const int alpha = 7;
    // The number of bits of B that are not used in the K1 computation
	static const int beta1 = 0;  //beta1 <= 0
    // The number of bits of B that are not used in the K2 computation
	static const int beta2 = 7;  //beta2 <= alpha

    static const int W_dout = fp_struct<T>::SIG_BITS+1+g;
    static const int W_Mx = fp_struct<T>::SIG_BITS+1+g;

    //    typedef first_order_float TABLES;

    template <int Wx, int W_Mx>
    static ap_ufixed<Wx, 1> sin_or_cos_approximation(bool do_cos,
                                               ap_uint<3> &k,
                                               ap_ufixed<Wx, 0> &x,
                                               ap_ufixed<W_Mx, 0> &Mx,
                                               ap_int<fp_struct<float>::EXP_BITS> &Ex){

        const bool swap_table[8] = {0,1,1,0,0,1,1,0};
#pragma HLS array_partition variable=swap_table complete
#pragma HLS RESOURCE variable=second_order_float::sin_cos_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=second_order_float::sin_cos_K1 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=second_order_float::sin_cos_K2 core=ROM_1P_LUTRAM

        ap_uint<alpha+1> A = x(x.wl()-x.iwl()-1,x.wl()-x.iwl()-alpha);
        ap_ufixed<Wx-alpha-beta1, -alpha> B = x;
        ap_ufixed<Wx-alpha-beta2, -alpha> B_trunc = B;
        ap_ufixed<Wx-2*alpha, -2*alpha> B_squared = B_trunc*B_trunc;

        bool sin_basis = do_cos? swap_table[k]:!swap_table[k];
        A[alpha] = sin_basis; // cos and sin are concatentated.

        ap_fixed<Wx, 1> t1 = second_order_float::sin_cos_K0[A];
        ap_fixed<Wx, 1> t2 = B*second_order_float::sin_cos_K1[A];
        ap_fixed<Wx, 1> t3 = B_squared*second_order_float::sin_cos_K2[A];
        ap_ufixed<Wx, 1> result =
            (t1+t2+t3) * Mx;

#ifdef DEBUG
        std::cout << "flipped_x         =" << x << " " << x.to_string(2) << "\n";
        std::cout << "A = " << A.to_string(2) << "\n";
        std::cout << "B         = " << B.to_string(2) << "\n";
        std::cout << "B_trunc         = " << B_trunc.to_string(2) << "\n";
        std::cout << "B*B         = " << (B*B).to_string(2) << "\n";
        std::cout << "B_squared         = " << B_squared.to_string(2) << "\n";
        std::cout << "t1 =" << t1.to_string(2) << "\n";
        std::cout << "t2 = " << t2.to_string(2) << "\n";
        std::cout << "t3 = " << t3.to_string(2) << "\n";
        std::cout << "Mx = " << Mx.to_string(2) << "\n";
        std::cout << "result = " << result.to_string(2) << " " << result << "\n";
#endif
        return result;
    }

    template <int Wx, int W_Mx>
    static void sincos_approximation(ap_uint<3> &k,
                              ap_ufixed<Wx, 0> &x,
                              ap_ufixed<W_Mx, 0> &Mx,
                              ap_int<fp_struct<float>::EXP_BITS> &Ex,
                              ap_ufixed<W_Mx, 1> &sin_result,
                              ap_ufixed<W_Mx, 1> &cos_result
                              ) {
#pragma HLS RESOURCE variable=second_order_float::cos_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=second_order_float::cos_K1 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=second_order_float::cos_K2 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=second_order_float::sin_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=second_order_float::sin_K1 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=second_order_float::sin_K2 core=ROM_1P_LUTRAM

        ap_uint<alpha> A = x(x.wl()-x.iwl()-1,
                             x.wl()-x.iwl()-alpha);
        ap_ufixed<Wx-alpha-beta1, -alpha> B = x;
        ap_ufixed<Wx-alpha-beta2, -alpha> B_trunc = B;
        ap_ufixed<Wx-2*alpha, -2*alpha> B_squared = B_trunc*B_trunc;

        //   TABLES tables;
        ap_ufixed<Wx, 1> cos_t1 = second_order_float::cos_K0[A];
        ap_ufixed<Wx, 1> cos_t2 = B*second_order_float::cos_K1[A];
        ap_ufixed<Wx, 1> cos_t3 = B_squared*second_order_float::cos_K2[A];
        cos_result = 1 - (cos_t1 + cos_t2 + cos_t3);

        ap_fixed<Wx, 1> sin_t1 = second_order_float::sin_K0[A];
        ap_fixed<Wx, 1> sin_t2 = B*second_order_float::sin_K1[A];
        ap_fixed<Wx, 1> sin_t3 = B_squared*second_order_float::sin_K2[A];
        sin_result = Mx * (sin_t1 + sin_t2 + sin_t3);

#ifdef DEBUG
        std::cout << "flipped_x         =" << x << " " << x.to_string(2) << "\n";
        std::cout << "A = " << A.to_string(2) << "\n";
        std::cout << "B         = " << B.to_string(2) << "\n";
        std::cout << "B_trunc         = " << B_trunc.to_string(2) << "\n";
        std::cout << "B*B         = " << (B*B).to_string(2) << "\n";
        std::cout << "B_squared         = " << B_squared.to_string(2) << "\n";

        std::cout << "cos_t1 =" << cos_t1.to_string(2) << "\n";
        std::cout << "cos_t2 = " << cos_t2.to_string(2) << "\n";
        std::cout << "cos_t3 = " << cos_t3.to_string(2) << "\n";
        std::cout << "cos_result = " << cos_result.to_string(2) << " " << cos_result << "\n";

        std::cout << "sin_t1 =" << sin_t1.to_string(2) << "\n";
        std::cout << "sin_t2 = " << sin_t2.to_string(2) << "\n";
        std::cout << "sin_t3 = " << sin_t3.to_string(2) << "\n";
        std::cout << "sin_result = " << sin_result.to_string(2) << " " << sin_result << "\n";
#endif
    }

};

template<>
struct value_list<half>{
    typedef half T;
	static const int g = 3;
	static const int p1 = 12;
	static const int alpha = 8;
	static const int beta1 = 4;

    static const int W_dout = fp_struct<T>::SIG_BITS+1+g;
    static const int W_Mx = fp_struct<T>::SIG_BITS+1+g;

    template <int Wx, int W_Mx>
    static ap_ufixed<Wx, 1> sin_or_cos_approximation(bool do_cos,
                                               ap_uint<3> &k,
                                               ap_ufixed<Wx, 0> &x,
                                               ap_ufixed<W_Mx, 0> &Mx,
                                               ap_int<fp_struct<half>::EXP_BITS> &Ex){

        const bool swap_table[8] = {0,1,1,0,0,1,1,0};
#pragma HLS array_partition variable=swap_table complete
#pragma HLS RESOURCE variable=first_order_half::sin_cos_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=first_order_half::sin_cos_K1 core=ROM_1P_LUTRAM

        ap_uint<alpha+1> A = x(x.wl()-x.iwl()-1,x.wl()-x.iwl()-alpha);
        ap_ufixed<Wx-alpha-beta1, -alpha> B = x;

        bool sin_basis = do_cos? swap_table[k]:!swap_table[k];
        A[alpha] = sin_basis; // cos and sin are concatentated.

        ap_fixed<Wx, 1> t1 = first_order_half::sin_cos_K0[A];
        ap_fixed<Wx, 1> t2 = B*first_order_half::sin_cos_K1[A];
        ap_ufixed<Wx, 1> result =
            (t1+t2) * Mx;

#ifdef DEBUG
        std::cout << "flipped_x = " << x << " " << x.to_string(2) << "\n";
        std::cout << "A = " << A.to_string(2) << "\n";
        std::cout << "B = " << B.to_string(2) << "\n";
        std::cout << "t1 = " << t1.to_string(2) << "\n";
        std::cout << "t2 = " << t2.to_string(2) << "\n";
        std::cout << "Mx = " << Mx.to_string(2) << "\n";
        std::cout << "result = " << result.to_string(2) << " " << result << "\n";
#endif
        return result;
    }
};

/**
 * This function performs a table lookup to get the relevant values
 * of 4oPi given an exponent value for halfs.
 *
 * @param SB Number of mantissa bits (e.g. 10 for half)
 * @param p1 Padding bits
 * @param exp Exponent value
 * @param dummy dummy value to indicate half type.
 */
template<int SB, int p1>
ap_uint<2*(SB+1)+p1+3>
table_lookup_4oPi_hotbm(int exp, half dummy)
{
#pragma HLS RESOURCE variable=ref_4oPi_table_80 core=ROM_1P_LUTRAM
    ap_int<9>   addr     = exp+14;	
    assert(addr >= 0);
    ap_uint<80> table_80 = ref_4oPi_table_80[addr >> 4];
    return (table_80 << (ap_uint<4>)(addr(3,0)))(79,80-((2*(SB+1)+p1+3)));
}


/**
 * This function performs a table lookup to get the relevant values
 * of 4oPi given an exponent value for floats.
 *
 * @param SB Number of mantissa bits (e.g. 23 for float)
 * @param p1 Padding bits
 * @param exp Exponent value
 * @param dummy dummy value to indicate float type.
 */
template<int SB, int p1>
ap_uint<2*(SB+1)+p1+3>
table_lookup_4oPi_hotbm(int exp, float dummy)
{
#pragma HLS RESOURCE variable=ref_4oPi_table_100 core=ROM_1P_LUTRAM
    ap_int<9>   addr     = exp+(1+4*16);
    assert(addr >= 0);
    ap_uint<100> table_100 = ref_4oPi_table_100[addr >> 4];
    return (table_100 << (ap_uint<4>)(addr(3,0)))(99,100-((2*(SB+1)+p1+3)));
}

/**
 * This function performs a table lookup to get the relevant values
 * of 4oPi given an exponent value for doubles.
 *
 * @param SB Number of mantissa bits (e.g. 53 for double)
 * @param p1 Padding bits
 * @param exp Exponent value
 * @param dummy dummy value to indicate float type.
 */
template<int SB, int p1>
ap_uint<2*(SB+1)+p1+3>
table_lookup_4oPi_hotbm(int exp, double dummy)
{
    ap_uint<2*(SB+1)+p1+3> ret;
    ap_int<12>   addr      = exp+76;
    ap_uint<256> table_256 = ref_4oPi_table_256[addr(10,7)];
    ret                    = (table_256 << (ap_uint<7>)(addr(6,0)))(255,256-((2*(SB+1))+p1+3));
    if(addr < 0) {
        ret = 0;
    }
    return ret;
}

/**
 * Algorithm for trigonometric range reduction, based on the method
 * published by Payne and Hanek.  Basically, this maps an arbitrary
 * range input onto the unit circle.  The output is expressed as the
 * quadrant k and fixed point number in [0,1).
 *
 * The Quadrants k for positive numbers:
 *  2 1
 * 3   0
 * 4   7
 *  5 6
 * The Quadrants k for negative numbers:
 *  5 6
 * 4   7
 * 3   0
 *  2 1
 * NOTE: p1 = 10 for double, p1 = 16 for single
 *
 * @param p1 Padding bits
 * @param T data type (float, double)
 * @param din Unreduced data input (float, double)
 * @param k quadrant of the circle that we are in.
 * @param dout Reduced data output
 */
template<int p1, class T, int W_dout, int W_Mx>
void
range_redux_payne_hanek_hotbm(
    T din,
    ap_uint<3> &k,
    ap_ufixed<W_dout, 0> &dout,
    ap_ufixed<W_Mx, 0> &Mx,
    ap_int<fp_struct<T>::EXP_BITS> &Ex)
{
    const int SB = fp_struct<T>::SIG_BITS;

    fp_struct<T> fps1(din);
    int expv = fps1.expv();
#ifdef DEBUG
    std::cout << "\nrange_redux_payne_hanek_hotbm: expv: " << std::dec << expv
              << ", p1: " << p1 << std::endl;
#endif

    // The primary result of range reduction, with roughly 2*Wf + p1 fractional bits.
    // The 3 least significant integer bits indicate the quadrant of the result.
    ap_ufixed<3*(SB+1)+p1+3,SB+1+3>        h_fixed;

    bool closepath = ((fps1.exp) < fp_struct<T>::EXP_BIAS-1);

    if(closepath) {
        Ex = expv+2;
        expv = -2;
    } else {
        Ex = 0;
    }

    {
        ap_uint<SB+1>                 X;        // mantissa (din) in integer format
        ap_uint<2*(SB+1)+p1+3>        Med;      // significant bits of 4oPi
        ap_uint<3*(SB+1)+p1+3>        h;        // X*Med
        // Construct the mantissa, Q1.SIG_BITS
        X(fp_struct<T>::SIG_BITS-1,0) = fps1.sig(fp_struct<T>::SIG_BITS-1,0);
        X[fp_struct<T>::SIG_BITS] = 1;
        Med = table_lookup_4oPi_hotbm<SB,p1>(expv,din);
        h = Med * X;
        //big_mult(Med, X, h);
        //reinterpret bits of h.	
        h_fixed(h_fixed.wl()-1, 0) = h(h_fixed.wl()-1, 0);
#ifdef DEBUG
        std::cout << "Med = " << Med.to_string(2) << "\n";
        std::cout << "X = " << X.to_string(2) << "\n";
        std::cout << "h = " << h.to_string(2) << " " << h << "\n";
        std::cout << "h_fixed = " << h_fixed.to_string(2) << " " << h_fixed << "\n";
        std::cout << "closepath = " << closepath << "\n";
#endif

    }

    // W_Mx is approximately Wf+g.
    ap_ufixed<W_Mx+value_list<T>::p1, 0> Mx_bits = h_fixed; // Mx_bits is not normalized !
    if(closepath) {
        k = 0;
    } else {
        k = h_fixed; // Extract the quadrant from the integer bits of h_fixed.
    }

    // If we're in an odd quadrant, then the base functions need to be flipped.
    if(k[0] == 1) {
        Mx_bits = 1-Mx_bits;
    }

    ap_uint<BitWidth<value_list<T>::p1>::Value> Mx_zeros = clz<value_list<T>::p1>(Mx_bits);

#ifdef DEBUG
    std::cout << "Mx_bits = " << Mx_bits << " " << Mx_bits.to_string(2) << "\n";
    std::cout << "Mx_zeros = " << Mx_zeros << "\n";
    std::cout << "Mx has " << Mx_bits.countLeadingZeros() << " leading zeros\n";
#endif
    // Can't round here, because Mx_bits might round up to one.
    // Probably better to just add more guard bits.
    Mx = ap_ufixed<W_Mx, 0>(Mx_bits << Mx_zeros);
    Ex -= Mx_zeros;
    dout = ap_ufixed<W_dout, 0>(Mx << Ex); // A negative shift!

#ifdef DEBUG
    std::cout << "k = " << k << "\n";
    std::cout << "dout = " << dout << " " << dout.to_string(2) << "\n";
    std::cout << "Mx = " << Mx << " " << Mx.to_string(2) << "\n";
    std::cout << "Ex = " << Ex << "\n";
    std::cout << "Mx has " << Mx.countLeadingZeros() << " leading zeros\n";
#endif
}

/**
 * Algorithm for trigonometric range reduction, based on the method
 * published by Payne and Hanek.  Basically, this maps an arbitrary
 * range input onto the unit circle.  The output is expressed as the
 * quadrant k and fixed point number in [0,1).
 *
 * The Quadrants k for positive numbers:
 *  2 1
 * 3   0
 * 4   7
 *  5 6
 * The Quadrants k for negative numbers:
 *  5 6
 * 4   7
 * 3   0
 *  2 1
 * NOTE: p1 = 10 for double, p1 = 16 for single
 *
 * @param p1 Padding bits
 * @param T data type (float, double)
 * @param din Unreduced data input (float, double)
 * @param k quadrant of the circle that we are in.
 * @param dout Reduced data output
 */
template<int p1, class T, int W_dout, int W_Mx>
void
range_redux_payne_hanek_hotbm_pi(
    T din,
    ap_uint<3> &k,
    ap_ufixed<W_dout, 0> &dout,
    ap_ufixed<W_Mx, 0> &Mx,
    ap_int<fp_struct<T>::EXP_BITS> &Ex,
    int &exactly_pi_over_2)
{
    const int SB = fp_struct<T>::SIG_BITS;

    fp_struct<T> fps1(din);
    int expv = fps1.expv();
#ifdef DEBUG
    std::cout << "\nrange_redux_payne_hanek_hotbm: expv: " << std::dec << expv
              << ", p1: " << p1 << std::endl;
#endif

    ap_ufixed<3*(SB+1)+p1+3,SB+1+3>        h_fixed;        // X*Med

    bool closepath = ((fps1.exp) < fp_struct<T>::EXP_BIAS-3);

    if(closepath) {
        Ex = expv+4;
        expv = -4;
    } else {
        Ex = 0;
    }

    {
        ap_uint<SB+1>                 X;        // mantissa (din) in integer format
        ap_uint<2*(SB+1)+p1+3>        Med;      // significant bits of 4oPi
        ap_uint<3*(SB+1)+p1+3>        h;        // X*Med
        // Construct the mantissa, Q1.SIG_BITS
        X(fp_struct<T>::SIG_BITS-1,0) = fps1.sig(fp_struct<T>::SIG_BITS-1,0);
        X[fp_struct<T>::SIG_BITS] = 1; // Don't handle subnormals (fps1.exp == 0) ? 0 : 1; // handle subnormals

        h = X;
        h <<= SB+p1+2; //align
        h <<= 2; // Multiply by 4
        h <<= expv;

        //reinterpret bits of h.
        h_fixed(h_fixed.wl()-1, 0) = h(h_fixed.wl()-1, 0);
#ifdef DEBUG
        std::cout << "Med = " << Med.to_string(2) << "\n";
        std::cout << "X = " << X.to_string(2) << "\n";
        std::cout << "h = " << h.to_string(2) << " " << h << "\n";
        std::cout << "h_fixed = " << h_fixed.to_string(2) << " " << h_fixed << "\n";
        std::cout << "closepath = " << closepath << "\n";
#endif

    }

    ap_ufixed<W_Mx+value_list<T>::p1, 1> Mx_bits_extended = h_fixed; // Mx_bits is not normalized !
    exactly_pi_over_2 = (Mx_bits_extended == 1);

    ap_ufixed<W_Mx+value_list<T>::p1, 0> Mx_bits = h_fixed; // Mx_bits is not normalized !

    if(closepath) {
        k = 0;
    } else {
        k = h_fixed; // Extract the quadrant from the integer bits of h_fixed.
    }

    // If we're in an odd quadrant, then the base functions need to be flipped.
    if(k[0] == 1) {
        Mx_bits = 1-Mx_bits;
    }

    ap_uint<BitWidth<value_list<T>::p1>::Value> Mx_zeros = clz<value_list<T>::p1>(Mx_bits);

#ifdef DEBUG
    std::cout << "Mx_bits = " << Mx_bits << " " << Mx_bits.to_string(2) << "\n";
    std::cout << "Mx_zeros = " << Mx_zeros << "\n";
    std::cout << "Mx has " << Mx_bits.countLeadingZeros() << " leading zeros\n";
#endif
    // Can't round here, because Mx_bits might round up to one.
    // Probably better to just add more guard bits.
    Mx = ap_ufixed<W_Mx, 0>(Mx_bits << Mx_zeros);
    Ex -= Mx_zeros;
    dout = ap_ufixed<W_dout, 0>(Mx << Ex); // A negative shift!

#ifdef DEBUG
    std::cout << "k = " << k << "\n";
    std::cout << "dout = " << dout << " " << dout.to_string(2) << "\n";
    std::cout << "Mx = " << Mx << " " << Mx.to_string(2) << "\n";
    std::cout << "Ex = " << Ex << "\n";
    std::cout << "Mx has " << Mx.countLeadingZeros() << " leading zeros\n";
#endif
}

template<class T>
void sincos(T t_in, T *s, T *c)
{

    const int g = value_list<T>::g;
    const int p1 = value_list<T>::p1;

    const bool swap_table[8] = {0,1,1,0,0,1,1,0};
    const bool neg_sin_table[16] = {0,0,1,0,1,1,0,1, 1,0,1,1,0,1,0,0};
    const bool neg_cos_table[16] = {0,0,0,1,1,1,1,0, 0,1,1,1,1,0,0,0};
#pragma HLS array_partition variable=swap_table complete
#pragma HLS array_partition variable=neg_sin_table complete
#pragma HLS array_partition variable=neg_cos_table complete
// #pragma HLS RESOURCE variable=swap_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_sin_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_cos_table core=ROM_1P_LUTRAM

    fp_struct<T> din(t_in);

#ifdef DEBUG
    std::cout << "sin_cos: t_in = " << t_in << "\n";
#endif

    ap_uint<3> k;
    ap_ufixed<fp_struct<T>::SIG_BITS+1+g, 0> x;
    ap_ufixed<fp_struct<T>::SIG_BITS+1+g, 0> Mx;
    ap_int<fp_struct<T>::EXP_BITS> Ex;

    range_redux_payne_hanek_hotbm<p1,T>(t_in,k,x,Mx,Ex);

    ap_ufixed<value_list<T>::W_Mx, 1> sin_result, cos_result;
    value_list<T>::sincos_approximation(k,x,Mx,Ex,sin_result,cos_result);

    T sin_resultf, cos_resultf;
    my_to_ieee(cos_result, cos_resultf, 0);
    my_to_ieee(sin_result, sin_resultf, -Ex);

    // Negate and swap the results, based on which quadrant we're in.
    // This decision is done using some tables.  We use the second
    // half of each table if the result is negative, which means
    // everything is quadrant swapped.

    ap_uint<4> index = k;
    index[3] = din.sign;
    fp_struct<T> sin_results(sin_resultf);
    fp_struct<T> cos_results(cos_resultf);

    if(neg_sin_table[index]) { sin_results.sign = 1; }
    if(neg_cos_table[index]) { cos_results.sign = 1; }

    float s_out, c_out;
    if(din.exp == 0 && din.sig == 0) { // t_in == 0.0
        s_out = ::hls::copysign((T)0.0, t_in);
        c_out = (T)1.0;
    } else if(::hls::__isinf(t_in) || ::hls::__isnan(t_in)) {//din.exp == 0x7FF) { // +inf, -inf, nan
        s_out = ::hls::nan("");
        c_out = ::hls::nan("");
    } else {
        if(swap_table[k]) {
            s_out = cos_results.to_ieee();
            c_out = sin_results.to_ieee();
        } else {
            s_out = sin_results.to_ieee();
            c_out = cos_results.to_ieee();
        }
    }
    *s = s_out;
    *c = c_out;
}

static void sincosf(float t_in, float *s, float *c) {
    return sincos(t_in, s, c);
}


template<class T>
T sin_or_cos(T t_in, bool do_cos, bool do_pi)
{

    const int g = value_list<T>::g;
    const int p1 = value_list<T>::p1;

    const bool swap_table[8] = {0,1,1,0,0,1,1,0};
    const bool neg_sin_table[16] = {0,0,1,0,1,1,0,1, 1,0,1,1,0,1,0,0};
    const bool neg_cos_table[16] = {0,0,0,1,1,1,1,0, 0,1,1,1,1,0,0,0};
	
#pragma HLS array_partition variable=swap_table complete
#pragma HLS array_partition variable=neg_sin_table complete
#pragma HLS array_partition variable=neg_cos_table complete

    fp_struct<T> din(t_in);


#ifdef DEBUG
    std::cout << "sin_cos: t_in = " << t_in << "\n";
#endif

    ap_uint<3> k;
    ap_ufixed<fp_struct<T>::SIG_BITS+1+g, 0> x;
    ap_ufixed<fp_struct<T>::SIG_BITS+1+g, 0> Mx;
    ap_int<fp_struct<T>::EXP_BITS> Ex;

    int exactly_pi_over_2 = 0;
    if(do_pi) {
        range_redux_payne_hanek_hotbm_pi<p1,T>(t_in,k,x,Mx,Ex,exactly_pi_over_2);
    } else {
        range_redux_payne_hanek_hotbm<p1,T>(t_in,k,x,Mx,Ex);
    }

    bool cos_basis = do_cos?!swap_table[k]: swap_table[k];
    if(cos_basis) {
        // In this case, we are using cos() as the basis, so we don't
        // multiply by Mx*2^Ex.
        Mx = ap_ufixed<value_list<T>::W_dout, 0, AP_RND, AP_SAT>(1);
        Ex = 0;
    }

    ap_ufixed<value_list<T>::W_dout, 1> result = value_list<T>::sin_or_cos_approximation(do_cos,k,x,Mx,Ex);
    T resultf;
    my_to_ieee(result, resultf, -Ex);

    //bool sin_basis = do_cos? swap_table[k]:!swap_table[k];

    if(exactly_pi_over_2) {
        resultf = 0.7071067811865475244008443;
    }
	fp_struct<T> results(resultf);

    // Negate and swap the results, based on which quadrant we're in.
    // This decision is done using some tables.  We use the second
    // half of each table if the result is negative, which means
    // everything is quadrant swapped.
    ap_uint<4> index = k;
    index[3] = din.sign;
    if(cos_basis ? neg_cos_table[index] : neg_sin_table[index]) {
        results.sign = 1;
    }

    if(din.exp == 0 && (do_pi ? 1 : din.sig == 0)) { // t_in == 0.0
        // Note that handling subnormals correctly for sinpif/cospif is harder
        // than sinf/cosf.  So for cospif and sinpif, we do flush-to-zero
        // for all subnormals.
        T one = 1.0;
        T zero = 0.0;
        return do_cos ? one : ::hls::copysign(zero, t_in);
    } else if(::hls::__isinf(t_in) || ::hls::__isnan(t_in)) {//din.exp == 0x7FF) { // +inf, -inf, nan
        return ::hls::nan("");
    } else {
        return results.to_ieee();
    }
}
    /*
template<int WO, int WI>
ap_fixed<WO,2>
    sinf_or_cosf(ap_fixed<32, WI> t_in, bool do_cos, bool do_pi)
{
    const int g = 7;

    const bool swap_table[8] = {0,1,1,0,0,1,1,0};
    const bool neg_sin_table[16] = {0,0,1,0,1,1,0,1, 1,0,1,1,0,1,0,0};
    const bool neg_cos_table[16] = {0,0,0,1,1,1,1,0, 0,1,1,1,1,0,0,0};
#pragma HLS array_partition variable=swap_table complete
#pragma HLS array_partition variable=neg_sin_table complete
#pragma HLS array_partition variable=neg_cos_table complete
// #pragma HLS RESOURCE variable=swap_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_sin_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_cos_table core=ROM_1P_LUTRAM

#ifdef DEBUG
    std::cout << "sin_cos: t_in = " << t_in.to_string(2) << "\n";
#endif

    ap_uint<3> k;
    ap_ufixed<WO+1+g, 0> x;
    ap_ufixed<WO+1+g, 0> Mx;
    ap_int<fp_struct<float>::EXP_BITS> Ex = 0;

    int exactly_pi_over_2 = 0;
    bool is_negative = t_in[t_in.wl()-1];
    ap_ufixed<32, WI> din = is_negative ? ap_fixed<32, WI>(-t_in) : t_in;

    if(do_pi) {
        k = din*4;
        x = din*4;
        ap_ufixed<32, 1> Mx_bits_extended = din*4;
#ifdef DEBUG
        std::cout << "sin_cos: Mx_bits_extended = " << Mx_bits_extended.to_string(2) << "\n";
#endif
        exactly_pi_over_2 = (Mx_bits_extended == 1);
    } else {
        k = din * ap_ufixed<32,2>(4/M_PI);
        x = din * ap_ufixed<32,2>(4/M_PI);
    }
#ifdef DEBUG
        std::cout << "sin_cos: k = " << k.to_string(2) << "\n";
#endif
    // If we're in an odd quadrant, then the base functions need to be flipped.
    if(k[0] == 1) {
        x = 1-x;
    }
    Mx = x;

    bool cos_basis = do_cos?!swap_table[k]: swap_table[k];
    if(cos_basis) {
        // In this case, we are using cos() as the basis, so we don't
        // multiply by Mx*2^Ex.
        Mx = ap_ufixed<value_list<float>::W_dout, 0, AP_RND, AP_SAT>(1);
        Ex = 0;
    }

    ap_ufixed<WO+1+g, 1> result = value_list<float>::sin_or_cos_approximation(do_cos,k,x,Mx,Ex);

    if(exactly_pi_over_2) {
        result = 0.7071067811865475244008443;
    }

    // Negate and swap the results, based on which quadrant we're in.
    // This decision is done using some tables.  We use the second
    // half of each table if the result is negative, which means
    // everything is quadrant swapped.

    ap_uint<4> index = k;
    index[3] = is_negative;
#ifdef DEBUG
    std::cout << "sin_cos: cos_basis = " << cos_basis << "\n";
    std::cout << "sin_cos: index = " << index << "\n";
#endif

    if(cos_basis ? neg_cos_table[index] : neg_sin_table[index]) {
        return -result;
    } else {
        return result;
    }
}
    */
template<int W, int I>
ap_fixed<W,2> // Result has same number of fractional bits as the input.
sinf_or_cosf(ap_fixed<W,I> t_in, bool do_cos, bool do_pi, typename enable_if<(W > 16) && (W <= 32), bool>::type dummy = true)
{
    const int WO = W;
    const int g = 3;

    const bool swap_table[8] = {0,1,1,0,0,1,1,0};
    const bool neg_sin_table[16] = {0,0,1,0,1,1,0,1, 1,0,1,1,0,1,0,0};
    const bool neg_cos_table[16] = {0,0,0,1,1,1,1,0, 0,1,1,1,1,0,0,0};
#pragma HLS array_partition variable=swap_table complete
#pragma HLS array_partition variable=neg_sin_table complete
#pragma HLS array_partition variable=neg_cos_table complete
// #pragma HLS RESOURCE variable=swap_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_sin_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_cos_table core=ROM_1P_LUTRAM

#ifdef DEBUG
    std::cout << "sin_cos: t_in = " << t_in.to_string(2) << "\n";
#endif

    ap_uint<3> k;
    ap_ufixed<WO+1+g, 0> x;
    ap_ufixed<WO+1+g, 0> Mx;
    ap_int<fp_struct<float>::EXP_BITS> Ex = 0;

    int exactly_pi_over_4 = 0;
    bool is_negative = t_in[t_in.wl()-1];
    ap_ufixed<W, I> din = is_negative ? ap_fixed<W, I>(-t_in) : t_in;

    if(do_pi) {
        k = din*4;
        x = din*4;
        ap_ufixed<W, 1> Mx_bits_extended = din*4;
#ifdef DEBUG
        std::cout << "sin_cos: Mx_bits_extended = " << Mx_bits_extended.to_string(2) << "\n";
#endif
        exactly_pi_over_4 = (Mx_bits_extended == 1);
    } else {
        ap_ufixed<WO+1+g+3, 3> Wx = din * ap_ufixed<W+1+g,2>(4/M_PI);
        k = Wx;
        x = Wx;
    }
#ifdef DEBUG
        std::cout << "sin_cos: k = " << k.to_string(2) << "\n";
#endif
    // If we're in an odd quadrant, then the base functions need to be flipped.
    if(k[0] == 1) {
        x = 1-x;
    }
    Mx = x;

    bool cos_basis = do_cos?!swap_table[k]: swap_table[k];
    if(cos_basis) {
        // In this case, we are using cos() as the basis, so we don't
        // multiply by Mx*2^Ex.
        Mx = ap_ufixed<value_list<float>::W_dout, 0, AP_RND, AP_SAT>(1);
        Ex = 0;
    }

    ap_ufixed<WO+1+g, 1> result = value_list<float>::sin_or_cos_approximation(do_cos,k,x,Mx,Ex);

    if(exactly_pi_over_4) {
        result = 0.7071067811865475244008443;
    }

    // Negate and swap the results, based on which quadrant we're in.
    // This decision is done using some tables.  We use the second
    // half of each table if the result is negative, which means
    // everything is quadrant swapped.

    ap_uint<4> index = k;
    index[3] = is_negative;
#ifdef DEBUG
    std::cout << "sin_cos: cos_basis = " << cos_basis << "\n";
    std::cout << "sin_cos: index = " << index << "\n";
#endif

    if(cos_basis ? neg_cos_table[index] : neg_sin_table[index]) {
        return -result;
    } else {
        return result;
    }
}

    struct sin_cos_fixed_16 {
    typedef half T;
	static const int g = 3;
	static const int p1 = 12;
	static const int alpha = 8;
	static const int beta1 = 4;

    template <int W_dout>
    static ap_ufixed<W_dout, 1> sin_or_cos_approximation(bool do_cos,
                                               ap_uint<2> &k,
                                               ap_ufixed<W_dout, 0> &x){

#pragma HLS RESOURCE variable=first_order_fixed_16::sin_cos_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=first_order_fixed_16::sin_cos_K1 core=ROM_1P_LUTRAM

        ap_uint<alpha> A = x(x.wl()-x.iwl()-1,x.wl()-x.iwl()-alpha);
        ap_ufixed<W_dout-alpha-beta1, -alpha> B = x;

        ap_fixed<W_dout, 1> t1 = first_order_fixed_16::sin_cos_K0[A];
        ap_fixed<W_dout, 1> t2 = B*first_order_fixed_16::sin_cos_K1[A];
        ap_ufixed<W_dout, 1> result = (t1+t2);

#ifdef DEBUG
        std::cout << "flipped_x = " << x << " " << x.to_string(2) << "\n";
        std::cout << "A = " << A.to_string(2) << "\n";
        std::cout << "B = " << B.to_string(2) << "\n";
        std::cout << "t1 = " << t1.to_string(2) << "\n";
        std::cout << "t2 = " << t2.to_string(2) << "\n";
        std::cout << "result = " << result.to_string(2) << " " << result << "\n";
#endif
        return result;
    }
};


/*
 * The Quadrants k for positive numbers:
 *  1 0
 *  2 3
 * The Quadrants k for negative numbers:
 *  2 3
 *  1 0
 */
template<int W, int I>
ap_fixed<W,2> // Result has same number of fractional bits as the input.
sinf_or_cosf(ap_fixed<W,I> t_in, bool do_cos, bool do_pi, typename enable_if<W <= 16, bool>::type dummy = true)
{
    const int WO = W;
    const int g = 4;

    const bool neg_sin_table[8] = {0,0,1,1, 1,1,0,0};
    const bool neg_cos_table[8] = {0,1,1,0, 0,1,1,0};
#pragma HLS array_partition variable=neg_sin_table complete
#pragma HLS array_partition variable=neg_cos_table complete
// #pragma HLS RESOURCE variable=swap_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_sin_table core=ROM_1P_LUTRAM
// #pragma HLS RESOURCE variable=neg_cos_table core=ROM_1P_LUTRAM

#ifdef DEBUG
    std::cout << "sin_cos: t_in = " << t_in.to_string(2) << "\n";
#endif

    ap_uint<2> k;
    ap_ufixed<WO+1+g, 0> x;

    bool is_negative = t_in[t_in.wl()-1];
    ap_ufixed<W, I> din = is_negative ? ap_fixed<W, I>(-t_in) : t_in;

    if(do_pi) {
        k = din*2;
        x = din*2;
        ap_ufixed<W, 1> Mx_bits_extended = din*2;
#ifdef DEBUG
        std::cout << "sin_cos: Mx_bits_extended = " << Mx_bits_extended.to_string(2) << "\n";
#endif
    } else {
        ap_ufixed<WO+1+g+2, 2> Wx = din * ap_ufixed<W+1+g,2>(2/M_PI);
        k = Wx;
        x = Wx;
    }
#ifdef DEBUG
    std::cout << "sin_cos: k = " << k.to_string(2) << "\n";
#endif
    bool is_special_case = false;
    // If we're in an odd quadrant, then the base functions need to be flipped.
    if((k[0] == 1) ^ !do_cos) {
        // The approximation is designed on the interval [0,1), and x has no integer bits.
        // If x == 0, then 1-x = 1 and we treat it as a special case later.
        ap_ufixed<WO+1+g+1, 1> Wx = 1-x;
        x = Wx;
        is_special_case = (Wx[Wx.wl()-1] == 1);
    }

    ap_ufixed<WO+1+g, 1> result = sin_cos_fixed_16::sin_or_cos_approximation(do_cos,k,x);

    if(is_special_case) {
        ap_uint<3> index = k;
        index[2] = do_cos;
        const bool special_case_table[8] = {0,1,0,1,1,0,1,0};
        result(result.wl()-2,0) = 0;
        result[result.wl()-1] = special_case_table[index];
    }

    // Negate and swap the results, based on which quadrant we're in.
    // This decision is done using some tables.  We use the second
    // half of each table if the result is negative, which means
    // everything is quadrant swapped.

    {
        ap_uint<3> index = k;
        index[2] = is_negative;
#ifdef DEBUG
        std::cout << "sin_cos: index = " << index << "\n";
#endif
        if(do_cos ? neg_cos_table[index] : neg_sin_table[index]) {
            return -result;
        } else {
            return result;
        }
    }
}

template<class T> ap_ufixed<31,1> tanh_approximation(ap_ufixed<31, 0> &dout)
{
    	const int g = value_list<T>::g;
	const int alpha = 9;
	const int beta1 = 0; 
        const int beta2 = 0;
#pragma HLS RESOURCE variable=second_tanh_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=second_tanh_K1 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=second_tanh_K2 core=ROM_1P_LUTRAM

    ap_uint<alpha> A = dout(dout.wl()-dout.iwl()-1,dout.wl()-dout.iwl()-alpha);

    ap_ufixed<fp_struct<T>::SIG_BITS+g+1-alpha-beta1, -alpha> B = dout;

    ap_ufixed<fp_struct<T>::SIG_BITS+g+1-alpha-beta2, -alpha> B_trunc = B;
    ap_ufixed<fp_struct<T>::SIG_BITS+g+1-2*alpha, -2*alpha> B_squared = B_trunc*B_trunc;

    ap_fixed<fp_struct<T>::SIG_BITS+g+1, 1> t1 = second_tanh_K0[A];
    ap_fixed<fp_struct<T>::SIG_BITS+g+1, 1> t2 = B*second_tanh_K1[A];
    ap_fixed<fp_struct<T>::SIG_BITS+g+1, 1> t3 = B_squared*second_tanh_K2[A];
    ap_ufixed<fp_struct<T>::SIG_BITS+g+1, 1> result =t1+t2+t3;

#ifdef DEBUG
    std::cout << "flipped_x         =" << dout<<" "<<dout.to_string(2) <<"\n";
    std::cout << "A = " << A.to_string(2) << "\n";
    std::cout << "B         = " << B.to_string(2) << "\n";
    std::cout << "B_trunc         = " << B_trunc.to_string(2) << "\n";
    std::cout << "B*B         = " << (B*B).to_string(2) << "\n";
    std::cout << "B_squared         = " << B_squared.to_string(2) << "\n";
    std::cout << "t1 =" << t1.to_string(2) << "\n";
    std::cout << "t2 = " << t2.to_string(2) << "\n";
    std::cout << "t3 = " << t3.to_string(2) << "\n";
    std::cout << "t1 =" << t1 << "\n";
    std::cout << "t2 = " << t2 << "\n";
    std::cout << "t3 =" << t3 << "\n";
    std::cout << "result = " << result.to_string(2) << "\n";
    std::cout << "result = " << result<< "\n";
#endif

    return result;
}

template<class T> ap_ufixed<15,1> tanh_approximation(ap_ufixed<15, 0> &dout)
{
    	const int g = value_list<T>::g;
	const int alpha = 9;
	const int beta1 = 0; 
#pragma HLS RESOURCE variable=first_tanh_K0 core=ROM_1P_LUTRAM
#pragma HLS RESOURCE variable=first_tanh_K1 core=ROM_1P_LUTRAM

    ap_uint<alpha> A = dout(dout.wl()-dout.iwl()-1,dout.wl()-dout.iwl()-alpha);

    ap_ufixed<fp_struct<T>::SIG_BITS+g+1-alpha-beta1, -alpha> B = dout;
    ap_fixed<fp_struct<T>::SIG_BITS+g+1, 1> t1 = second_tanh_K0[A];
    ap_fixed<fp_struct<T>::SIG_BITS+g+1, 1> t2 = B*second_tanh_K1[A];
    ap_ufixed<fp_struct<T>::SIG_BITS+g+1, 1> result =t1+t2;

#ifdef DEBUG
    std::cout << "flipped_x         =" << dout<<" "<<dout.to_string(2) <<"\n";
    std::cout << "A = " << A.to_string(2) << "\n";
    std::cout << "B         = " << B.to_string(2) << "\n";
    std::cout << "t1 =" << t1.to_string(2) << "\n";
    std::cout << "t2 = " << t2.to_string(2) << "\n";
    std::cout << "t1 =" << t1 << "\n";
    std::cout << "t2 = " << t2 << "\n";
    std::cout << "result = " << result.to_string(2) << "\n";
    std::cout << "result = " << result<< "\n";
#endif

    return result;
}


static
float
cosf(float t_in)
{
    return sin_or_cos(t_in, 1, 0);
}

static
float
sinf(float t_in)
{
    return sin_or_cos(t_in, 0, 0);
}

static
float
cospif(float t_in)
{
    return sin_or_cos(t_in, 1, 1);
}

static
float
sinpif(float t_in)
{
    return sin_or_cos(t_in, 0, 1);
}

static
half
half_cos(half t_in)
{
    return sin_or_cos(t_in, 1, 0);
}

static
half
half_sin(half t_in)
{
    return sin_or_cos(t_in, 0, 0);
}

static
half
half_cospi(half t_in)
{
    return sin_or_cos(t_in, 1, 1);
}

static
half
half_sinpi(half t_in)
{
    return sin_or_cos(t_in, 0, 1);
}

static
double
cos(double t_in)
{
    return sin_or_cos(t_in, 1, 0);
}

static
double
sin(double t_in)
{
    return sin_or_cos(t_in, 0, 0);
}

static
double
cospi(double t_in)
{
    return sin_or_cos(t_in, 1, 1);
}

static
double
sinpi(double t_in)
{
    return sin_or_cos(t_in, 0, 1);
}

template<int W, int I>
ap_fixed<W, 2> cosf(ap_fixed<W, I> t_in)
{
    return sinf_or_cosf<W>(t_in, 1, 0);
}
template<int W, int I>
ap_fixed<W, 2> sinf(ap_fixed<W, I> t_in)
{
    return sinf_or_cosf<W>(t_in, 0, 0);
}

template<int W, int I>
ap_fixed<W, 2> cospif(ap_fixed<W, I> t_in)
{
    return sinf_or_cosf<W>(t_in, 1, 1);
}
template<int W, int I>
ap_fixed<W, 2> sinpif(ap_fixed<W, I> t_in)
{
    return sinf_or_cosf<W>(t_in, 0, 1);
}


}


/* @}*/

#endif


// XSIP watermark, do not delete 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
