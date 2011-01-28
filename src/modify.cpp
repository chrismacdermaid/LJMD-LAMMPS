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

/* Creates and manages fixes and computes dynamically,
thus, anything that modifes the system's properties 
belongs to modify */


#include "stdio.h"
#include "string.h"
#include "modify.h"
//#include "style_compute.h"
#include "style_fix.h"
#include "atom.h"
#include "fix.h"
//#include "compute.h"
#include "update.h"
#include "memory.h"
#include "error.h"

using namespace LJMD_NS;

/* Number of finite fixes/computes to create each time we run out
   of space to store them */

#define DELTA 4

// Mask Settings
#define INITIAL_INTEGRATE 1
#define POST_INTEGRATE 2
#define FINAL_INTEGRATE 4
#define PRE_FORCE 8
#define POST_FORCE 16
#define END_OF_STEP 32
#define THERMO_ENERGY 64

#define MIN(A,B) ((A) < (B)) ? (A) : (B)
#define MAX(A,B) ((A) > (B)) ? (A) : (B)

#define BIG 1.0e20

Modify::Modify(LJMD *ljmd) : Pointers(ljmd)
{
  // Initalize Fix variables 
  nfix = maxfix = 0;
  n_initial_integrate = n_post_integrate = n_final_integrate = 0;
  n_pre_force = n_post_force = 0;
  n_end_of_step = 0;
  n_thermo_energy = 0;

  fix = NULL;
  fmask = NULL;
  
  list_initial_integrate = list_post_integrate = list_final_integrate = NULL;
  list_pre_force = list_post_force = NULL;
  list_end_of_step = NULL;
  list_thermo_energy = NULL;

  // Initialize compute variables
  ncompute = maxcompute = 0;
  compute = NULL;

}

Modify::~Modify()
{

  // Delete fixes
  while (nfix) delete_fix(fix[0]->id);

  memory->sfree(fix);
  memory->sfree(fmask);

  // Delete computes

  for (int i = 0; i < ncompute; i++) delete compute[i];
  memory->sfree(compute);

  delete [] list_initial_integrate;
  delete [] list_post_integrate;
  delete [] list_final_integrate;
  delete [] list_pre_force;
  delete [] list_post_force;
  delete [] list_end_of_step;
  delete [] list_thermo_energy;

}

Modify::init()
{

  /* All the fixes and computes should be created
     before this is called... */

  // Make list of specific fix functions to call during timestepping using bit mask
  list_init(INITIAL_INTEGRATE,n_initial_integrate,list_initial_integrate);
  list_init(POST_INTEGRATE,n_post_integrate,list_post_integrate);
  list_init(FINAL_INTEGRATE,n_final_integrate,list_final_integrate);
  list_init(PRE_FORCE,n_pre_force,list_pre_force);
  list_init(POST_FORCE,n_post_force,list_post_force);
  list_init_end_of_step(END_OF_STEP,n_end_of_step,list_end_of_step);
  list_init_thermo_energy(THERMO_ENERGY,n_thermo_energy,list_thermo_energy);

  // Initilize all the fixes
  for (i = 0; i < nfix; i++) fix[i]->init();

  // Make a list of computes and initizlize them
  list_init_compute();
  for (i = 0; i < ncompute; i++) compute[i]->init();

}


 /* ----------------------------------------------------------------------
   1st half of integrate call, only for relevant fixes
------------------------------------------------------------------------- */

void Modify::initial_integrate()
{
  for (int i = 0; i < n_initial_integrate; i++)
    fix[list_initial_integrate[i]]->initial_integrate();
}

/* ----------------------------------------------------------------------
   post_integrate call, only for relevant fixes
------------------------------------------------------------------------- */

void Modify::post_integrate()
{
  for (int i = 0; i < n_post_integrate; i++)
    fix[list_post_integrate[i]]->post_integrate();
}

