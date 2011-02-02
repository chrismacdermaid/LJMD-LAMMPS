CC=mpicc
CXX=mpicxx
OPT=-ffast-math -fexpensive-optimizations -O3 -funroll-all-loops -msse3
DBG=-g -pg -O0
LIB=-lm

OBJ= atom.o compute.o compute_ke.o compute_pe.o compute_temp.o\
	domain.o error.o fix.o fix_nve.o force.o ljmd.o main.o\
	memory.o modify.o output.o pair.o pair_lj_cut.o thermo.o\
	universe.o update.o

ALL=ljmd

all: $(ALL)

ljmd : 		$(OBJ)
		$(CXX) $(OPT) $(OBJ) -o ljmd $(LIB)

ljmd-gdb :	$(OBJ)
		$(CXX) $(OPT) $(OBJ) $(DBG) -o ljmd $(LIB)


%.o : %.cpp
		$(CXX) $(OPT) -c $<
clean:
	rm -f $(OBJ)
