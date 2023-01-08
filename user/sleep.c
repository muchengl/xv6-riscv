#include "kernel/types.h"                                                             
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    //fprintf(1,"%d %s",argc,argv[0]);

    if(argc!=2){
        write(1,"error",5);
        exit(0);
    }

    // with a right argument
    // write(1,"sleep",5);

    int time=atoi(argv[1])*10;
    sleep(time);

    //fprintf(1,"fail");
    exit(0);
}

