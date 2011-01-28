/* ***********************************************************
 * JLMD++ A C++ version of LJMD motived by LAMMPS.
 
 * http://sites.google.com/site/akohlmey/software/ljmd
  and http://lammps.sandia.gov/
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


#include "mpi.h"
#include "string.h"
#include "ljmd.h"
#include "memory.h"
#include "error.h"
#include "universe.h"
#include "atom.h"
#include "update.h"
#include "force.h"

/* Creates force "styles", e.g. pair-styles (LJ, coul),
   bond, angle and dihedral styles used in tabulating the force */


using namespace LJMD_NS;

Force::Force(LJMD *ljmd) : Pointers(ljmd)
{

  // Class Constructor

  /* Initialize constants and conversions they 
     are defined in update.cpp/update.h */

  kboltz = 0.0;
  mvsq2e = 0.0;
  mvv2e = 0.0;

  // Styles
  pair = NULL;

  char *str = (char *) "none";
  int n = strlen(str) + 1;
  pair_style = new char[n];
  strcpy(pair_style,str);
}

Force::~Force()
{
  //Class Destructor

  delete [] pair_style;

  // Delete styles
  if (pair) delete pair;

}

void Force::init()
{
  // Initialize the styles
  if (pair) pair->init();      
}

void Force::create_pair(const char *style)
{
  delete [] pair_style;
  if (pair) delete pair;

  pair = new_pair(style);
  int n = strlen(style) + 1;
  pair_style = new char[n];
  strcpy(pair_style,style);
}

Pair *Force::new_pair(const char *style)
{
  if (strcmp(style,"none") == 0) return NULL;

  #define PAIR_CLASS
  #define PairStyle(key,Class) \
    else if (strcmp(style,#key) == 0) return new Class(lmp);
  #include "style_pair.h"
  #undef PAIR_CLASS

  else error->all("Invalid pair style");
  return NULL;
}
