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
 * to illustrate modularity.
 *
 * This code is intended for educational use only
 * and should not be used in a production type environment.
 * Results should not be trusted. You've been warned.  
 ********************************************************** */

/* Setup and define arrays for storing atom coordinates and propertities */

#ifndef LJMD_ATOM_H
#define LJMD_ATOM_H

#include "pointers.h"
#include "lmptype.h"

namespace LJMD_NS {

	class Atom : protected Pointers {
	  public :
	    
	    bigint natoms;   // Total number of atoms in the system
	    double **x;      // coordinates
	    double **v;      // velocities
	    double **f;      // force
	    double *mass;    // mass of atoms
	    
	    Atom(class LJMD *); // Constructor
	    ~Atom();            // Destructor
	    
	    // Functions
	    void init();
	

  };
}

#endif
