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

#define Table 3


struct Dish {
        int count;
        int wash_time;
        int wipe_time;
}; 

struct current_dish {
    int type;
    int wipe_time;
};

    struct mymsgbuf
        {
        long mtype;
        struct current_dish current_dish;
        }mybuf;
//Почему - то обычный встренный max не работал 
int maxx (int a, int b) {
    if (a > b) return a;
    if (a <= b) return b;
}



int main() {
    
    int type;
    int time;
    int count;
    FILE* fp = NULL;

    fp = fopen("count.txt", "r");
    int max_type;
    while (fscanf(fp, "%d:%d\n", &type, &count) == 2) {
        max_type = maxx(max_type, type);
	}
	fclose(fp);
    //printf("!!\n");
    struct Dish* dish = (struct Dish*)calloc(max_type + 2, sizeof(struct Dish));
    //Прописываем "Стоп - слово"
    dish[max_type + 1].count=-1;
    //printf("!!\n");


    //printf("45 %d\n", dish[0].wash_time);
    dish[0].wash_time = 1;
    //printf("47 %d\n", dish[0].wash_time);
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

    printf ("%d %d %d %d\n", 0, dish[0].count, dish[0].wash_time, dish[0].wipe_time);
    printf ("%d %d %d %d\n", 1, dish[1].count, dish[1].wash_time, dish[1].wipe_time);
    printf ("%d %d %d %d\n", 2, dish[2].count, dish[2].wash_time, dish[2].wipe_time);
    printf ("%d %d %d %d\n", 3, dish[3].count, dish[3].wash_time, dish[3].wipe_time);

    return 0; 
}
