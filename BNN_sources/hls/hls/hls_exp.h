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
 *     (c) Copyright 2016 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/
/* File: hls_exp.h
 *
 * Description:
 *     exponential function based on the algorithm introduced in the paper
 *     "A parameterized floating-point exponential function for FPGA"
 *     by J.Detrey and F.d.Dinechin
 *
 */

#ifndef _HLS_EXP_HH
#define _HLS_EXP_HH

namespace exp_reduce {

#include "hls_exp_tables.h"


	template <typename T, typename R>
		void fixed2Float(T in, int exponent, R &result) {
			fp_struct<R> out;
			out.sign = 0;
			out.exp = exponent;

			ap_ufixed<1, -fp_struct<R>::SIG_BITS> RC;
			RC[0] = 1;
			ap_ufixed<3 + fp_struct<R>::SIG_BITS, 2> ain = in;
			if(ain[0]) {
				ain += RC;
			}
			if (ain[ain.wl() - 1]){
				++out.exp;
				ain >>= 1;
			}
			out.sig = ain(ain.wl() - ain.iwl() - 1, ain.wl() - ain.iwl() -
					fp_struct<R>::SIG_BITS);

			result = out.to_ieee();
		}

	template<typename T> class Exp {
		public:
		static const int EXP_FACTOR = 0;
		static const ap_ufixed<1, 1> BASE_FACTOR;// = 0.69314718055994530941723212145818;
	};

	template<typename T> class Exp2 {
		public:
		static const int EXP_FACTOR = -1;
		static const ap_ufixed<1 + fp_struct<T>::SIG_BITS + 5, 1> 
			BASE_FACTOR;// = 0.69314718055994530941723212145818;
	};
	template<typename T> class Exp10 {
		public:
		static const int EXP_FACTOR = 1;
		static const ap_ufixed<1 + fp_struct<T>::SIG_BITS + 5, 1> 
			BASE_FACTOR;// = 2.3025850929940456840179914546844;
	};

	template<typename T> const ap_ufixed<1, 1> Exp<T>::BASE_FACTOR = 1;
	template<typename T> const ap_ufixed<1 + fp_struct<T>::SIG_BITS + 5, 1> 
		Exp2<T>::BASE_FACTOR = 0.69314718055994530941723212145818 * 2.0;
	template<typename T> const ap_ufixed<1 + fp_struct<T>::SIG_BITS + 5, 1> 
		Exp10<T>::BASE_FACTOR = 2.3025850929940456840179914546844/2.0;

