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
/* File: hls_exp_tables.h
 * 
 * Description:
 *     LUTs for exponential functions 
 *     
 */

#ifndef _HLS_EXP_TABLES_HH
#define _HLS_EXP_TABLES_HH
namespace {
	template <typename T, typename R,typename F> class exp1_lut_table {};

	template <typename T, typename R,typename F> class exp2_lut_table {};

	template <> class exp1_lut_table<ap_ufixed<4, 0, (ap_q_mode)5, (ap_o_mode)3, 0>,
					 ap_ufixed<16, 1, (ap_q_mode)5, (ap_o_mode)3, 0>,half > {
						 public:
							 exp1_lut_table<ap_ufixed<4, 0, (ap_q_mode)5, (ap_o_mode)3, 0>,
								 ap_ufixed<16, 1, (ap_q_mode)5, (ap_o_mode)3, 0>,half >();
							 static const ap_ufixed<16, 1, (ap_q_mode)5, (ap_o_mode)3, 0> array[16];

					 };

	const ap_ufixed<16, 1, (ap_q_mode)5, (ap_o_mode)3, 0>
		exp1_lut_table<ap_ufixed<4, 0, (ap_q_mode)5, (ap_o_mode)3, 0>,
		ap_ufixed<16, 1, (ap_q_mode)5, (ap_o_mode)3, 0>,half>:: array[16]={
			0x1,
			0x1.1082p0,
			0x1.2216p0,
			0x1.34cap0,
			0x1.48b4p0,
			0x1.5de8p0,
			0x1.747ap0,
			0x1.8c80p0,
			0x1.a612p0,
			0x1.c14ap0,
			0x1.de44p0,
			0x1.fd1cp0,
			0x0.1df2p0,
			0x0.40e6p0,
			0x0.661cp0,
			0x0.8db8p0,
		};
	template <> class exp2_lut_table<ap_ufixed<4, -4, (ap_q_mode)5, (ap_o_mode)3, 0>,
					 ap_ufixed<5, -8, (ap_q_mode)5, (ap_o_mode)3, 0>,half > {
						 public:
							 exp2_lut_table<ap_ufixed<4, -4, (ap_q_mode)5, (ap_o_mode)3, 0>,
								 ap_ufixed<5, -8, (ap_q_mode)5, (ap_o_mode)3, 0>,half >();
							 static const ap_ufixed<5, -8, (ap_q_mode)5, (ap_o_mode)3, 0> array[16];
					 };

