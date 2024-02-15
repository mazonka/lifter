

CL=g++ -O3 -std=c++17 $(INCS)

EOUT=-o 
OOUT=-o 
OEXT=o
EEXT=exe

LDFS=-lpthread -ldl

ifeq ($(MPIR),1)
MPIRD1 = $(MPIRD0)/native
LDF1+=$(MPIRD1)/libmpir.a $(MPIRD1)/libcxx.a
endif
