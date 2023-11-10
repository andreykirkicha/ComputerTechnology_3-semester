#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHMEM_SIZE	4096
#define SH_MESSAGE	"Hello World!\n"

// Написать комментарии, отладить работу

union semnum 
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} sem_arg;


int main(void)
{
	int shm_id, sem_id;
	char *shm_buf;
	int shm_size;
	int key;
	char *pathname = "3_shmsem-owner.c";
	struct shmid_ds ds;
	struct sembuf sb[1];
	unsigned short sem_vals[1];

	key = ftok(pathname, 0);
	
	shm_id = shmget(key, SHMEM_SIZE, IPC_CREAT | 0600);

	if (shm_id == -1) 
	{
		fprintf(stderr, "shmget() error\n");
		return 1;
	}

	sem_id = semget(key, 1, 0600 | IPC_CREAT);

	if (sem_id == -1)
	{
		fprintf(stderr, "semget() error\n");
		return 1;
	}

	printf("Semaphore: %d\n", sem_id);
	sem_vals[0] = 1;
	sem_arg.array = sem_vals;

	if (semctl(sem_id, 0, SETALL, sem_arg) == -1)
	{
		fprintf(stderr, "semctl() error\n");
		return 1;
	}

	shm_buf = (char *)shmat(shm_id, NULL, 0);
	if (shm_buf == (char *)-1)
	{
		fprintf(stderr, "shmat() error\n");
		return 1;
	}

	shmctl(shm_id, IPC_STAT, &ds);
	
	shm_size = ds.shm_segsz;
	if (shm_size < strlen (SH_MESSAGE))
	{
		fprintf(stderr, "error: segsize=%d\n", shm_size);
		return 1;
	}
	
	strcpy(shm_buf, SH_MESSAGE);

	printf("ID: %d\n", shm_id);

	sb[0].sem_num = 0;
	sb[0].sem_flg = SEM_UNDO;
	
	sb[0].sem_op = -1;
	semop(sem_id, sb, 1);

	sb[0].sem_op = -1;
	semop(sem_id, sb, 1);
	
	semctl(sem_id, 1, IPC_RMID, sem_arg);
	shmdt(shm_buf);
	shmctl(shm_id, IPC_RMID, NULL);

	return 0;
}
