#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

/* 
	Дочерний процесс, анализирующий год и возвращающий различные сигналы,
	в зависимости от его високосности.
*/
void signal_handler1(int)
{
	printf("signal_handler1 is working now\n"
			"Год високосный\n");
}

void signal_handler2(int)
{
	printf("signal_handler2 is working now\n"
		   "Год не високосный\n");
}

int main (int argc, char **argv)
{
	int year;
	pid_t chpid = fork();

	if (argc < 2) 
	{
		fprintf(stderr, "child: too few arguments\n");
		return 2;
	}

	year = atoi(argv[1]);

	signal(SIGUSR1, signal_handler1);
	signal(SIGUSR2, signal_handler2);

	if (chpid == 0)
	{
		if (year <= 0)
			return 2;

		/* Объяснить как считаем условие */

		/*  
			Любой год, который делится на 4 без остатка, является високосным годом: 
			например, 1988, 1992 и 1996 годы являются високосными годами.
			Тем не менее, есть еще небольшая ошибка, которая должна быть учтена.
			Чтобы устранить эту ошибку, григорианский календарь предусматривает, что год, 
			который делится без остатка на 100 (например, 1900) является високосным годом
			только в том случае, если он также без остатка делится на 400.
		*/	
		if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
			kill(getppid(), SIGUSR1);
		else
			kill(getppid(), SIGUSR2);
	}

	usleep(1000 * 1000);

	return 0;
}