/* ----------------------------------------------------------------------
   2nd half of integrate call, only for relevant fixes
------------------------------------------------------------------------- */

void Modify::final_integrate()
{
  for (int i = 0; i < n_final_integrate; i++)
    fix[list_final_integrate[i]]->final_integrate();
}

/* ----------------------------------------------------------------------
   pre_force call, only for relevant fixes
------------------------------------------------------------------------- */

void Modify::pre_force(int vflag)
{
  for (int i = 0; i < n_pre_force; i++)
    fix[list_pre_force[i]]->pre_force();
}

/* ----------------------------------------------------------------------
   post_force call, only for relevant fixes
------------------------------------------------------------------------- */

void Modify::post_force(int vflag)
{
  for (int i = 0; i < n_post_force; i++)
    fix[list_post_force[i]]->post_force();
}

void Modify::end_of_step()
{
  for (int i = 0; i < n_end_of_step; i++)
    fix[list_end_of_step[i]]->end_of_step();
}

/* ----------------------------------------------------------------------
   thermo energy call, only for relevant fixes
   called by Thermo class
   compute_scalar() is fix call to return energy
------------------------------------------------------------------------- */

double Modify::thermo_energy()
{
  double energy = 0.0;
  for (int i = 0; i < n_thermo_energy; i++)
    energy += fix[list_thermo_energy[i]]->compute_scalar();
  return energy;
}

/* ----------------------------------------------------------------------
   add a new fix
------------------------------------------------------------------------- */

