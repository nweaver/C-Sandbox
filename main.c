#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "confuzzle.h"

/* This routine checks to make sure we are running on a Linux system
such as in a container.  As a requriemnet for the class we are runnig on
Linux evironments so if you are using Windows or Mac you should be running in
a container */
void check_unix(){
   FILE *f = popen("/usr/bin/uname", "r");
   char data[512];
   fgets(data,512,f);
   pclose(f);
   if(strcmp(data,"Linux\n")){
    printf("This program must be run in a Linux environment\n");
    exit(0);

   }
}

int main(int argc, char **argv) {
  printf("Hello World!!!\n");
  check_unix();
  if (argc < 2) {
    printf("Specify student ID as a command line argument\n");
    exit(0);
  }
  printf("Confuzzle executing, return value %i\n", confuzzle(argv[1]));
  return 0;
}
