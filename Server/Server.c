#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

#define LIMIT 1
int semid; /* IPC дескриптор для массива IPC семафоров */




void mysemop(int a) {
    struct sembuf mybuf;
	mybuf.sem_op = a;
	mybuf.sem_flg = 0;
    mybuf.sem_num = 0;
	if (semop(semid , &mybuf , 1) < 0) {
		printf("Can\'t wait for condition\n");
        exit(-1);
	}
}



int main()
    {


    

    int msqid; /* IPC дескриптор для очереди сообщений */

    char pathname[] = "Server.c"; /* Имя файла, использующееся для генерации ключа.
    Файл с таким именем должен существовать в текущей директории */

    key_t key; /* IPC ключ */

    if((semid = semget(key, 2, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }

    int len, maxlen; /* Реальная длина и максимальная длина информативной части сообщения */

    /* Ниже следует пользовательская структура для сообщения */

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

    /* Генерируем IPC ключ из имени файла Server.c в текущей директории
    и номера экземпляра очереди сообщений 0 */

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
    //maxlen = sizeof(struct Info) + 1;
    maxlen = 300;
    mysemop(LIMIT );
    while(1){
        
        // В бесконечном цикле принимаем сообщения 

        if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0){
                printf("88 Can\'t receive message from queue\n");
                exit(-1);
            }
        //Отправлем это же сообщение в процесс обработчик(такие сообщения будут иметь тип 2)
        mybuf.mtype = 2;
        len = sizeof(struct Info);
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
            }
        mysemop(-1);
        int result = fork();
        sleep(5);
        if (result > 0) {

            if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 2, 0)) < 0){
                printf("103 Can\'t receive message from queue\n");
                exit(-1);
            }

        //И вот уже тут отсылаем сообщение обратно клиенту
        mybuf.mtype = mybuf.info.id;
        
        mybuf.info.c = mybuf.info.a * mybuf.info.b;
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
            }
        }
        mysemop(1);
    }

    return 0; /* Исключительно для отсутствия warning'ов при компиляции. */
} 
