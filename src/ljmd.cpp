/* ***********************************************************
 * JLMD++ A C++ version of LJMD motived by LAMMPS.
 
 * http://sites.google.com/site/akohlmey/software/ljmd
  and http://lammps.sandia.gov/
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

#include "mpi.h"
#include "string.h"
#include "ljmd.h"
#include "memory.h"
#include "error.h"
#include "universe.h"
#include "atom.h"
#include "update.h"
#include "force.h"
#include "modify.h"
#include "force.h"
#include "modify.h"

#include "stdlib.h"

/*
#include "input.h"
#include "neighbor.h"
#include "comm.h"
#include "domain.h"
#include "output.h"
#include "timer.h"
*/

using namespace LJMD_NS;

/* ************************************************************
   Startup an instance of LJMD and allocate fundamental classes
   memory, error, universe, input. Initilize comm. Parse input
   switches.
  ********************************************************** */

LJMD::LJMD(int narg, char **arg, MPI_Comm communicator)
{
  // Fundamental Classes
  memory = new Memory(this);
  error = new Error(this);
  universe = new Universe(this, communicator);

  world = universe->uworld;

  if (universe->me == 0)
    fprintf(universe->uscreen, "LJMD Running on %d procs\n",universe->nprocs);

  create();
  //init();
}

LJMD::~LJMD()
{

  //Kill top level classes
  destroy ();
  
  // Delete fundamental classes
  delete universe;
  delete error;
  delete memory;
}

/* create top level classes */

void LJMD::create()
{
  atom = new Atom(this);
  force = new Force(this);
  update = new Update(this);
  modify = new Modify(this);
}

/* delete top level classes */

void LJMD::destroy()
{
  delete modify;
  delete update;
  delete force;
  delete atom;
}

/* Init top level classes */

void LJMD::init()
{
  atom->init();
  force->init();
  update->init();
  modify->init();
}
  
