include ../makedefs

PGM = testRecorderRead

OBJS = main.o DataRecordTest.o factory.o PrintMyData.o

LDFLAGS += -lmixr_models -lJSBSim
LDFLAGS += -lxrecorder
LDFLAGS += -lmixr_recorder -lmixr_simulation
LDFLAGS += -lmixr_terrain -lmixr_base
LDFLAGS += -lprotobuf
LDFLAGS += -lrt -lpthread

CPPFLAGS += -I/usr/local/include

EPPFLAGS +=  -I.

all: edl $(PGM)

edl:
	cpp configs/test.epp >test.edl   $(EPPFLAGS)

$(PGM): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) 

clean:
	-rm -f *.o
	-rm -f *.edl
	-rm -f $(PGM)
