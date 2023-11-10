/* 
  Создать программу (программы) для последовательного вычетания 1 двумя процессами
  из некоторого числа, записанного в разделяемой памяти.
  Очерёдность работы процессов регулируем при помощи семафора.
  Ход выполнения следующий:
    - Открываем два терминала и запускаем в них созданную(ые) программы
    - В одном из них вводим некоторое число.
    - После чего обе программы начинают выводить по очереди уменьшенное на 1,
      полученное значение.
  Предусмотреть задержки для наглядности.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SEM_KEY 2007
#define SHM_KEY 2008

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void wait_semaphore(int sem_id)
{
    struct sembuf wait_buf = {0, -1, 0};
    semop(sem_id, &wait_buf, 1);
}

void signal_semaphore(int sem_id)
{
    struct sembuf signal_buf = {0, 1, 0};
    semop(sem_id, &signal_buf, 1);
}

int main()
{
    int shm_id, sem_id;
    key_t key;
    int *shared_data;

    // Генерируем ключи для семафора и разделяемой памяти
    key = ftok("semshm.c", 1);

    // Создаём семафор
    sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Инициализируем семафор значением 1
    union semun arg;
    arg.val = 1;
    if (semctl(sem_id, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    // Создаём сегмент разделяемой памяти
    shm_id = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if (shm_id == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Подключаем сегмент разделяемой памяти
    shared_data = shmat(shm_id, NULL, 0);
    if (*shared_data == -1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Вводим начальное значение в разделяемую память
    printf("Введите начальное значение: ");
    scanf("%d", shared_data);

    // Запускаем два процесса
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) 
    {
        // Дочерний процесс
        while (*shared_data >= 1)
        {
            wait_semaphore(sem_id);
            (*shared_data)--;
            printf("Процесс-потомок:\t%d\n", *shared_data);
            signal_semaphore(sem_id);
            sleep(1);
        }
    } else 
    {
        // Родительский процесс
        while (*shared_data >= 2) 
        {
            wait_semaphore(sem_id);
            (*shared_data)--;
            printf("Процесс-родитель:\t%d\n", *shared_data);
            signal_semaphore(sem_id);
            sleep(1);
        }
    }

    // Освобождаем ресурсы
    shmdt(shared_data);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID, arg);

    return 0;
}