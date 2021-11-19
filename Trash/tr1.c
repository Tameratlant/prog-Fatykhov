//Запускать с - pthread -lm -O3
//Для упрощения дебага почти все, что можно, вынесено в отдельные функции
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>


#define NUMBER_OF_THREADS 2
#define DATA_SIZE 100000000

float *array = NULL;
double segment_sum[NUMBER_OF_THREADS];
double segment_sum_square[NUMBER_OF_THREADS];

typedef struct {
	int start;
	int end;
	int id;
} Segment;

void* Sum (void* dummy) {
	Segment segment = *((Segment *)dummy);
	int i = 0;
	segment_sum[segment.id] = 0;
	segment_sum_square[segment.id] = 0;
	for (i = segment.start; i < segment.end; i++) {
		segment_sum[segment.id] += array[i];
		segment_sum_square[segment.id] += pow(array[i], 2);
	}
    return NULL;
}

void Fill_segments(Segment segments[], int segment_size){

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
		segments[i].id = i;
		segments[i].start = segment_size * i;
		if (i != NUMBER_OF_THREADS - 1) {
			segments[i].end = segments[i].start + segment_size;
		} else {
			segments[i].end = DATA_SIZE;
		}
	}
}

void make_data() {														//Заполняем массив мусором, который будем анализировать
	//printf("1");
	for (int i = 0; i < DATA_SIZE; i++) {
		array[i] = rand() % 100;
	}
}

void results_out(double a, double b, double c, double time) {						//Просто выводит все посчитанное
	printf("The sum of all experimental data = %lf\n", a);
	printf("Math expectation = %lf\n", b);
	printf("Dispersion = %lf\n", c);
	printf("Time working = %lf\n", time);
}

void Generate_threads(Segment segments[]){
    pthread_t array_thread_id[NUMBER_OF_THREADS];
    int result, i = 0;
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
}

int main () {
	array = (float *) calloc(DATA_SIZE, sizeof(float));
	int i = 0;
	srand(time(NULL));
    make_data();
	struct timespec start, finish;
	double time_working = 0;
	clock_gettime(CLOCK_MONOTONIC, &start);
	Segment segments[NUMBER_OF_THREADS];
	int segment_size = DATA_SIZE/NUMBER_OF_THREADS;//делим массив на одинаковые части - сегменты (по кол-ву нитей)
	//заполнение сегментов началом, концом и индексами

    Fill_segments(segments, segment_size);
    Generate_threads(segments);   

	double sum = 0, sum_square = 0, math_expectation = 0, math_expectation_of_square = 0;
	//сложение посчитанных значений для каждого сегмента
	for (i = 0; i < NUMBER_OF_THREADS; i++) {
		sum += segment_sum[i];
		sum_square += segment_sum_square[i];
	}
	
	math_expectation = sum / DATA_SIZE;
	math_expectation_of_square = sum_square / DATA_SIZE;

	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_working = (finish.tv_sec - start.tv_sec); 
	time_working += (finish.tv_nsec - start.tv_nsec) / 1e9; 
	
    results_out(sum, math_expectation, math_expectation_of_square - pow(math_expectation, 2), time_working);
	free(array);
    return 0;
}