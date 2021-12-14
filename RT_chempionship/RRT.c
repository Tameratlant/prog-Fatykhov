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


int result = -1;

void handler(int flag) {                                
    first_part = "РТ";
    second_part = " - ЧЕМПИОН\n";
    if (flag == SIGUSR1) {                              
        kill(result, SIGUSR2);
        write(1, first_part, strlen(first_part));
        
    }
    else {
        kill(getppid(), SIGUSR1);
        write(1, second_part, strlen(second_part));
       
    }
}

int main() {
int i=10;
(void)signal(SIGUSR2, handler);
(void)signal(SIGUSR1, handler);
while(i > 0){
int result = fork();
if(result < 0){
printf("Can't have a child\n");
exit(-1);
}
if (result > 0){
kill(getppid(), SIGUSR1);
}

if(result == 0){
kill(getpid(), SIGUSR2);
}
i--;
}
return 0;
}
