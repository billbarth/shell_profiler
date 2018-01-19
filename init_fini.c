#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

static struct timeval ts; // Start time from myinit
static FILE* mystdout; // Hold our own stdout pointer

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
  bash_line=getenv("LINENO");
  gettimeofday(&ts,NULL);
  print_argv(argc,argv);
  if (bash_line) fprintf(mystdout,"----%s----\n",bash_line);
}

static void myfini(int argc, char **argv, char **envp) {
  
  struct timeval te;
  gettimeofday(&te,NULL);
  double elapsed=(double)(te.tv_sec-ts.tv_sec) +
    1.e-6*(double)(te.tv_usec-ts.tv_usec);
  //  fprintf(mystdout,"%s: %s\nelapsed time: %15.7g",
  //	  __FILE__, __FUNCTION__,elapsed);
  fprintf(mystdout,"elapsed time: %15.7g\n",elapsed);
}


// Put our functions in the init and fini arrays
__attribute__((section(".init_array"))) typeof(myinit) *__init = myinit;
__attribute__((section(".fini_array"))) typeof(myfini) *__fini = myfini;
