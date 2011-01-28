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

/* Fix parent class. A fix is anything that modifies the system during timestepping
   such as the atom positions or velocities, forces, momentum etc.. The child classes
   actually provide the specific implementations such as NVE or NPT 
*/

#include "string.h"
#include "ctype.h"
#include "fix.h"
#include "memory.h"
#include "error.h"


Fix::Fix(JLMD *ljmd, int narg, char **arg) : Pointers(lmp)
{

  // Fix ID
  int n = strlen(arg[0]) + 1;
  id = new char[n];
  strcpy(id,arg[0]);

  for (int i = 0; i < n-1; i++)
    if (!isalnum(id[i]) && id[i] != '_')
      error->all("Fix ID must be alphanumeric or underscore characters");

  // Fix style
  int n = strlen(arg[2]) + 1;
  style = new char[n];
  strcpy(style,arg[2]);

  time_integrate = 0;

  INITIAL_INTEGRATE = 1;
  POST_INTEGRATE = 2;
  FINAL_INTEGRATE = 4;
  PRE_FORCE = 8;
  POST_FORCE = 16;
  END_OF_STEP 32;
  THERMO_ENERGY 64;
}

Fix::~Fix()
{
  delete [] id;
  delete [] style;
}

