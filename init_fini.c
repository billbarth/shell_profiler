#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

static struct timeval t_start; // Start time from myinit
static struct rusage r_start; // rusage from myinit
static FILE* myout; // Hold our own stdout pointer
static char myargv0[256]; // a place to hold a copy of argv[0]


//Loop over argv and print the whole command line
void print_argv(int argc, char **argv)
{
  for(int i=0; i<argc; ++i)
    fprintf(myout,"%s ",argv[i]);

  fprintf(myout,": ");
}

void myinit(int argc, char **argv, char **envp) {
  // Check for SP_OUTFILE and either open it or dup stdout
  char *sp_outfile=getenv("SP_OUTFILE");
  if(sp_outfile)
    {
      myout=fopen(sp_outfile,"w+");
      if(!myout)
	{
	  perror("Failed to open SP_OUTFILE");
	  abort();
	}
    }
  else
    {
      // dup stdout to global before anyone else, like the executable
      // itself has a chance to close(1)
      int outfd = dup(STDOUT_FILENO);
      if (outfd < 0)
	{
	  perror("bad file desc");
	  abort();
	}
      myout=fdopen(outfd,"w");
      if (!myout)
	{
	  perror("bad file handle: ");
	  abort();
	}
    }
  //  printf("%s: %s\n", __FILE__, __FUNCTION__);
  char *bash_line=calloc(256,sizeof(char));
  strncpy(myargv0,argv[0],strlen(argv[0]));
  bash_line=getenv("LINENO");
  gettimeofday(&t_start,NULL);
  getrusage(RUSAGE_SELF,&r_start);
  //  print_argv(argc,argv);
  //fprintf(myout,"%s: ",argv[0]);
  if (bash_line) fprintf(myout,"\n----%s----\n",bash_line);
}

static void myfini(int argc, char **argv, char **envp) {
  
  struct timeval te;
  struct rusage r_end;
  gettimeofday(&te,NULL);
  getrusage(RUSAGE_SELF,&r_end);
  double user_elapsed=(r_end.ru_utime.tv_sec - r_start.ru_utime.tv_sec) +
    1.e-6*(r_end.ru_utime.tv_usec - r_start.ru_utime.tv_usec);
  double sys_elapsed=(r_end.ru_stime.tv_sec - r_start.ru_stime.tv_sec) +
    1.e-6*(r_end.ru_stime.tv_usec - r_start.ru_stime.tv_usec);

  double elapsed=(double)(te.tv_sec-t_start.tv_sec) +
    1.e-6*(double)(te.tv_usec-t_start.tv_usec);
  //  fprintf(myout,"%s: %s\nelapsed time: %15.7g",
  //	  __FILE__, __FUNCTION__,elapsed);
  fprintf(myout,"+ %15.7g %15.7g %15.7g: %s\n",
	  elapsed,user_elapsed,sys_elapsed,myargv0);
}


// Put our functions in the init and fini arrays
static __attribute__((section(".init_array"))) typeof(myinit) *__init = myinit;
static __attribute__((section(".fini_array"))) typeof(myfini) *__fini = myfini;
