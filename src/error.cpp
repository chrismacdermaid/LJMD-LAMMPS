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

#include "mpi.h"
#include "stdlib.h"
#include "error.h"
#include "universe.h"
#include "memory.h"

using namespace LJMD_NS;

/* ---------------------------------------------------------------------- */

Error::Error(LJMD *ljmd) : Pointers(ljmd) {}

/* ----------------------------------------------------------------------
   called by all procs in universe
   close all output, screen, and log files in world and universe
------------------------------------------------------------------------- */

void Error::universe_all(const char *str)
{
  MPI_Barrier(universe->uworld);

  if (universe->me == 0) {
    if (universe->uscreen) fprintf(universe->uscreen,"ERROR: %s\n",str);
  }

  MPI_Finalize();
  exit(1);
}

/* ----------------------------------------------------------------------
   called by one proc in universe
------------------------------------------------------------------------- */

void Error::universe_one(const char *str)
{
  if (universe->uscreen)
    fprintf(universe->uscreen,"ERROR on proc %d: %s\n",universe->me,str);
    MPI_Abort(universe->uworld,1);
}
