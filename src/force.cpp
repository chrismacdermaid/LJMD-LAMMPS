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

//Sets up pair styles

using namespace LJMD_NS;

Force::Force(LJMD *ljmd) : Pointers(ljmd)
{

  // Class Constructor

  kboltz = 0.0;
  mvsq2e = 0.0;
  mvv2e = 0.0;

}

Force::~Force()
{
  //Class Destructor
}

void Force::init()
{

}
