/* Программа для записи в FIFO */
/* Для отладки использовать утилиту strace: strace -e trace=open,read ./имя программы */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int fd;
  size_t size;
  char name[] = "aaa.fifo";
  char string[] = "Погладь Кота!\n";
  size_t str_len = strlen(string);

  /* Обнуляем маску создания файлов текущего процесса для того, чтобы
  права доступа у создаваемого FIFO точно соотвествовали параметру вызова mknod() */
  (void)umask(0);

  // printf("1\n");

  /* Попытаемся создать FIFO с именем  aaa.fifo в текущей директории,
  если FIFO уже существует, то этот if нужно удалить !!! или добавить анализ */
  // if (mknod(name, S_IFIFO | 0666, 0) < 0)
  // {
  //   /* Если создать FIFO не удалось, печатаем об этом сообщение и прекращаем работу */
  //   printf("Can\'t create FIFO\n");
  //   exit(-1);
  // }
  mknod(name, S_IFIFO | 0666, 0);

  // printf("2\n");

  /* Открываем FIFO на запись */
  if ((fd = open(name, O_WRONLY)) < 0)
  {
    /* Если открыть FIFO не удалось, печатаем об этом сообщение и прекращаем работу */
    printf("Can\'t open FIFO for writting\n");
    exit(-1);
  }

  // printf("3\n");

  /* Пробуем записать в FIFO ?? байт, т.е. всю строку "Погладь Кота!"
  вместе с ... */
  size = write(fd, string, str_len);

  // printf("4\n");

  if (size != str_len)
  {
    /* Если записалось меньшее количество байт, сообщаем об ошибке и завершаем работу */
    printf("Can\'t write all string to FIFO\n");
    exit(-1);
  }

  close(fd);

  // printf("5\n");

  return 0;
} 
