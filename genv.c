#include <stdlib.h>
#include <stdio.h>
void myinit(int argc, char **argv, char **envp) {
  printf("genv: %s\n",getenv("LN")?getenv("LN"):"not set");
  printf("----------\n");
}

int main(int argc, char* argv[])
{


  return 0;
}

static __attribute__((section(".init_array"))) typeof(myinit) *__init = myinit;
