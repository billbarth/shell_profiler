#include <stdio.h>

void __attribute__ ((constructor)) myinit2(void){
  printf("%s: %s\n", __FILE__, __FUNCTION__);
}

