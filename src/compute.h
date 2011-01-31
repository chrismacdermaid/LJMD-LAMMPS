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

/* Compute parent class*/

#ifndef LMP_COMPUTE_H
#define LMP_COMPUTE_H

#include "pointers.h"
#include "lmptype.h"


namespace LJMD_NS {

  class Compute : protected Pointers {
  public:
  
  char *id, *style;         // Style ID and name
  int timeflag;             // 1 if Compute stores list of timesteps it's called on

  double scalar;            // computed global scalar
  double *vector;           // computed global vector
  double **array;           // computed global array

  double dof;         // degrees-of-freedom for temperature

  Compute(class LJMD *, int, char **);
  virtual ~Compute();

  virtual void init() = 0;
  virtual double compute_scalar() {return 0.0;}
  };
}

#endif
