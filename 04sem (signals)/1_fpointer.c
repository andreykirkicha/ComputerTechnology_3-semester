/* 
  Добавить вторую функцию, сравнивающую количество букв,
  Добавить вызов различных функций в зависимости от аргумента программы
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void check(char *a, char *b, int (*cmp)(const char *, const char *));
int cmp_letters(const char *str1, const char *str2);

int main(int argc, char **argv)
{
  char s1[80], s2[80];
  int (*p)(const char *, const char *);

  printf("Введите две строки:\n");
  fgets(s1, 79, stdin);
  fgets(s2, 79, stdin);

  p = cmp_letters;
  if (!strcmp(argv[1], "strcmp")) p = strcmp;

  check(s1, s2, p);

  return 0;
}

int cmp_letters(const char *str1, const char *str2)
{
  int len1 = strlen(str1), len2 = strlen(str2);
  int cnt1 = 0, cnt2 = 0, i = 0;

  for (i = 0; i < len1; ++i)
  {
    if (isalpha(str1[i])) ++cnt1;
  }

  for (i = 0; i < len2; ++i)
  {
    if (isalpha(str2[i])) ++cnt2;
  }

  return !(cnt1 == cnt2);
}

void check(char *a, char *b, int (*cmp)(const char *, const char *))
{
  printf("Проверка на совпадение:\n");
  if (!(*cmp)(a, b)) printf("Равны\n");
  else printf("Не равны\n");
}