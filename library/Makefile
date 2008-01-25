NAME = TTBlue
CC = g++

########################################

INCLUDES = -I../includes/core
INCLUDES += -I../includes/units

#LIB_INCLUDES = -L../../library/build


#########################################
#MAX_LIBS = -lMaxAPI -lMaxAudio
#TT_LIB = -lTTBlue
#
#LIBS = ${TT_LIB} ${MAX_LIBS} 


#########################################

OPTIMIZATION_FLAGS = -O2
DEFINES = -D__DEBUG__

CFLAGS = ${OPTIMIZATION_FLAGS} -Wall -Wno-unknown-pragmas -Wno-conversion -shared -mno-cygwin $(WIN_DEFINES) $(INCLUDES)
LDFLAGS = $(INCLUDES) $(LIB_INCLUDES) $(LIBS)


#########################################

SRC = ../source/core/TTAudioObject.cpp
SRC += ../source/core/TTAudioSignal.cpp
SRC += ../source/core/TTElement.cpp
SRC += ../source/core/TTGlobal.cpp
SRC += ../source/core/TTObject.cpp
SRC += ../source/core/TTSymbol.cpp
SRC += ../source/core/TTValue.cpp
SRC += ../source/core/TTValueCache.cpp

SRC += ../source/units/TTBandpassButterworth2.cpp
SRC += ../source/units/TTBandrejectButterworth2.cpp
SRC += ../source/units/TTCrossfade.cpp
SRC += ../source/units/TTDCBlock.cpp
SRC += ../source/units/TTDegrade.cpp
SRC += ../source/units/TTGain.cpp
SRC += ../source/units/TTHighpassButterworth2.cpp
SRC += ../source/units/TTHighpassLinkwitzRiley4.cpp
SRC += ../source/units/TTLimiter.cpp
SRC += ../source/units/TTLowpassButterworth2.cpp
SRC += ../source/units/TTLowpassLinkwitzRiley4.cpp
SRC += ../source/units/TTLowpassOnePole.cpp
SRC += ../source/units/TTOverdrive.cpp
SRC += ../source/units/TTRamp.cpp


#########################################

all:
	$(CC) $(CFLAGS) $(SRC) -o build/TTBlue.dll -export-all-symbols $(LDFLAGS)

clean:
	rm -f *.o