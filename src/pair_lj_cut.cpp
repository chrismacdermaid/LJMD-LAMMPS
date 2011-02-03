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

/* ----------------------------------------------------------------------
   Contributing author: Paul Crozier (SNL)
------------------------------------------------------------------------- */

#include "mpi.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "pair_lj_cut.h"
#include "atom.h"
#include "force.h"
#include "update.h"
#include "universe.h"
#include "domain.h"
//#include "integrate.h"
#include "memory.h"
#include "error.h"

using namespace LJMD_NS;

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

/* ---------------------------------------------------------------------- */

PairLJCut::PairLJCut(LJMD *ljmd) : Pair(ljmd)
{

  c12 = 0.0;
  c6 = 0.0;  
  
  cx = NULL;
  cy = NULL;
  cz = NULL;
}

/* ---------------------------------------------------------------------- */

PairLJCut::~PairLJCut()
{

  if (allocated) {
    
    delete [] cx;
    delete [] cy;
    delete [] cz;

  }

}

/* ---------------------------------------------------------------------- */

void PairLJCut::init_style()
{

  /* Compute c12 and c6 prefactors since we have a  
     atomically pure system */ 

     c12 = 4.0*atom->epsilon*pow(atom->sigma,12.0);
     c6  = 4.0*atom->epsilon*pow(atom->sigma, 6.0);

     rcsq = rcut * rcut;

     // Allocate temp storage for forces
     allocate(); 
}


void PairLJCut::compute()
{
  /* Calculate the force from the LJ 6,12 potential */ 
  
   int natoms = atom->natoms;

  // vdw pe
  eng_vdwl = 0.0; 

  // Zero the force arrays
  clear_force();

  // Broadcast atom positions 
  MPI_Bcast(atom->rx, natoms, MPI_DOUBLE, 0, world); 
  MPI_Bcast(atom->ry, natoms, MPI_DOUBLE, 0, world); 
  MPI_Bcast(atom->rz, natoms, MPI_DOUBLE, 0, world); 

  /* The main force loop, assign each proc an index to work on
     in the upper triangular part of the force matrix */  
 
  int ii;
  for (ii = 0; ii < (natoms-1); ii += universe->nprocs) {   
    int i,j;
    double rx1, ry1, rz1;

    i = ii + universe->me;
    if (i >= (natoms-1)) break;
 
    rx1 = atom->rx[i];     
    ry1 = atom->ry[i];     
    rz1 = atom->rz[i];     

    for (j = i+1; j < natoms; ++j) {
      double drx, dry, drz, rsq;
     
      /* Use pbc to get minimum distance between particle i and j */
      drx = domain->pbc(rx1 - atom->rx[j], domain->xby2, domain->x);
      dry = domain->pbc(ry1 - atom->ry[j], domain->yby2, domain->y);
      drz = domain->pbc(rz1 - atom->rz[j], domain->zby2, domain->z);
      
      rsq = drx*drx + dry*dry + drz*drz;

      /* Compute force if within cutoff */

      if (rsq < rcsq) {
        double r6, rinv, ffac;
        
        rinv = 1.0/rsq;
        r6 = rinv * rinv * rinv;

        ffac = (12.0*c12*r6 - 6.0*c6)*r6*rinv;
        
        eng_vdwl += r6*(c12*r6 - c6);

        cx[i] += drx * ffac;
        cy[i] += dry * ffac;
        cz[i] += drz * ffac;
        cx[j] -= drx * ffac;
        cy[j] -= dry * ffac;
        cz[j] -= drz * ffac;
      }
    }
  }

  /* Sum partial forces to process with rank 0 */ 
  MPI_Reduce(cx, atom->fx, natoms, MPI_DOUBLE, MPI_SUM, 0, world); 
  MPI_Reduce(cy, atom->fy, natoms, MPI_DOUBLE, MPI_SUM, 0, world); 
  MPI_Reduce(cz, atom->fz, natoms, MPI_DOUBLE, MPI_SUM, 0, world); 
 
}

void PairLJCut::clear_force()
{  
  for (int i = 0; i < atom->natoms; i++) {
    cx[i] = 0.0;
    cy[i] = 0.0;
    cz[i] = 0.0;
  }
}

void PairLJCut::allocate()
{

    allocated = 1;

    /* Allocate memory for temporary storage of forces */  
    cx = new double[atom->natoms];
    cy = new double[atom->natoms];
    cz = new double[atom->natoms];

    /*
    cx = memory->create_1d_double_array(0, atom->natoms, "pair:cx");
    cy = memory->create_1d_double_array(0, atom->natoms, "pair:cy");
    cz = memory->create_1d_double_array(0, atom->natoms, "pair:cz");
    */
}

