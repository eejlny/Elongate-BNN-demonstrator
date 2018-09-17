/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include "clkgen_elo.h"
#include "xio.h"




// The purpose this test is to show that users can get to devices in user
// mode for simple things like GPIO. This is not to say this should replace
// a kernel driver, but does provide some short term solutions sometimes
// or a debug solution that can be helpful.

// This test maps a GPIO in the hardware into the user space such that a
// GPIO signal can be toggled fast. On the ML507 reference system, the
// signal could be toggled about every 50 ns which is pretty fast.

// This test was derived from devmem2.c.

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
//#define IIC_DEVICE_ID	XPAR_XIICPS_0_DEVICE_ID


/*
 * The following constant defines the address of the IIC Slave device on the
 * IIC bus. Note that since the address is only 7 bits, this constant is the
 * address divided by 2.
 */


//#define XPAR_AXI_CLKGEN_0_HIGHADDR 0x6E40FFFF

//#define GPIO_BASE_ADDRESS     0x81400000
//#define GPIO_DATA_OFFSET     0
//#define GPIO_DIRECTION_OFFSET     4

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)


//#define MAP_SIZE_PMBUS 262144UL
//#define MAP_MASK_PMBUS (MAP_SIZE_PMBUS - 1)

//float gen_freqs[549] = {301.18,301.33,301.54,301.75,301.82,301.96,302.22,302.50,302.56,302.63,302.78,302.86,302.94,303.03,303.13,303.33,303.57,303.70,303.85,304.00,304.17,304.55,304.62,304.76,305.00,305.26,305.45,305.56,305.71,305.88,306.25,306.67,307.14,307.41,307.50,307.69,307.89,308.00,308.33,308.57,308.77,308.82,309.09,309.33,309.52,309.80,310.00,310.53,310.71,310.77,311.11,311.43,311.54,311.76,312.00,312.28,312.50,312.73,312.82,313.16,313.33,313.64,313.73,313.85,314.29,314.67,314.81,315.00,315.15,315.38,315.56,315.63,315.79,316.00,316.36,316.67,316.92,317.14,317.33,317.50,317.65,317.86,317.95,318.18,318.42,318.52,318.75,319.05,319.23,319.30,319.44,320.00,320.59,320.83,321.05,321.21,321.43,321.57,321.88,322.22,322.73,322.81,323.08,323.33,323.53,323.68,323.81,324.44,325.00,325.49,325.93,326.32,326.47,326.67,326.92,327.27,327.78,328.13,328.21,328.57,328.89,328.95,329.17,329.41,329.63,329.82,330.00,330.56,330.77,331.25,331.58,331.82,332.14,332.35,333.33,334.21,334.38,334.62,335.00,335.29,335.71,336.11,336.36,336.67,336.84,337.04,337.25,337.50,337.78,338.10,338.24,338.46,338.89,339.29,339.39,340.00,340.35,340.63,340.74,340.91,341.18,341.67,342.22,342.31,342.86,343.33,343.59,343.75,343.86,344.12,344.44,345.00,345.10,345.45,345.83,346.15,346.43,346.67,346.88,347.06,347.22,347.37,347.62,348.15,348.72,349.02,350.00,350.88,351.11,351.52,351.85,352.38,352.78,352.94,353.13,353.33,353.57,353.85,354.17,354.39,354.55,355.00,355.56,355.88,356.25,356.67,356.86,357.14,357.58,357.69,357.89,358.33,358.82,358.97,359.09,359.26,359.38,360.00,360.71,360.78,361.11,361.40,361.54,361.76,361.90,362.50,362.96,363.33,363.64,364.10,364.29,364.44,364.71,364.91,365.00,365.38,365.63,366.67,367.65,367.86,368.18,368.42,368.63,368.75,368.89,369.23,369.70,370.00,370.37,370.59,370.83,371.43,371.88,371.93,372.22,372.55,372.73,373.08,373.33,373.53,374.07,374.36,375.00,375.44,375.76,376.19,376.47,376.67,376.92,377.27,377.78,378.13,378.57,378.95,379.17,379.49,380.00,380.39,380.77,380.95,381.25,381.48,381.82,382.14,382.22,382.46,383.33,384.31,384.38,384.62,385.00,385.19,385.71,385.96,386.36,386.67,387.50,387.88,388.24,388.46,388.89,389.29,389.47,389.74,390.00,390.48,390.63,390.91,391.11,391.67,392.16,392.31,392.59,392.86,392.98,393.33,393.75,393.94,394.44,394.87,395.00,395.24,395.45,395.56,395.83,396.08,396.15,396.30,396.43,396.49,396.67,396.88,400.00,403.51,403.70,403.92,404.17,404.44,404.76,405.13,405.26,405.56,405.88,406.06,406.25,406.67,407.02,407.14,407.41,407.69,407.84,408.33,408.89,409.09,409.52,410.00,410.26,410.53,411.11,411.76,412.12,412.50,413.33,414.04,414.29,414.81,415.38,415.69,415.79,416.67,417.54,417.65,417.78,418.18,418.52,418.75,419.05,419.61,420.00,420.51,420.83,421.05,421.43,422.22,423.08,423.53,423.81,424.24,424.56,425.00,425.64,425.93,426.32,426.67,427.27,427.45,427.78,428.07,428.57,429.17,429.41,429.63,430.00,430.30,430.77,431.11,431.25,431.37,431.58,433.33,435.09,435.29,435.56,435.71,435.90,436.36,436.84,437.04,437.50,438.10,438.46,438.60,438.89,439.22,440.00,440.74,441.03,441.18,441.67,442.11,442.42,442.86,443.14,443.75,444.44,445.45,445.61,445.83,446.15,446.67,447.06,447.37,447.62,448.15,448.48,448.89,449.12,450.00,450.98,451.28,451.85,452.38,452.63,452.94,453.33,453.85,454.17,454.55,454.90,455.56,456.25,456.41,457.14,457.78,457.89,458.33,458.82,459.26,460.00,460.61,461.11,461.54,461.90,462.22,462.50,462.75,462.96,463.16,463.64,464.29,464.71,466.67,468.42,468.75,469.23,470.00,470.37,470.59,470.83,471.11,471.43,471.79,472.22,472.73,473.33,473.68,474.07,474.51,475.00,475.56,476.19,476.47,476.92,477.78,478.43,478.57,478.79,478.95,479.17,480.00,480.95,481.25,481.48,481.82,482.05,482.35,483.33,484.21,484.44,484.62,484.85,485.71,486.27,486.67,487.18,487.50,488.24,488.89,489.47,490.00,490.20,490.48,490.91,491.67,492.31,492.86,493.33,493.75,494.12,494.44,494.74,495.24,495.83,496.30,496.97,497.44,497.78,498.04,500.00};


