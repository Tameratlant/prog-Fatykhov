#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
//Описание первой нити
void* mythread1(void* name2) {
    pthread_t mythid; /* Для идентификатора нити исполнения */
    mythid = pthread_self();
    int fd;
    char massage[100];
    if ((fd = open(name2, O_WRONLY)) < 0) {
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }
    while (1) {
        fgets(massage, 100, stdin);

        write(fd, massage, 100);                                        //надеюсь, написали в FIFO
    }
    close(fd);
    printf("Thread %ld \n", mythid);
    return NULL;
}
//Описание 2 нити
void* mythread2(void* name1) {
    pthread_t mythid; /* Для идентификатора нити исполнения */
    mythid = pthread_self();
    int fd;
    char massage[100];
    if ((fd = open(name1, O_RDONLY)) < 0) {

        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }
    while (1) {
        read(fd, massage, 100);
        printf("%s\n", massage);
    }
    close(fd);
    printf("Thread %ld \n", mythid);
    return NULL;
}

//Функция, запускающая нити
void fifo(char* name1, char* name2) {
    int result1, result2;
    //printf("my");
    //Запуск первой нити
    pthread_t thid, mythid;
    result1 = pthread_create(&thid, (pthread_attr_t*)NULL, mythread1(name1), NULL);

    if (result1 != 0) {
        printf("Error on thread create, return value = %d\n", result1);
        exit(-1);
    }
    //Запуск второй нити
    result2 = pthread_create(&thid, (pthread_attr_t*)NULL, mythread2(name2), NULL);

    if (result2 != 0) {
        printf("Error on thread create, return value = %d\n", result2);
        exit(-1);
    }





}


int main(int argc, char** argv) {
    //int main() {
    char name1[] = "fifo1.fifo";
    char name2[] = "fifo2.fifo";

    (void)umask(0);
    if (mknod(name1, S_IFIFO | 0666, 0) < 0) {
        printf("Can\'t create FIFO 1\n");
        exit(-1);
    }
    if (mknod(name2, S_IFIFO | 0666, 0) < 0) {
        printf("Can't\'t create FIFO 2\n");
        exit(-1);
    }




    if (strcmp(argv[1], "1") == 0) {
        //printf("1");
        fifo(name1, name2);
    }



    if (strcmp(argv[1], "1") != 0) {
        //printf("2");
        fifo(name2, name1);
    }




    return 0;
}
