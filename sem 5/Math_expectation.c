//Запускать с - pthread -lm -O3
//Для упрощения дебага почти все, что можно, вынесено в отдельные функции

/*При попытке использовать функции clock() вылетала ошибка:
error: initializer element is not constant
38 | double start_time =  clock();
Поэтому вместо нее использовал clock_gettime*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#define THREADS_NUMBER 2
#define DATA_SIZE 100000000

/*	При DATA_SIZE 1000000000; THREADS_NUMBER 1 ноут намертво виснет
	При DATA_SIZE 100000000; THREADS_NUMBER 1 Time working = 0.99
	При DATA_SIZE 100000000; THREADS_NUMBER 2 Time working = 0,73
	При DATA_SIZE 100000000; THREADS_NUMBER 3 Time working = 0,65
	При DATA_SIZE 100000000; THREADS_NUMBER 4 Time working = 4,56
	При DATA_SIZE 100000000; THREADS_NUMBER 5 Time working = 3,58
	При DATA_SIZE 100000000; THREADS_NUMBER 10 Time working = 5.516035
	При DATA_SIZE 100000000; THREADS_NUMBER 50 Time working = 1.709575
	При DATA_SIZE 100000000; THREADS_NUMBER 100 Time working = 1.376533
	При DATA_SIZE 100000000; THREADS_NUMBER 500 Time working = 1.300596
	При DATA_SIZE 100000000; THREADS_NUMBER 1000 Time working = 1.425162
	При DATA_SIZE 100000000; THREADS_NUMBER 1500 Time working = 1.546174
	При DATA_SIZE 100000000; THREADS_NUMBER 2000 Time working = 1.588876

	Сказать, что результат отличается от ожиданий - это ничего не сказать
	Есть подозрение, что на четных время работы больше, чем на нечетных.
	Так или иначе, видно, что после 500 нитей процесс начинет замедляться из-за затрат на переключение между нитями
*/
double* data = NULL;
double sum_of_segment[THREADS_NUMBER];
double sum_square_of_segment[THREADS_NUMBER];

typedef struct {
	int start;
	int end;
	int id;
} Segment;
Segment segments[THREADS_NUMBER];
int seze_of_segment = DATA_SIZE / THREADS_NUMBER;

pthread_t thread_id[THREADS_NUMBER];


void make_data() {														//Заполняем массив мусором, который будем анализировать
	//printf("1");
	for (int i = 0; i < DATA_SIZE; i++) {
		data[i] = rand();
	}
}

clock_t start, end;

//double start =  clock();

//clock_t begin = clock();









void* Sum(void* dummy) {												//Находим сумму и сумму квадратов конкретного куска
	//printf("2");
	Segment segment = *((Segment*)dummy);
	sum_of_segment[segment.id] = 0;
	sum_square_of_segment[segment.id] = 0;
	for (int i = segment.start; i < segment.end; i++) {
		sum_of_segment[segment.id] += data[i];
		sum_square_of_segment[segment.id] += pow(data[i], 2);
	}
}

void fill_segments() {													//Распределяем куски данных по сегментам
	//printf("3");
	for (int i = 0; i < THREADS_NUMBER; i++) {
		segments[i].id = i;
		segments[i].start = seze_of_segment * i;
		segments[i].end = segments[i].start + seze_of_segment;
	}
}

void thread_create() {
	int result = 0;
	
	for (int i = 0; i < THREADS_NUMBER; i++) {							//Создаем нити и отдаем каждой по сегменту
		if ((result = (pthread_create(&(thread_id[i]), 
					  (pthread_attr_t*)NULL, 
				      Sum, 
					  &(segments[i])))) != 0) {
			printf("Error on thread create, return value = %d\n", result);
			exit(-1);
		}
	}
	for (int i = 0; i < THREADS_NUMBER; i++) {							//Ждем завершения исполнения
		pthread_join(thread_id[i], NULL);
	}
}
void results_out(double a, double b, double c, double time) {						//Просто выводит все посчитанное
	printf("The sum of all experimental data = %lf\n", a);
	printf("Math expectation = %lf\n", b);
	printf("Dispersion = %lf\n", c);
	printf("Time working = %lf", time);
}


int main() {
	srand(13);
	/*
	static clock_t startTime = (clock_t) -1;

	if (startTime == -1) {startTime = clock();}
    else if ((startTime - clock() / CLOCKS_PER_SEC) > -1)
    {
        startTime = clock();
        
    }
	*/
	int i = 0;
	data = (double*)calloc(DATA_SIZE, sizeof(double));
	make_data();
	fill_segments();
	static clock_t startTime = (clock_t) -1;

        if (startTime == -1) {startTime = clock();}
    else if ((startTime - clock() / CLOCKS_PER_SEC) > -1)
    {
        startTime = clock();

    }

	thread_create();
	double sum = 0, 
		sum_square = 0, 
		math_expectation = 0, 
		math_expectation_of_square = 0;

	for (i = 0; i < THREADS_NUMBER; i++) {								//Объединение того, что посчитали нити
		sum += sum_of_segment[i];
		sum_square += sum_square_of_segment[i];
	}

	math_expectation = sum / DATA_SIZE;
	math_expectation_of_square = sum_square / DATA_SIZE;

	//clock_t end = clock();
	//double end =  clock();
	static clock_t endTime = (clock_t) -1;

    if (endTime == -1) endTime = clock();
    else if ((endTime - clock() / CLOCKS_PER_SEC) > -1)
    {
        endTime = clock();
        
    }




	double time_spent = (double)(endTime - startTime) / CLOCKS_PER_SEC;

	results_out(sum, math_expectation, math_expectation_of_square - pow(math_expectation, 2), time_spent);
	free(data);
	return 0;
}

