#include <stdio.h>
#include <unistd.h>
#include <string.h>

static void myfini(int argc, char **argv, char **envp) {
  int outfd = dup(STDOUT_FILENO);
  FILE * out=fdopen(outfd,"w");
  fprintf(out,"%s: %s\n", __FILE__, __FUNCTION__);
}

__attribute__((section(".fini_array"))) typeof(myfini) *__fini = myfini;
