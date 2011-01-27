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

#include "mpi.h"
#include "stdlib.h"
#include "error.h"
#include "universe.h"
#include "memory.h"
#include "atom.h"

using namespace LJMD_NS;

Atom::Atom(LJMD *ljmd) : Pointers(ljmd)
{

  // Init values
  natoms = 100;
  x = v = f = NULL;
  mass = NULL;

}

Atom::~Atom()
{
  //Cleanup
  memory->destroy_2d_double_array(x);
  memory->destroy_2d_double_array(v);
  memory->destroy_2d_double_array(f);
}

void Atom::init()
{
  
  // Allocate memory for coordinates, velocities and forces

  x = memory->create_2d_double_array(natoms+1, 3, "atom:x");
  v = memory->create_2d_double_array(natoms+1, 3, "atom:v");
  f = memory->create_2d_double_array(natoms+1, 3, "atom:f");

}

