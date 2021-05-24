#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<sys/time.h>
#define num 6
struct my_msg_16
{
    long int my_msg_type;
    char some_text[16];
    my_msg_16()
    {
        my_msg_type = 1;
        memset(some_text, '1', 16);
    }
};
struct my_msg_64
{
    long int my_msg_type;
    char some_text[64];
    my_msg_64()
    {
        my_msg_type = 2;
        memset(some_text, '2', 64);
    }
};
struct my_msg_1024
{
    long int my_msg_type;
    char some_text[1024];
    my_msg_1024()
    {
        my_msg_type = 3;
        memset(some_text, '3', 1024);
    }
};
struct my_msg_4096
{
    long int my_msg_type;
    char some_text[4096];
    my_msg_4096()
    {
        my_msg_type = 4;
        memset(some_text, '4', 4096);
    }
};
int main()
{
    my_msg_16 data16;
    my_msg_64 data64;
    my_msg_1024 data1024;
    my_msg_4096 data4096;
    // struct timeval *tp;
    void *p[num] = {&data16,&data16, &data64, &data1024, &data4096,&data4096};
    int size[num] = {16,16, 64, 1024, 4096,4096};
    int msgid;
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT); //用一个整数作为一个键值
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with errno: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    // printf("time size:%d",sizeof(struct timeval));
    // printf("long size:%d",sizeof(data4.my_msg_type));
    
    // tp=(struct timeval *)(p[2]+8);
    // printf("time %llus, %lluus\n", tp->tv_sec,tp->tv_usec);
    char tmp[10];
    for (int i = 0; i < num; i++)
    {   
        scanf("%s",tmp); 
        gettimeofday( (struct timeval *)(p[i]+8), NULL );
        if (msgsnd(msgid, p[i], size[i], 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}