/* ***********************************************************
 * JLMD++ A C++ version of LJMD motived by LAMMPS.
 *
 * http://sites.google.com/site/akohlmey/software/ljmd
 * and http://lammps.sandia.gov/
 * 
 * Some routines are taken in whole or in part from LJMD
 * or LAMMPS. This software is distributed under the 
 * GNU General Public License.
 *
 * Written by Chris MacDermaid; chris.macdermaid@gmail.com
 *
 * Basic goal: Write LJMD in CPP, but designed like LAMMPS
 * to illustrate the LAMMPS implementation without all the
 * other stuff.
 *
 * This code is intended for educational use only
 * and should not be used in a production type environment.
 * Results should not be trusted. You've been warned.  
 ********************************************************** */

/* Creates potential energy "styles", e.g. pair-styles (LJ, coul),
   bond, angle and dihedral styles */

#ifndef LJMD_FORCE_H
#define LJMD_FORCE_H

#include "pointers.h"

namespace LJMD_NS {
  class Force : protected Pointers {
  public:

    class Pair *pair;
    char *pair_style;

    //Constants and Conversions
    double kboltz;     // Boltzman constant
    double mvsq2e;     // m*v^2 
    double mvv2e;      // Conversion of mv^2 to energy
    
    // Constructor & Destructor
    Force(class LJMD *);
    ~Force();

    // Functions
    void init();

    void create_pair(const char *);
    class Pair *new_pair(const char *);
    class Pair *pair_match(const char *, int);

  };
}

#endif
