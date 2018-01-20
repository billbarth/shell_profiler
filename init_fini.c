#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

static struct timeval t_start; // Start time from myinit
static struct rusage r_start; // rusage from myinit
static FILE* mystdout; // Hold our own stdout pointer
static char myargv0[256];


//Loop over argv and print the whole command line
void print_argv(int argc, char **argv)
{
  for(int i=0; i<argc; ++i)
    fprintf(mystdout,"%s ",argv[i]);

  fprintf(mystdout,": ");
}

void myinit(int argc, char **argv, char **envp) {
  // dup stdout to global before anyone else, like the executable
  // itself has a chance to close(1)
  int outfd = dup(STDOUT_FILENO);
  if (outfd < 0)
    {
      perror("bad file desc");
      abort();
    }
  mystdout=fdopen(outfd,"w");
  if (!mystdout)
    {
      perror("bad file handle: ");
      abort();
    }
  //  printf("%s: %s\n", __FILE__, __FUNCTION__);
  char *bash_line=calloc(256,sizeof(char));
  strncpy(myargv0,argv[0],strlen(argv[0]));
  bash_line=getenv("LINENO");
  gettimeofday(&t_start,NULL);
  getrusage(RUSAGE_SELF,&r_start);
  //  print_argv(argc,argv);
  //fprintf(mystdout,"%s: ",argv[0]);
  if (bash_line) fprintf(mystdout,"\n----%s----\n",bash_line);
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
  //  fprintf(mystdout,"%s: %s\nelapsed time: %15.7g",
  //	  __FILE__, __FUNCTION__,elapsed);
  fprintf(mystdout,"+ %15.7g %15.7g %15.7g: %s\n",
	  elapsed,user_elapsed,sys_elapsed,myargv0);
}


// Put our functions in the init and fini arrays
static __attribute__((section(".init_array"))) typeof(myinit) *__init = myinit;
static __attribute__((section(".fini_array"))) typeof(myfini) *__fini = myfini;