void Modify::add_fix(int narg, char **arg)
{

  if (narg < 3) error->all("Illegal fix command");

  
  // Check to see if the fix is already defined...
  int ifix;
  for (ifix = 0; ifix < nfix; ifix++)
    if (strcmp(arg[0],fix[ifix]->id) == 0) break;

  /* if we don't have enough space to add a new fix, or no space has been allocated
     to store fixes, do that now */

  if (ifix < nfix)
    error->all("That fix already exists, you can't apply the same fix twice. Delete it first");

 if (nfix == maxfix) {
      maxfix += DELTA;
      fix = (Fix **) memory->srealloc(fix,maxfix*sizeof(Fix *),"modify:fix");
      fmask = (int *) 
	memory->srealloc(fmask,maxfix*sizeof(int),"modify:fmask");
 }

 // Create the fix

  if (0) return;         // dummy line to enable else-if macro expansion
   
  #define FIX_CLASS
  #define FixStyle(key,Class) \
    else if (strcmp(arg[2],#key) == 0) fix[ifix] = new Class(lmp,narg,arg);
  #include "style_fix.h"
  #undef FIX_CLASS

  else error->all("Invalid fix style");

  // Set the fix mask
  fmask[ifix] = fix[ifix]->setmask();

  // Update the total number of fixes
  nfix++;

}


/* ----------------------------------------------------------------------
   delete a Fix from list of Fixes
------------------------------------------------------------------------- */

void Modify::delete_fix(const char *id)
{
  
  int ifix = find_fix(id); // Find the fix index 

  if (ifix < 0) error->all("Could not find fix ID to delete");
  delete fix[ifix];

  // move other Fixes and fmask down in list one slot

  for (int i = ifix+1; i < nfix; i++) fix[i-1] = fix[i];
  for (int i = ifix+1; i < nfix; i++) fmask[i-1] = fmask[i];
  nfix--;
}

/* ----------------------------------------------------------------------
   find a fix by ID
   return index of fix or -1 if not found
------------------------------------------------------------------------- */

int Modify::find_fix(const char *id)
{
  int ifix;
  for (ifix = 0; ifix < nfix; ifix++)
    if (strcmp(id,fix[ifix]->id) == 0) break;
  if (ifix == nfix) return -1;
  return ifix;
}


void Modify::add_compute(int narg, char **arg)
{
  if (narg < 3) error->all("Illegal compute command");

  // error check

  for (int icompute = 0; icompute < ncompute; icompute++)
    if (strcmp(arg[0],compute[icompute]->id) == 0)
      error->all("Reuse of compute ID");

  // extend Compute list if necessary

  if (ncompute == maxcompute) {
    maxcompute += DELTA;
    compute = (Compute **)
      memory->srealloc(compute,maxcompute*sizeof(Compute *),"modify:compute");
  }

  // create the Compute

  if (0) return;         // dummy line to enable else-if macro expansion

#define COMPUTE_CLASS
#define ComputeStyle(key,Class) \
  else if (strcmp(arg[2],#key) == 0) \
    compute[ncompute] = new Class(lmp,narg,arg);
#include "style_compute.h"
#undef COMPUTE_CLASS

  else error->all("Invalid compute style");

  ncompute++;
}

/* ----------------------------------------------------------------------
   delete a Compute from list of Computes
------------------------------------------------------------------------- */

void Modify::delete_compute(char *id)
{
  int icompute = find_compute(id);
  if (icompute < 0) error->all("Could not find compute ID to delete");
  delete compute[icompute];

  // move other Computes down in list one slot

  for (int i = icompute+1; i < ncompute; i++) compute[i-1] = compute[i];
  ncompute--;
}

/* ----------------------------------------------------------------------
   find a compute by ID
   return index of compute or -1 if not found
------------------------------------------------------------------------- */

int Modify::find_compute(char *id)
{
  int icompute;
  for (icompute = 0; icompute < ncompute; icompute++)
    if (strcmp(id,compute[icompute]->id) == 0) break;
  if (icompute == ncompute) return -1;
  return icompute;
}

/* ----------------------------------------------------------------------
   create list of fix indices for fixes which match mask
------------------------------------------------------------------------- */

void Modify::list_init(int mask, int &n, int *&list)
{
  delete [] list;

  n = 0;
  for (int i = 0; i < nfix; i++) if (fmask[i] & mask) n++;
  list = new int[n];

  n = 0;
  for (int i = 0; i < nfix; i++) if (fmask[i] & mask) list[n++] = i;
}

/* ----------------------------------------------------------------------
   create list of fix indices for end_of_step fixes
   also create end_of_step_every[]
------------------------------------------------------------------------- */

void Modify::list_init_end_of_step(int mask, int &n, int *&list)
{
  delete [] list;
  delete [] end_of_step_every;

  n = 0;
  for (int i = 0; i < nfix; i++) if (fmask[i] & mask) n++;
  list = new int[n];
  end_of_step_every = new int[n];

  n = 0;
  for (int i = 0; i < nfix; i++)
    if (fmask[i] & mask) {
      list[n] = i;
      end_of_step_every[n++] = fix[i]->nevery;
    }
}

/* ----------------------------------------------------------------------
   create list of fix indices for thermo energy fixes
   only added to list if fix has THERMO_ENERGY mask
   and its thermo_energy flag was set via fix_modify
------------------------------------------------------------------------- */

void Modify::list_init_thermo_energy(int mask, int &n, int *&list)
{
  delete [] list;

  n = 0;
  for (int i = 0; i < nfix; i++)
    if (fmask[i] & mask && fix[i]->thermo_energy) n++;
  list = new int[n];

  n = 0;
  for (int i = 0; i < nfix; i++)
    if (fmask[i] & mask && fix[i]->thermo_energy) list[n++] = i;
}

/* ----------------------------------------------------------------------
   create list of compute indices for computes which store invocation times
------------------------------------------------------------------------- */

void Modify::list_init_compute()
{
  delete [] list_timeflag;

  n_timeflag = 0;
  for (int i = 0; i < ncompute; i++)
    if (compute[i]->timeflag) n_timeflag++;
  list_timeflag = new int[n_timeflag];

  n_timeflag = 0;
  for (int i = 0; i < ncompute; i++)
    if (compute[i]->timeflag) list_timeflag[n_timeflag++] = i;
}


