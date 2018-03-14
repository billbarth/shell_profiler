#define _GNU_SOURCE
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <time.h>

// Need a message macro. Do not print to stdout or risk breaking things that rely on stdout of other programs in pipes. Either open own file for errors or write to stderr

#define HERE fprintf(stderr,"%s %d: ",__FILE__,__LINE__)

// This program sends YAML to $SP_OUTFILE or stdout 

static struct timeval t_start; // Start time from myinit
static struct rusage r_start; // rusage from myinit
static FILE* myout; // Hold our own stdout pointer
static char myargv0[256]; // a place to hold a copy of argv[0]

static char **myargv; // place to hold a pointer to the top of argv
			// (will this even work?)
static int myargc;

// These two are shell dependent, but ought to work in ksh with some work
static int bash_line; // a place to hold the bash line number
static char* bash_source; // bash script name
 
static int shlvl; //bash shell level to detect nesting, maybe won't work

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

  bash_line=-100;

  char *shlvl_str=getenv("SHLVL");
  if(shlvl_str) shlvl=atoi(shlvl_str);

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
    bash_line=-17;
  //  printf("bash_line %d\n",bash_line);
  bash_source=getenv("BS");
  if (!bash_source) bash_source="Not set:";
  

  //  if (bash_line)
  //    strncpy(bash_line,getenv("LN"),strlen(getenv("LN")));

  //  printf("bash line: %s\n",bash_line);

  strncpy(myargv0,argv[0],strlen(argv[0]));
  
  myargv=malloc(argc*sizeof(char*));
  for (int iarg=0; iarg<argc; ++iarg)
    myargv[iarg]=argv[iarg];
  
  myargc=argc;
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

  char start_time_str[256]="";
  
  time_t nowtime = t_start.tv_sec;
  struct tm *nowtm;
  nowtm = localtime(&nowtime);

  strftime(&start_time_str, 256, "%Y-%m-%d %H:%M:%S", nowtm);

  pid_t pid=getpid();

//  Save this print for later since it works. Now implement something
//  that prints the wole command line instead of just argv[0]
//  fprintf(myout,
//  	  "%d: \n  file: \"%-s\"\n  cmd: \"%-s\"\n  etime: %-15.7g\n  utime: %-15.7g\n  stime: %-15.7g\n  line: %-10d\n",
//  	  pid,bash_source,myargv0,elapsed,user_elapsed,sys_elapsed,bash_line);

  fprintf(myout,"%d: \n  file: \"%-s\"\n",pid,bash_source); // first part of YAML record  
  // print the whole command line to a single-line YAML record. Does this work with quotes?
  fprintf(myout,"  cmdline: \"");
  for(int iarg=0; iarg<myargc; iarg++)
    {
      fprintf(myout,"%s ",myargv[iarg]);
    }
  fprintf(myout,"\"\n");
  

  fprintf(myout,"  etime: %-15.7g\n  utime: %-15.7g\n  stime: %-15.7g\n  line: %-10d\n  shlvl: %-10d\n  start-time: \"%s\"\n",
	  elapsed,user_elapsed,sys_elapsed,bash_line,shlvl,start_time_str);


}


// No longer trying to replace ksh with bash, so commenting this out!

// typedef int(*orig_execve_f_type)(const char* filename, char *const argv[], char* const envp[]);
// 
// int execve(const char* filename, char *const argv[], char* const envp[])
// {
//   printf("Wrapping execve to call: %s\n",filename);
//   orig_execve_f_type orig_execve;
//   orig_execve = (orig_execve_f_type)dlsym(RTLD_NEXT,"execve");
// 
//   char myfn[10];
// 
//   if (strstr(filename,"/bin/ksh"))
//     {
//       strncpy(myfn,"/bin/bash",9);
//       printf("Found ksh, switching to %s\n",myfn);
//     }
//   else if(strstr(filename,"/bin/sh"))
//     {
//       strncpy(myfn,"/bin/bash",9);
//       printf("Found sh, switching to %s\n",myfn);
//     }
//   else
//     {
//       strcpy(myfn,filename);
//     }
// 
//   
//   
//   return orig_execve(myfn,argv,envp);
// }


// Put our functions in the init and fini arrays
static __attribute__((section(".init_array"))) __typeof__(myinit) *__init = myinit;
static __attribute__((section(".fini_array"))) __typeof__(myfini) *__fini = myfini;