float gen_freqs[486] = {100.79,101.30,101.82,102.38,102.88,103.41,104.00,104.55,105.05,105.56,106.06,106.59,107.14,107.69,108.33,108.89,109.40,110.00,110.58,111.11,111.67,112.22,112.73,113.27,113.85,114.42,115.00,115.56,116.07,116.67,117.17,117.71,118.27,118.89,119.44,120.00,120.51,121.11,121.67,122.22,122.73,123.23,123.75,124.29,125.00,125.56,126.14,126.67,127.27,127.78,128.28,128.79,129.29,129.87,130.56,131.11,131.67,132.22,132.73,133.33,133.93,134.44,135.00,135.56,136.11,136.67,137.50,138.10,138.64,139.29,140.00,140.63,141.25,141.82,142.42,143.06,143.64,144.16,144.90,145.68,146.25,146.88,147.50,148.15,148.75,149.38,150.00,150.62,151.25,151.79,152.38,153.06,153.57,154.17,154.69,155.36,156.00,156.67,157.41,158.00,158.57,159.18,159.72,160.32,160.94,161.90,162.50,163.27,163.89,164.44,165.00,165.63,166.67,167.19,167.86,168.52,169.05,169.64,170.31,170.83,171.43,172.00,172.92,173.44,174.07,174.60,175.51,176.19,176.79,177.50,178.13,179.17,179.69,180.36,180.95,181.48,182.14,182.81,183.33,183.93,184.44,185.00,185.71,186.67,187.30,188.00,188.57,189.29,189.80,190.48,191.07,191.67,192.19,192.86,193.65,194.29,195.00,195.56,196.30,196.83,197.50,198.15,200.00,201.59,202.22,202.78,203.33,204.00,204.76,205.36,206.12,206.67,207.41,208.00,208.57,209.26,210.00,210.71,211.43,212.00,212.96,213.89,214.58,215.56,216.07,216.67,217.50,218.37,219.05,219.64,220.37,221.43,222.22,222.86,223.81,224.44,225.00,225.71,226.53,227.08,227.78,228.57,229.17,230.00,230.56,231.11,232.00,233.33,234.29,235.00,235.56,236.11,236.67,237.50,238.10,238.89,239.58,240.48,241.67,242.22,242.86,243.75,244.44,245.00,245.71,246.43,247.22,247.92,248.57,250.00,251.11,252.00,252.78,253.33,254.17,254.76,255.56,256.25,257.14,257.78,258.33,259.38,260.00,260.71,261.90,262.50,263.33,263.89,264.44,265.00,265.63,266.67,267.50,268.57,269.44,270.00,270.83,271.43,272.00,273.33,274.07,275.00,275.56,276.19,277.14,277.78,278.57,279.17,280.00,280.56,281.25,282.14,282.86,284.00,285.00,285.71,286.67,287.50,288.57,289.29,290.00,290.63,291.43,292.00,292.59,293.33,294.29,295.00,295.83,296.43,297.14,300.00,302.50,303.13,303.70,304.76,305.56,306.25,307.14,308.00,308.57,309.38,310.00,310.71,311.43,312.00,313.33,313.89,314.81,315.63,316.67,317.50,318.52,319.05,320.00,320.83,321.43,322.22,323.81,325.00,325.93,326.67,327.78,328.57,329.17,330.00,330.56,331.25,332.14,333.33,334.38,335.00,335.71,337.04,338.10,338.89,340.00,340.63,341.67,342.86,343.75,344.44,345.00,345.83,346.43,347.22,348.15,350.00,351.85,352.38,353.13,354.17,355.00,355.56,356.25,357.14,358.33,359.26,360.00,360.71,361.90,362.50,364.29,365.00,365.63,366.67,367.86,368.75,370.00,370.83,371.43,372.22,373.33,374.07,375.00,376.19,377.78,378.57,379.17,380.00,380.95,381.48,382.14,383.33,384.38,385.00,385.71,386.67,387.50,388.89,390.00,390.63,391.67,392.59,393.33,394.44,395.00,395.83,396.43,400.00,403.70,404.76,405.56,406.25,407.14,408.33,409.52,411.11,412.50,413.33,414.29,414.81,416.67,418.52,419.05,420.00,420.83,421.43,422.22,423.81,425.00,425.93,426.67,427.78,428.57,429.17,430.00,431.25,433.33,435.71,437.04,438.10,438.89,440.00,440.74,441.67,442.86,443.75,444.44,445.83,446.67,447.62,448.15,450.00,451.85,452.38,453.33,454.17,455.56,456.25,457.14,458.33,459.26,460.00,461.11,461.90,462.50,464.29,466.67,468.75,470.00,470.83,471.43,472.22,473.33,474.07,475.00,476.19,477.78,478.57,479.17,480.00,480.95,483.33,485.71,486.67,487.50,488.89,490.00,491.67,492.86,493.75,494.44,495.24,495.83,500.00};

