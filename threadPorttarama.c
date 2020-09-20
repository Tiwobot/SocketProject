/*
 * lwsock32 lpthread eklenmeli
 * EA_Socket.h EA_Socket.c ayný dizinde olmalý
 */
#include <stdio.h>
#include <pthread.h>
#include "lib\EA_Socket.h"

#ifdef _WINDOWS
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(x) usleep((x)*1000)
#endif

char ip[16] = {0};

void * portScan(void *arg)
{
	int port = *(int *)arg;
	EA_SOKET soket;
	struct sockaddr_in veriler;

	soket = EA_Socket(AF_INET, SOCK_STREAM, 0);
		
	EA_SockaddrIn(&veriler, AF_INET, htons(port), inet_addr(ip));
		
	if(EA_Connect(soket, (struct  sockaddr *)&veriler) != -1)
		printf("%d. port : Open\n", port);

	EA_Close(soket);

}
int main()
{
	pthread_t *thread;
	int *id, min, maks, i;
	
	EA_InitW();
	
	printf("IP MinPort MaksPort X.X.X.X 0 65534 > ");
	scanf("%s %d %d", ip, &min, &maks);
	
	id = (int *) malloc((maks - min + 1) * sizeof(int));
	thread = (pthread_t *) malloc((maks - min + 1) * sizeof(pthread_t));
	
	for (i = 0; i < (maks - min + 1); i++)
	{
		*(id + i) = min + i;
		pthread_create(thread + i, NULL, portScan, (void *)(id + i));
	}
	
	for (i = 0; i < (maks - min + 1); i++)
		pthread_join(*(thread + i), NULL);
	
	printf("\nProgram Sonu!\n");
	system("PAUSE");
	
	EA_CleanW();

	return 0;
}

#include "lib\EA_Socket.c"
