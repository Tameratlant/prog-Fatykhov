#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
int main() {

    int fd, result;
    size_t size;
    char string[100];
    char massage[100];
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
    
    
    ///////////////////////////////////////////////
    if ((result = fork()) > 0) {                                              //сидим в пишущем в консоль процессе
        if ((fd = open(name1, O_WRONLY)) < 0) {
            printf("Can\'t open FIFO for writting\n");
            exit(-1);
        }
        fgets(massage, 100, stdin);

        write(fd, massage, 100);                                        //надеюсь, написали в FIFO
        close(fd);
    }
    ///////////////////////////////////////////////

    if (result == 0) {                                                       //сидим в читающем из FIFO процессе
        if ((fd = open(name2, O_RDONLY)) < 0) {

            printf("Can\'t open FIFO for writting\n");
            exit(-1);
        }
        read(fd, massage, 100);

        printf("%s\n", massage);
        close(fd);
    }
    return 0;
}