//float gen_freqs[470] = {50.52,51.04,51.56,52.08,52.59,53.09,53.62,54.17,54.67,55.21,55.73,56.25,56.77,57.29,57.81,58.33,58.85,59.38,59.90,60.42,60.94,61.46,61.98,62.50,63.02,63.54,64.06,64.58,65.10,65.63,66.15,66.67,67.19,67.71,68.25,68.84,69.38,69.89,70.39,71.02,71.53,72.06,72.62,73.13,73.68,74.22,75.00,75.56,76.14,76.67,77.19,77.78,78.29,78.85,79.38,80.00,80.56,81.11,81.62,82.14,82.69,83.33,83.93,84.44,85.00,85.56,86.11,86.67,87.18,87.78,88.28,88.89,89.39,90.00,90.63,91.18,91.91,92.42,92.97,93.59,94.17,94.79,95.31,95.83,96.43,96.97,97.50,98.08,98.61,99.17,100.00,100.83,101.39,101.92,102.50,103.03,103.57,104.17,104.69,105.21,105.77,106.41,106.94,107.50,108.04,108.65,109.26,109.82,110.42,110.94,111.46,112.12,112.82,113.33,113.89,114.42,115.00,115.63,116.25,117.05,117.71,118.27,119.05,119.64,120.19,120.83,121.43,122.12,122.73,123.33,123.86,125.00,125.76,126.39,127.08,127.78,128.33,129.17,129.69,130.21,130.95,131.48,132.14,132.81,133.33,133.93,134.72,135.23,135.94,136.67,137.50,138.10,138.64,139.29,140.00,140.63,141.25,142.05,142.59,143.18,143.75,144.32,145.00,145.83,146.43,147.22,147.92,148.44,150.00,151.25,151.79,152.38,153.13,153.70,154.69,155.36,156.25,156.94,157.50,158.33,158.93,159.52,160.42,160.94,161.90,162.50,163.33,163.89,164.58,165.28,166.07,166.67,167.19,167.86,168.52,169.05,169.64,170.31,170.83,171.43,172.22,172.92,173.44,174.07,175.00,175.93,176.56,177.08,177.78,178.57,179.17,179.69,180.36,180.95,182.14,182.81,183.33,183.93,185.00,185.71,186.67,187.50,188.10,188.89,189.58,190.48,191.07,191.67,192.19,192.86,193.75,194.44,195.00,195.83,196.43,197.22,197.92,198.44,200.00,201.79,202.38,203.13,204.17,204.76,205.36,206.25,207.14,208.33,208.93,209.52,210.42,211.11,211.90,212.50,213.33,213.89,214.58,215.63,216.67,217.50,218.75,219.44,220.00,220.83,221.43,222.22,222.92,223.81,225.00,226.19,226.79,227.50,228.13,229.17,230.00,230.56,231.25,232.50,233.33,234.38,235.00,236.11,236.67,237.50,238.89,239.58,240.63,241.67,242.50,243.33,244.44,245.00,245.83,246.67,247.22,247.92,250.00,252.08,252.78,253.33,254.17,255.00,255.56,256.25,257.50,258.33,259.38,260.00,261.11,262.50,263.33,263.89,264.58,265.63,266.67,267.50,268.75,270.00,270.83,271.88,272.50,273.33,275.00,276.67,277.50,278.13,279.17,280.00,281.25,282.50,283.33,284.38,285.00,286.67,287.50,288.89,290.00,290.63,291.67,292.50,293.33,294.44,295.00,295.83,296.67,297.50,300.00,302.50,303.13,304.17,305.00,305.56,306.25,307.50,308.33,309.38,310.00,311.11,312.50,313.33,315.00,315.63,316.67,317.50,318.75,320.83,321.88,325.00,327.78,329.17,331.25,333.33,334.38,337.50,338.89,340.63,341.67,343.75,344.44,345.83,346.88,350.00,353.13,354.17,355.56,356.25,358.33,359.38,361.11,362.50,365.63,366.67,368.75,370.83,371.88,375.00,377.78,379.17,381.25,383.33,384.38,387.50,388.89,390.63,391.67,393.75,394.44,395.83,396.88,400.00,404.17,405.56,406.25,408.33,411.11,412.50,416.67,418.75,420.83,422.22,425.00,427.78,429.17,431.25,433.33,437.50,438.89,441.67,443.75,444.44,445.83,450.00,454.17,455.56,456.25,458.33,461.11,462.50,466.67,468.75,470.83,472.22,475.00,477.78,479.17,481.25,483.33,487.50,488.89,491.67,493.75,494.44,495.83,500.00};

/*
float gen_freqs[549] = {21.98,22.08,22.22,22.34,22.45,22.55,22.66,
22.77,22.87,22.97,23.08,23.21,23.33,23.44,23.56,23.66,23.78,23.89,
24.04,24.17,24.29,24.39,24.50,24.61,25.00,25.40,25.52,25.62,25.74,
25.86,26.00,26.11,26.22,26.34,26.44,26.56,26.67,26.79,26.92,27.03,
27.14,27.27,27.38,27.50,27.60,27.72,27.84,27.94,28.05,28.21,28.33,
28.45,28.57,28.68,28.79,28.91,29.03,29.17,29.29,29.41,29.55,29.69,
29.81,30.00,30.11,30.26,30.41,30.56,30.68,30.83,30.95,31.06,31.25,
31.43,31.55,31.67,31.77,31.88,32.00,32.14,32.26,32.39,32.50,32.61,
32.74,32.86,33.00,33.12,33.33,33.52,33.65,33.82,33.93,34.09,34.21,
34.38,34.48,34.62,34.72,34.85,35.00,35.12,35.23,35.34,35.48,35.61,
35.71,35.83,35.94,36.11,36.25,36.36,36.54,36.67,36.81,36.96,37.07,
37.50,37.90,38.04,38.16,38.28,38.39,38.54,38.75,38.89,39.00,39.13,
39.29,39.42,39.58,39.71,39.81,40.00,40.13,40.28,40.38,40.52,40.62,
40.74,40.91,41.07,41.18,41.30,41.41,41.67,41.91,42.05,42.19,42.31,
42.50,42.65,42.86,42.97,43.18,43.33,43.48,43.75,44.00,44.12,44.23,
44.44,44.57,44.74,44.85,45.00,45.19,45.31,45.45,45.59,45.83,46.00,
46.25,46.43,46.59,46.74,46.88,47.00,47.12,47.22,47.37,47.50,47.62,
47.73,47.92,48.08,48.21,48.33,48.44,48.61,48.75,48.86,49.04,49.17,
50.00,50.83,50.96,51.09,51.19,51.32,51.47,51.67,51.79,51.92,52.08,
52.27,52.38,52.50,52.63,52.78,52.88,53.12,53.33,53.57,53.75,53.95,
54.17,54.41,54.69,54.81,55.00,55.21,55.36,55.56,55.68,55.88,56.25,
56.58,56.73,56.94,57.14,57.29,57.50,57.69,57.81,57.95,58.33,58.65,
58.82,58.93,59.09,59.21,59.38,59.62,59.72,60.00,60.23,60.42,60.53,
60.71,60.94,61.11,61.25,61.36,61.54,61.67,61.84,62.50,63.24,63.46,
63.64,63.75,63.89,64.06,64.29,64.58,64.71,65.00,65.28,65.38,65.62,
65.91,66.07,66.18,66.67,67.05,67.19,67.31,67.50,67.65,67.86,68.06,
68.18,68.33,68.75,69.12,69.23,69.44,69.64,70.00,70.31,70.45,70.83,
71.15,71.43,71.59,71.88,72.22,72.50,72.73,72.92,73.08,73.21,73.33,
73.44,73.61,73.75,75.00,76.25,76.39,76.56,76.67,76.79,76.92,77.08,
77.27,77.50,77.78,78.12,78.33,78.57,78.75,79.17,79.55,79.69,80.00,
80.36,80.56,80.77,81.25,81.82,81.94,82.14,82.50,82.69,82.81,83.33,
83.93,84.09,84.38,84.62,84.72,85.00,85.42,85.94,86.11,86.36,86.54,
87.50,88.46,88.64,88.89,89.06,89.29,89.58,90.00,90.38,90.62,90.91,
91.07,91.67,92.19,92.50,92.86,93.18,93.75,94.44,94.64,95.00,95.31,
95.45,95.83,96.43,96.88,97.22,97.50,97.73,97.92,98.21,98.44,100.00,
101.79,102.08,102.27,102.50,102.78,103.12,103.57,104.17,104.55,105.00,
105.36,105.56,106.25,106.82,107.14,107.50,108.33,108.93,109.38,110.00,
110.42,110.71,111.11,112.50,113.89,114.29,114.58,115.00,115.62,116.67,
117.50,117.86,118.75,119.44,120.83,121.43,121.88,122.22,122.50,122.92,
125.00,127.08,127.50,127.78,128.12,128.57,129.17,130.00,130.56,131.25,
132.14,132.50,133.33,134.38,135.00,135.71,137.50,139.29,140.00,140.62,
141.67,142.50,142.86,143.75,145.00,145.83,146.43,146.88,147.50,150.00,
152.50,153.12,153.57,154.17,155.00,156.25,157.14,157.50,158.33,159.38,
160.00,160.71,162.50,164.29,165.00,165.62,166.67,167.86,168.75,170.00,
170.83,171.88,175.00,178.12,179.17,180.00,181.25,183.33,184.38,185.00,
187.50,190.00,190.62,191.67,193.75,195.00,195.83,196.88,200.00,204.17,
205.00,206.25,208.33,210.00,212.50,215.00,216.67,218.75,220.00,220.83,
225.00,229.17,230.00,231.25,233.33,235.00,237.50,241.67,243.75,245.83,
250.00,254.17,256.25,258.33,262.50,266.67,268.75,275.00,281.25,283.33,
287.50,291.67,293.75,300.00,306.25,308.33,312.50,316.67,318.75,325.00,
331.25,333.33,337.50,341.67,343.75,350.00,356.25,358.33,362.50,366.67,
368.75,375.00,381.25,383.33,387.50,391.67,393.75,400.00};
*/


