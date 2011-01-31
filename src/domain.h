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

/* Defines box and boundry conditions */

#ifndef LJMD_DOMAIN_H
#define LJMD_DOMAIN_H

#include "pointers.h"

namespace LJMD_NS {

  class Domain : protected Pointers {
  public:

    int box_exist;                         // 0 = not yet created, 1 = exists
    double x,y,z;                          // Box dimensions in angstroms
    double xby2,yby2,zby2;                 // x/2, y/2, z/2 dimensions

     Domain(class LJMD *);
     ~Domain();

     void init();
     void set_initial_box();                 // Setup initial box
     double pbc(double, const double, const double);  // pbc helper

  };
}

#endif
   
