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
  delete [] rx; 
  delete [] ry; 
  delete [] rz; 
  
  delete [] vx; 
  delete [] vy; 
  delete [] vz; 
  
  delete [] fx; 
  delete [] fy; 
 // delete [] fz; 

}

void Atom::init()
{
  
  // Allocate memory for coordinates, velocities and forces

  rx = new double[natoms]; 
  ry = new double[natoms]; 
  rz = new double[natoms]; 
  vx = new double[natoms]; 
  vy = new double[natoms]; 
  vz = new double[natoms]; 
  fx = new double[natoms]; 
  fy = new double[natoms]; 
  fz = new double[natoms]; 

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

/* Zero out the position, velocity and force arrays */

void Atom::azzero()
{  
  for (int i = 0; i < natoms; i++) {
    *rx++ = 0.0;
    *ry++ = 0.0;
    *rz++ = 0.0;

    *vx++ = 0.0;
    *vy++ = 0.0;
    *vz++ = 0.0;
    
    *fx++ = 0.0;
    *fy++ = 0.0;
    *fz++ = 0.0;
  }
}

/* Print out the positions, velocities and forces */ 

void Atom::aprint()
{  
  
  if (universe->me == 0)
    for (int i = 0; i < natoms; i++) {
      
      fprintf(screen, "r = %5d %15.6lf %15.6lf %15.6lf\n", i, rx[i], ry[i], rz[i]);
      fprintf(screen, "v = %5d %15.6lf %15.6lf %15.6lf\n", i, vx[i], vy[i], vz[i]); 
      fprintf(screen, "f = %5d %15.6lf %15.6lf %15.6lf\n", i, fx[i], fy[i], fz[i]); 
      
    }
}
