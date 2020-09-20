/*
 * lwsock32 lpthread eklenmeli
 * TCP: Port : 3485, Service : 3485, Open
 * TCP: Port : 49152, Service : 49152, Open
 */
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#include <ws2tcpip.h>

unsigned short port = 135;
char ip[16] = "192.168.1.27";

void * recver(void * arg);

int main()
{
	SOCKET sckt;
	WSADATA wsdata;
	char buffer[1024];
	struct sockaddr_in veriler;
	pthread_t th_for_recv;
		
	if (WSAStartup(MAKEWORD(2, 0), &wsdata) != 0)
	{
		puts("WSAStartup failed.");
		exit(0);
	}
	
	if((sckt = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		perror("socket");
		exit(0);
	}
	
	veriler.sin_family = AF_INET;
	veriler.sin_port = htons(port);
	veriler.sin_addr.s_addr = inet_addr(ip);
	memset(&veriler.sin_zero, '\0', sizeof(veriler.sin_zero));
		
	if (connect(sckt, (struct  sockaddr *)&veriler, sizeof(struct sockaddr)) != SOCKET_ERROR)
	{
		char host[256];
        char service[256];
        getnameinfo((struct sockaddr *)&veriler, sizeof(struct sockaddr), host, sizeof(host), service, sizeof(service), 0);
        printf("TCP: Host: %s, Port : %d, Service : %s, Open\n", host, port, service);
		pthread_create(&th_for_recv, NULL, recver, &sckt);
		
		while (1)
		{
			gets(buffer);
			
			if (send(sckt, buffer, 1023, 0) <= 0)
			{
				printf("no database connection\n");
				break;
			}
			memset(buffer, 0, 1023);
		}
		
		pthread_join(th_for_recv, NULL);
	}
	
    printf("no connection\n");
	closesocket(sckt);
	
	WSACleanup();
	
	return 0;
}

void * recver(void * arg)
{
	char buffer[1024];
	SOCKET sckt = *(SOCKET *) arg;
	
	while (1)
	{
		if (recv(sckt, buffer, 1023, 0) <= 0)
		{
			printf("No Respond\n");
		}
		
		printf("R : %s\n", buffer);
		memset(buffer, 0, 1023);
	}
}
