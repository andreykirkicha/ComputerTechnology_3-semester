// Написать программу получающую в качестве параметров два номера. Первый номер это 
// номер для получения сообщений, второй номер для передачи сообщений. Можно  
// запустить несколько экземпляров такой программы и они все будут обмениваться  
// сообщениями между собой по номерам.
// Всё это нужно реализовать через одну очередь. 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MESSAGE_LENGTH 255

int main(int an, char *arg[])
{
  int msqid;
  char pathname[] = "2_Q.c";
  key_t key;
  pid_t chpid;
  int len, maxlen;
  struct mymsgbuf
  {
    long mtype;
    char mtext[MESSAGE_LENGTH];
  } mybuf;
  
  key = ftok(pathname, 0);

  // Если очередь уже существует, то ничего не делаем  
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
  {
    printf("Can\'t get msqid\n");
    exit(-1);
  }
    
  chpid = fork();
  
  // Один из процессов проверяет очередь на предмет наличия в ней сообщений,
  // адресованных данному терминалу и выводит их на экран
  if (chpid == 0)
  {
    if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 0, 0)) < 0)
    {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }

    printf("Message: %s\n", mybuf.mtext);
  }

  // Второй процесс ожидает ввода с клавиатуры и отправляет сообщения,
  // встретив символ перевода строки 
  else
  {
    char message[MESSAGE_LENGTH];

    scanf("%s", message);
  
    strcpy(mybuf.mtext, message);
    len = strlen(mybuf.mtext) + 1;
       
    if (msgsnd(msqid, (struct msgbuf *)&mybuf, len, 0) < 0)
    {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(-1);
    }
  }
}