int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // ignore break signal
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                               // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;
        if (tcsetattr (fd, TCSANOW, &tty) != 0)
               {
                       printf ("error %d from tcsetattr", errno);
                       return -1;
               }
               return 0;
       }

void set_blocking (int fd, int should_block)
{
               struct termios tty;
               memset (&tty, 0, sizeof tty);
               if (tcgetattr (fd, &tty) != 0)
               {
                       printf ("error %d from tggetattr", errno);
                       return;
               }

               tty.c_cc[VMIN]  = should_block ? 1 : 0;
               tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

               if (tcsetattr (fd, TCSANOW, &tty) != 0)
                       printf ("error %d setting term attributes", errno);
       }



float convert_pmbus_reading(uint16_t pmbus_reading)
{
	uint8_t x_e_u;
	int x_e,exp_v;
	int i;
	float x_p;
	int16_t x_m;

	/* TODO: Handle negative mantissa:
		It will never happen since current and power can't be negative */

	/* Extract the mantissa and exponent. See Section 7.1 in PMBus Spec. - Part 2*/
	x_m = (pmbus_reading & 0x07FF);
	x_e_u = ((pmbus_reading & 0xF800) >> 11);

	//printf ("mantisa is %d ,exponent is %d\n",x_m,x_e_u);

	/* Convert from 2's complement */
	if (x_e_u > 15) {
		x_e_u = ((x_e_u ^ 255) + 1) & 0x1F;
		x_e = -x_e_u;
	}
	else {
		x_e = x_e_u;
	}

	x_e = -x_e;

	//printf ("Exponent after adjustment %d  \n",x_e);

	exp_v = 1;
	//x_p = 1/exp2((float)x_e);
	for (i=0;i<x_e;i++)
		exp_v = exp_v * 2;

	x_p = 1.0/(float)exp_v;
	//printf ("x_p is %f  \n",x_p);
	return ((float)x_m)*x_p;
}

int main(int argc, char *argv[])
{

	unsigned int Status,wait_loop,ips_value,pe_loop,pe_count,firing_rate,operations,force_freq,new_freq,loop,fatal_error,temp_value,i, num_a, num_b, led_write,  dip_check, freq_count,freq_address_value;
	long long  math_temp,math_soft,math_result,math_result_low,math_result_high;

	uintptr_t  address;

    //static uint32_t dpram_baseaddr;
	//float power_ps,power,voltage;
	//int power_int,
	int sample_loop;

	int current_freq, last_freq;

	//uint32_t * power_core;
	uint32_t ret = 0;

	//uint16_t pout_int;
	//float pout_f;

	if(argc!=7)/*3 because even the executables name string is on argc*/
	{
	    printf("expected 6 args: first_freq_index (multiple of 3), error_rate_tolerable (1 or larger), phase correction steps, phase direction (1 inc, 0 dec), adapt (1 or 0 run until breaks),core count (1 or 4)\n");
	    printf("example: ../../freq_control_zcu102_1816.elf 900 1 67 0 1 4\n");
	    exit(1);
	}



    sample_loop = 0;
    printf("Elongate monitor tool for zcu102\r\n");

    int memfd;
    void *mapped_base, *mapped_dev_base;
    off_t dev_base = XPAR_AXI_CLKGEN_0_BASEADDR;


    memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if (memfd == -1) {
    	printf("Can't open /dev/mem.\n");
        exit(0);
    }
    printf("/dev/mem opened for clkgen.\n");

    // Map one page of memory into user space such that the device is in that page, but it may not
      // be at the start of the page.


      mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~MAP_MASK);
          if (mapped_base == (void *) -1) {
          printf("Can't map the memory to user space.\n");
          exit(0);
      }
       printf("CLKGEN mapped at address %p.\n", mapped_base);



      // get the address of the device in user space which will be an offset from the base
      // that was mapped as memory is mapped at the start of a page

      mapped_dev_base = mapped_base + (dev_base & MAP_MASK);

    //open clkmux
/*
      int memfd_clkmux;
         void *mapped_base_clkmux, *mapped_dev_base_clkmux;
         off_t dev_base_clkmux = XPAR_CLKMUX_0_BASEADDR;


         memfd_clkmux = open("/dev/mem", O_RDWR | O_SYNC);
         if (memfd_clkmux == -1) {
         	printf("Can't open /dev/mem clkmux.\n");
             exit(0);
         }
         printf("/dev/mem opened for clkmux.\n");

         // Map one page of memory into user space such that the device is in that page, but it may not
           // be at the start of the page.


           mapped_base_clkmux = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_clkmux, dev_base_clkmux & ~MAP_MASK);
               if (mapped_base_clkmux == (void *) -1) {
               printf("Can't map the memory to user space clkmux.\n");
               exit(0);
           }
            printf("CLKMUX mapped at address %p.\n", mapped_base_clkmux);


           // get the address of the device in user space which will be an offset from the base
           // that was mapped as memory is mapped at the start of a page

           mapped_dev_base_clkmux = mapped_base_clkmux + (dev_base_clkmux & MAP_MASK);
*/
         //open clkpl
