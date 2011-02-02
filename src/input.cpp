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
#include "lmptype.h"
#include "atom.h"
#include "memory.h"
#include "error.h"
#include "universe.h"
#include "input.h"

using namespace LJMD_NS;

Input::Input(LJMD *ljmd) : Pointers(ljmd)
{

 restfile = NULL;

}

Input::~Input()
{
  // Empty
}

void Input::read_restart()
{

/* read restart, only master. */

  double *rx = atom->rx;
  double *ry = atom->ry;
  double *rz = atom->rz;
  
  double *vx = atom->vx;
  double *vy = atom->vy;
  double *vz = atom->vz;
  
  
  if (universe->me == 0) {

        fp=fopen(restfile,"r");
        
        if(fp) {
         
          int i;

          //Read positions
          for (i=0; i<atom->natoms; ++i) {
                fscanf(fp,"%lf%lf%lf",rx+i, ry+i, rz+i);
          }
          
          // Read velocities 
          for (i=0; i<atom->natoms; ++i) {
                fscanf(fp,"%lf%lf%lf",vx+i, vy+i, vz+i);
          }

          fclose(fp);
        
        } else {
          error->all("cannot read restart file");  
        }
    
    } else {
        atom->azzero();
    }
}
