#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main()
{
    //C接口
    printf("%s","hello printf\n");
    fprintf(stdout, "hello fprintf\n");
    const char *s = "hello fputs\n";
    fputs(s, stdout);

    //OS接口
    const char *ss = "hello write\n";
    write(1, ss, strlen(ss));

    fork();

    return 0;
}
