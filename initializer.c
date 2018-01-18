#include <stdio.h>

void myinit(int argc, char **argv, char **envp) {
  printf("%s: %s\n", __FILE__, __FUNCTION__);
}

__attribute__((section(".init_array"))) typeof(myinit) *__init = myinit;
