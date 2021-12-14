/*
К сожалению я отсутствовал на занятии по сигналам, поэтому возможно код ниже - надругательство над прогой.
При первом запуске я вообще написал перезагружалку для Linux - после запуска ОС ложилась. Смеха ради ее тоже приложил в этой же папке
*/

#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
char* first_part;
char* second_part;

int result;
void handler(int flag) {                                //Первый if - родителький процесс, второй - ребенок. Порядок важен!
    first_part = "РТ";
    second_part = " - ЧЕМПИОН\n";
    if (flag == SIGUSR1) {                              //именно такой порядок расположения вызовов позволяет избежать race condition.
        kill(result, SIGUSR2);
        write(1, first_part, strlen(first_part));
        
    }
    if (flag == SIGUSR2) { 
        kill(getppid(), SIGUSR1);
        write(1, second_part, strlen(second_part));
       
    }
}

int main() {
    (void)signal(SIGUSR2, handler);
    (void)signal(SIGUSR1, handler);
    
    if((result = fork()) < 0){
        printf("Can't have a child\n");
        exit(-1);
    }

    //int i = 100;
    while(1){
        if (result == 0) {
            kill(getppid(), SIGUSR1);
        }
        //i--;
    }
    return 0;
}