/*
         int memfd_clkpl;
         void *mapped_base_clkpl, *mapped_dev_base_clkpl;
         off_t dev_base_clkpl = 0xF8000000;

          memfd_clkpl = open("/dev/mem", O_RDWR | O_SYNC);
          if (memfd_clkpl == -1) {
               	printf("Can't open /dev/mem clkpl.\n");
                   exit(0);
             }
             printf("/dev/mem opened for clkpl.\n");

             // Map one page of memory into user space such that the device is in that page, but it may not
                     // be at the start of the page.


           mapped_base_clkpl = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_clkpl, dev_base_clkpl & ~MAP_MASK);
           if (mapped_base_clkpl == (void *) -1) {
                   printf("Can't map the memory to user space clkpl.\n");
                   exit(0);
             }
             printf("CLKPL mapped at address %p.\n", mapped_base_clkpl);


             // get the address of the device in user space which will be an offset from the base
             // that was mapped as memory is mapped at the start of a page

         mapped_dev_base_clkpl = mapped_base_clkpl + (dev_base_clkpl & MAP_MASK);
*/

    //open pmbus
/*
    int memfd_pmbus;

    void *mapped_base_pmbus, *mapped_dev_base_pmbus;

    off_t dev_base_pmbus = XPAR_DPRAM_0_BASEADDR;


    memfd_pmbus = open("/dev/mem", O_RDWR | O_SYNC);
    if (memfd_pmbus == -1) {
    	   	printf("Can't open /dev/mem for pmbus.\n");
    	    exit(0);
    	}
    printf("/dev/mem opened for pmbus.\n");


    mapped_base_pmbus = mmap(0, MAP_SIZE_PMBUS, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_pmbus, dev_base_pmbus & ~MAP_MASK_PMBUS);
          if (mapped_base_pmbus == (void *) -1) {
          printf("Can't map the memory to user space.\n");
          exit(0);
      }
       printf("DPRAM PMBUS mapped at address %p.\n", mapped_base);



      // get the address of the device in user space which will be an offset from the base
      // that was mapped as memory is mapped at the start of a page

      mapped_dev_base_pmbus = mapped_base_pmbus + (dev_base_pmbus & MAP_MASK_PMBUS);

      dpram_baseaddr = mapped_dev_base_pmbus;
*/
/*      printf("Inititilize i2c\n");

      Xil_Out32(dpram_baseaddr, 0x1);

      power_core = (uint32_t *)mapped_dev_base_pmbus;
*/
    // opening serial port

    char *portname = "/dev/ttyPS0";

    int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);

    if (fd < 0)
    {
            printf ("error %d opening %s", errno, portname);
            return -1;
    }

    set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking (fd, 0);                // set no blocking

    int len = 1;
    int write_command;
    //int new_volt = 1000;
    char buffer[200];

    len=sprintf (buffer, "Serial port %d ready ",len);
    write(fd,buffer,len);

   /* printf("Clock gate BNN\n");

    address = mapped_dev_base;

    CLKGEN_ELO_mWriteReg(address, 0x00000010); // bit 5 set to 1 to clock gate



    exit(0);*/

	address =  (uintptr_t)(mapped_dev_base + 0x1C);

	printf("Configuring address jump %p\n",address);

    CLKGEN_ELO_mWriteReg(address, 0x00000006); // address jump back when errors 2x

//	 CLKGEN_ELO_mWriteReg(address, 0x00000000);

    printf("Set tolerable error rate in reg4\n");

	address =  (uintptr_t)(mapped_dev_base + 0x10);

	int error_rate = atoi(argv[2]);

	CLKGEN_ELO_mWriteReg(address, error_rate);

	int phase_correction = atoi(argv[3]);
	int phase_direction = atoi(argv[4]);
	int adapt_enable = atoi(argv[5]);
	int core_count = atoi(argv[6]); //1 or 4

