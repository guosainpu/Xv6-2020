#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    // 0 is read end, 1 is write end
    int pp[2], cp[2];
    pipe(pp);
    pipe(cp);
    
    int pid = fork();
    if(pid==0) {
        char c;
        
        read(pp[0], &c, 1);
        printf("%d: received ping\n", getpid());
        write(cp[1], &c, 1);
        
        exit(0);
    } else {
        char c = 'c';

        write(pp[1], &c, 1);
        read(cp[0], &c, 1);
        printf("%d: received pong\n", getpid());

        exit(0);    
    }
}
