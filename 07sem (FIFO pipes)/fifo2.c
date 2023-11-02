/* Программа для чтения из FIFO */
/* Для отладки использовать утилиту strace: strace -e trace=open,read ./имя программы */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fd;
  size_t size;
  char res_string[24];
  char name[] = "aaa.fifo";

  // printf("1\n");

  /* Открываем FIFO на чтение */
  if ((fd = open(name, O_RDONLY)) < 0)
  {
    /* Если открыть FIFO не удалось, 
    печатаем об этом сообщение и прекращаем работу */
    printf("Can\'t open FIFO for reading\n");
    exit(-1);
  }

  // printf("2\n");

  /* Пробуем прочитать из FIFO 20 байт в массив,
  т.е. заполнить весь доступный буфер */
  size = read(fd, res_string, 24);
  if (size < 0)
  {
    /* Если прочитать не смогли, сообщаем об ошибке и завершаем работу */
    printf("Can\'t read string\n");
    exit(-1);
  }

  // printf("3\n");

  /* Печатаем прочитанную строку */
  printf("Result string: %s\n", res_string);

  // printf("4\n");

  // Зачем ??
  unlink(name);
  close(fd);

  // printf("5\n");
  
  return 0;
}