/*	printf("Set phase correction in reg5\n");

    address =  (uintptr_t)(mapped_dev_base + 0x14);

    int phase_down = 0x00000003;
    CLKGEN_ELO_mWriteReg(address, phase_down);
    for(int loop_count =0 ; loop_count < phase_correction;loop_count++)
    {
    	int current_value = CLKGEN_ELO_mReadReg(address);
    	if (0x80000000&current_value)
    	{
    		CLKGEN_ELO_mWriteReg(address, phase_down);
    	}
    	else
    	{
    		printf("clock phase correction not done reg5 is %x\n",current_value);
    		exit(1);
    	}
    }
    printf("Phase correction completed\n");*/





     // the upper 16 bits is the error rate.
 //    CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00010003); // enable error detect and force error



    operations = 0;
    fatal_error = 0;
    led_write = 0xcfffffff;
    freq_count = 0;
    firing_rate = 0;

    //while (1)
     //{
    	// sleep(1);
    	// operations +=1;
    //	 printf("Writing CLKGEN_ELO\n");
    	 //asm volatile ("dmb");
    	 //*((volatile unsigned long *) (mapped_dev_base)) = 0x0;
    	// *((volatile unsigned long *) (mapped_dev_base)) = 0x00010003;
    	 //CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000000); // enable error detect and force error
    // }
    //}

    /*
    printf("Switching off ntc_clk\n");

        address = mapped_dev_base_clkmux;

        //CLKGEN_ELO_mWriteReg(address, 0x00000009); //200 MHz axi
        CLKGEN_ELO_mWriteReg(address, 0x00000005); //10 MHz axi

        //0 bus_ip clk 10 mhz
        //1 ntc clk
        //2 bus ip clk 10 mhz
        //3 fast_clock axi 200 mhz
        //default 9
        //stop clock


        //printf("Stopping CLK1,CLK0, CLK2 PL\n");

          //  address = mapped_dev_base_clkpl + 0x00000178;

          //  CLKGEN_ELO_mWriteReg(address, 0x00000000)
        //; //25 MHz


          //  address = mapped_dev_base_clkpl + 0x00000188;

          //  CLKGEN_ELO_mWriteReg(address, 0x00000000); //25 MHz


         //   address = mapped_dev_base_clkpl + 0x00000198;

          //  CLKGEN_ELO_mWriteReg(address, 0x00000000); //25 MHz

        //new_freq = 1350;
        //address = mapped_dev_base + 24;
        //CLKGEN_ELO_mWriteReg(address, new_freq);
        //printf("Forcing clock synthesis with %f Mhz\r\n", gen_freqs[(new_freq/3)]);



        sleep(2);



    printf("Switching on ntc_clk\n");


        //to use ntc clock write CLKGEN_ELO_mWriteReg(address, 0x00000002);


     address = mapped_dev_base_clkmux;

     //CLKGEN_ELO_mWriteReg(address, 0x0000000A); //200 MHz axi
     CLKGEN_ELO_mWriteReg(address, 0x00000006); //10 MHz axi
*/
     //new_freq = 1350; //~150 mhz
     //new_freq = 1080; // ~90 Mz
     //new_freq = 120; //  Mz
     //printf("Force error\n");
     //CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000005); // force error
     //sleep(2);

    // new_freq = 0; // ~100 Mz
    // new_freq = 1080; // ~360 Mz
     // new_freq = 300; // ~160 Mz
     //new_freq = 600; // ~220 Mz
     //new_freq = 900; // ~309 Mz
     new_freq = atoi(argv[1]);
     //new_freq = 1646; //400 MHz
     current_freq = 0;
     last_freq = 0;
     //address = mapped_dev_base + 24;

     address =  (uintptr_t)(mapped_dev_base + 0x18);

     CLKGEN_ELO_mWriteReg(address, new_freq);



     printf("Forcing clock synthesis with %f Mhz\r\n", (gen_freqs[(new_freq/3)]));

     // read clkgen#


     address =  (uintptr_t)(mapped_dev_base + 0x18);
     freq_address_value =  CLKGEN_ELO_mReadReg(address);
     printf("Current freq is %f\r\n",(gen_freqs[(freq_address_value/3)]));

     /*

     printf("Setting nominal voltage\r\n");
	 //write_command =   0x011003e8; //1000 mv

	 write_command =   0x01100000 + new_volt; //1000 mv
	 Xil_Out32(dpram_baseaddr, write_command); // write voltage
	 while(1)
	 {
	 	  ret = Xil_In32(dpram_baseaddr);
	 	  if (ret == 0x10000000)
	           	  {break;}
	      else
	          	  {usleep(100000);}
	 }
     Status = power_core[0];
	 if (Status != 0x10000000)
	 {
				  printf("problem writing pl power with status %x\n",Status);
	 }

*/

    // printf("Clock gate BNN\n"); everything hangs

    // address = mapped_dev_base;

    // CLKGEN_ELO_mWriteReg(address, 0x00000010); // bit 5 set to 1 to clock gate

    // usleep(5000000); // wait*/


     printf("Reset error\r\n");
	 CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000008); // reset error
     printf("Adaption enable\r\n");
	 CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00010003);


     printf("Starting loop\n");

     while(1)
     {



//      Xil_Out32(dpram_baseaddr, 0x01002000); // read power pl once

      //printf("Trigger freq adaptation %d\n", operations);

      //len=sprintf (buffer, "Triggering frequency adaptation\r\n");
      //write(fd,buffer,len);




   	  // read clkgen#

 	  address = mapped_dev_base + 0x18;
      freq_address_value =  CLKGEN_ELO_mReadReg(address);
      current_freq = freq_address_value;
      printf("Current freq is %f\r\n",gen_freqs[(freq_address_value/3)]);

 	  //freq_address_value = 50.0;
   	  //len=sprintf (buffer, "#F%03x\r\n", freq_address_value);
   	  //write(fd,buffer,len);

   	  //len=sprintf (buffer, "Valid freq is %f\r\n", gen_freqs[(freq_address_value/3)]);
   	  //write(fd,buffer,len);

     /* printf("Reset error\r\n");
      CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000008); // reset error
      printf("Adaption reenable\r\n");
      CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00010003);*/

      printf("Reset error\r\n");
      CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000008); // reset error
      usleep(1000000); // 3 sec execution*/
      if (adapt_enable == 1)
      {
    	  printf("Adaption reenable\r\n");
    	  CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000003); //adapt
      }
      else
      {
    	  printf("Adaption disable\r\n");
	 	  CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00010001); //run until break
      }
    	  // usleep(100000000); // 3 sec execution*/

      //phase correction

      printf("Set phase correction in reg5\n");
      int phase_step;
      address =  (uintptr_t)(mapped_dev_base + 0x14);

         if (phase_direction == 1) //increase phase
        	 phase_step = 0x00000003;
         else
        	 phase_step = 0x00000001;
         CLKGEN_ELO_mWriteReg(address, phase_step);
         for(int loop_count =0 ; loop_count < phase_correction;loop_count++)
         {
         	int current_value = CLKGEN_ELO_mReadReg(address);
         	if (0x80000000&current_value)
         	{
         		CLKGEN_ELO_mWriteReg(address, phase_step);
         	}
         	else
         	{
         		printf("clock phase correction not done reg5 is %x\n",current_value);
         		exit(1);
         	}
         }

         //int M[] = {127,78,56,43,107,91,52,23,104,19,35,97,15,14,13,49,128,11,115,40,67,101,62,111,74,119,23,52,65,49,116,113,123,107,43,12,47,109,73,77,27,122,99,87,10,113,111,38,14,23,127,85,128,100,47,59,79,119,73,40,75,121,27,61,49,41,11,29,61,39,14,45,113,78,47,103,79,111,71,118,117,47,59,40,119,121,9,122,121,85,32,75,43,37,99,87,78,47,85,79,111,78,115,101,103,34,13,80,59,74,33,53,10,107,47,91,71,95,109,41,12,43,83,111,94,110,86,37,99,71,57,43,115,101,38,98,51,117,55,103,83,37,78,56,118,47,66,53,93,40,107,23,123,27,122,68,39,88,106,124,79,107,10,127,91,73,61,51,43,115,101,31,112,52,73,113,21,59,74,53,115,77,103,97,121,13,87,107,46,123,119,31,40,78,47,101,9,79,111,109,82,96,55,23,83,104,58,14,82,47,106,85,71,19,100,43,115,101,29,109,85,39,110,49,86,69,89,119,87,10,113,63,91,38,61,107,46,41,72,116,31,83,13,73,110,21,79,95,119,53,85,40,107,94,97,27,65,57,68,41,74,11,124,58,97,25,78,67,14,101,45,79,99,71,57,60,43,23,101,81,29,93,102,73,79,44,103,59,71,83,104,9,121,97,82,64,55,49,86,77,108,99,31,87,109,78,47,113,85,101,95,127,86,67,48,77,90,29,68,13,88,49,59,69,79,66,119,53,93,10,107,67,94,91,71,61,51,109,41,96,55,31,69,83,97,125,94,14,95,74,113,85,71,128,57,75,43,97,54,101,76,29,102,73,117,11,103,59,74,89,78,67,56,101,15,79,34,106,91,57,80,103,107,23,123,77,81,58,31,35,78,125,47,106,59,71,79,95,111,12,109,85,73,65,57,49,86,37,33,62,58,112,25,113,88,42,101,59,38,89,17,115,64,77,60,103,43,69,13,61,118,92,79,44,119,53,62,71,40,107,67,94,121,9,122,95,68,109,82,73,96,55,124,46,83,97,37,65,14,75,47,113,99,85,71,128,19,100,43,67,115,48,101,29,68,73,39,44,49,59,69,79,89,104,119,10};

         //int D[] = {9,7,5,3,8,8,5,2,9,2,3,7,1,1,1,5,9,1,8,3,5,9,5,7,5,8,2,5,7,6,9,8,8,9,3,1,3,9,5,9,2,9,8,7,1,9,8,3,1,2,9,6,9,7,4,5,5,9,5,3,7,9,2,5,4,5,1,3,4,4,1,4,8,5,3,8,5,7,7,9,8,4,4,3,8,9,1,9,8,7,3,7,4,3,8,7,5,5,6,5,7,7,8,9,8,3,1,7,4,9,4,4,1,8,4,9,6,7,8,3,1,5,6,8,9,7,7,3,7,5,4,3,8,7,3,9,4,8,6,7,9,4,7,5,9,5,5,4,7,3,7,2,8,2,9,5,4,9,9,9,5,9,1,9,9,9,6,5,3,8,7,3,9,5,5,9,2,4,5,5,9,6,8,9,8,1,8,7,3,8,9,2,3,7,3,9,1,5,7,8,9,7,4,2,6,9,5,1,7,4,9,6,5,2,7,3,8,7,2,9,7,4,9,4,7,7,6,8,7,1,9,5,9,3,4,7,3,4,7,9,2,8,1,7,7,2,5,6,9,4,8,3,8,7,9,2,6,7,5,5,9,1,9,7,7,3,7,6,1,9,4,7,7,5,4,7,3,2,7,7,2,8,7,5,9,5,7,4,6,7,7,1,8,8,9,7,6,4,7,5,7,8,2,7,7,5,5,9,9,8,6,8,9,7,5,6,7,3,7,1,9,5,6,7,6,5,9,4,7,1,8,5,7,9,7,6,5,8,3,7,4,3,5,6,7,9,9,1,9,7,8,6,5,9,4,7,3,9,5,7,7,2,7,5,8,1,7,4,5,6,7,6,5,9,1,7,3,7,6,5,7,9,7,2,8,5,7,5,2,3,5,8,3,9,5,6,5,6,7,1,9,7,9,8,7,4,7,3,4,5,7,9,2,9,7,5,8,7,3,7,2,9,5,6,7,8,5,8,1,7,9,7,6,5,9,4,7,8,3,8,5,7,9,1,9,7,5,8,9,8,7,4,9,5,6,7,4,7,1,8,5,8,7,6,5,9,2,7,3,7,8,5,7,2,7,5,4,3,5,4,7,8,6,7,8,1};


         int index = (freq_address_value/3);

         //Fvco = Fclkin * (M / D)
         //Fout = Fclkin * (M / D*O)
         //Phase shift increment = 1 / (56 * Fvco)

         float fvco = (float)gen_freqs[index]*4;

         float new_phase = (360-phase_correction*360/(56*4));

         //vco freq should be > 800 and < 1600 Mhz

         printf("Fvco is %f New phase is %f\n",fvco,new_phase);

      if(core_count == 1)
      {
    	  printf("Using 1 compute unit\n");
    	  system("./BNN  ./params_zcu102/cifar10/ ./test/test_batch.bin 1000 1");
      }
      else
      {
    	  printf("Using 4 compute units\n");
    	  system("./BNN  ./params_zcu102/cifar10/ ./test/test_batch.bin 1000 4");
      }
      usleep(10000); // 3 sec execution*/
   	  //usleep(3000000); // 3 sec execution

   	// printf("Reset error\r\n");
   	// CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000008); // reset error

   //	 exit(0);

	//  printf("Disable\r\n");
    //  CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000000); // disable error detection


