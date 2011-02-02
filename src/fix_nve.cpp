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

  double *rx = atom->rx;
  double *ry = atom->ry;
  double *rz = atom->rz;
  double *vx = atom->vx;
  double *vy = atom->vy;
  double *vz = atom->vz;
  double *fx = atom->fx;
  double *fy = atom->fy;
  double *fz = atom->fz;

  /* Update velocities by half, positions by full step */

  for (int i = 0; i < atom->natoms; i++) {
    vx[i] += dtmf * fx[i];
    vy[i] += dtmf * fy[i];
    vz[i] += dtmf * fz[i];

    //fprintf(screen, "%15.6f %15.6f %15.6f\n", vx[i], vy[i], vz[i]); 
    //fprintf(screen, "%15.6f %15.6f %15.6f\n", fx[i], fy[i], fz[i]); 

    rx[i] += update->dt * vx[i]; 
    ry[i] += update->dt * vy[i]; 
    rz[i] += update->dt * vz[i]; 
  }

}

/* ---------------------------------------------------------------------- */

void FixNVE::final_integrate()
{
  double *vx = atom->vx;
  double *vy = atom->vy;
  double *vz = atom->vz;
  double *fx = atom->fx;
  double *fy = atom->fy;
  double *fz = atom->fz;

  /* Propagate velocities by another half step */
  
  for (int i = 0; i < atom->natoms; i++) {
    vx[i] += dtmf * fx[i];
    vy[i] += dtmf * fy[i];
    vz[i] += dtmf * fz[i];
  }
}

/* ---------------------------------------------------------------------- */
