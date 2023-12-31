//написать программу получающую в качестве параметров два номера. Первый номер это 
//номер для получения сообщений, второй номер для передачи сообщений. Можно  
//запустить несколько экземпляров такой программы и они все будут обмениваться  
//сообщениями между собой по номерам.
// Всё это нужно реализовать через одну очередь. 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MESSAGE_LENGTH 255

int main(int an, char* arg[])
{
  int msqid;
  char pathname[] = "2_Q.c";
  key_t  key;
  int len, maxlen;
  struct mymsgbuf
  {
    long mtype;
    char mtext[MESSAGE_LENGTH];
  } mybuf;
    
  key = ftok(pathname, 0);
   
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
  {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  int receiveNumber = atoi(arg[1]);
  int sendNumber = atoi(arg[2]);
    
  pid_t pid = fork();

  if (pid < 0)
  {
    perror("Can\'t make another process\n");
    exit(-1);
  }
  else if (pid == 0)
  {
    while (1)
    {
      if (msgrcv(msqid, &mybuf, MESSAGE_LENGTH, receiveNumber, 0) < 0)
      {
        perror("Can\'t receive message\n");
        exit(-1);
      }
      printf("Received: %s\n", mybuf.mtext);
    }
  }
  else
  {
    while(1)
    {
      char message[MESSAGE_LENGTH];
      fgets(message, MESSAGE_LENGTH, stdin);
      message[strcspn(message, "\n")] = '\0';

      mybuf.mtype = sendNumber;
      strncpy(mybuf.mtext, message, MESSAGE_LENGTH);

      if (msgsnd(msqid, &mybuf, strlen(mybuf.mtext) + 1, 0) < 0)
      {
        perror("Can\'t send message\n");
        exit(-1);
      }
    }
  }
}