	template <typename T, int p_bits, int guard_bits, typename S> T exp_fixed(S x, bool sign){
    const static int wf = fp_struct<T>::SIG_BITS;
    const static int we = fp_struct<T>::EXP_BITS;
    const static int e0 = fp_struct<T>::EXP_BIAS;
    ap_int<1 + we> expK = 0 , minK = -e0 , maxK = 1 << (we - 1);
    static const ap_ufixed<wf + we + guard_bits, 0> LOG2 = 0.69314718055994530941723212145;
    static const ap_ufixed<we + 1, 1> LOG2_R = 1.4426950408889634073599246810019;
		T result;
		ap_fixed<1 + wf + we + guard_bits, we + 1> sig = x;
		if (sign) {
			expK = -1;	//used to round expK to minus infinity
		}
		expK += static_cast<ap_int<1 + we> >(static_cast<ap_fixed<we + 5, 2 + we> >(sig) * LOG2_R);
    ap_fixed<1 + wf + guard_bits, 1> frac;
    frac = sig - LOG2 * expK;

		// check frac locating in range 0 to LOG2 
		if(frac < 0) {
      frac += LOG2;
      expK--;
    }
    if(frac >= LOG2){
      frac -= LOG2;
      expK++;
    }
#ifndef STDSUBNORMALS
    if(expK <= minK)
      return 0;
#else
    //TODO this should be checked to accord the definition of subnormal values
    if(expK <= minK)
      return 0;
#endif
    if(expK >= maxK)
      return fp_struct<T>::infinity();

    typedef ap_ufixed<p_bits, 0> type_LUT1;
    typedef ap_ufixed<wf + guard_bits + 1, 1> type_LUT1Out;

    typedef ap_ufixed<wf + guard_bits - 2 * p_bits - 3, -p_bits> type_LUT2;
    typedef ap_ufixed<wf + guard_bits - 2 * p_bits - 2, -2 * p_bits> type_LUT2Out ;

    type_LUT1 y1 = frac;
    ap_uint<p_bits> index1 = y1(y1.wl() - y1.iwl() - 1, 0);
    type_LUT1Out expY1 = exp1_lut_table<type_LUT1,type_LUT1Out,T>::array[index1];

    ap_ufixed<wf + guard_bits - p_bits, -p_bits> y2 = frac;
    type_LUT2 y2_lut = y2;
    ap_uint<wf + guard_bits - 2 * p_bits - 3> index2 = y2_lut(y2_lut.wl() - 1, 0);
    type_LUT2Out expY2 = exp2_lut_table<type_LUT2, type_LUT2Out, T>::array[index2];

    type_LUT1Out sum = expY1 + expY1 * (expY2 + y2);

    fixed2Float(sum, expK + e0, result);
    return result;
	}
  template<typename T, typename Base> T exp_generic(T x) {
    const static int wf = fp_struct<T>::SIG_BITS;
    const static int we = fp_struct<T>::EXP_BITS;
    const static int e0 = fp_struct<T>::EXP_BIAS;
    const static int guard_bits = 5;
    const static int p_bits = wf / 3 + 1;

    fp_struct<T> xs(x);
		typedef ap_ufixed<1 + wf, 1> FxType;
    if(::hls::__isnan(x)){
			return ::hls::nan("");
    }


    int eX = xs.expv() + Base::EXP_FACTOR, boundEx = we - 1;
    // check if the input is too large or too small 
		if (eX >= boundEx) {
      if (xs.sign) {
				return static_cast<T>(0);
			} else
        return fp_struct<T>::infinity();
    }

		// when input is zero
#ifndef STDSUBNORMALS
		if(eX == -e0) {
			return static_cast<T>(1);
		}
#else
    //TODO
		if(eX == -e0) {
			return static_cast<T>(1);
		}
#endif

    ap_fixed<1 + wf + we + guard_bits, we + 1> sig;
    sig(sig.wl() - 1,0) = 0;
    sig[sig.wl() - sig.iwl()] = 1;
    sig(sig.wl() - 1 - sig.iwl(), sig.wl() - sig.iwl() - wf) = xs.sig(wf - 1, 0);
    
		//sig *= static_cast<ap_ufixed<1 + wf + guard_bits, 1> >(Base::BASE_FACTOR);
		sig *= Base::BASE_FACTOR;
		bool sign = false;
		if (xs.sign) {
    	sig = -sig;
			sign = true;
		}
    sig <<= eX;
		return exp_fixed<T, p_bits, guard_bits>(sig, sign);
	}

	template<typename T> T exp(T x) {
		return exp_generic<T, Exp<T> >(x);
	}
	template<typename T> T exp2(T x) {
		return exp_generic<T, Exp2<T> >(x);
	}
	template<typename T> T exp10(T x) {
		return exp_generic<T, Exp10<T> >(x);
	}
// expf passed the rdi regression test
  static float expf(float x) {
    return exp<float>(x);
  }

// exp10f may fail the rdi regression test
  static float exp10f(float x) {
    return exp10<float>(x);
  }
// exp2f may fail the rdi regression test
  static float exp2f(float x) {
    return exp2<float>(x);
  }



  static half half_exp(half x) {
    return exp<half>(x);
  }

  static half half_exp10(half x) {
    return exp10<half>(x); 
  }
  static half half_exp2(half x) {
    return exp2<half>(x); 
	}
}

#endif
