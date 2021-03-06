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

#include "stdio.h"
#include "string.h"
#include "fix_nve.h"
#include "atom.h"
#include "force.h"
#include "update.h"
#include "error.h"

using namespace LJMD_NS;

/* ---------------------------------------------------------------------- */

FixNVE::FixNVE(LJMD *ljmd, int narg, char **arg) :
  Fix(ljmd, narg, arg)
{
  if (strcmp(style,"nve") != 0 && narg < 3)
    error->all("Illegal fix nve command");

  time_integrate = 1;
}

/* ---------------------------------------------------------------------- */

int FixNVE::setmask()
{
  int mask = 0;
  mask |= INITIAL_INTEGRATE;
  mask |= FINAL_INTEGRATE;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixNVE::init()
{

    dtmf = 0.5*update->dt / force->mvsq2e / atom->mass;
}

/* ----------------------------------------------------------------------
------------------------------------------------------------------------- */

void FixNVE::initial_integrate()
{

   /* Update velocities by half, positions by full step */

  for (int i = 0; i < atom->natoms; i++) {
    atom->vx[i] += dtmf * atom->fx[i];
    atom->vy[i] += dtmf * atom->fy[i];
    atom->vz[i] += dtmf * atom->fz[i];

    atom->rx[i] += update->dt * atom->vx[i]; 
    atom->ry[i] += update->dt * atom->vy[i]; 
    atom->rz[i] += update->dt * atom->vz[i]; 
  }

}

/* ---------------------------------------------------------------------- */

void FixNVE::final_integrate()
{
  /* Propagate velocities by another half step */
  
  for (int i = 0; i < atom->natoms; i++) {
    atom->vx[i] += dtmf * atom->fx[i];
    atom->vy[i] += dtmf * atom->fy[i];
    atom->vz[i] += dtmf * atom->fz[i];
  }
}

/* ---------------------------------------------------------------------- */
