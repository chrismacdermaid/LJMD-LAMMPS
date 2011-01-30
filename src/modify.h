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

/* Creates and manages fixes and computes dynamically,
thus, anything that modifes the system's properties 
belongs to modify */



#ifndef LJMD_MODIFY_H
#define LJMD_MODIFY_H

#include "stdio.h"
#include "pointers.h"
#include "lmptype.h"

namespace LJMD_NS {
  class Modify : protected Pointers {
  public:

    //Fixes
    int nfix, maxfix; // Number of fixes, max number of fixes
    int n_initial_integrate, n_post_integrate, n_final_integrate;
    int n_pre_force, n_post_force;
    int n_end_of_step;
    int n_thermo_energy;

    class Fix **fix;      //List of fixes initilized
    int *fmask;           // Bit mask for applying fixes

    // Computes
    int ncompute, maxcompute;  // Number of computes, max number of computes 
    class Compute **compute;  // List of computes initilized  
    
    // Constructor & Destructor
    Modify(class LJMD *);
    ~Modify();

    // Functions
    void init();
    void initial_integrate();
    void post_integrate();
    void final_integrate();
    void pre_force();
    void post_force();
    void end_of_step();
    double thermo_energy();

    //Fixes
    void add_fix(int, char **);
    void delete_fix(const char *);
    int find_fix(const char *);
    
    //Computes
    void add_compute(int, char **);
    void delete_compute(char *);
    int find_compute(char *);

  private:
    int *list_initial_integrate, *list_post_integrate, *list_final_integrate;
    int *list_pre_force, *list_post_force;
    int *list_end_of_step;
    int *list_thermo_energy;

    int *end_of_step_every;  // ?Not sure about this one. Invoked every step or every n steps?
    int n_timeflag;          // list of computes that store time invocation
    int *list_timeflag;

    /* Functions to create lists of invoked functions at specific 
       parts of the timestep. */

    void list_init(int, int &, int *&);
    void list_init_end_of_step(int, int &, int *&);
    void list_init_thermo_energy(int, int &, int *&);
    void list_init_compute();
    
  };
}

#endif
