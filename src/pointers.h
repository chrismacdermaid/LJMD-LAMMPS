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

// Pointers class contains ptrs to master copy of
// fundamental LJMD class ptrs stored in ljmd.h
// every LJMD class inherits from Pointers to access ljmd.h ptrs
// these variables are auto-initialized by Pointer class constructor
// *& variables are really pointers to the pointers in lammps.h
// & enables them to be accessed directly in any class, e.g. error->all()

#ifndef LJMD_POINTERS_H
#define LJMD_POINTERS_H

#include "mpi.h"
#include "ljmd.h"

namespace LJMD_NS {

	class Pointers{
	 public:
	  Pointers(LJMD *ptr) : 
	  ljmd(ptr),
	  memory(ptr->memory),
	  error(ptr->error),
	  universe(ptr->universe),
	  atom(ptr->atom),
	  force(ptr->force),  
	  update(ptr->update),
	  modify(ptr->modify),
          domain(ptr->domain),
          output(ptr->output),
          input(ptr->input),
          screen(ptr->screen),
	  world(ptr->world) {}

	  virtual ~Pointers() {}
	 
	 protected:
	  LJMD *ljmd;
	  Memory *&memory;
	  Error *&error;
	  Universe *&universe;
	  
	  Atom *&atom;
	  Force *&force;
	  Update *&update;
	  Modify *&modify;
          Domain *&domain;
          Output *&output;
          Input *&input;
          
          FILE *&screen;
	  MPI_Comm &world;
	};

}

#endif
