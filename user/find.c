#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
//#include "string.h"

//char* fmtname(char *path){
//    //static char *buf;
//    char *p;
//
//    // Find first character after last slash.
//    for(p=path+strlen(path); p >= path && *p != '/'; p--);
//    p++;
//
//    // Return blank-padded name.
////    if(strlen(p) >= DIRSIZ)
////        return p;
//
//    //static char buf[strlen(p)];
//    //memmove(buf, p, strlen(p));
//    //memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
//    return p;
//}

char* fmtname(char *path) {
    static char buf[DIRSIZ + 1];
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/';p--);
    p++;
    if (strlen(p) >= DIRSIZ) return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    buf[strlen(p)] = 0;
    return buf;
}


void dfs(char *path,char *name){
    //printf("%s\n",path);
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    //step1: step into target dir(if it exists)
    if((fd = open(path, 0)) < 0){
        //fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    //fd = open(path, 0);


    if(fstat(fd, &st) < 0){
        //fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }


    //fstat(fd, &st);

    switch(st.type) {
        case T_DEVICE:
        case T_FILE:
            //printf("%s is a file\n",path);
            //printf("%s %s\n",fmtname(path),name);

            if(strcmp(fmtname(path),name)==0){
                printf("%s\n",path);
            }
            break;
        case T_DIR:
            //printf("%s is a dir\n",path);
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }

            strcpy(buf, path);
            // p指向buf末端
            p = buf+strlen(buf);
            // buf之后赋值为/
            *p++ = '/';

            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;

                //把名字复制到p所指的位置，也就是buf之后
                memmove(p, de.name, DIRSIZ);
                //添加字符串结束标志位
                p[DIRSIZ] = 0;

                if(!strcmp(de.name,".")==0 && !strcmp(de.name,"..")==0){
                    //printf("%s %d\n",fmtname(buf),strcmp(fmtname(buf),"."));
                    dfs(buf,name);
                }


            }
            break;


    }
    close(fd);
}

int main(int argc, char *argv[]){
    // find target-dir name

    if(argc != 3){
        printf("error");
        exit(0);
    }
    else{
        //printf("%s %s\n",argv[1],argv[2]);
        dfs(argv[1],argv[2]);

    }


    exit(0);
}