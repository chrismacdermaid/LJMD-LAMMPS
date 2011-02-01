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

#include "mpi.h"
#include "string.h"
#include "compute_temp.h"
#include "atom.h"
#include "update.h"
#include "force.h"
#include "domain.h"
#include "modify.h"
#include "fix.h"
#include "error.h"
#include "universe.h"

using namespace LJMD_NS;

/* ---------------------------------------------------------------------- */

ComputeTemp::ComputeTemp(LJMD *ljmd, int narg, char **arg) : 
  Compute(ljmd, narg, arg)
{
  if (narg != 3) error->all("Illegal compute temp command");

}

/* ---------------------------------------------------------------------- */

ComputeTemp::~ComputeTemp()
{

}

/* ---------------------------------------------------------------------- */

void ComputeTemp::init()
{
  dof = 3.0*atom->natoms-3.0;

  tfactor = force->mvsq2e * atom -> mass / (3.0*atom->natoms-3.0) / force->kboltz;
}

/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */

double ComputeTemp::compute_scalar()
{
  double *vx = atom->vx;
  double *vy = atom->vy;
  double *vz = atom->vz;
  double t = 0.0;

  /* Do we need to broadcast the velocities here? */

  for (int ii = 0; ii < atom->natoms; ii += universe->nprocs) {
    int i;

    i = ii + universe->me;

    if (i > atom->natoms) break;

    t += vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i];
  }

  MPI_Allreduce(&t,&scalar,1,MPI_DOUBLE,MPI_SUM,universe->uworld);

  scalar *= tfactor;
  return scalar;
}

/* ---------------------------------------------------------------------- */
