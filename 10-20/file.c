#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>

struct _FILE{
    int fd;
    char buffer[64];
    int end;
};
typedef struct _FILE _FILE;

_FILE* _fopen(const char* fileName, const char* mode){
    assert(fileName);
    assert(mode);

    _FILE* fp = NULL;
    if(strcmp(mode, "r") == 0){

    }else if(strcmp(mode, "r+") == 0){

    }else if(strcmp(mode, "w") == 0){
        int fd = open(fileName, O_WRONLY | O_TRUNC | O_CREAT, 0666);
        if(fd >= 0){
            fp = (_FILE*)malloc(sizeof(_FILE));
            memset(fp, 0, sizeof(_FILE));
            fp->fd = fd;
        }

    }else if(strcmp(mode, "w+") == 0){

    }else if(strcmp(mode, "a") == 0){

    }else if(strcmp(mode, "a+") == 0){

    }else{
        
    }
    return fp;
}
void _fputs(const char* writeStr, _FILE* fp){
    assert(writeStr);
    assert(fp);
    
    strcpy(fp->buffer, writeStr);
    fp->end += strlen(writeStr);
    //制定缓冲区刷新策略
    if(fp->fd == 0){
        //标准输入无法写入
    }else if(fp->fd == 1){
        //标准输出->行缓冲
        if(fp->buffer[fp->end-1] == '\n'){
            write(fp->fd, fp->buffer, fp->end);
            syncfs(fp->fd);
            fp->end = 0;
        }
    }else if(fp->fd == 2){
        //标准错误
    }else{
        //其它文件->全缓冲
    }
}
void _fflush(_FILE* fp){
    assert(fp);

    write(fp->fd, fp->buffer, fp->end);
    fp->end = 0;
}
void _fclose(_FILE* fp){
    assert(fp);
    _fflush(fp);
    free(fp);
}
int main()
{
    //close(1);
    _FILE* fp = _fopen("./log.txt","w");

    if(fp == NULL){
        printf("open file error\n");
        return 1;
    }

    _fputs("hello world\n",fp);
    //fork();
    _fclose(fp);
    return 0;
}
