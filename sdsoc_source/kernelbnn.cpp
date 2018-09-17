#include <stdio.h>
#include <stdlib.h>
#include <sds_lib.h>
#include "top.h"

int kernelbnn(
ap_uint<64> * in, ap_uint<64> * out, bool doInit,
unsigned int targetLayer, unsigned int targetMem,
unsigned int targetInd, ap_uint<64> val, unsigned int numReps,unsigned int psi, unsigned int pso, unsigned int core_count)
{
  int k;
  ap_uint<64> * in_async;
  ap_uint<64> * out_async;
  unsigned int numReps_async = numReps/core_count;
  
  in_async = in;
  out_async = out;
  //
 /* if (doInit == 1)
  {
        #pragma SDS resource(1)
	BlackBoxJam(in_async,out_async, doInit,targetLayer,targetMem,targetInd,val,numReps_async,psi,pso); 
        if(core_count > 1)
        {
         #pragma SDS resource(2)
	 BlackBoxJam(in_async,out_async, doInit,targetLayer,targetMem,targetInd,val,numReps_async,psi,pso);
        }
        if(core_count > 2)
        {
         #pragma SDS resource(3)
	 BlackBoxJam(in_async,out_async, doInit,targetLayer,targetMem,targetInd,val,numReps_async,psi,pso);
        }
        if(core_count > 3)
        { 
          #pragma SDS resource(4)
	  BlackBoxJam(in_async,out_async, doInit,targetLayer,targetMem,targetInd,val,numReps_async,psi,pso);
        }
  }
  else*/
  {
     #pragma SDS resource(1)
     #pragma SDS async(1)
     BlackBoxJam(in_async,out_async, doInit,targetLayer,targetMem,targetInd,val,numReps_async,psi,pso);
     if(core_count > 1)
     {
  	in_async = (ap_uint<64> *)(in+psi*numReps_async);
  	out_async = (ap_uint<64> *)(out+pso*numReps_async);
	#pragma SDS resource(2)
	#pragma SDS async(2)
  	BlackBoxJam(in_async,out_async, doInit,targetLayer,targetMem,targetInd,val,numReps_async,psi,pso);
      }
      if(core_count > 2)
      {
  	in_async = (ap_uint<64> *)(in+2*psi*numReps_async);
  	out_async = (ap_uint<64> *)(out+2*pso*numReps_async);
	#pragma SDS resource(3)
	#pragma SDS async(3)
  	BlackBoxJam(in_async,out_async, doInit,targetLayer,targetMem,targetInd,val,numReps_async,psi,pso);
     }
     if(core_count >3)
     {
	in_async = (ap_uint<64> *)(in+3*psi*numReps_async);
        out_async = (ap_uint<64> *)(out+3*pso*numReps_async);
	#pragma SDS resource(4)
	#pragma SDS async(4)
        BlackBoxJam(in_async,out_async, doInit,targetLayer,targetMem,targetInd,val,numReps_async,psi,pso);
     }
  }
  #pragma SDS wait(1)
  if(core_count > 1)
  { 
	 #pragma SDS wait(2)
  }
  if(core_count > 2)
  { 
	  #pragma SDS wait(3)
  }
 if(core_count > 3)
  { 
	  #pragma SDS wait(4)
  }
  return k;
}
