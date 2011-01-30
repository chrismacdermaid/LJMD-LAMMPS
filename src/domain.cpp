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

/* Defines box and boundry conditions */

#include "mpi.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "domain.h"
#include "atom.h"
#include "force.h"
#include "update.h"
#include "modify.h"
#include "fix.h"
#include "memory.h"
#include "error.h"

using namespace LJMD_NS;

#define BIG   1.0e20
#define SMALL 1.0e-4
#define DELTA 1
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

Domain::Domain(LJMD *ljmd) : Pointers(ljmd)
{

  x = y = z = 0.0;
  xby2 = yby2 = zby2 = 0.0;

}

Domain::~Domain()
{

  //Empty

}


void Domain::init()
{
  xby2 = 0.5 * x;
  yby2 = 0.5 * y;
  zby2 = 0.5 * z;
}

void Domain::set_initial_box()
{

}

void Domain::pbc()
{

  // Helper for pbc

}
  



