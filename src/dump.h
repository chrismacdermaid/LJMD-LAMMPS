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

#ifndef LJMD_DUMP_H
#define LJMD_DUMP_H

#include "stdio.h"
#include "pointers.h"
#include "lmptype.h"

namespace LJMD_NS {

class Dump : protected Pointers {
 public:

  char *id;                  // user-defined name of Dump
  char *style;               // style of Dump
  char *filename;            // user-specified file

// static Dump *dumpptr;         // holds a ptr to Dump currently being used

  Dump(class LJMD *, int, char **);
  virtual ~Dump();
  void init();
  void write();

  protected:
  int me,nprocs;             // proc info
  char *format_default;      // default format string
  char *format;              // format string for the file write  

  virtual void init_style() = 0;
  virtual void openfile();
  virtual void write_header() = 0;
  virtual void write_data() = 0;

  FILE *fp;                  // file to write dump to
   
  };
}

#endif
