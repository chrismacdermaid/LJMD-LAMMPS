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

#include "mpi.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "compute.h"
#include "lmptype.h"
#include "atom.h"
#include "memory.h"
#include "error.h"

using namespace LJMD_NS;

#define DELTA 4
#define BIG 2000000000

#define MIN(A,B) ((A) < (B)) ? (A) : (B)
#define MAX(A,B) ((A) > (B)) ? (A) : (B)

Compute::Compute(LJMD *ljmd, int narg, char **arg) : Pointers(ljmd)
{
  if (narg < 3) error->all("Illegal compute command");

  // compute ID, group, and style
  // ID must be all alphanumeric chars or underscores

  // The ID of the compute
  int n = strlen(arg[0]) + 1;
  id = new char[n];
  strcpy(id,arg[0]);

 for (int i = 0; i < n-1; i++)
    if (!isalnum(id[i]) && id[i] != '_')
      error->all("Compute ID must be alphanumeric or underscore characters");


  // The style of the compute
  n = strlen(arg[2]) + 1;
  style = new char[n];
  strcpy(style,arg[2]);

  scalar = 0.0;
  vector = NULL;
  array = NULL;

}

Compute::~Compute()
{
  delete [] id;
  delete [] style;
}






