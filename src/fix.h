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

/* Fix parent class. A fix is anything that modifies the system during timestepping
   such as the atom positions or velocities, forces, momentum etc.. The child classes
   actually provide the specific implementations such as NVE or NPT 
*/

#ifndef LJMD_FIX_H
#define LJMD_FIX_H

#include "pointers.h"
#include "lmptype.h"

namespace LJMD_NS {
  class Fix : protected Pointers {
  public:
    
    char *id,*style;
    int time_integrate;      // 1 if fix performs time integration, 0 otherwise.
    int thermo_energy;       // 1 if fix_modify enabled ThEng, 0 if not
    int nevery;              // how often to call an end_of_step fix

    // Mask settings, same in modify.cpp

    int INITIAL_INTEGRATE, POST_INTEGRATE, FINAL_INTEGRATE;
    int PRE_FORCE, POST_FORCE;
    int END_OF_STEP;
    int THERMO_ENERGY;

    virtual int setmask() = 0;
    
    // Constructor & Destructor
    Fix(class LJMD *, int, char **);
    virtual ~Fix();

    // Child Class functions
    virtual void init() {}
    virtual void initial_integrate() {}
    virtual void post_integrate() {}
    virtual void final_integrate() {}
    virtual void pre_force() {}
    virtual void post_force() {}
    virtual void end_of_step() {}

    virtual double compute_scalar() {return 0.0;}
    virtual double compute_vector(int) {return 0.0;}
    virtual double compute_array(int,int) {return 0.0;}    

  private:

  };
}

#endif
