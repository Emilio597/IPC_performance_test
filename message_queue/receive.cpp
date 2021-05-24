#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<sys/time.h>
#define num 6
struct my_msg_16
{
    long int my_msg_type;
    char some_text[16];
};
struct my_msg_64
{
    long int my_msg_type;
    char some_text[64];
};
struct my_msg_1024
{
    long int my_msg_type;
    char some_text[1024];
};
struct my_msg_4096
{
    long int my_msg_type;
    char some_text[4096];
};
int main()
{
    int msgid;
    my_msg_16 data16;
    my_msg_64 data64;
    my_msg_1024 data1024;
    my_msg_4096 data4096;
    void *p[num] = {&data16,&data16, &data64, &data1024, &data4096,&data4096};
    int size[num] = {16,16, 64, 1024, 4096,4096};
    //long int msg_to_receive[4] = {1, 2, 3, 4}; //接收第一个消息
    struct timeval tp[num];
    struct timeval *tt[num];
    //创建消息队列：
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    double time1, time2;
    //接收消息队列中的消息直到遇到一个end消息。最后，消息队列被删除。
    for (int i = 0; i < num; i++)
    {
        //第5个参数为0表示阻塞方式，当消息队列为空，一直等待
        if (msgrcv(msgid, p[i], size[i], 0, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed with errno: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        gettimeofday( &tp[i], NULL );
        printf("receive time %llus, %lluus\n", tp[i].tv_sec, tp[i].tv_usec);
        tt[i]=(struct timeval *)(p[i]+8);
        printf("send time %llus, %lluus\n", tt[i]->tv_sec, tt[i]->tv_usec);
        time1 = tp[i].tv_sec + (tp[i].tv_usec / 1000000.0);
        time2 = tt[i]->tv_sec + (tt[i]->tv_usec / 1000000.0);
        time1 = time1 - time2;
        printf("sub time %lf s\n", time1);
        printf("\n");
    }

    if (msgctl(msgid, IPC_RMID, 0) == -1) // 删除消息队列
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}