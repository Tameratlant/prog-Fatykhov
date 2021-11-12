

/* Программа 1 для иллюстрации работы с очередями сообщений */

 

/* Эта программа получает доступ к очереди сообщений,
отправляет в нее 5 текстовых сообщений с типом 1
и одно пустое сообщение с типом 255, которое будет служить
для программы 2 сигналом прекращения работы. */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define LAST_MESSAGE 255 /* Тип сообщения для прекращения работы программы 2 */

int main()
    {
    int msqid; /* IPC дескриптор для очереди сообщений */

    char pathname[] = "Server.c"; /* Имя файла, использующееся для генерации ключа.
    Файл с таким именем должен существовать в текущей директории */

    key_t key; /* IPC ключ */

    int i,len; /* Cчетчик цикла и длина информативной части сообщения */

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

    /* Посылаем в цикле 5 сообщений с типом 1 в очередь сообщений, идентифицируемую msqid.*/

    

    /* Сначала заполняем структуру для нашего сообщения и определяем длину информативной части */
    printf ("Enter the numbers\n");
    int ref = scanf("%lf%lf", &mybuf.info.a, &mybuf.info.b);
    assert(ref == 2);

    mybuf.mtype = 1;
    mybuf.info.id = getpid();
    len = sizeof(mybuf) + 1;

    /* Отсылаем сообщение. В случае ошибки сообщаем об этом и удаляем очередь сообщений из системы. */

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
        }
    

    /* Отсылаем сообщение, которое заставит получающий процесс прекратить работу, с типом LAST_MESSAGE и длиной 0 */

    

    if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, len, getpid(), 0)) < 0){
    printf("Can\'t receive message from queue\n");
    exit(-1);
    }

    printf("%lf\n", mybuf.info.c);

    

    return 0;
} 