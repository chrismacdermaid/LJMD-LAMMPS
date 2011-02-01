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
#include "compute_pe.h"
#include "atom.h"
#include "update.h"
#include "force.h"
#include "pair.h"
#include "modify.h"
#include "domain.h"
#include "error.h"

using namespace LJMD_NS;

/* ---------------------------------------------------------------------- */

ComputePE::ComputePE(LJMD *ljmd, int narg, char **arg) : 
  Compute(ljmd, narg, arg)
{
  if (narg < 3) error->all("Illegal compute pe command");
}

/* ---------------------------------------------------------------------- */

double ComputePE::compute_scalar()
{
  double one = 0.0;
  if (force->pair)
    one += force->pair->eng_vdwl;

  MPI_Allreduce(&one,&scalar,1,MPI_DOUBLE,MPI_SUM,world);

  return scalar;
}
