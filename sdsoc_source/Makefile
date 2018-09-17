LIBRARY = libkernelbnn.a

PLATFORM = zcu102
SDSFLAGS = -sds-pf ${PLATFORM} \
	 -sds-hw BlackBoxJam top.cpp -sds-end\
	-Wno-unused-label -Wno-unused-function

DEPS = top.h config.h bnn-library.h dma.h slidingwindow.h config.h fclayer.h matrixvector.h streamtools.h convlayer.h maxpool.h

#SDSFLAGS = -sds-pf ${PLATFORM} \ 
#	-Wno-unused-label -Wno-unused-function


#CC = sdscc ${SDSFLAGS}

CC = sds++ ${SDSFLAGS}

CFLAGS = -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -O3 -c 
LFLAGS = -O3

.PHONY: all

all: ${LIBRARY}


top.o: top.cpp $(DEPS)
	#${CC} ${CFLAGS}  $< -o $@
	${CC} ${CFLAGS} -std=c++11 $< -o $@

kernelbnn.o: kernelbnn.cpp
	${CC} ${CFLAGS} $< -o $@

# Create dummy linked file to build the hardware
kernelbnn.linked: top.o kernelbnn.o
	${CC} $^ -nostartfiles -o $@ 

# Add the interface functions to the library
${LIBRARY}: kernelbnn.linked
	cp _sds/swstubs/${LIBRARY} .
	arm-linux-gnueabihf-ar rcs ${LIBRARY} _sds/swstubs/kernelbnn.o
	ranlib libkernelbnn.a
	#bootgen -w -image ./boot.bif -o i ./BOOT.bin

clean:
	${RM} ${LIBRARY} top.o kernelbnn.o  kernelbnn.linked

ultraclean: clean
	${RM}  kernelbnn.linked.bit 
	${RM} -rf _sds sd_card

# LocalWords:  cpp
