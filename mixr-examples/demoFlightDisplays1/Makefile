#
include ../makedefs

PGM = demoFlightDisplays1

OBJS =  \
	CrsPntr.o \
	Hsi.o \
	main.o \
	Pfd.o \
	RdrAlt.o \
	SituationalDisplay.o \
	SpdLines.o \
	TerrainFollower.o \
	TestPfd.o \
	TestSD.o

LDFLAGS += -lmixr_ui_glut -lmixr_instruments -lmixr_graphics -lmixr_base
LDFLAGS += -lftgl -lfreetype -lglut -lGLU -lGL -lX11 -lpthread
#LDFLAGS += -lXi -lXmu
LDFLAGS += -lrt

all: edl $(PGM)

edl:
	cpp configs/test.epp >test.edl $(EPPFLAGS)

$(PGM): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -f *.o
	-rm -f *.edl
	-rm -f $(PGM)
