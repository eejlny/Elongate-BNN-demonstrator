# Elongate BNN demonstrator

This repository contains files to validate the elongated results on the binarized neural network based on the Xilinx FINN neural network. 

Elongate is a voltage and frequency scaling framework for Xilinx FPGAs. The neural network is created in the FPGA reconfigurable fabric and coupled to a multiprocessor host that controls the operational point to obtain energy proportionality. Elongate instruments a design netlist by inserting timing detectors to enable the exploitation of the operating margins of a device reliably.  The elongated neural network has been validated with devices with different nominal operating voltages and fabricated with 28nm (i.e. Zynq) and 16nm (i.e. Zynq Ultrascale) feature sizes showing the portability of the framework to advanced process nodes. 

The results show that Elongate can obtain new performance and energy points that are up to 86% better than nominal at the same level of classification accuracy. Trade-offs between energy and performance are also possible with a large dynamic range of valid working points available. 

The Linux configuration running on the Zynq Ultrascale is obtained with Petalinux and based on Xilinx tools:

Description:    Ubuntu 14.04.5 LTS
Release:        14.04
Codename:       trusty

Files included:

1.zcu102_wrapper_gate.bit.bin : instrumented binarized neural network with 4 compute units. The Elongated instrumentation detects failures in the critical paths before the affect the behaviour of the neural network.

To load this bitstream you need to do:

cp ./zcu102_wrapper_gate.bit.bin /lib/firmware/

echo zcu102_wrapper_gate.bit.bin > /sys/class/fpga_manager/fpga0/firmware

2.freq_control_zcu102_1816.elf: This executable runs on the ARM host processor and issues commands to the Elongate framework implemented in the FPGA. 

The use mode is:

freq_control_zcu102_1816.elf 900 1 77 0 1 4  

Parameters are:
first_freq_index (multiple of 3, indicates the first frequency that the Elongate will use for the BNN)

error_rate_tolerable: indicates how many errors must be present in the dectectors before frequency is reduced. This can be setup to 1 so after one detector error the frequency is mantained. Notice that this are detectors errors and not functional errors. The system detects these errors before they affect the logic itself.

phase correction steps, phase direction (1 inc, 0 dec) set to 77 and 0 to indicate the difference in phase between logic detectors and memory detectors. please read our paper for an explanation.
 
adapt (1 or 0 run until breaks). If set to 1 the system will keep increasing the frequency and checking for detector activity and will find the maximum frequency that can be supported for any given voltage. If 0 it will ignore the errors and keep increasing the frequency until the system stops working. This mode is useful to evaluate the margin from error detection until system failure.

To change the voltage maxim integrated maxpowertool has been used to connect to the zcu102 and adjusts the values of the voltage regulators from nominal of 0.85v to a min of 0.55v. It is possible to get the FPGA to do this directly and this has been done in the lower cost zc702.  

This adaptive voltage scaling finds the best frequency that can be supported for any given voltage and adjusts the frequency automatically if the device is is slower or if other parameters such as temperature change. 

To use the demo the user should initially programmed the FPGA with the supply bit.bin file and then launch freq_control_zcu102_1816 that will launch the BNN application and increase the frequency in steps until finding the frequency when the detectors become active. This is the maximum frequency that can be supported by the voltage level safely. Then using the maxpowertool from MAxim the user could lower the voltage and do a new run to detect the max frequency at the new voltage. 

3.control_source: Source code for freq_control_zcu102_1816

4.BNN_source: source code, parameters and test files for BNN host code that is called by freq_control_zcu102_1816.elf and that calls the BNN implementation in the hardware device.

6.sdsoc_source : c++ BNN source code for sdsoc tool to create hardware bit.bin and hardware library .a

5.elongate_bnn.pdf: BNN processor architecture with 4 compute units and the Elongate IP that determines frequencies in function of voltage.   


 


