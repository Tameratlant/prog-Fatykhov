#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void wash(struct Dish dish, int N){
    int i,j = 0;
    double sleep = dish[i].wash_time;
    j = dish[i].count 
} 

int main() {
    struct Dish{
        int type;
        int count;
        double wash_time;
        double wipe_time;
    };

    struct Dish dish[1000];
    
    int i = 0;
    printf("Enter Type - Count info\n");
    while(1) {
        if (dish[i-1].type == -1) break;
        scanf("%d%d", &dish[i].type, &dish[i].count);
        i++;
    }
    printf("Enter Type - Time info\n");
    i = 0;
    int k;
    while(1) {
        if (dish[i-1].wash_time == (double)-1) break;
        scanf ("%d", &k);
        for (int j = 0; j< 1000; j++){
            if (dish[i].type == k){
                scanf("%lf%lf", k&dish[j].wash_time, &dish[j].wipe_time);
            }
        }
        scanf("%lf%lf", k&dish[i].wash_time, &dish[i].wipe_time);
        i++;
    }

    printf("Enter N\n");
    int N;
    scanf("%d", &N);
    
    //printf("%d",dish[0].type);

    //struct sembuf buf[2] = {0, 1, 0}, {1, -1, 0};

    struct sembuf bufop = {1, 1, 0};
    struct sembuf bufdn = {0, -1, 0};

    int semid; /* IPC дескриптор для массива IPC семафоров */
    char pathname[] = "dish.c"; /* Имя файла, использующееся для генерации ключа. Файл с таким именем должен существовать в текущей директории */

    key_t key; /* IPC ключ */

    /* Генерируем IPC ключ из имени файла 08-1a.c в текущей директории
    и номера экземпляра области разделяемой памяти 0 */

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    /* Пытаемся получить доступ по ключу к массиву семафоров, если он существует,
    или создать его из одного семафора, если он еще не существует, с правами доступа
    read & write для всех пользователей */

    if((semid = semget(key, 2, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }

    if(semop(semid, &bufop, 1) < 0){
        printf("Can\'t wait for condition\n");
        exit(-1);
    }

    printf("Condition is present\n");
    return 0;
}
