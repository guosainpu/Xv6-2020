#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: sleep number\n");
    exit(1);
  }

  int n = atoi(argv[1]);

  if(n<=0){
    fprintf(2, "number should >0\n");
    exit(1);
  } else {
    sleep(n);
    exit(0);
  }
}
