#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define NUM 1024
#define SIZE 32
#define SEP " "

#define NONE_REDIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3

int redir_status = NONE_REDIR;

char cmd_line[NUM];
char* g_argv[SIZE];
char g_val[64];//保存环境变量

char* checkReDir(char* cmd_line){
    char* end = cmd_line + strlen(cmd_line) - 1;
    while(end >= cmd_line){
        if(*end == '>'){
            if(*(end-1) == '>'){
                *(end - 1) = '\0';
                redir_status = APPEND_REDIR;
                end += 1;
                break;
            }
            *end = '\0';
            redir_status = OUTPUT_REDIR;
            end += 1;
            break;
        }else if(*end == '<'){
            *end = '\0';
            redir_status = INPUT_REDIR;
            end += 1;
            break;
        }else{
            end--;
        }
    }
    if(end >= cmd_line){
        return end;
    }else{
        return NULL;
    }
}
int main()
{
    while(1){
        //打印提示信息
        printf("[root@localhost myshell]# ");
        fflush(stdout);
        memset(cmd_line, '\0', sizeof cmd_line);
        //读取命令行字符串
        if(fgets(cmd_line, sizeof cmd_line, stdin) == NULL){
            continue;
        }
        //解析命令行字符串
        cmd_line[strlen(cmd_line)-1] = '\0';

        //分析命令行是否存在重定向
        char* file = checkReDir(cmd_line);

        g_argv[0] = strtok(cmd_line, SEP);
        int index = 1;

        //让ls命令带配色
        if(strcmp(g_argv[0], "ls") == 0){
            g_argv[index++] = (char*)"--color=auto";
        }
        //分割命令字符串
        while(g_argv[index++] = strtok(NULL, SEP));
        if(strcmp(g_argv[0],"export") == 0 && g_argv[1] != NULL){
            strcpy(g_val,g_argv[1]);
            putenv(g_val);
            continue;
        } 
        //int i = 0;
        //for(i = 0; g_argv[i]; i++){
        //    printf("%d->%s\n",i, g_argv[i]);
        //}
        //处理内置命令,chdir为shell内部自己实现的方法，由自己（父进程）来执行
        if(strcmp(g_argv[0], "cd") == 0){
            if(g_argv[1] != NULL){
                chdir(g_argv[1]);
            }
            continue;
        }
        //执行命令
        pid_t id = fork();
        if(id == 0){
            //如果需要重定向
            int fd = -1;
            if(file != NULL){
                switch(redir_status){
                    case INPUT_REDIR:
                        fd = open(file, O_RDONLY);
                        if(fd == -1){
                            perror("open file:");
                        }
                        dup2(fd, 0);
                        break;
                    case OUTPUT_REDIR:
                        fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0666);
                        if(fd == -1){
                            perror("open file:");
                        }
                        dup2(fd, 1);
                        break;
                    case APPEND_REDIR:
                        fd = open(file, O_WRONLY | O_APPEND | O_CREAT, 0666);
                        if(fd == -1){
                            perror("open file:");
                        }
                        dup2(fd, 1);
                        break;
                    default:
                        break;
                }        
            }
            //printf("以下功能为子进程执行\n");
            execvp(g_argv[0],g_argv);
            exit(1);
        }
        int status = 0;
        //等待子进程完成并返回其pid
        pid_t ret = waitpid(id, &status, 0);
        if(ret > 0){
            printf("exit code: %d\n",WEXITSTATUS(status));
        }
    }
    return 0;
}
