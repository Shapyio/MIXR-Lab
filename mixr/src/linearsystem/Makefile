#
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_linearsystem.a

OBJS =  \
	DiffEquation.o \
	FirstOrderTf.o \
	LagFilter.o \
	Limit01.o \
	Limit11.o \
	Limit.o \
	LimitFunc.o \
	LowpassFilter.o \
	SaH.o \
	ScalerFunc.o \
	SecondOrderTf.o \
	Sz1.o \
	Sz2.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
