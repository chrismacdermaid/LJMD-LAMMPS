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

/* Parent "pair" class denoting common functions/calls to all pair
   calculations */

#include "mpi.h"
#include "float.h"
#include "limits.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "pair.h"
#include "atom.h"
#include "force.h"
#include "update.h"
#include "memory.h"
#include "error.h"

using namespace LJMD_NS;

Pair::Pair(LJMD *ljmd) : Pointers(ljmd)
{

  eng_vdwl = 0.0;

}

Pair::~Pair()
{

  //Empty

}

void Pair::init()
{

  // Style specific initialization
  init_style();

}



