#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>


#define NUMBER_OF_THREADS 4
#define DATA_SIZE ((int)1e8)
#define KEY "%lf"

float *array = NULL;
double segment_sum[NUMBER_OF_THREADS];
double segment_sum_square[NUMBER_OF_THREADS];

typedef struct {
	int start;
	int end;
	int id;
} Segment;

void* Sum (void* arg) {
	Segment segment = *((Segment *)arg);//разыменование указателя на структуру iго объекта segments, приведенного к типу Segment *
	int i = 0;
	segment_sum[segment.id] = 0;
	segment_sum_square[segment.id] = 0;
	for (i = segment.start; i < segment.end; i++) {
		segment_sum[segment.id] += array[i];
		segment_sum_square[segment.id] += pow(array[i], 2);
	}
    return NULL;
}

int main (int argc, char* argv[]) {
	array = (float *) calloc(DATA_SIZE, sizeof(float));
	int i = 0;

	srand(time(NULL));
    for (i = 0; i < DATA_SIZE; i++) {
		array[i] = (1 + rand()%100);
	}

	struct timespec start, finish;
	double time_working = 0;
	clock_gettime(CLOCK_MONOTONIC, &start);

	Segment segments[NUMBER_OF_THREADS];
	int segment_size = DATA_SIZE/NUMBER_OF_THREADS;//делим массив на одинаковые части - сегменты (по кол-ву нитей)
	
	//заполнение сегментов началом, концом и индексами
	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		segments[i].id = i;
		segments[i].start = segment_size * i;
		if (i != NUMBER_OF_THREADS - 1) {
			segments[i].end = segments[i].start + segment_size;
		} else {
			segments[i].end = DATA_SIZE;//не забыть последний элемент, из-за округления segment_size может потеряться
		}
	}
	
	pthread_t array_thread_id[NUMBER_OF_THREADS];
	
    int result = 0;
    //множим нити и отправляем каждый сегмент на обработку в его нить
	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		result = pthread_create(&(array_thread_id[i]), (pthread_attr_t *)NULL, Sum, &(segments[i]));
        if (result != 0) {
			printf ("Error on thread create, return value = %d\n", result);
			exit(-1);
    	}
	}

	//чтобы всех дождались
	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		pthread_join(array_thread_id[i], NULL);
	}

	double sum = 0, sum_square = 0, math_expectation = 0, math_expectation_of_square = 0;
	
	//сложение посчитанных значений для каждого сегмента
	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		sum += segment_sum[i];
		sum_square += segment_sum_square[i];
	}
	
	math_expectation = sum / DATA_SIZE;//е от кси (мат.ожид)
	math_expectation_of_square = sum_square / DATA_SIZE;//е от кси-квадрат

	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_working = (finish.tv_sec - start.tv_sec); //время в секундах
	time_working += (finish.tv_nsec - start.tv_nsec) / 1e9; //хвост времени в наносекундах
	
	printf("Time of working = " KEY "\n", time_working);
	printf("The sum of all experimental data = " KEY "\n", sum);
	printf("Math expectation = " KEY "\n", math_expectation);
	printf("Dispersion = " KEY "\n", math_expectation_of_square - pow(math_expectation, 2));

	free(array);
    return 0;
}