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

/* Just does screen output now, dumps later */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "output.h"
#include "atom.h"
#include "update.h"
#include "domain.h"
#include "thermo.h"
#include "modify.h"
#include "compute.h"
#include "force.h"
#include "memory.h"
#include "error.h"
#include "universe.h"

using namespace LJMD_NS;

#define DELTA 1

#define MYMIN(a,b) ((a) < (b) ? (a) : (b))
#define MYMAX(a,b) ((a) > (b) ? (a) : (b))

/* ----------------------------------------------------------------------
   initialize all output 
------------------------------------------------------------------------- */

Output::Output(LJMD *ljmd) : Pointers(ljmd)
{
  // create default computes for temp,pressure,pe

  char **newarg = new char*[4];
  newarg[0] = (char *) "thermo_temp";
  newarg[1] = (char *) "all";
  newarg[2] = (char *) "temp";
  modify->add_compute(3,newarg);

  newarg[0] = (char *) "thermo_pe";
  newarg[1] = (char *) "all";
  newarg[2] = (char *) "pe";
  modify->add_compute(3,newarg);

  delete [] newarg;

  // create default Thermo class
  newarg = new char*[1];
  newarg[0] = (char *) "one";
  thermo = new Thermo(ljmd,1,newarg);
  delete [] newarg;

}


Output::~Output()
{
  if (thermo) delete thermo;
}

void Output::init()
{
    /* Initialize thermo class */
    thermo->init();
}


/* ----------------------------------------------------------------------
   new Thermo style 
------------------------------------------------------------------------- */

void Output::create_thermo(int narg, char **arg)
{
  if (narg < 1) error->all("Illegal thermo_style command");

  delete thermo;
  thermo = NULL;
  thermo = new Thermo(ljmd,narg,arg);
}

/* ----------------------------------------------------------------------
   perform output for setup of run/min
   do dump first, so memory_usage will include dump allocation
   do thermo last, so will print after memory_usage
------------------------------------------------------------------------- */

void Output::setup()
{
  bigint ntimestep = update->ntimestep;

  thermo->header();
  thermo->compute();
}

void Output::write()
{
  thermo->compute();
}
