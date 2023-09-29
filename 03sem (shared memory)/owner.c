/* Проверить совместную работу с 4.
   Написать комментарии, В ТОМ ЧИСЛЕ К ПАРАМЕТРАМ! */
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

#define SHMEM_SIZE  4096
#define SH_MESSAGE	"Poglad Kota!"

int main(void)
{
	int shm_id;
	char *shm_buf;
	int shm_size;
	struct shmid_ds ds;
	
    // Создаём разделяемую память
	shm_id = shmget(IPC_PRIVATE,						// IPC_PRIVATE - приватный ключ (0)
					SHMEM_SIZE,							// SHMEM_SIZE - размер разделяемой памяти
					IPC_CREAT | IPC_EXCL | 0600);   	// Если значение key равно IPC_PRIVATE или если key не равно IPC_PRIVATE,
														// но не существует общего сегмента памяти, который бы соответствовал
														// значению key и в shmflg есть флаг IPC_CREAT, то создаётся новый общий 
														// сегмент памяти размером SHMEM_SIZE, округлённым до значения, кратного 
														// PAGE_SIZE. Если в одновременно указаны IPC_CREAT и IPC_EXCL и
														// для данного ключа уже существует общий сегмент памяти,
														// то вызов shmget завершается с ошибкой и errno присваивается EEXIST

    // Если не удалось создать
	if (shm_id == -1) 
	{
		fprintf(stderr, "shmget() error\n");
		return 1;
	}

    // Привязываем разделяемую память к указателю shm_buf
	shm_buf = (char *)shmat(shm_id,			// shm_id - идентификатор разделяемой памяти
							NULL,			// NULL - адрес подключения, в нашем случае система выбирает подходящий
											// (неиспользуемый) адрес для подключения сегмента
							0);				// Последний параметр - флаги

	// Если не привязали 
	if (shm_buf == (char *)NULL)
	{
		fprintf(stderr, "shmat() error\n");
		return 1;
	}

	// Выполняет управляющую операцию
	shmctl(shm_id,			// Идентификатор разделяемой памяти
		   IPC_STAT,		// cmd - управляющая операция
		   					// IPC_STAT - копирует информацию из структуры данных ядра, 
							// связанной с shmid, в структуру shsid_ds, расположенную по адресу buf
		   &ds);			// Указатель на встроенную структуру, хранящую данные о разделяемой памяти
	
	shm_size = ds.shm_segsz;
	if (shm_size < strlen(SH_MESSAGE)) 
	{
		fprintf(stderr, "error: segsize = %d\n", shm_size);
		return 1;
	}
	
	strcpy(shm_buf, SH_MESSAGE);
	shm_buf[strlen(SH_MESSAGE)] = '\0';

	printf("ID: %d\n", shm_id);
	printf("Press <Enter> to exit...");	
	fgetc(stdin);

	shmdt(shm_buf);
	shmctl(shm_id, IPC_RMID, &ds);

	return 0;
}