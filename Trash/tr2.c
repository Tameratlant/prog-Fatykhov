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
    char *way = (char *)calloc(1, sizeof(char));//выделение памяти под маршрут
	char *way = (char *)calloc(sizeof(char), strlen(dirr) + strlen(dirrect->d_name) + 20);//выделение памяти под маршрут

    while ((dirrect = readdir(direct)) != NULL) { //пока объекты в DIR читаются (то есть пока не прочитаны все объекты)
        way = realloc(way, sizeof(char) * (strlen(dirr) + strlen(dirrect->d_name) + 8)); //расширение памяти под маршрут
        strcpy(way, dirr); //копируем название текущей директории в маршрут
        strcat(way, "/");//добавляем в маршрут слеш
        strcat(way, dirrect->d_name);//добавляем в маршрут текущий читаемый объект
        stat(way, &tmp);//для получения информации об объекте
        if (S_ISDIR(tmp.st_mode) && depth > 0) {            //если директория, рекурсивно продолжаем поиск
            if ((strcmp(dirrect->d_name, "..") != 0) && (strcmp(dirrect->d_name, ".") != 0)) { //ls -a, всегда есть скрытые директории такого типа, их проверку исключаем
                Fidn_object(way, name, depth - 1, files_count);
            } 
        }
        else if (S_ISREG(tmp.st_mode) && strcmp(name, dirrect->d_name) == 0) { //если файл, и имя этого файла совпадает с искомым, печатаем, что нашли этот файл
            printf("Файл найден, путь к файлу: %s\n", way);
            ++(*files_count);
            continue; 
        }
    }
    free(way);
    closedir(direct);
    return;
}

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Это не название файла\n");
        return 0;
    }
    int depth = 0;

    int count = 0;
    printf("Введите глубину поиска (количество проверяемых директорий, начиная с текущей): ");
    scanf("%d", &depth);
	if (depth < 1) {
		printf("В следующий раз введи нормальное число\n");
		exit(-1);
	}
	
    char cur_dir[8] = ".";
    Fidn_object(cur_dir, argv[1], depth, &count);
    if (count != 0) {
        printf("Нашел %d файлов с названием %s\n", count, argv[1]);
    }
    else 
        printf("Нема\n");
    return 0;
}