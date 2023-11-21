#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
/* Модифицировать программу, 
 * чтобы замерить среднее время завершения нити после сигнала на завершение.
 */
void *any_func(void * arg)
{
	while(1) 
	{
		fprintf(stderr, ".");
		sleep(1);
	}

	return NULL;
}

int main(void)
{
	pthread_t thread;
	void *result;
	if (pthread_create(&thread, NULL, &any_func, NULL) != 0) 
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	// sleep(5);
	clock_t t1 = clock();
	pthread_cancel(thread);
	if (!pthread_equal(pthread_self(), thread))
	{
		pthread_join(thread, &result);
	}
	if (result == PTHREAD_CANCELED)
	{
		clock_t t2 = clock();
		fprintf(stderr, "Canceled\n");

		printf("%ld\n", t2 - t1);
	}

	return 0;
}