#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void fifo(char* name1, char* name2) {
    //printf("my");
    int fd, result;
    size_t size;
    char string[100];
    char massage[100];
    result = fork();
    ///////////////////////////////////////////////
    if (result > 0) {            //сидим в пишущем в консоль процессе

        //printf("my");

        if ((fd = open(name2, O_WRONLY)) < 0) {
            printf("Can\'t open FIFO for writting\n");
            exit(-1);
        }
        while (1) {
            fgets(massage, 100, stdin);

            write(fd, massage, 100);                                        //надеюсь, написали в FIFO
        }
        close(fd);
    }


    ///////////////////////////////////////////////

    if (result == 0) {                      //сидим в читающем из FIFO процессе
        if ((fd = open(name1, O_RDONLY)) < 0) {

            printf("Can\'t open FIFO for writting\n");
            exit(-1);
        }

        

        while (1) {
            read(fd, massage, 100);
            printf("%s\n", massage);
        }
        close(fd);
    }
}


int main(int argc, char** argv) {
    //int main() {
    char name1[] = "fifo1.fifo";
    char name2[] = "fifo2.fifo";

    printf("Suck\n");
    //printf("%c\n", *argv[1]);

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