/*
	  //len=sprintf (buffer, "Core temperature is %.2f\r\n", (((float)temp_value * 503.975/4096.0) - 273.15));
	  //write(fd,buffer,len);


	  len=sprintf (buffer, "#T%3x\r\n",temp_value);
	  write(fd,buffer,len);


	  address = mapped_dev_base + 0x10;
	  temp_value =  CLKGEN_ELO_mReadReg(address);

	  temp_value =  temp_value >> 4;
	  //len=sprintf (buffer, "Vint is %.2f\r\n", ((float)temp_value * 3.0/4096.0));
	  //write(fd,buffer,len);

/*
	  len=sprintf (buffer, "#V%3x\r\n",temp_value);
	  write(fd,buffer,len);


   	  voltage = ((float)temp_value * 3.0/4096.0);*/


   	  //len=sprintf (buffer, "Instant firing rate is %d\r\n", temp_value);
   	  //write(fd,buffer,len);

   	  //if (temp_value > 0)
   	  //	  firing_rate = temp_value;

   	  //printf ("Max firing rate is %d\r\n", firing_rate);

   	  //len=sprintf (buffer, "#E%03x\r\n",temp_value);
 	  //write(fd,buffer,len);

	 // printf("#E%03x\r\n",temp_value);

 	  //address = mapped_dev_base + 20;
   	  //temp_value =  CLKGEN_ELO_mReadReg(address);

   	  //ips_value =  (temp_value & 0xffff0000) >> 20;

   	  //temp_value =  (temp_value & 0xffff) >> 4;

   	  //power = ((((float)temp_value*1000.0/4095.0)/307) / 0.005) * (voltage-0.2);




   	  //power_ps = ((((float)ips_value*1000.0/4095.0 + 29.91)/307) / 0.005) * 1.0;
