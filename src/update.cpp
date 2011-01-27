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

#include "string.h"
#include "stdlib.h"
#include "update.h"
#include "memory.h"
#include "error.h"
#include "force.h"

using namespace LJMD_NS;

Update::Update(LJMD *ljmd) : Pointers(ljmd)
{

  // Class Constructor

  dt = 0.0;
  nsteps = 0;
  ntimestep = 0;  
}

Update::~Update()
{
  // Class Destructor

}

void Update::init()
{
  //Initilize

}

void Update::set_units(const char *style)
{
  
  // Setup constants and conversions for a particluar system
  
   if (strcmp(style,"lj") == 0) {
     force->kboltz = 0.0019872067;       // Boltzman constant in kcal/mol/K 
     force->mvsq2e = 2390.05736153349;   // m*v^2 in kcal/mol, Argon	    
     force->mvv2e = 1.0;                 // conversion of mv^2 to energy
     dt = 5.0;                           // Timestep in ns
   }
  
}
