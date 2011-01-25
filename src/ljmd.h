/* ***********************************************************
 * JLMD++ A C++ version of LJMD motived by LAMMPS.
 
 * http://sites.google.com/site/akohlmey/software/ljmd
  and http://lammps.sandia.gov/
 * 
 * Some routines are taken in whole or in part from LJMD
 * or LAMMPS. This software is distributed under the 
 * GNU General Public License.
 *
 * Written by Chris MacDermaid; chris.macdermaid@gmail.com
 *
 * Basic goal: Write LJMD in CPP, but designed like LAMMPS
 * to illustrate modularity.
 *
 * This code is intended for educational use only
 * and should not be used in a production type environment.
 * Results should not be trusted. You've been warned.  
 ********************************************************** */

#ifndef LJMD_LJMD_H
#define LJMD_LJMD_H

#include "mpi.h"
#include "stdio.h"

namespace LJMD_NS {
  
  class LJMD {
    public:

      //Pointers to fundamental LJMD Classes 
                                
      class Memory *memory;     // Memory Allocation Routines:  memory.cpp/memory.h
      class Error  *error;      // Error handling:              error.cpp/error.h
      class Universe *universe; // Universe of processors       universe.cpp/universe.h

      //Pointers to top level LJMD Classes 
      class Atom *atom;         // Atom storage:                atom.cpp/atom.h
      class Update *update;     // Integrators:                 update.cpp/update.h
      class Neighbor *neighbor; // Neighbor List                neighbor.cpp/neighbor.h
      class Comm *comm;         // Inter-processor comm         comm.cpp/comm.h
      class Domain *domain;     // simulation domain/pbc        domain.cpp/domain.h
      class Force *force;       // inter-particle forces        force.cpp/force.h
      class Output *output;     // output dumps                 output.cpp/output.h
      class Timer *timer;       // CPU timing info              timer.cpp/timer.h
      class Modify *modify;     // Fixes and computes           fix.cpp/fix.h
      class Input *input        // Input file parsing           input.cpp/input.h

      MPI_Comm world;           // MPI Comm
      FILE *infile;             // The Infile

      //Constructor * Destructor 
      LJMD(int, char** MPI_Comm);
      ~LJMD();

      void create();
      void init();
      void destroy();
  };
}

#endif
