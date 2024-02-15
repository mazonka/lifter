
INCS = -I$(RT) -I.
HEAD =

ifeq ($(MPIR),1)
INCS += -I$(MPIRD0)
HEAD += $(MPIRD0)/mpir.h
endif

