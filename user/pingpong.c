#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){


    int fd[2];
    char buf[64];

    // build a pipe
    pipe(fd);


    int pid = fork();
    if(pid==0){

        //child
        int id=getpid();


        read(fd[0], buf, sizeof(buf));

        fprintf(1,"%d: received ping\n",id);

        write(fd[1],"1",1);

        exit(0);


    }
    else{

        // parent
        // send a byte first
        write(fd[1],"1",1);

        //waiting for child
        wait(&pid);

        read(fd[0], buf, sizeof(buf));

        int id=getpid();
        fprintf(1,"%d: received pong\n",id);


    }


    exit(0);
}