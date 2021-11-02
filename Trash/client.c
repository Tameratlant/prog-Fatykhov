#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

struct Info {
    int id;
    double a;
    double b;
    double c;
};

struct mymsgbuf
    {
    long mtype;
    struct Info info;
    }mybuf;



int main() {
    int msqid; /* IPC дескриптор для очереди сообщений */

    char pathname[] = "server.c"; /* Имя файла, использующееся для генерации ключа.
    Файл с таким именем должен существовать в текущей директории */
    //struct Info info;
    key_t key; /* IPC ключ */
    int len;
    //int size = size_of(Info);

    /* Ниже следует пользовательская структура для сообщения */
    
    
    

    /* Генерируем IPC ключ из имени файла 09-1a.c в текущей директории
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

    int result;
    result = fork();
    /* Часть, принимающая числа с консоли и отправлющая на сервер */
    if (result == 0) {
        printf ("Enter the numbers\n");
        int ref = scanf("%lf%lf", &mybuf.info.a,&mybuf.info.b);
        assert(ref == 2);
        mybuf.mtype = 1;
        mybuf.info.id = getppid();
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, 200, 0) < 0){
            printf("Can\'t send message to queue\n");
            printf ("%d", errno);
            if (errno == EINVAL) printf("EAGAIN");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }

    }
    /* Часть, принимающая ответ с сервера */
    if (result > 0) {
        if((len = msgrcv(msqid, (struct msgbuf *) &mybuf, 200, getpid(), 0)) < 0){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        printf("%lf\n", mybuf.info.c);
    }

    return 0;
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
} 
