LIBRARY_SOURCES = $(wildcard src/common/*.cpp)
LIBRARY_OBJECT_FILES = $(patsubst src/common/%.cpp,obj/%.o,${LIBRARY_SOURCES})
LIBRARY = lib/libTopAnalysisTools.so

EXECUTABLE_SOURCES = $(wildcard src/common/*.cxx)
EXECUTABLE_OBJECT_FILES = $(patsubst src/common/%.cxx,obj/%.o,${EXECUTABLE_SOURCES})
EXECUTABLES = $(patsubst src/common/%.cxx,bin/%.exe,${EXECUTABLE_SOURCES})

LIBRARY_PATH = 	$(shell root-config --libs) \

LIBRARIES = 	-lCore \
		-lCint  \
		-lRIO  \
		-lNet  \
		-lHist  \
		-lGraf  \
		-lGraf3d  \
		-lGpad  \
		-lTMVA  \
		-lTree  \
		-lRint  \
		-lPostscript  \
		-lMatrix  \
		-lPhysics  \
		-lMathCore \
		-lThread  \
		-pthread  \
		-lm  \
		-ldl

INCLUDE_PATH = 	-Iinclude  \
		-I$(shell root-config --incdir) \
		-I/usr/include

CPP_FLAGS = -g -O2 -pipe -Wall -W -Woverloaded-virtual -MMD -MP -fPIC -pthread $(shell root-config --cflags)

LINK_LIBRARY_FLAGS = -shared -Wall -g -O0 -rdynamic ${LIBRARY_PATH} ${LIBRARIES}
LINK_EXECUTABLE_FLAGS = -Wall -g -O0 -rdynamic ${LIBRARY_PATH} ${LIBRARIES}

.PHONY: all _all clean _cleanall build _buildall install _installall rpm _rpmall test _testall spec_update

default: build

clean: _cleanall
_cleanall:
#	Current file types found below. Transition into structure above
	rm -rf *.exe*
	rm -rf *.o*
	rm -rf *.d*

all: _all
build: _all
buildall: _all

build: _all
buildall: _all
_all: BTaggingPlotter.exe

${LIBRARY_OBJECT_FILES}: %.o : %.cc
	g++ -c ${CPP_FLAGS}  $< -o $@
-include $(LIBRARY_OBJECT_FILES:.o=.d)


BTaggingPlotter.exe: bTaggingPlotting.o AnalysisFileEvent.o
	g++ $(CPP_FLAGS) ${LIBRARY_PATH} ${LIBRARIES} -o BTaggingPlotter.exe bTaggingPlotting.o AnalysisFileEvent.o

bTaggingPlotting.o: bTaggingPlotting.cxx
	g++ $(CPP_FLAGS) $(INCLUDE_PATH) -c $<

AnalysisFileEvent.o: AnalysisFileEvent.cc
	g++ $(CPP_FLAGS) $(INCLUDE_PATH) -c $<