/*
 	  while(1)
 		  {
 			  ret = Xil_In32(dpram_baseaddr);
 			  if (ret == 0x10000000)
 			  	  {break;}
 			  else
 				  {usleep(10000);}
 		  }

 	  Status = power_core[0];
 	  if (Status != 0x10000000)
 	  {
 		  printf("problem reading pl power with status %x\n",Status);
 	  }

 	  pout_int = power_core[3];
 	  pout_f = convert_pmbus_reading(pout_int);

	  //len=sprintf (buffer, "#S%3x\r\n",pout_f);
   	  //write(fd,buffer,len);

 	  pout_f = (pout_f*1000);

 	  power_int = (int)(pout_f);

 	  //len=sprintf (buffer, "Integer power is %d\n",power_int);
 	  //write(fd,buffer,len);

   	  len=sprintf (buffer, "#P%03x\r\n",power_int);
   	  write(fd,buffer,len);

 	  //len=sprintf (buffer, "FPGA power is %f mW\r\n",(pout_f));
   	  //write(fd,buffer,len);

   	//  len=sprintf (buffer, "Instant Power processor is %f mW \r\n",power_ps);
   //	  write(fd,buffer,len);

   	 // printf("Total arithmetic errors %d\r\n", fatal_error);

   	 // len=sprintf (buffer, "Total arithmetic errors %d\r\n",fatal_error);
   	  //write(fd,buffer,len);

   	  //len=sprintf (buffer, "Total arithmetic operations %d millions\r\n",(operations / 1000000));
   	  //write(fd,buffer,len);
   	  // Read DDR power


      Xil_Out32(dpram_baseaddr, 0x01022000); // read power ddr once

      while(1)
      {
      	  ret = Xil_In32(dpram_baseaddr);
      	  if (ret == 0x10000000)
      	  	  {break;}
      	  else
      		  {usleep(10000);}
       }

       Status = power_core[0];
       if (Status != 0x10000000)
       {
      	  printf("problem reading ddr power with status %x\n",Status);
       }

       pout_int = power_core[3];

       pout_f = convert_pmbus_reading(pout_int);

     	  //len=sprintf (buffer, "#S%3x\r\n",pout_f);
        	  //write(fd,buffer,len);

      pout_f = (pout_f*1000);


      len=sprintf (buffer, "DDR power is %f mW\r\n",pout_f);
      write(fd,buffer,len);


      // Read CPU power


      Xil_Out32(dpram_baseaddr, 0x01012000); // read power cpu once

       while(1)
            {
            	  ret = Xil_In32(dpram_baseaddr);
            	  if (ret == 0x10000000)
            	  	  {break;}
            	  else
            		  {usleep(10000);}
             }

             Status = power_core[0];
             if (Status != 0x10000000)
             {
            	  printf("problem reading cpu power with status %x\n",Status);
             }

             pout_int = power_core[3];

             pout_f = convert_pmbus_reading(pout_int);





            pout_f = (pout_f*1000);

            power_int = (int)(pout_f);

            len=sprintf (buffer, "#S%3x\r\n",power_int);
             write(fd,buffer,len);


            //len=sprintf (buffer, "CPU power is %f mW \r\n",pout_f);
            //write(fd,buffer,len);

*/

    /*   if (current_freq == last_freq) // frequency froze
       {
    	   sample_loop ++;
       }
*/
  /*     if (current_freq > 120 && sample_loop >= 20)
       {
    	  sample_loop = 0;
    	  //printf("Reset error\r\n");
    	  //CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000008)
    	  //new_freq = freq_address_value - 15;
      	  new_freq = freq_address_value - 60;
    	  address = mapped_dev_base + 24;
      	  CLKGEN_ELO_mWriteReg(address, new_freq);
      	  printf("Forcing clock synthesis with %f Mhz\r\n", gen_freqs[(new_freq/3)]);
      	  firing_rate = 0;
     	  printf("Reset error\r\n");
          CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000008); // reset error
     	  printf("Adaption reenable\r\n");
     	  CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00010003);
     	  //lower voltage
     	  usleep(100000);
     	  new_volt = new_volt - 20;
     	  if (new_volt == 800)
     	  {
     		   //Slow down axi clk1

     		    printf("Slowing down CLK1,CLK0, CLK2 PL for deep AVLS\n");

     		   address = mapped_dev_base_clkpl + 0x00000170;

    		    CLKGEN_ELO_mWriteReg(address, 0x00102800); //25 MHz

    		   //CLKGEN_ELO_mWriteReg(address, 0x00203200); //10 MHz

     		    address = mapped_dev_base_clkpl + 0x00000180;

     		    CLKGEN_ELO_mWriteReg(address, 0x00102800); //25 MHz

     		    //CLKGEN_ELO_mWriteReg(address, 0x00203200); //10 MHz

     		    address = mapped_dev_base_clkpl + 0x00000190;

    		    CLKGEN_ELO_mWriteReg(address, 0x00102800); //25 MHz

    		    //CLKGEN_ELO_mWriteReg(address, 0x00203200); //10 MHz

                sleep(1);
     	  }
     	  if (new_volt > 700)
     	  {
     		  printf("Lowering voltage at %d mw\r\n", new_volt);
     		  write_command =   0x01100000 + new_volt;
     		  printf("Command is %x \r\n", write_command);
     		  Xil_Out32(dpram_baseaddr, write_command); // write voltage
     		  while(1)
     		  {
     			  ret = Xil_In32(dpram_baseaddr);
     			  if (ret == 0x10000000)
             	  {break;}
     			  else
            	  {usleep(100000);}
     		  }

     		  Status = power_core[0];
     		  if (Status != 0x10000000)
     		  {
     			  printf("problem writing pl power with status %x\n",Status);
     		  }
     	  }

        }
*/
        last_freq = current_freq;
        //else
        //{
  	    //printf("Adaption reenable\r\n");
        //CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000003);
        //}
    //  	  printf("CPU did not force freq because firing rate is %d \r\n", firing_rate);
      //	  printf("freq address value is %d\r\n", freq_address_value);
      //  }





          //CLKGEN_ELO_mWriteReg(XPAR_CLKGEN_ELO_0_BASEADDR, 0, 0x00000005); // force error
       	  //CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000008); // reset error
       	  //CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000005); // force error
       	  if (gen_freqs[(freq_address_value/3)] < 500)
       	  {

       	 	/*  address = mapped_dev_base + 0xC;
       	   	  temp_value =  CLKGEN_ELO_mReadReg(address);
       		  temp_value =  temp_value >> 4;*/

       	     // printf("Reset error\r\n");
       	     // CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00000008); // reset error


     		  num_b = 0x00004450;

       	   	  address =  (uintptr_t *)(mapped_dev_base + 0x4);
       	   	  //address = mapped_dev_base + 4;
       	   	  CLKGEN_ELO_mWriteReg(address, num_b);


       	   	  num_b = 0x00004454;

       	   	  CLKGEN_ELO_mWriteReg(address, num_b);

       	      usleep(10000); // 3 sec execution*/

       	 	 address = mapped_dev_base + 0x8;
       	   	 temp_value =  CLKGEN_ELO_mReadReg(address);

       	   	  printf ("Instant firing rate is %d\r\n", temp_value);

       	      if (temp_value > firing_rate)
       	    	  	  firing_rate = temp_value;

       	     printf ("Max firing rate is %d\r\n", firing_rate);


       	  }
       	  else
       	  {
       		  printf("Max freq reached without detection :(\r\n");
       		  //exit(0);
       		  printf("Adaptation frozen\r\n");
       		  CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00010005); // freeze with force error and auto tune off
       		  printf("Force error\r\n");
       		  CLKGEN_ELO_mWriteReg(mapped_dev_base, 0x00010007); // force error with autotune on

       	  }


       //if (firing_rate > 10)
    	//   exit(0);

     } // while loop





    // unmap the memory before exiting

    if (munmap(mapped_base, MAP_SIZE) == -1) {
        printf("Can't unmap memory from user space.\n");
        exit(0);
    }

   //usleep(1000);


   close(memfd);
    //close(fd);

   // unmap the memory before exiting
/*
      if (munmap(mapped_base_pmbus, MAP_SIZE) == -1) {
          printf("Can't unmap memory pmbus from user space.\n");
          exit(0);
      }
*/
     //usleep(1000);


  //   close(memfd_pmbus);

    return 0;
}

