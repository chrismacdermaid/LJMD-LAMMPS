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
    memory->destroy_1d_double_array(cx, 0);
    memory->destroy_1d_double_array(cy, 0);
    memory->destroy_1d_double_array(cz, 0);
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
}


void PairLJCut::compute()
{
  /* Calculate the force from the LJ 6,12 potential */ 
  
  double *rx = atom->rx;
  double *ry = atom->ry;
  double *rz = atom->rz;

  double *fx = atom->fx;
  double *fy = atom->fy;
  double *fz = atom->fz;
  
  int natoms = atom->natoms;

  // vdw pe
  eng_vdwl = 0.0; 

  // Zero the force array
  clear_force();

  // Broadcast atom positions 
  MPI_Bcast(rx, natoms, MPI_DOUBLE, 0, universe->uworld); 
  MPI_Bcast(ry, natoms, MPI_DOUBLE, 0, universe->uworld); 
  MPI_Bcast(rz, natoms, MPI_DOUBLE, 0, universe->uworld); 

  /* The main force loop, assign each proc an index to work on
     in the upper triangular part of the force matrix */  
  
  int ii;
  for (ii = 0; ii < (natoms-1); ii += universe->nprocs) {   
    int i,j;
    double rx1, ry1, rz1;

    i = ii + universe->me;
    if (i >= (natoms-1)) break;
    
    rx1 = rx[i];     
    ry1 = ry[i];     
    rz1 = rz[i];     

    for (j = i+1; j < (atom->natoms); j++) {
      double drx, dry, drz, rsq;
      
      drx = domain->pbc(rx1 - rx[j], domain->xby2, domain->x);
      dry = domain->pbc(ry1 - ry[j], domain->yby2, domain->y);
      drz = domain->pbc(rz1 - rz[j], domain->zby2, domain->z);
    
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

  /* Sum partial forces */ 
  MPI_Reduce(cx, fx, natoms, MPI_DOUBLE, MPI_SUM, 0, universe->uworld); 
  MPI_Reduce(cy, fy, natoms, MPI_DOUBLE, MPI_SUM, 0, universe->uworld); 
  MPI_Reduce(cz, fz, natoms, MPI_DOUBLE, MPI_SUM, 0, universe->uworld); 
  
  /* Sum potential - This is done in the compute_pe section */
  //MPI_Reduce(&epot, &eng_vdw, natoms, MPI_DOUBLE, MPI_SUM, 0, universe->uworld); 
}

void PairLJCut::clear_force()
{  
  
  double *x = atom->fx;
  double *y = atom->fy;
  double *z = atom->fz;
  
  for (int i = 0; i < atom->natoms; i++) {
    *x++ = 0.0;
    *y++ = 0.0;
    *z++ = 0.0;
  }
}

void PairLJCut::allocate()
{

    allocated = 1;

    /* Allocate memory for temporary storage of forces */  
    cx = memory->create_1d_double_array(0, atom->natoms, "pair:cx");
    cy = memory->create_1d_double_array(0, atom->natoms, "pair:cy");
    cz = memory->create_1d_double_array(0, atom->natoms, "pair:cz");
}

