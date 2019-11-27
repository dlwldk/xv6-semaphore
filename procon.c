#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define N 100


/*struct semaphore {
	//int mutex = 1;
	int empty = N;
	int full = 0;
};*/
//sem_t empty;
//sem_t full;

int  buffer[N];
int in, out;
int empty = N, full = 0;
int pro_counter = 0, con_counter = 0;

void *producer(int data) {
	sem_wait(empty);
	//sem_wait(mutex);
	buffer[in] = data;
	in = (in + 1) % N;
	pro_counter++;
	//sem_signal(mutex);
	sem_signal(full);
	if (pro_counter == 1000)
		kill(data);
	return 0;
}


void *consumer(int data) {
	sem_wait(full);
	//sem_wait(mutex);
	data = buffer[out];
	out = (out + 1) % N;
	con_counter++;
	//sem_signal(mutex);
	sem_signal(empty);
	if (con_counter == 1000)
		kill(data);
	return 0;
	
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf(2, "Input data \n");
		exit();
	}
	int *temp_data=atoi(argv[1]);
	int tmp=atoi(argv[1]);

	//스레드 생성
	hufs_thread_create(&producer, &temp_data);
	hufs_thread_create(&producer, &temp_data);
	hufs_thread_create(&consumer, &temp_data);
	hufs_thread_create(&consumer, &temp_data);

	// 스레드 조인
	hufs_thread_join(tmp);
	
	//세마포어 종료
	sem_destroy(empty); 
	sem_destroy(full);

	return 0;
}
