#
include ../makedefs

PGM = testRadar

OBJS =	DspRadar.o DspRwr.o TdAzPtr.o TdElevPtr.o \
	TestDisplay.o TestStation.o factory.o main.o

LDFLAGS += -lmixr_interop_dis -lmixr_interop
LDFLAGS += -lmixr_models -lJSBSim
LDFLAGS += -lmixr_simulation
LDFLAGS += -lmixr_ui_glut -lmixr_instruments -lmixr_graphics
LDFLAGS += -lmixr_linkage -lmixr_terrain
LDFLAGS += -lmixr_base
LDFLAGS += -lftgl -lfreetype -lglut -lGLU -lGL -lX11
#LDFLAGS += -lXi -lXmu
LDFLAGS += -lpthread -lrt

EPPFLAGS += -I.

all: edl $(PGM)

edl:
	cpp configs/test1.epp  >test1.edl    $(EPPFLAGS)
	cpp configs/test2a.epp >test2a.edl   $(EPPFLAGS)
	cpp configs/test2b.epp >test2b.edl   $(EPPFLAGS)
	cpp configs/test2c.epp >test2c.edl   $(EPPFLAGS)
	cpp configs/test3a.epp >test3a.edl   $(EPPFLAGS)
	cpp configs/test3b.epp >test3b.edl   $(EPPFLAGS)
	cpp configs/test4a.epp >test4a.edl   $(EPPFLAGS)
	cpp configs/test4b.epp >test4b.edl   $(EPPFLAGS)

$(PGM): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -f *.o
	-rm -f *.edl
	-rm -f $(PGM)
