
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
#include "ljmd.h"

using namespace LJMD_NS;

int main(int argc, char **argv)
{
  // Initilize MPI
  MPI_Init(&argc, &argv);

  // Create a LJMD instance
  LJMD *ljmd = new LJMD(argc, argv, MPI_COMM_WORLD);
  
  // begin reading and interpreting input file
  //ljmd->input->file();

  // Delete the instance
  delete ljmd;

  // Cleanup MPI
  MPI_Finalize();
}
