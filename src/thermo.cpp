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


/* Customize log file output formats and computes to call 
   This has been dramatically simplified from the LAMMPS version */

#include "mpi.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "thermo.h"
#include "atom.h"
#include "update.h"
#include "domain.h"
#include "modify.h"
#include "fix.h"
#include "compute.h"
#include "force.h"
#include "pair.h"
#include "memory.h"
#include "error.h"

using namespace LJMD_NS;

#define ONE "step temp epair ekin etotal"

enum{ONELINE,MULTILINE};
enum{INT,FLOAT,BIGINT};
enum{SCALAR,VECTOR,ARRAY};

#define MAXLINE 8192               // make this 4x longer than Input::MAXLINE
#define DELTA 8

#define MIN(A,B) ((A) < (B)) ? (A) : (B)
#define MAX(A,B) ((A) > (B)) ? (A) : (B)

Thermo::Thermo(LJMD *ljmd, int narg, char **arg) : Pointers(ljmd)
{
  MPI_Comm_rank(world,&me);

  int n = strlen(arg[0]) + 1;
  style = new char[n];
  strcpy(style,arg[0]);

  // Thermo Defaults
  lineflag = ONELINE;

  /* Just use the "ONE" line output */
  line = new char[MAXLINE];
  strcpy(line,ONE);

  id_temp = (char *) "thermo_temp";
  id_pe = (char *) "thermo_pe";
 
  /* How many fields to output */
  nfield_initial = atom->count_words(line);
  
  /* Allocate memory for each field */
  allocate();
  
  /* Now parse the fields, and setup computes, fixes, customs */
  parse_fields(line);

  // Format strings
  char *bigint_format = BIGINT_FORMAT;

  format_float_one_def = (char *) "%12.8g";
  format_int_one_def = (char *) "%8d";
  sprintf(format_bigint_one_def,"%%8%s",&bigint_format[1]);

}

Thermo::~Thermo()
{
  delete [] style;
  delete [] line;

  deallocate();
}

/* ---------------------------------------------------------------------- */

void Thermo::init()
{
  int i,n;

  nfield = nfield_initial;

  // set format string for each field
  // include keyword if lineflag = MULTILINE
  // add '/n' every 3 values if lineflag = MULTILINE
  // add trailing '/n' to last value

  char *ptr;
  for (i = 0; i < nfield; i++) {
    format[i][0] = '\0';

   if (vtype[i] == FLOAT) {
     ptr = format_float_one_def;
   } else if (vtype[i] == INT) {
     ptr = format_int_one_def;
   } else if (vtype[i] == BIGINT) {
     ptr = format_bigint_one_def;
   }

    n = strlen(format[i]);
    sprintf(&format[i][n],"%s ",ptr);
   
    if (i == nfield-1) strcat(format[i],"\n");
  }

  /* find current ptr for each Compute ID, they should already exist
     as a result of calling parse_fields in the constructor */

  int icompute;
  for (i = 0; i < ncompute; i++) {
    icompute = modify->find_compute(id_compute[i]);
    if (icompute < 0) error->all("Could not find thermo compute ID");
    computes[i] = modify->compute[icompute];
  }

  // set ptrs to keyword-specific Compute objects

  if (index_temp >= 0) temperature = computes[index_temp];
  if (index_pe >= 0) pe = computes[index_pe];
}


/* ----------------------------------------------------------------------
   allocate all per-field memory
------------------------------------------------------------------------- */

void Thermo::allocate()
{
  // n = specified fields

  int n = nfield_initial;

  keyword = new char*[n];
  for (int i = 0; i < n; i++) keyword[i] = new char[32];
  vfunc = new FnPtr[n];
  vtype = new int[n];

  field2index = new int[n];
  argindex1 = new int[n];
  argindex2 = new int[n];

  // factor of 3 is max number of computes a single field can add

  ncompute = 0;
  id_compute = new char*[3*n];
  compute_which = new int[3*n];
  computes = new Compute*[3*n];

}

/* ----------------------------------------------------------------------
   deallocate all per-field memory
------------------------------------------------------------------------- */

