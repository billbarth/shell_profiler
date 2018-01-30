#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>

// Need a message macro. Do not print to stdout or risk breaking things that rely on stdout of other programs in pipes. Either open own file for errors or write to stderr

#define HERE fprintf(stderr,"%s %d: ",__FILE__,__LINE__)

// This program sends YAML to $SP_OUTFILE or stdout 

static struct timeval t_start; // Start time from myinit
static struct rusage r_start; // rusage from myinit
static FILE* myout; // Hold our own stdout pointer
static char myargv0[256]; // a place to hold a copy of argv[0]

// These two are shell dependent, but ought to work in ksh with some work
static int bash_line; // a place to hold the bash line number
static char* bash_source; // bash script name

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
  // bash_line=calloc(256,sizeof(char));
  // printf("LN: %s\n",getenv("LN"));
  char *ln=getenv("LN");
  if(ln)
    bash_line=atoi(ln);
  else
    bash_line=-1;
  bash_source=getenv("BS");
  if (!bash_source) bash_source="Not set:";
  

  //  if (bash_line)
  //    strncpy(bash_line,getenv("LN"),strlen(getenv("LN")));

  //  printf("bash line: %s\n",bash_line);

  strncpy(myargv0,argv[0],strlen(argv[0]));
  gettimeofday(&t_start,NULL);
  getrusage(RUSAGE_SELF,&r_start);

  fprintf(myout,"\n");
  //  print_argv(argc,argv);
  //fprintf(myout,"%s: ",argv[0]);
  // if (bash_line) fprintf(myout,"\n----%s----\n",bash_line);
}

static void myfini()
{
  struct timeval te;
  struct rusage r_end;
  gettimeofday(&te,NULL);
  getrusage(RUSAGE_SELF,&r_end);

  double user_elapsed=(r_end.ru_utime.tv_sec) +
    1.e-6*(r_end.ru_utime.tv_usec);
  double sys_elapsed=(r_end.ru_stime.tv_sec) +
    1.e-6*(r_end.ru_stime.tv_usec);

  double elapsed=(double)(te.tv_sec-t_start.tv_sec) +
    1.e-6*(double)(te.tv_usec-t_start.tv_usec);

  pid_t pid=getpid();
  // bash_line commented out above, remove here
//  fprintf(myout,
//	  "%d: \n  line=%d\n  cmd=\"%s\"\n  etime=%15.7g\n  utime=%15.7g\n  stime=%15.7g\n",
//	  pid,atoi(bash_line),myargv0,elapsed,user_elapsed,sys_elapsed);
  // Pre-line number code (save for posterity)
  //fprintf(myout,
  //	  "%d: \n  cmd: \"%-s\"\n  etime: %-15.7g\n  utime: %-15.7g\n  stime: %-15.7g\n",
  // 	  pid,myargv0,elapsed,user_elapsed,sys_elapsed);

  // Now print with line number
  fprintf(myout,
	  "%d: \n  file: \"%-s\"\n  cmd: \"%-s\"\n  etime: %-15.7g\n  utime: %-15.7g\n  stime: %-15.7g\n  line: %-10d\n",
	  pid,bash_source,myargv0,elapsed,user_elapsed,sys_elapsed,bash_line);
}


// Put our functions in the init and fini arrays
static __attribute__((section(".init_array"))) typeof(myinit) *__init = myinit;
static __attribute__((section(".fini_array"))) typeof(myfini) *__fini = myfini;
