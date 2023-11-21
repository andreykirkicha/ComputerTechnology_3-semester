#include <stdio.h>
#include <pthread.h>
//-lpthread или -pthread  для успешной компиляции добавтить одну из этих опций
// Добавить вызов any_func в основную нить выполнения, чтобы распечатать 2009
void *any_func(void *arg)
{
  // Воспринимаем 'a' как область памяти типа int
  int *a = (int *)arg; // Добавить описание этой строки
  (*a)++;
  return a;
}

int main(void)
{
  pthread_t thread;
  int parg = 2007, pdata;
	
  if (pthread_create(&thread, NULL, &any_func, &parg) != 0) 
  {
    fprintf(stderr, "Error\n");
    return 1;
  }

  any_func((void *)&parg);

  void *result;
  pthread_join(thread, &result);
  int *p = (int *)result;
  pdata = *p;
  
  printf("%d\n", pdata);

  return 0;
}