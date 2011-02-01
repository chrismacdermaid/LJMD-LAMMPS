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


#ifndef LJMD_THERMO_H
#define LJMD_THERMO_H

#include "pointers.h"
#include "lmptype.h"

namespace LJMD_NS {

class Thermo : protected Pointers {

 public:
  char *style;

  Thermo(class LJMD *, int, char **);
  ~Thermo();
  void init();
  void header();
  void compute();

 private:
  int me;

  char *line;
  int nfield,nfield_initial;
  char **keyword;
  int *vtype;

  char **format;
  char *format_float_one_def;
  char *format_int_one_def;
  char format_bigint_one_def[8];

  int lineflag;

  bigint natoms;

                         // data used by routines that compute single values
  int ivalue;            // integer value to print
  double dvalue;         // double value to print
  bigint bivalue;        // big integer value to print
  int ifield;            // which field in thermo output is being computed
  int *field2index;      // which compute,fix,variable calcs this field
  int *argindex1;        // indices into compute,fix scalar,vector
  int *argindex2;
                         // data for keyword-specific Compute objects
                         // index = where they are in computes list
                         // id = ID of Compute objects
                         // Compute * = ptrs to the Compute objects
  int index_temp,index_pe;
  char *id_temp,*id_pe;
  class Compute *temperature,*pe;

  int ncompute;                // # of Compute objects called by thermo
  char **id_compute;           // their IDs
  int *compute_which;          // 0/1 if should call scalar() or vector()
  class Compute **computes;    // list of ptrs to the Compute objects

  void allocate();
  void deallocate();

  void parse_fields(char *);
  int add_compute(const char *, int);
 
  typedef void (Thermo::*FnPtr)();
  void addfield(const char *, FnPtr, int);
  FnPtr *vfunc;                // list of ptrs to functions

  // functions that compute a single value
  // customize a new keyword by adding a method prototype

  void compute_step();
  void compute_temp();
  void compute_pe();
  void compute_ke();
  void compute_etotal();
 };

}

#endif
