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
#include "domain.h"

#include "stdlib.h"

/*
#include "input.h"
#include "neighbor.h"
#include "comm.h"
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
  screen = universe->uscreen;      

  if (universe->me == 0)
    fprintf(universe->uscreen, "LJMD Running on %d procs\n",universe->nprocs);

  create();  //Create the top level classes
//  init();    // Initialize the top level classes 

  setup(); // Setup the LJ system  
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
  domain = new Domain(this);
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
  delete domain;
}

/* Init top level classes */

void LJMD::init()
{
  domain->init();  
  atom->init();
  force->init();
  update->init();
  modify->init();
}

void LJMD::setup()
{
    /* Setup an LJMD run. This is temporary until an input parser is written */

    // Set up 20x20x20 bounding box and initialize it
    domain->x = domain->y = domain->z = 17.1580;
    domain->init();

    /* Specify number of atoms in the system, allocate memory
       for the positions, velocities, and forces, specify atom properties */
    atom->natoms = 100;
    atom->init();
    atom->mass = 39.948; 
    atom->epsilon = 0.2379; 
    atom->sigma = 3.405; 

    // Set the appropirate units, conversions and timestep for our system
    update->set_units("lj");    
    update->nsteps = 10000;

    // Setup the pair potential for the force calculation 
    force->create_pair("lj/cut"); 
    force->init(); 

    // Create fix for nve calculation and integration   
    char **newarg = new char*[1];
    newarg[0] = 0;
    newarg[1] = (char *) "all";
    newarg[2] = (char *) "nve";
    modify->add_fix(0, newarg);

    // Initialize fix and computes
    modify->init();
}
