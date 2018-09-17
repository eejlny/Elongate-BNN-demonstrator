
#ifndef __TOP_H__
#define __TOP_H__

#include "ap_int.h"

void BlackBoxJam(ap_uint<64> * in, ap_uint<64> * out, bool doInit,
		unsigned int targetLayer, unsigned int targetMem,
		unsigned int targetInd, ap_uint<64> val, unsigned int numReps,unsigned int psi, unsigned int pso);

#endif //__TOP_H__



