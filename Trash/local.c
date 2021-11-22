#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>


void Fidn_object (char *dirr, char *name, int depth, int *files_count) {

	//Магические слова
    struct stat tmp;
    struct dirent *dirrect;

	//Открываем директорию и ругаемся, если не получается
    DIR *direct = opendir(dirr);
    if (direct == NULL) {
		printf("Can't open dirrectory\n");
		exit(-1);                         
	}
    // Выделяем под маршрут памяти абы как, потом, если надо, добавим (+ если сразу вызвать коллоком достаточно много, оно ложится, поэтому это и своего рода костыль)
    char *way = (char *)calloc(1, sizeof(char));
	//char *way = (char *)calloc(sizeof(char), strlen(dirr) + strlen(dirrect->d_name) + 20);
    while ((dirrect = readdir(direct)) != NULL) { //Перебираем элменты, пока они не кончатся
        way = realloc( way, sizeof(char) * (strlen(dirr) + strlen(dirrect->d_name))); //как и обещал, даем то, что не дали коллоком




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Составляем карту маршрута
        strcpy(way, dirr);                           //Дабавляем начальную точку
        strcat(way, "/");                           //добавляем /
        strcat(way, dirrect->d_name);              //добавляем то, что мы, собственно ищем
        stat(way, &tmp);                            //для получения информации об объекте

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (S_ISDIR(tmp.st_mode) && depth > 0) {            //если это не тип файла, идем дальше, проверяя на .. и . чтобы не попасть в цикл
            if ((strcmp(dirrect->d_name, "..") != 0) && (strcmp(dirrect->d_name, ".") != 0)) { 
                Fidn_object(way, name, depth - 1, files_count);
            } 
        }

        else if (S_ISREG(tmp.st_mode) && strcmp(name, dirrect->d_name) == 0) { //если файл нашелся, выводим его путь и добавляем 1 к счетчику
            printf("The file was found along the path: %s\n", way);
            (*files_count) ++;
        }

    }
    free(way);
    closedir(direct);
}

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Это не название файла\n");
        return 0;
    }
    int depth, count = 0;
    printf("Введите глубину поиска (количество проверяемых директорий, начиная с текущей): ");
    scanf("%d", &depth);
	if (depth < 1) {
		printf("В следующий раз введи нормальное число\n");
		exit(-1);
	}
	
    char cur_dir[1] = "."; //иначе не сможет открыть директорию
    Fidn_object(cur_dir, argv[1], depth, &count);
    if (count > 0) {
        printf("Нашел %d файлов с названием %s\n", count, argv[1]);
    }
    else 
        printf("Нема\n");
    return 0;
}