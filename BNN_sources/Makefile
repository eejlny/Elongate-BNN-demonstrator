
# BNN

LIB_energy_arm = ../lib
LIB_bnn = ./lib_bnn
LIB_hls = ./hls

CXX= g++-4.9

CFLAGS+= -O3 -I . 
CFLAGS+= -Wno-unused-result -Wno-write-strings -Wno-deprecated-register  -std=c++11
#CFLAGSA = $(CFLAGS) -std=c++11

#XI_CFLAGS = $(CFLAGS) -DXILINX -DOFFLOAD --std=gnu++11 -march=armv7-a -I $(LIB_bnn)  -I $(LIB_hls) -I ${CURDIR} -mfloat-abi=hard 

XI_CFLAGS = $(CFLAGS) -DXILINX -DOFFLOAD   -I $(LIB_bnn)  -I $(LIB_hls) -I ${CURDIR}  

#XI_CFLAGS+= -DNEON  -funsafe-math-optimizations -ftree-vectorize  -ftree-vectorizer-verbose=2 #If NEON SIMD instructions are to be used


XI_LDFLAGS = -L $(LIB_bnn)

LDFLAGS = -pthread -lpthread -z muldefs

XI_LDFLAGS+= -lrt -lkernelbnn 

XI_PROGs= BNN

SOURCE= main.cpp   kernelbnn.h

.PHONY: all clean odroid libraries

all: odroid

libraries:
	$(MAKE) -C ../energy-meter

odroid: libraries $(XI_PROGs)

foldedmv-offload.o: foldedmv-offload.cpp foldedmv-offload.h
	$(CXX) -c foldedmv-offload.cpp $(XI_CFLAGS)

rawhls-offload.o: rawhls-offload.cpp 
	$(CXX) -c rawhls-offload.cpp $(XI_CFLAGS)


BNN: $(SOURCE) foldedmv-offload.o rawhls-offload.o
	$(CXX) -o $@ $< foldedmv-offload.o rawhls-offload.o $(XI_CFLAGS) $(XI_LDFLAGS)  $(LDFLAGS)
	

clean:
	rm -f  $(XI_PROGs) foldedmv-offload.o rawhls-offload.o

