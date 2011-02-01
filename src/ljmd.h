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

/* The main "RUN" loop is found in the verlet.cpp/verlet.h */


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

      // Top level classes
      class Atom *atom;        // Atom based quantities         atom.cpp/atom.h
      class Update *update;    // Integrator based quantities   update.cpp/update.h
      class Force *force;      // Forces                        force.cpp/force.h
      class Modify *modify;    // Fixes and Computes            modify.cpp/modify.h
      class Domain *domain;    // Box boundries and pbc         domain.cpp/domain.h 

      MPI_Comm world;           // MPI Comm
      FILE *screen;             // screen output

      //Constructor and Destructor 
      LJMD(int, char**, MPI_Comm);
      ~LJMD();

      void create();
      void init();
      void destroy();
      void setup();  
  };
}

#endif
