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

/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#ifndef LJMD_OUTPUT_H
#define LJMD_OUTPUT_H

#include "pointers.h"
#include "lmptype.h"

namespace LJMD_NS {

class Output : protected Pointers {
 public:

    class Thermo *thermo;        // Thermodynamic computations

    int ndump;                   // # of Dumps defined
    int max_dump;                // max size of Dump list
    class Dump **dump;           // list of defined Dumps


    Output(class LJMD *);
    ~Output();

    void init();
    void setup();                      // initial output before run/min
    void create_thermo(int, char **);  // create a thermo style
    void write();                      // output for current timestep


    void add_dump(int, char **);       // add a Dump to Dump list
    void delete_dump(char *);          // delete a Dump from Dump list

  };
}

#endif
