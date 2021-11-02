#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

double mult(double a, double b) {
    return a*b;
}

int main() {
    int msqid; /* IPC дескриптор для очереди сообщений */

    char pathname[] = "server.c"; /* Имя файла, использующееся для генерации ключа.
    Файл с таким именем должен существовать в текущей директории */

    key_t key; /* IPC ключ */

    int i,len; /* Cчетчик цикла и длина информативной части сообщения */

    /* Ниже следует пользовательская структура для сообщения */

    struct Info {
        int id;
        double a;
        double b;
        double c;
    }info;

    struct mymsgbuf
        {
        long mtype;
        struct Info info;
        }mybuf;


    /* Генерируем IPC ключ из имени файла server.c в текущей директории
    и номера экземпляра очереди сообщений 0. */

    if((key = ftok(pathname,0)) < 0){
    printf("Can\'t generate key\n");
    exit(-1);
    }

    /* Пытаемся получить доступ по ключу к очереди сообщений, если она существует,
    или создать ее, если она еще не существует, с правами доступа
    read & write для всех пользователей */

    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t get msqid\n");
    exit(-1);
    }
    printf("!");
    printf ("%d", msqid);

    i = 1;
    while( i !=0){
        i =0;
    printf("!!");
    

        
        if((len = msgrcv(msqid, (struct mymsgbuf *) &mybuf, 200, 1, 0)) < 0){
            printf("Can\'t receive message from queue\n");
            //printf ("%d", errno);
            if (errno == E2BIG) printf("1");
            if (errno == EACCES) printf("2");
            if (errno == EFAULT) printf("3");
            if (errno == EIDRM) printf("4");
            if (errno == EINTR) printf("5");
            if (errno == EINVAL) printf("6");
            if (errno == ENOMSG) printf("7");
            exit(-1);
        }
        mybuf.mtype = mybuf.info.id;
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

        int result;
        if ((result = fork()) == 0) {
            mybuf.info.c = mult(mybuf.info.a, mybuf.info.b);
            if (msgsnd(msqid, (struct msgbuf *) &mybuf, 200, 0) < 0){
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }
        }
    }
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    return 0;
} 
