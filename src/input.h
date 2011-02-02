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

/* Parses input file and reads restarts */

#ifndef LJMD_INPUT_H
#define LJMD_INPUT_H

#include "stdio.h"
#include "pointers.h"

namespace LJMD_NS {
  class Input : protected Pointers {
  public:

    char *restfile;  // Restart filename 
    
    // Constructor & Destructor
    Input(class LJMD *);
    ~Input();

    void read_restart();  // Read the restart file 

  private:
  
    FILE *fp;   // File Pointer

  };
}

#endif
