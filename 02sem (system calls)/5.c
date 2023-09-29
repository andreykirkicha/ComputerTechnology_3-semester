/* Программа, иллюстрирующая использование системных вызовов open(), read() и close() для чтения информации из файла */

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
    int fd, other_fd, result;
    size_t size, write_size;
    char string[60];

    /* Попытаемся открыть файл с именем в первом параметре вызова только
    для операций чтения */
    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        /* Если файл открыть не удалось, печатаем об этом сообщение и прекращаем работу */
        printf("Can\'t open file\n");
        exit(-1);
    }

    /* Читаем файл пока не кончится и печатаем */
    while (size = read(fd, string, 60))
    {
        printf("%s\n", string); /* Печатаем прочитанное*/

        /* Записываем файл под новым именем */
        if ((other_fd = open(argv[2], O_WRONLY)) < 0)
        {
            /* Если файл открыть не удалось, печатаем об этом сообщение и прекращаем работу */
            printf("Can\'t open file\n");
            exit(-1);
        }

        if (!(write_size = write(other_fd, string, 60)))
        {
            printf("Can\'t write to file\n");
            exit(-1);
        }
    }

    /* Закрываем файл */
    if (close(fd) < 0)
    {
        printf("Can\'t close file\n");
    }

    if (close(other_fd) < 0)
    {
        printf("Can\'t close file\n");
    }

    /* Открываем файл в редакторе */
    result = execle("/bin/cat", "/bin/cat", argv[2], (char *)NULL, envp);

    if (result < 0)
    {
        /* Если возникла ошибка, то попадаем сюда*/
        printf("Error on program start\n");
        exit(-1);
    }
    
    return 0;
} 