	const ap_ufixed<5, -8, (ap_q_mode)5, (ap_o_mode)3, 0>
		exp2_lut_table<ap_ufixed<4, -4, (ap_q_mode)5, (ap_o_mode)3, 0>,
		ap_ufixed<5, -8, (ap_q_mode)5, (ap_o_mode)3, 0>,half >::array[16]={
			0x0,
			0x0,
			0x0.0002p0,
			0x0.0004p0,
			0x0.0008p0,
			0x0.000cp0,
			0x0.0012p0,
			0x0.0018p0,
			0x0.0020p0,
			0x0.0028p0,
			0x0.0032p0,
			0x0.003cp0,
			0x0.0048p0,
			0x0.0054p0,
			0x0.0062p0,
			0x0.0072p0,
		};
	template <> class exp1_lut_table<ap_ufixed<8, 0, (ap_q_mode)5, (ap_o_mode)3, 0>, 
					 ap_ufixed<29, 1, (ap_q_mode)5, (ap_o_mode)3, 0>, float > {
						 public:
							 exp1_lut_table<ap_ufixed<8, 0, (ap_q_mode)5, (ap_o_mode)3, 0>, 
								 ap_ufixed<29, 1, (ap_q_mode)5, (ap_o_mode)3, 0>,float >();
							 static const ap_ufixed<29, 1, (ap_q_mode)5, (ap_o_mode)3, 0> array[256];
					 };
	const ap_ufixed<29, 1, (ap_q_mode)5, (ap_o_mode)3, 0> exp1_lut_table<ap_ufixed<8, 0, (ap_q_mode)5, (ap_o_mode)3, 0>,
				ap_ufixed<29, 1, (ap_q_mode)5, (ap_o_mode)3, 0>, float >::array[256]={
					0x1,
					0x1.0100802p0,
					0x1.0202015p0,
					0x1.0304848p0,
					0x1.04080abp0,
					0x1.050c94ep0,
					0x1.0612243p0,
					0x1.0718b98p0,
					0x1.0820560p0,
					0x1.0928fa9p0,
					0x1.0a32a84p0,
					0x1.0b3d603p0,
					0x1.0c49236p0,
					0x1.0d55f2dp0,
					0x1.0e63cfap0,
					0x1.0f72badp0,
					0x1.1082b57p0,
					0x1.1193c09p0,
					0x1.12a5dd5p0,
					0x1.13b90cbp0,
					0x1.14cd4fcp0,
					0x1.15e2a7ap0,
					0x1.16f9157p0,
					0x1.18109a3p0,
					0x1.1929370p0,
					0x1.1a42ed0p0,
					0x1.1b5dbd3p0,
					0x1.1c79a8dp0,
					0x1.1d96b0ep0,
					0x1.1eb4d69p0,
					0x1.1fd41afp0,
					0x1.20f47f3p0,
					0x1.2216045p0,
					0x1.2338ab9p0,
					0x1.245c761p0,
					0x1.258164ep0,
					0x1.26a7793p0,
					0x1.27ceb43p0,
					0x1.28f7170p0,
					0x1.2a20a2cp0,
					0x1.2b4b58bp0,
					0x1.2c7739ep0,
					0x1.2da4478p0,
					0x1.2ed282dp0,
					0x1.3001ecfp0,
					0x1.3132871p0,
					0x1.3264526p0,
					0x1.3397502p0,
					0x1.34cb817p0,
					0x1.3600e78p0,
					0x1.373783ap0,
					0x1.386f56fp0,
					0x1.39a862bp0,
					0x1.3ae2a82p0,
					0x1.3c1e287p0,
					0x1.3d5ae4ep0,
					0x1.3e98deap0,
					0x1.3fd8170p0,
					0x1.41188f4p0,
					0x1.425a489p0,
					0x1.439d443p0,
					0x1.44e1838p0,
					0x1.462707bp0,
					0x1.476dd20p0,
					0x1.48b5e3cp0,
					0x1.49ff3e3p0,
					0x1.4b49e2ap0,
					0x1.4c95d26p0,
					0x1.4de30ecp0,
					0x1.4f3198fp0,
					0x1.5081726p0,
					0x1.51d29c4p0,
					0x1.5325180p0,
					0x1.5478e6fp0,
					0x1.55ce0a4p0,
					0x1.5724837p0,
					0x1.587c53cp0,
					0x1.59d57c9p0,
					0x1.5b2fff3p0,
					0x1.5c8bdd0p0,
					0x1.5de9176p0,
					0x1.5f47afap0,
					0x1.60a7a73p0,
					0x1.6208ff6p0,
					0x1.636bb9ap0,
					0x1.64cfd75p0,
					0x1.663559cp0,
					0x1.679c427p0,
					0x1.690492cp0,
					0x1.6a6e4c1p0,
					0x1.6bd96fdp0,
					0x1.6d45ff7p0,
					0x1.6eb3fc5p0,
					0x1.702367ep0,
					0x1.719443ap0,
					0x1.730690ep0,
					0x1.747a513p0,
					0x1.75ef860p0,
					0x1.776630cp0,
					0x1.78de52ep0,
					0x1.7a57edep0,
					0x1.7bd3034p0,
					0x1.7d4f946p0,
					0x1.7ecda2ep0,
					0x1.804d303p0,
					0x1.81ce3dcp0,
					0x1.8350cd3p0,
					0x1.84d4dfep0,
					0x1.865a777p0,
					0x1.87e1955p0,
					0x1.896a3b1p0,
					0x1.8af46a5p0,
					0x1.8c80247p0,
					0x1.8e0d6b1p0,
					0x1.8f9c3fdp0,
					0x1.912ca42p0,
					0x1.92be99ap0,
					0x1.945221dp0,
					0x1.95e73e6p0,
					0x1.977df0ep0,
					0x1.99163adp0,
					0x1.9ab01ddp0,
					0x1.9c4b9b9p0,
					0x1.9de8b59p0,
					0x1.9f876d8p0,
					0x1.a127c50p0,
					0x1.a2c9bdap0,
					0x1.a46d590p0,
					0x1.a61298ep0,
					0x1.a7b97ecp0,
					0x1.a9620c6p0,
					0x1.ab0c437p0,
					0x1.acb8258p0,
					0x1.ae65b44p0,
					0x1.b014f17p0,
					0x1.b1c5debp0,
					0x1.b3787dcp0,
					0x1.b52cd04p0,
					0x1.b6e2d7fp0,
					0x1.b89a968p0,
					0x1.ba540dbp0,
					0x1.bc0f3f3p0,
					0x1.bdcc2ccp0,
					0x1.bf8ad82p0,
					0x1.c14b431p0,
					0x1.c30d6f4p0,
					0x1.c4d15e8p0,
					0x1.c697129p0,
					0x1.c85e8d4p0,
					0x1.ca27d04p0,
					0x1.cbf2dd7p0,
					0x1.cdbfb6ap0,
					0x1.cf8e5d8p0,
					0x1.d15ed3fp0,
					0x1.d3311bcp0,
					0x1.d50536cp0,
					0x1.d6db26dp0,
					0x1.d8b2edbp0,
					0x1.da8c8d4p0,
					0x1.dc68076p0,
					0x1.de455dfp0,
					0x1.e02492cp0,
					0x1.e205a7bp0,
					0x1.e3e89ebp0,
					0x1.e5cd799p0,
					0x1.e7b43a4p0,
					0x1.e99ce2bp0,
					0x1.eb8774bp0,
					0x1.ed73f24p0,
					0x1.ef625d3p0,
					0x1.f152b7ap0,
					0x1.f345035p0,
					0x1.f539424p0,
					0x1.f72f768p0,
					0x1.f927a1ep0,
					0x1.fb21c66p0,
					0x1.fd1de61p0,
					0x1.ff1c02ep0,
					0x0.011c1ecp0,
					0x0.031e3bcp0,
					0x0.05225bep0,
					0x0.0728812p0,
					0x0.0930ad9p0,
					0x0.0b3ae33p0,
					0x0.0d47240p0,
					0x0.0f55722p0,
					0x0.1165cfap0,
					0x0.13783e7p0,
					0x0.158cc0dp0,
					0x0.17a358bp0,
					0x0.19bc083p0,
					0x0.1bd6d17p0,
					0x0.1df3b68p0,
					0x0.2012b99p0,
					0x0.2233dcbp0,
					0x0.2457220p0,
					0x0.267c8bbp0,
					0x0.28a41bdp0,
					0x0.2acdd4ap0,
					0x0.2cf9b83p0,
					0x0.2f27c8cp0,
					0x0.3158088p0,
					0x0.338a799p0,
					0x0.35bf1e3p0,
					0x0.37f5f88p0,
					0x0.3a2f0aep0,
					0x0.3c6a576p0,
					0x0.3ea7e04p0,
					0x0.40e7a7ep0,
					0x0.4329b05p0,
					0x0.456dfc0p0,
					0x0.47b48d1p0,
					0x0.49fd65ep0,
					0x0.4c4888ap0,
					0x0.4e95f7bp0,
					0x0.50e5b55p0,
					0x0.5337c3ep0,
					0x0.558c25ap0,
					0x0.57e2dcfp0,
					0x0.5a3bec2p0,
					0x0.5c97559p0,
					0x0.5ef51bap0,
					0x0.6155409p0,
					0x0.63b7c6ep0,
					0x0.661cb0fp0,
					0x0.6884011p0,
					0x0.6aedb9cp0,
					0x0.6d59dd5p0,
					0x0.6fc86e5p0,
					0x0.72396f0p0,
					0x0.74ace1fp0,
					0x0.7722c9ap0,
					0x0.799b286p0,
					0x0.7c1600cp0,
					0x0.7e93553p0,
					0x0.8113284p0,
					0x0.83957c6p0,
					0x0.861a541p0,
					0x0.88a1b1ep0,
					0x0.8b2b984p0,
					0x0.8db809ep0,
					0x0.9047093p0,
					0x0.92d898dp0,
					0x0.956cbb4p0,
					0x0.9803732p0,
					0x0.9a9cc30p0,
					0x0.9d38ad8p0,
					0x0.9fd7354p0,
					0x0.a2785cdp0,
					0x0.a51c26ep0,
					0x0.a7c2960p0,
					0x0.aa6bacfp0,
					0x0.ad176e4p0,
					0x0.afc5dcbp0,
					0x0.b276faep0,
					0x0.b52acb8p0,
				};

