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

#ifndef LJMD_ERROR_H
#define LJMD_ERROR_H

#include "pointers.h"

namespace LJMD_NS {

	class Error : protected Pointers {
	 public:
	  Error(class LJMD *);
	 
	  void universe_all(const char *);
	  void universe_one(const char *);
	 
	};

}

#endif
