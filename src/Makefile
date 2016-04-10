# C++ compiler (and linker)
CXX=ccache g++

# Include libraries
STRICT_WARNINGS=-Wall -Wno-unused-function -Wno-sign-compare #-Werror 
CFLAGS+= -I. -DDEBUGGING_ON ${STRICT_WARNINGS}
LFLAGS+=

# VTK library
CFLAGS+=-I$(wildcard /usr/include/vtk-5.*)
LFLAGS+=-L$(wildcard /usr/lib/vtk-5.*) -lvtkIO -lvtkCommon -lvtkFiltering

# Linker flags
CFLAGS+= -frounding-math
LFLAGS+=-lpthread

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
EXE=bc

all: $(SRC) $(EXE) $(OBJ) tags
all: CFLAGS+=-O3 -fopenmp
all: LFLAGS+= -fopenmp

debug: $(SRC) $(EXE) $(OBJ)
debug: CFLAGS+=-g -fopenmp
debug: LFLAGS+=-g -fopenmp

profile: $(SRC) $(EXE) $(OBJ)
profile: CFLAGS+=-O2 -pg
profile: LFLAGS+=-pg

doc:
	@cd ../doc && doxygen

tags: $(SRC)
	@ctags -R .

$(EXE): $(OBJ) $(EX_LIB_FILE)
	$(CXX) $(OBJ) $(LFLAGS) -o $@

.cpp.o:
	$(CXX) -c -std=c++14 $(CFLAGS) $< -o $@

clean:
	@rm -f $(OBJ) tags
