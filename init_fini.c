#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>

#define HERE fprintf(stderr,"%s %d: ",__FILE__,__LINE__)

// This program sends YAML to $SP_OUTFILE or stdout 

static struct timeval t_start; // Start time from myinit
static struct rusage r_start; // rusage from myinit
static FILE* myout; // Hold our own stdout pointer
static char myargv0[256]; // a place to hold a copy of argv[0]

//Loop over argv and print the whole command line (remove this or consolidate to something that returns a single string of the whole command line, if needed.
void print_argv(int argc, char **argv)
{
  for(int i=0; i<argc; ++i)
    fprintf(myout,"%s ",argv[i]);

  fprintf(myout,": ");
}

void myinit(int argc, char **argv, char **envp) {
  // Check for SP_OUTFILE and either open it or dup stdout
  char *sp_outfile=getenv("SP_OUTFILE");

  struct stat b;
  int stat_error=stat(sp_outfile,&b);
  if(sp_outfile && !stat_error) // file already exists, no header, keep going
    {
      myout=fopen(sp_outfile,"a+");
      setvbuf(myout, NULL, _IONBF, 0); // unbuffer output
    }
  else // file didn't already exist. Write header. (Document this)
    {
      myout=fopen(sp_outfile,"a+");
      setvbuf(myout, NULL, _IONBF, 0); // unbuffer output
      fprintf(myout,"---\n");
    }
  // fix error check later
  // Hide LINENO testing code for now
  // char *bash_line=calloc(256,sizeof(char));
  strncpy(myargv0,argv[0],strlen(argv[0]));
  // bash_line=getenv("LINENO");

  gettimeofday(&t_start,NULL);
  getrusage(RUSAGE_SELF,&r_start);

  fprintf(myout,"\n");
  //  print_argv(argc,argv);
  //fprintf(myout,"%s: ",argv[0]);
  // if (bash_line) fprintf(myout,"\n----%s----\n",bash_line);
}

static void myfini(int argc, char **argv, char **envp) {
  
  struct timeval te;
  struct rusage r_end;
  gettimeofday(&te,NULL);
  getrusage(RUSAGE_SELF,&r_end);
  // getrusage times aren't points in time, but elapsed already. No
  // need to diff.
//  double user_elapsed=(r_end.ru_utime.tv_sec - r_start.ru_utime.tv_sec) +
//    1.e-6*(r_end.ru_utime.tv_usec - r_start.ru_utime.tv_usec);
//  double sys_elapsed=(r_end.ru_stime.tv_sec - r_start.ru_stime.tv_sec) +
//    1.e-6*(r_end.ru_stime.tv_usec - r_start.ru_stime.tv_usec);

  double user_elapsed=(r_end.ru_utime.tv_sec) +
    1.e-6*(r_end.ru_utime.tv_usec);
  double sys_elapsed=(r_end.ru_stime.tv_sec) +
    1.e-6*(r_end.ru_stime.tv_usec);

  double elapsed=(double)(te.tv_sec-t_start.tv_sec) +
    1.e-6*(double)(te.tv_usec-t_start.tv_usec);
//  fprintf(myout,"+ %15.7g %15.7g %15.7g: %s\n",
//	  elapsed,user_elapsed,sys_elapsed,myargv0);
  pid_t pid=getpid();
  fprintf(myout,
	  "%d: cmd=\"%s\"\n  etime=%15.7g\n  utime=%15.7g\n  stime= %15.7g\n",
	  pid,myargv0,elapsed,user_elapsed,sys_elapsed);

  // Old attempt at JSON, delete later
  //    fprintf(myout,"\"%d\": {\"cmd\": \"%s\", \"etime\": %15.7g, \"utime\": %15.7g, \"stime\": %15.7g}\n",
  // pid,myargv0,elapsed,user_elapsed,sys_elapsed);
//  fprintf(myout,"elapsed_time+ %15.7g %15.7g %15.7g: %s\n",
  //	  elapsed,user_elapsed,sys_elapsed,myargv0);
}


// Put our functions in the init and fini arrays
static __attribute__((section(".init_array"))) typeof(myinit) *__init = myinit;
static __attribute__((section(".fini_array"))) typeof(myfini) *__fini = myfini;
