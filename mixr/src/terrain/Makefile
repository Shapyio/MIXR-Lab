#
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_terrain.a

OBJS =  \
	ded/DedFile.o \
	dted/DtedFile.o \
	srtm/SrtmHgtFile.o \
	DataFile.o \
	factory.o \
	QuadMap.o \
	Terrain.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f ded/*.o
	-rm -f dted/*.o
	-rm -f srtm/*.o
	-rm -f *.o
	-rm -f $(LIB)
