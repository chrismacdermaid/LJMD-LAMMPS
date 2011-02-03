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
#include "compute_ke.h"
#include "atom.h"
#include "update.h"
#include "force.h"
#include "error.h"
#include "universe.h"

using namespace LJMD_NS;

/* ---------------------------------------------------------------------- */

ComputeKE::ComputeKE(LJMD *ljmd, int narg, char **arg) :
  Compute(ljmd, narg, arg)
{
  if (narg != 3) error->all("Illegal compute ke command");

}

/* ---------------------------------------------------------------------- */

void ComputeKE::init()
{
  pfactor = 0.5 * force->mvsq2e * atom -> mass;
}

/* ---------------------------------------------------------------------- */

double ComputeKE::compute_scalar()
{
  double ke = 0.0;
  
  for (int i = 0; i < atom->natoms; i++)
    ke += atom->vx[i]*atom->vx[i] 
      + atom->vy[i]*atom->vy[i] 
      + atom->vz[i]*atom->vz[i];
  
  scalar = ke * pfactor;
  return scalar;
}
