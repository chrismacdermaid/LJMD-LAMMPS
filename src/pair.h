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

/* Parent "pair" class denoting common functions/calls to all pair
   calculations */

#ifndef LJMD_PAIR_H
#define LJMD_PAIR_H

#include "pointers.h"

namespace LJMD_NS {
  class Pair : protected Pointers {
  public:
    
    double eng_vdwl;    // vdw Energies
    double cutforce;    // distance cutoff for pair computation  

  Pair(class LAMMPS *);
  virtual ~Pair();
  
  // Parent class functions
  void init();

  // child class functions
  virtual void compute() {}
 
 };
}
