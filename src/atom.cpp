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
#include "stdio.h"
#include "string.h"
#include "error.h"
#include "universe.h"
#include "memory.h"
#include "atom.h"

using namespace LJMD_NS;

Atom::Atom(LJMD *ljmd) : Pointers(ljmd)
{

  // Init values
  natoms = 0;
  epsilon = 0;
  sigma = 0;
  mass = NULL;

  rx = ry = rz = NULL; 
  vx = vy = vz = NULL; 
  fx = fy = fz = NULL; 

}

Atom::~Atom()
{
  //Cleanup
  memory->destroy_1d_double_array(rx, 0);
  memory->destroy_1d_double_array(ry, 0);
  memory->destroy_1d_double_array(rz, 0);
  memory->destroy_1d_double_array(vx, 0);
  memory->destroy_1d_double_array(vy, 0);
  memory->destroy_1d_double_array(vz, 0);
  memory->destroy_1d_double_array(fx, 0);
  memory->destroy_1d_double_array(fy, 0);
  memory->destroy_1d_double_array(fz, 0);
}

void Atom::init()
{
  
  // Allocate memory for coordinates, velocities and forces

  rx = memory->create_1d_double_array(0, natoms, "atom:rx");
  ry = memory->create_1d_double_array(0, natoms, "atom:ry");
  rz = memory->create_1d_double_array(0, natoms, "atom:rz");
  vx = memory->create_1d_double_array(0, natoms, "atom:vx");
  vy = memory->create_1d_double_array(0, natoms, "atom:vy");
  vz = memory->create_1d_double_array(0, natoms, "atom:vz");
  fx = memory->create_1d_double_array(0, natoms, "atom:fx");
  fy = memory->create_1d_double_array(0, natoms, "atom:fy");
  fz = memory->create_1d_double_array(0, natoms, "atom:fz");
}

/* ----------------------------------------------------------------------
   count and return words in a single line
   make copy of line before using strtok so as not to change line
   trim anything from '#' onward
------------------------------------------------------------------------- */

int Atom::count_words(const char *line)
{
  int n = strlen(line) + 1;
  char *copy = (char *) memory->smalloc(n*sizeof(char),"copy");
  strcpy(copy,line);

  char *ptr;
  if (ptr = strchr(copy,'#')) *ptr = '\0';

  if (strtok(copy," \t\n\r\f") == NULL) {
    memory->sfree(copy);
    return 0;
  }
  n = 1;
  while (strtok(NULL," \t\n\r\f")) n++;

  memory->sfree(copy);
  return n;
}
