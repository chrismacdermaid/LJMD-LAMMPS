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

#include "string.h"
#include "dump_xyz.h"
#include "atom.h"
#include "error.h"
#include "memory.h"
#include "update.h"

using namespace LJMD_NS;

/* ---------------------------------------------------------------------- */

DumpXYZ::DumpXYZ(LJMD *ljmd, int narg, char **arg) : Dump(ljmd, narg, arg)
{
  if (narg != 5) error->all("Illegal dump xyz command");
  
  char *str = (char *) "Ar  %20.8f %20.8f %20.8f";
  int n = strlen(str) + 1;
  format_default = new char[n];
  strcpy(format_default,str);
}

/* ---------------------------------------------------------------------- */

void DumpXYZ::init_style()
{
  delete [] format;
  char *str;
  str = format_default;

  int n = strlen(str) + 2;
  format = new char[n];
  strcpy(format,str);
  strcat(format,"\n");

  openfile();
}

/* ---------------------------------------------------------------------- */

void DumpXYZ::write_header()
{
  if (me == 0)
    fprintf(fp,BIGINT_FORMAT "\n nfi=%d\n", atom->natoms, update->ntimestep);
}

/* ---------------------------------------------------------------------- */

void DumpXYZ::write_data()
{
  if (me == 0) 
  for (int i = 0; i < atom->natoms; i++) {
    fprintf(fp,format, atom->rx[i], atom->ry[i], atom->rz[i]);
  }
}
