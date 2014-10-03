# source files.

SRC = $(wildcard src/*.c) $(wildcard src/*.cpp)

HDR = $(wildcard include/*.h)

OBJ = $(patsubst src%, ../buildtemp%.o, $(SRC))

OUTLIB = lib/libCGF.a

# include directories
INCLUDES = -I. -I./include/ -I/usr/local/include

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g

# compiler
CCC = g++

# library paths
LIBS = -L../ -L/usr/local/lib -lm -lglut -lglui -lglew

# compile flags
LDFLAGS = -g

.SUFFIXES: .c .cpp

default: $(OUTLIB)

../buildtemp/%.o: src/% $(HDR)
	mkdir -p ../buildtemp
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUTLIB): $(OBJ) $(HDR)
	ar rcs $(OUTLIB) $(OBJ)

clean:
	rm -f $(OBJ) $(OUTLIB)

test:
	echo $(SRC)
	echo $(OBJ)

