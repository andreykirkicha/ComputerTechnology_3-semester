/* Создать программу для запуска на двух терминалах для обмена сообщениями между ними
через файл - один передаёт(записывает файл) второй получает, можно чтобы он ещё и рассчитывал 
чего-либо (например возводил строку, если это число, в квадрат). Что будет, если запустить
более двух териналов на запись и на чтение? */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    pid_t chpid;

    chpid = fork();
    
    if (chpid < 0)
    {
        /* Ошибка */
        printf("Ошибка\n");
    }
    else if (chpid == 0)
    {
        /* Порожденный процесс */
        char string[60];
        size_t size;
        int fd;

        if ((fd = open(argv[1], O_RDONLY)) < 0)
        {
            /* Если файл открыть не удалось, печатаем об этом сообщение и прекращаем работу */
            printf("Can\'t open file\n");
            exit(-1);
        }

        while (size = read(fd, string, 60))
            printf("%s\n", string);
    }
    else
    {
        /* Родительский процесс */
        char string[60];
        int fd;
        size_t size;

        if ((fd = open(argv[1], O_WRONLY)) < 0)
        {
            /* Если файл открыть не удалось, печатаем об этом сообщение и прекращаем работу */
            printf("Can\'t open file\n");
            exit(-1);
        }

        while (scanf("%s", string))
        {
            size = write(fd, string, 60);
        }
    }

    return 0;
}