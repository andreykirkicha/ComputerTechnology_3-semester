/* Программа, осуществляющая двунаправленную связь через pipe
между процессом-родителем и процессом-ребенком */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fd0[2], fd1[2], result;
  size_t size;
  char res_string[80];
  char string_child[] = "Привет, дитя!", string_parent[] = "Ну привет, папик!";

  /* Создаем два pip'а */
  if (pipe(fd0) < 0)
  {
    printf("Can\'t create pipe\n");
    exit(-1);
  }

  if (pipe(fd1) < 0)
  {
    printf("Can\'t create pipe\n");
    exit(-1);
  }

  /* Порождаем новый процесс */
  result = fork();

  if (result < 0)
  {
    /* Если создать процесс не удалось, сообщаем об этом и завершаем работу */
    printf("Can\'t fork child\n");
    exit(-1);
  } 
  else if (result > 0) 
  {
    /* Мы находимся в родительском процессе. Закрываем ненужные потоки данных */
    close(fd0[0]);
    close(fd1[1]);

    /* Пишем в первый pipe и читаем из второго */
    size = write(fd0[1], string_child, sizeof(string_child));
    
    if (size != sizeof(string_child))
    {
      printf("Parent: can\'t write all string\n");
      exit(-1);
    }
    
    size = read(fd1[0], res_string, 80);
    if (size < 0)
    {
      printf("Parent: can\'t read string\n");
      exit(-1);
    }

    printf("Parent: %s\n", res_string);
    close(fd0[1]);
    close(fd1[0]);
    printf("Parent exit\n");
  } 
  else 
  {
    /* Мы находимся в порожденном процессе. Закрываем ненужные потоки данных */
    close(fd0[1]);
    close(fd1[0]);
    
    /* Читаем из первого pip'а и пишем во второй */
    size = read(fd0[0], res_string, 80);
    if (size < 0)
    {
      printf("Child: can\'t read string\n");
      exit(-1);
    }
    
    printf("Child: %s\n", res_string);
    
    size = write(fd1[1], string_parent, sizeof(string_parent));
    if (size != sizeof(string_parent))
    {
      printf("Child: can\'t write all string\n");
      exit(-1);
    }

    close(fd0[0]);
    close(fd1[1]);
    // printf("Child exit\n");
  }

  return 0;
} 