void Thermo::deallocate()
{
  int n = nfield_initial + 1;

  for (int i = 0; i < n; i++) delete [] keyword[i];
  delete [] keyword;
  delete [] vfunc;
  delete [] vtype;

  for (int i = 0; i < n; i++) delete [] format[i];
  delete [] format;

  delete [] field2index;
  delete [] argindex1;
  delete [] argindex2;

  for (int i = 0; i < ncompute; i++) delete [] id_compute[i];
  delete [] id_compute;
  delete [] compute_which;
  delete [] computes;

}

void Thermo::parse_fields(char *str)
{
  nfield = 0;

  // customize a new keyword by adding to if statement

  char *word = strtok(str," \0");
  while (word) {
    
    if (strcmp(word,"step") == 0) {
      addfield("Step",&Thermo::compute_step,BIGINT);
    
    } else if (strcmp(word,"temp") == 0) {
      addfield("Temp",&Thermo::compute_temp,FLOAT);
      index_temp = add_compute(id_temp,SCALAR);
    
    } else if (strcmp(word,"ke") == 0) {
      addfield("KinEng",&Thermo::compute_ke,FLOAT);
      index_temp = add_compute(id_temp,SCALAR);

    } else if (strcmp(word,"pe") == 0) {
      addfield("PotEng",&Thermo::compute_pe,FLOAT);
      index_pe = add_compute(id_pe,SCALAR);
    
    } else if (strcmp(word,"etotal") == 0) {
      addfield("TotEng",&Thermo::compute_etotal,FLOAT);
      index_temp = add_compute(id_temp,SCALAR);
      index_pe = add_compute(id_pe,SCALAR);
    
    } else error->all("Invalid keyword in thermo_style custom command");

    word = strtok(NULL," \0");
  }
}

/* ----------------------------------------------------------------------
   add field to list of quantities to print
------------------------------------------------------------------------- */

void Thermo::addfield(const char *key, FnPtr func, int typeflag)
{
  strcpy(keyword[nfield],key);
  vfunc[nfield] = func;
  vtype[nfield] = typeflag;
  nfield++;
}

/* ---------------------------------------------------------------------- */

void Thermo::header()
{

  int loc = 0;
  for (int i = 0; i < nfield; i++)
    loc += sprintf(&line[loc],"%s ",keyword[i]);
  sprintf(&line[loc],"\n");
  
  if (me == 0) {
    //fprintf(screen,line);
  }
}

/* ---------------------------------------------------------------------- */

void Thermo::compute()
{
  int i;

  bigint ntimestep = update->ntimestep;

   for (i = 0; i < ncompute; i++)
 	computes[i]->compute_scalar();

  // add each thermo value to line with its specific format
  int loc = 0;
  for (ifield = 0; ifield < nfield; ifield++) {
    (this->*vfunc[ifield])();
    if (vtype[ifield] == FLOAT)
      loc += sprintf(&line[loc],format[ifield],dvalue);
    else if (vtype[ifield] == INT) 
      loc += sprintf(&line[loc],format[ifield],ivalue);
    else if (vtype[ifield] == BIGINT) {
      loc += sprintf(&line[loc],format[ifield],bivalue);
    }
  }

  if (me == 0) {
    //fprintf(screen,line);
  }
}

/* ----------------------------------------------------------------------
   add compute ID to list of Compute objects to call
   return location of where this Compute is in list
   if already in list with same which, do not add, just return index
------------------------------------------------------------------------- */

int Thermo::add_compute(const char *id, int which)
{
  int icompute;
  for (icompute = 0; icompute < ncompute; icompute++)
    if ((strcmp(id,id_compute[icompute]) == 0) && 
	which == compute_which[icompute]) break;
  if (icompute < ncompute) return icompute;

  int n = strlen(id) + 1;
  id_compute[ncompute] = new char[n];
  strcpy(id_compute[ncompute],id);
  compute_which[ncompute] = which;
  ncompute++;
  return ncompute-1;
}

void Thermo::compute_step()
{
  bivalue = update->ntimestep;
}

void Thermo::compute_temp()
{
  dvalue = temperature->scalar;
}

void Thermo::compute_pe()
{
  dvalue = pe->scalar;
 }

void Thermo::compute_ke()
{
  dvalue = temperature->scalar;
  dvalue *= 0.5 * temperature->dof * force->kboltz;
 }

void Thermo::compute_etotal()
{
  compute_pe();
  double ke = temperature->scalar;
  ke *= 0.5 * temperature->dof * force->kboltz;
  dvalue += ke;
}