	template <> class exp2_lut_table<ap_ufixed<9, -8, (ap_q_mode)5, (ap_o_mode)3, 0>,
					 ap_ufixed<10, -16, (ap_q_mode)5, (ap_o_mode)3, 0>, float > {
						 public:
							 exp2_lut_table<ap_ufixed<9, -8, (ap_q_mode)5, (ap_o_mode)3, 0>,
								 ap_ufixed<10, -16, (ap_q_mode)5, (ap_o_mode)3, 0>,float >();
							 static const ap_ufixed<10, -16, (ap_q_mode)5, (ap_o_mode)3, 0> array[512];
					 };
	const ap_ufixed<10, -16, (ap_q_mode)5, (ap_o_mode)3, 0> exp2_lut_table<ap_ufixed<9, -8, (ap_q_mode)5, (ap_o_mode)3, 0>,
				ap_ufixed<10, -16, (ap_q_mode)5, (ap_o_mode)3, 0>, float >::array[512]={
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0,
					0x0.0000004p0,
					0x0.0000004p0,
					0x0.0000004p0,
					0x0.0000004p0,
					0x0.0000004p0,
					0x0.0000004p0,
					0x0.0000004p0,
					0x0.0000004p0,
					0x0.0000004p0,
					0x0.0000008p0,
					0x0.0000008p0,
					0x0.0000008p0,
					0x0.0000008p0,
					0x0.0000008p0,
					0x0.0000008p0,
					0x0.0000008p0,
					0x0.0000008p0,
					0x0.000000cp0,
					0x0.000000cp0,
					0x0.000000cp0,
					0x0.000000cp0,
					0x0.000000cp0,
					0x0.000000cp0,
					0x0.0000010p0,
					0x0.0000010p0,
					0x0.0000010p0,
					0x0.0000010p0,
					0x0.0000010p0,
					0x0.0000014p0,
					0x0.0000014p0,
					0x0.0000014p0,
					0x0.0000014p0,
					0x0.0000014p0,
					0x0.0000018p0,
					0x0.0000018p0,
					0x0.0000018p0,
					0x0.0000018p0,
					0x0.000001cp0,
					0x0.000001cp0,
					0x0.000001cp0,
					0x0.000001cp0,
					0x0.0000020p0,
					0x0.0000020p0,
					0x0.0000020p0,
					0x0.0000020p0,
					0x0.0000024p0,
					0x0.0000024p0,
					0x0.0000024p0,
					0x0.0000024p0,
					0x0.0000028p0,
					0x0.0000028p0,
					0x0.0000028p0,
					0x0.0000028p0,
					0x0.000002cp0,
					0x0.000002cp0,
					0x0.000002cp0,
					0x0.0000030p0,
					0x0.0000030p0,
					0x0.0000030p0,
					0x0.0000034p0,
					0x0.0000034p0,
					0x0.0000034p0,
					0x0.0000038p0,
					0x0.0000038p0,
					0x0.0000038p0,
					0x0.000003cp0,
					0x0.000003cp0,
					0x0.000003cp0,
					0x0.0000040p0,
					0x0.0000040p0,
					0x0.0000040p0,
					0x0.0000044p0,
					0x0.0000044p0,
					0x0.0000048p0,
					0x0.0000048p0,
					0x0.0000048p0,
					0x0.000004cp0,
					0x0.000004cp0,
					0x0.000004cp0,
					0x0.0000050p0,
					0x0.0000050p0,
					0x0.0000054p0,
					0x0.0000054p0,
					0x0.0000054p0,
					0x0.0000058p0,
					0x0.0000058p0,
					0x0.000005cp0,
					0x0.000005cp0,
					0x0.0000060p0,
					0x0.0000060p0,
					0x0.0000060p0,
					0x0.0000064p0,
					0x0.0000064p0,
					0x0.0000068p0,
					0x0.0000068p0,
					0x0.000006cp0,
					0x0.000006cp0,
					0x0.0000070p0,
					0x0.0000070p0,
					0x0.0000074p0,
					0x0.0000074p0,
					0x0.0000078p0,
					0x0.0000078p0,
					0x0.000007cp0,
					0x0.000007cp0,
					0x0.0000080p0,
					0x0.0000080p0,
					0x0.0000084p0,
					0x0.0000084p0,
					0x0.0000088p0,
					0x0.0000088p0,
					0x0.000008cp0,
					0x0.000008cp0,
					0x0.0000090p0,
					0x0.0000090p0,
					0x0.0000094p0,
					0x0.0000094p0,
					0x0.0000098p0,
					0x0.0000098p0,
					0x0.000009cp0,
					0x0.000009cp0,
					0x0.00000a0p0,
					0x0.00000a4p0,
					0x0.00000a4p0,
					0x0.00000a8p0,
					0x0.00000a8p0,
					0x0.00000acp0,
					0x0.00000acp0,
					0x0.00000b0p0,
					0x0.00000b4p0,
					0x0.00000b4p0,
					0x0.00000b8p0,
					0x0.00000b8p0,
					0x0.00000bcp0,
					0x0.00000c0p0,
					0x0.00000c0p0,
					0x0.00000c4p0,
					0x0.00000c8p0,
					0x0.00000c8p0,
					0x0.00000ccp0,
					0x0.00000ccp0,
					0x0.00000d0p0,
					0x0.00000d4p0,
					0x0.00000d4p0,
					0x0.00000d8p0,
					0x0.00000dcp0,
					0x0.00000dcp0,
					0x0.00000e0p0,
					0x0.00000e4p0,
					0x0.00000e4p0,
					0x0.00000e8p0,
					0x0.00000ecp0,
					0x0.00000ecp0,
					0x0.00000f0p0,
					0x0.00000f4p0,
					0x0.00000f4p0,
					0x0.00000f8p0,
					0x0.00000fcp0,
					0x0.0000100p0,
					0x0.0000100p0,
					0x0.0000104p0,
					0x0.0000108p0,
					0x0.0000108p0,
					0x0.000010cp0,
					0x0.0000110p0,
					0x0.0000114p0,
					0x0.0000114p0,
					0x0.0000118p0,
					0x0.000011cp0,
					0x0.0000120p0,
					0x0.0000120p0,
					0x0.0000124p0,
					0x0.0000128p0,
					0x0.000012cp0,
					0x0.000012cp0,
					0x0.0000130p0,
					0x0.0000134p0,
					0x0.0000138p0,
					0x0.0000138p0,
					0x0.000013cp0,
					0x0.0000140p0,
					0x0.0000144p0,
					0x0.0000148p0,
					0x0.0000148p0,
					0x0.000014cp0,
					0x0.0000150p0,
					0x0.0000154p0,
					0x0.0000158p0,
					0x0.000015cp0,
					0x0.000015cp0,
					0x0.0000160p0,
					0x0.0000164p0,
					0x0.0000168p0,
					0x0.000016cp0,
					0x0.0000170p0,
					0x0.0000170p0,
					0x0.0000174p0,
					0x0.0000178p0,
					0x0.000017cp0,
					0x0.0000180p0,
					0x0.0000184p0,
					0x0.0000188p0,
					0x0.0000188p0,
					0x0.000018cp0,
					0x0.0000190p0,
					0x0.0000194p0,
					0x0.0000198p0,
					0x0.000019cp0,
					0x0.00001a0p0,
					0x0.00001a4p0,
					0x0.00001a8p0,
					0x0.00001acp0,
					0x0.00001acp0,
					0x0.00001b0p0,
					0x0.00001b4p0,
					0x0.00001b8p0,
					0x0.00001bcp0,
					0x0.00001c0p0,
					0x0.00001c4p0,
					0x0.00001c8p0,
					0x0.00001ccp0,
					0x0.00001d0p0,
					0x0.00001d4p0,
					0x0.00001d8p0,
					0x0.00001dcp0,
					0x0.00001e0p0,
					0x0.00001e4p0,
					0x0.00001e8p0,
					0x0.00001ecp0,
					0x0.00001f0p0,
					0x0.00001f4p0,
					0x0.00001f8p0,
					0x0.00001fcp0,
					0x0.0000200p0,
					0x0.0000204p0,
					0x0.0000208p0,
					0x0.000020cp0,
					0x0.0000210p0,
					0x0.0000214p0,
					0x0.0000218p0,
					0x0.000021cp0,
					0x0.0000220p0,
					0x0.0000224p0,
					0x0.0000228p0,
					0x0.000022cp0,
					0x0.0000230p0,
					0x0.0000234p0,
					0x0.0000238p0,
					0x0.000023cp0,
					0x0.0000240p0,
					0x0.0000244p0,
					0x0.0000248p0,
					0x0.000024cp0,
					0x0.0000250p0,
					0x0.0000254p0,
					0x0.000025cp0,
					0x0.0000260p0,
					0x0.0000264p0,
					0x0.0000268p0,
					0x0.000026cp0,
					0x0.0000270p0,
					0x0.0000274p0,
					0x0.0000278p0,
					0x0.000027cp0,
					0x0.0000280p0,
					0x0.0000288p0,
					0x0.000028cp0,
					0x0.0000290p0,
					0x0.0000294p0,
					0x0.0000298p0,
					0x0.000029cp0,
					0x0.00002a0p0,
					0x0.00002a8p0,
					0x0.00002acp0,
					0x0.00002b0p0,
					0x0.00002b4p0,
					0x0.00002b8p0,
					0x0.00002bcp0,
					0x0.00002c4p0,
					0x0.00002c8p0,
					0x0.00002ccp0,
					0x0.00002d0p0,
					0x0.00002d4p0,
					0x0.00002dcp0,
					0x0.00002e0p0,
					0x0.00002e4p0,
					0x0.00002e8p0,
					0x0.00002ecp0,
					0x0.00002f4p0,
					0x0.00002f8p0,
					0x0.00002fcp0,
					0x0.0000300p0,
					0x0.0000304p0,
					0x0.000030cp0,
					0x0.0000310p0,
					0x0.0000314p0,
					0x0.0000318p0,
					0x0.0000320p0,
					0x0.0000324p0,
					0x0.0000328p0,
					0x0.000032cp0,
					0x0.0000334p0,
					0x0.0000338p0,
					0x0.000033cp0,
					0x0.0000344p0,
					0x0.0000348p0,
					0x0.000034cp0,
					0x0.0000350p0,
					0x0.0000358p0,
					0x0.000035cp0,
					0x0.0000360p0,
					0x0.0000368p0,
					0x0.000036cp0,
					0x0.0000370p0,
					0x0.0000378p0,
					0x0.000037cp0,
					0x0.0000380p0,
					0x0.0000384p0,
					0x0.000038cp0,
					0x0.0000390p0,
					0x0.0000394p0,
					0x0.000039cp0,
					0x0.00003a0p0,
					0x0.00003a8p0,
					0x0.00003acp0,
					0x0.00003b0p0,
					0x0.00003b8p0,
					0x0.00003bcp0,
					0x0.00003c0p0,
					0x0.00003c8p0,
					0x0.00003ccp0,
					0x0.00003d0p0,
					0x0.00003d8p0,
					0x0.00003dcp0,
					0x0.00003e4p0,
					0x0.00003e8p0,
					0x0.00003ecp0,
					0x0.00003f4p0,
					0x0.00003f8p0,
					0x0.0000400p0,
					0x0.0000404p0,
					0x0.000040cp0,
					0x0.0000410p0,
					0x0.0000414p0,
					0x0.000041cp0,
					0x0.0000420p0,
					0x0.0000428p0,
					0x0.000042cp0,
					0x0.0000434p0,
					0x0.0000438p0,
					0x0.000043cp0,
					0x0.0000444p0,
					0x0.0000448p0,
					0x0.0000450p0,
					0x0.0000454p0,
					0x0.000045cp0,
					0x0.0000460p0,
					0x0.0000468p0,
					0x0.000046cp0,
					0x0.0000474p0,
					0x0.0000478p0,
					0x0.0000480p0,
					0x0.0000484p0,
					0x0.000048cp0,
					0x0.0000490p0,
					0x0.0000498p0,
					0x0.000049cp0,
					0x0.00004a4p0,
					0x0.00004a8p0,
					0x0.00004b0p0,
					0x0.00004b4p0,
					0x0.00004bcp0,
					0x0.00004c4p0,
					0x0.00004c8p0,
					0x0.00004d0p0,
					0x0.00004d4p0,
					0x0.00004dcp0,
					0x0.00004e0p0,
					0x0.00004e8p0,
					0x0.00004ecp0,
					0x0.00004f4p0,
					0x0.00004fcp0,
					0x0.0000500p0,
					0x0.0000508p0,
					0x0.000050cp0,
					0x0.0000514p0,
					0x0.000051cp0,
					0x0.0000520p0,
					0x0.0000528p0,
					0x0.000052cp0,
					0x0.0000534p0,
					0x0.000053cp0,
					0x0.0000540p0,
					0x0.0000548p0,
					0x0.000054cp0,
					0x0.0000554p0,
					0x0.000055cp0,
					0x0.0000560p0,
					0x0.0000568p0,
					0x0.0000570p0,
					0x0.0000574p0,
					0x0.000057cp0,
					0x0.0000584p0,
					0x0.0000588p0,
					0x0.0000590p0,
					0x0.0000598p0,
					0x0.000059cp0,
					0x0.00005a4p0,
					0x0.00005acp0,
					0x0.00005b0p0,
					0x0.00005b8p0,
					0x0.00005c0p0,
					0x0.00005c4p0,
					0x0.00005ccp0,
					0x0.00005d4p0,
					0x0.00005dcp0,
					0x0.00005e0p0,
					0x0.00005e8p0,
					0x0.00005f0p0,
					0x0.00005f4p0,
					0x0.00005fcp0,
					0x0.0000604p0,
					0x0.000060cp0,
					0x0.0000610p0,
					0x0.0000618p0,
					0x0.0000620p0,
					0x0.0000628p0,
					0x0.000062cp0,
					0x0.0000634p0,
					0x0.000063cp0,
					0x0.0000644p0,
					0x0.000064cp0,
					0x0.0000650p0,
					0x0.0000658p0,
					0x0.0000660p0,
					0x0.0000668p0,
					0x0.000066cp0,
					0x0.0000674p0,
					0x0.000067cp0,
					0x0.0000684p0,
					0x0.000068cp0,
					0x0.0000690p0,
					0x0.0000698p0,
					0x0.00006a0p0,
					0x0.00006a8p0,
					0x0.00006b0p0,
					0x0.00006b8p0,
					0x0.00006bcp0,
					0x0.00006c4p0,
					0x0.00006ccp0,
					0x0.00006d4p0,
					0x0.00006dcp0,
					0x0.00006e4p0,
					0x0.00006ecp0,
					0x0.00006f0p0,
					0x0.00006f8p0,
					0x0.0000700p0,
					0x0.0000708p0,
					0x0.0000710p0,
					0x0.0000718p0,
					0x0.0000720p0,
					0x0.0000728p0,
					0x0.000072cp0,
					0x0.0000734p0,
					0x0.000073cp0,
					0x0.0000744p0,
					0x0.000074cp0,
					0x0.0000754p0,
					0x0.000075cp0,
					0x0.0000764p0,
					0x0.000076cp0,
					0x0.0000774p0,
					0x0.000077cp0,
					0x0.0000784p0,
					0x0.000078cp0,
					0x0.0000790p0,
					0x0.0000798p0,
					0x0.00007a0p0,
					0x0.00007a8p0,
					0x0.00007b0p0,
					0x0.00007b8p0,
					0x0.00007c0p0,
					0x0.00007c8p0,
					0x0.00007d0p0,
					0x0.00007d8p0,
					0x0.00007e0p0,
					0x0.00007e8p0,
					0x0.00007f0p0,
					0x0.00007f8p0,
				};
}
#endif // !