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

#define TABLE 3

int maxx (int a, int b) {
    if (a > b) return a;
    if (a <= b) return b;
}

struct Dish {
        int count;
        int wash_time;
        int wipe_time;
}; 
int semid; /* IPC дескриптор для массива IPC семафоров */
int msqid;
struct current_dish {
    int type;
    int wipe_time;
    int stop;
    int next_time;
};

    struct mymsgbuf
        {
        long mtype;
        struct current_dish current_dish;
        }mybuf;

void Fill_data (struct Dish dish[]) { 
    int type;
    int time;
    int count;
    FILE* fp = NULL;
    //printf("43 %d\n", dish[0].wash_time);
    dish[0].wash_time = 1;
    //printf("45 %d\n", dish[0].wash_time);
    fp = (FILE*)fopen("wash.txt", "r");
    while (fscanf(fp, "%d:%d\n", &type, &time) == 2) {
		dish[type].wash_time = time;
	}
    //printf("%d\n", dish[0].wash_time);
	fclose(fp);

    fp = fopen("wipe.txt", "r");
    while (fscanf(fp, "%d:%d\n", &type, &time) == 2) {
		dish[type].wipe_time = time;
	}
	fclose(fp);

    fp = fopen("count.txt", "r");
    while (fscanf(fp, "%d:%d\n", &type, &count) == 2) {
		dish[type].count = count;
	}
	fclose(fp);
}



void mysemop(int a) {
    struct sembuf mybuf;
	mybuf.sem_op = a;
	mybuf.sem_flg = 0;
    mybuf.sem_num = 0;
	if (semop(semid , &mybuf , 1) < 0) {
		printf("Can\'t wait for condition\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        semctl(semid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
	}
}

/////////////////////////////////////////////////////////////
//ВСЕ ВЫШЕ РАБОТАЕТ

void wash(struct Dish dish[], int msqid) {
    //printf("!!\n");
    
    int i = 0;
    int len = sizeof(struct Dish);
    while(1){
        mysemop(-1);
        
        if (dish[i].count == -1) {
            mybuf.current_dish.type = -1;
            mybuf.mtype = 1;
            if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }
            //printf("91\n");
            break;
        }
        



        printf ("Start washing type %d plate\n", i);
        sleep(dish[i].wash_time);
        printf ("End washing type %d plate\n", i);
        mybuf.current_dish.type = i;
        mybuf.current_dish.wipe_time = dish[i].wipe_time;



            
        if (dish[i+1].count == -1){
            mybuf.mtype = 1;
            mybuf.current_dish.stop = 1;
            mybuf.current_dish.next_time = dish[i+1].wipe_time;
            if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }
            //printf("send stop\n");
        }
            
        mybuf.mtype = 1;
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        i++;
        sleep(0.1);
    }
    //mysemop(1);
}


void wipe(int msqid) {
    
    int len = 2*sizeof(struct Dish);
    int status;
    while(1){
        
        if(( status = msgrcv(msqid, (struct msgbuf *) &mybuf, len, 1, 0)) < 0){
            printf("Can\'t receive message from queue\n");
            if (errno == EINVAL) printf("EINVAL");
            exit(-1);
        }
        mysemop(1);
        if (mybuf.current_dish.stop == 1){
            printf("Start to wipe type %d plate\n", mybuf.current_dish.type);
            sleep(mybuf.current_dish.wipe_time);
            printf("End wipe type %d palte\n", mybuf.current_dish.type);
            /*
            printf("Start wipe last palte\n");
            sleep(mybuf.current_dish.next_time);
            printf("End wipe last palte\n");
            */
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            semctl(semid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("Start to wipe type %d plate\n", mybuf.current_dish.type);
        sleep(mybuf.current_dish.wipe_time);
        printf("End wipe type %d palte\n", mybuf.current_dish.type);
        sleep(0.2);
    }
    
}



int main(){
     fflush(stdout);
     //fflush(stdin);

    // Определяем размер будующего массива
    FILE* fp = NULL;
    fp = fopen("count.txt", "r");
    int type = -1, count = -1, max_type=-1;
    while (fscanf(fp, "%d:%d\n", &type, &count) == 2) {
        max_type = maxx(max_type, type);
	}
    
	fclose(fp);
    
    struct Dish* dish = (struct Dish*)calloc(max_type + 2, sizeof(struct Dish));
    //Прописываем "Стоп - слово"
    dish[max_type + 1].count=-1;
    

    Fill_data (dish);
  

   char pathname[] = "Dish.c"; /* Имя файла, использующееся для генерации ключа. Файл с таким именем должен существовать в текущей директории */

    key_t key; /* IPC ключ */

    /* Генерируем IPC ключ из имени файла dish.c в текущей директории
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

    mysemop(TABLE);

    int result;
     if ((result = fork()) < 0){
        printf("Can't do chiled\n");
        exit(-1);
    }

    //Соорудим очередь сообщений для общения между процессами
    
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t get msqid\n");
    exit(-1);
    }

    int pid_1, pid_2;
    
    if (result == 0) {
        //printf("!!\n");
        pid_1 = getpid();
        wash(dish, msqid);
        
        exit(1);
    }

    if (result > 0) {
        pid_2 = getpid();
        wipe(msqid);
    }
    

    

    
    //free(&dish);
    
    return 0; 



}
