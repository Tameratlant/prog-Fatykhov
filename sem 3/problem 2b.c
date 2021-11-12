#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
 
int main(){
    int n = 0;
    printf("Число процессов:");
    scanf("%d", &n);

    for(int i = 0; i < n; i++){
        if(fork() != 0){                                                    // Проверка на срабатывание fork
            printf("son № %d from parent № %d\n", getpid(), getppid());     // n процессов по очереди создают себе сыновей и вывоят инфу о сыновьях и отцах
            break;
        }
    }

    for(int i = 0; i < n; i++) {
        wait(NULL);                                                         //каждый процесс ждет завершения
    }
return 0;    
}
 
