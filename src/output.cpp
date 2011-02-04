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

/* Just does screen output now, dumps later */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "output.h"
#include "atom.h"
#include "update.h"
#include "domain.h"
#include "thermo.h"
#include "modify.h"
#include "compute.h"
#include "force.h"
#include "memory.h"
#include "error.h"
#include "universe.h"
#include "dump.h"
#include "style_dump.h"

using namespace LJMD_NS;

#define DELTA 1

#define MYMIN(a,b) ((a) < (b) ? (a) : (b))
#define MYMAX(a,b) ((a) > (b) ? (a) : (b))

/* ----------------------------------------------------------------------
   initialize all output 
------------------------------------------------------------------------- */

Output::Output(LJMD *ljmd) : Pointers(ljmd)
{
  // create default computes for temp,pe,ke

  char **newarg = new char*[4];
  newarg[0] = (char *) "thermo_temp";
  newarg[1] = (char *) "all";
  newarg[2] = (char *) "temp";
  modify->add_compute(3,newarg);

  newarg[0] = (char *) "thermo_pe";
  newarg[1] = (char *) "all";
  newarg[2] = (char *) "pe";
  modify->add_compute(3,newarg);

  newarg[0] = (char *) "thermo_ke";
  newarg[1] = (char *) "all";
  newarg[2] = (char *) "ke";
  modify->add_compute(3,newarg);

  delete [] newarg;

  // create default Thermo class
  newarg = new char*[1];
  newarg[0] = (char *) "one";
  thermo = new Thermo(ljmd,1,newarg);
  delete [] newarg;

  ndump = 0;
  max_dump = 0;
  dump = NULL;

}


Output::~Output()
{
  // Kill Thermo
  if (thermo) delete thermo;
  
  // Kill Dumps
  for (int i = 0; i < ndump; i++) delete dump[i];
  memory->sfree(dump);


}

void Output::init()
{
    /* Initialize thermo class */
    thermo->init();

    /* Initialize the dumps */
    for (int i = 0; i < ndump; i++) dump[i]->init();
}


/* ----------------------------------------------------------------------
   new Thermo style 
------------------------------------------------------------------------- */

void Output::create_thermo(int narg, char **arg)
{
  if (narg < 1) error->all("Illegal thermo_style command");

  delete thermo;
  thermo = NULL;
  thermo = new Thermo(ljmd,narg,arg);
}

/* ----------------------------------------------------------------------
   perform output for setup of run/min
   do dump first, so memory_usage will include dump allocation
   do thermo last, so will print after memory_usage
------------------------------------------------------------------------- */

void Output::setup()
{
  bigint ntimestep = update->ntimestep;


  // Write out initial configuration
  for (int i = 0; i < ndump; i++)
    dump[i]->write();

  thermo->header();
  thermo->compute();
}

void Output::write()
{

  // Write out dumps
  for (int i = 0; i < ndump; i++)
    dump[i]->write();
  
  // Write out thermodynamics 
  thermo->compute();
}

/* ----------------------------------------------------------------------
   add a Dump to list of Dumps 
------------------------------------------------------------------------- */

void Output::add_dump(int narg, char **arg)
{
  if (narg < 5) error->all("Illegal dump command");

  // error checks

  for (int idump = 0; idump < ndump; idump++)
    if (strcmp(arg[0],dump[idump]->id) == 0) error->all("Reuse of dump ID");
  if (atoi(arg[3]) <= 0) error->all("Invalid dump frequency");
  
  // extend Dump list if necessary

  if (ndump == max_dump) {
    max_dump += DELTA;
    dump = (Dump **)
      memory->srealloc(dump,max_dump*sizeof(Dump *),"output:dump");
  }


  // create the Dump

  if (0) return;         // dummy line to enable else-if macro expansion

#define DUMP_CLASS
#define DumpStyle(key,Class) \
  else if (strcmp(arg[2],#key) == 0) dump[ndump] = new Class(ljmd,narg,arg);
#include "style_dump.h"
#undef DUMP_CLASS

  else error->all("Invalid dump style");

  ndump++;
}

/* ----------------------------------------------------------------------
   delete a Dump from list of Dumps 
------------------------------------------------------------------------- */

void Output::delete_dump(char *id)
{
  // find which dump it is and delete it

  int idump;
  for (idump = 0; idump < ndump; idump++)
    if (strcmp(id,dump[idump]->id) == 0) break;
  if (idump == ndump) error->all("Could not find undump ID");

  delete dump[idump];

  // move other dumps down in list one slot

  for (int i = idump+1; i < ndump; i++) {
    dump[i-1] = dump[i];
  }
  ndump--;
}

