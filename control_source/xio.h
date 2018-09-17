/*****************************************************************************
* Filename:          D:\projects\zynq_ntc\zynq_ntc.srcs\sources_1\edk\system/drivers/me64_v1_00_a/src/me64.h
* Version:           1.00.a
* Description:       me64 Driver Header File
* Date:              Wed Mar 13 14:20:31 2013 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef XIO_H
#define XIO_H

/*
 * Write/Read 32 bit value to/from ME64 user logic memory (BRAM).
 *
 * @param   Address is the memory address of the ME64 device.
 * @param   Data is the value written to user logic memory.
 *
 * @return  The data from the user logic memory.
 *
 * @note
 * C-style signature:
 * 	void ME64_mWriteMemory(Xuint32 Address, Xuint32 Data)
 * 	Xuint32 ME64_mReadMemory(Xuint32 Address)
 *
 */



#define Xil_Out32(Address, Data)\
 	*((volatile unsigned int *) (Address)) = Data;

#define Xil_In32(Address)\
	 *((volatile unsigned int *) (Address));




#endif /** */
