#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
/* Переделать программу для доказательстава: sin*sin + coc*cos == 1
 * Квадрат синуса считать в одном потоке, косинуса во втором,
 * а результат суммировать в главной программе.*/

double *sin_ptr, *cos_ptr;

void *thread_func1(void *arg)
{
	*sin_ptr = sin(*((double *)arg));
	return sin_ptr;
}

void *thread_func2(void *arg)
{
	*cos_ptr = cos(*((double *)arg));
	return cos_ptr;
}

int main(void)
{
	pthread_t thread1, thread2;
	void *sin_void, *cos_void;
	double *sin_d, *cos_d;
	double x = 0;

	sin_ptr = (double *)malloc(sizeof(double));
	cos_ptr = (double *)malloc(sizeof(double));

	if (pthread_create(&thread1, NULL, &thread_func1, &x) != 0)
	{
		fprintf(stderr, "Error (thread1)\n");
		return 1;
	}

	if (pthread_create(&thread2, NULL, &thread_func2, &x) != 0) 
	{
		fprintf(stderr, "Error (thread2)\n");
		return 1;
	}

	for (x = 0; x < 4 * 3.14; x += 0.1)
	{
		thread_func1((void *)&x);
		pthread_join(thread1, &sin_void);
		sin_d = (double *)sin_void;

		thread_func2((void *)&x);
		pthread_join(thread2, &cos_void);
		cos_d = (double *)cos_void;

		fprintf(stderr, "x = %f\t", x);
		fprintf(stderr, "sin^2(x) + cos^2(x) = %f\n", (*sin_d) * (*sin_d) + (*cos_d) * (*cos_d));
	}
	while(1);
	return 0;
}
