/*
 * lwsock32 lpthread eklenmeli
 */
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT_THREAD_COUNT 1000

char ip[16];

void * portScan(void * arg);
void getSocketInfoFromUser(char * ip, unsigned short * min_port, unsigned short * max_port);

int main()
{
	WSADATA wsdata;
	unsigned short pthc;
	unsigned short min_port;
	unsigned short max_port;
	unsigned short * curr_ports;
	pthread_t * port_scan_threads;
	
	getSocketInfoFromUser(ip, &min_port, &max_port);
	
	if (max_port < pthc)
	{
		pthc = max_port;
	}
	else
	{
		pthc = PORT_THREAD_COUNT;
	}
	
	curr_ports = (unsigned short *) calloc(pthc, sizeof(curr_ports));
	port_scan_threads = (pthread_t *) calloc(pthc, sizeof(pthread_t));
	
	if (WSAStartup(MAKEWORD(2, 0), &wsdata) != 0)
	{
		puts("WSAStartup failed.");
		exit(0);
	}
	
	while (1)
	{
		static int x, index;
		
		if (curr_ports[index] == 0)
		{
			if ((min_port == max_port + 1) || (min_port == 0))
			{
				if (++x == pthc)
				{
					break;
				}
			}
			else
			{
				curr_ports[index] = min_port++;
				pthread_create(&port_scan_threads[index], NULL, portScan, (void *)&curr_ports[index]);
				pthread_detach(port_scan_threads[index]);
			}
		}
		else
		{
			x = 0;
			Sleep(10);
		}
		
		index = (index + 1) % pthc;
	}
	
	WSACleanup();
	free(curr_ports);
	free(port_scan_threads);
	
	return 0;
}

void getSocketInfoFromUser(char * ip, unsigned short * min_port, unsigned short * max_port)
{
	printf("IP MinPort MaksPort X.X.X.X 1 65535 > ");
	scanf("%s %hu %hu", ip, min_port, max_port);
	(*min_port == 0) && (*min_port = 1);
}

void * portScan(void * arg)
{
	SOCKET sckt, rckt;
	struct sockaddr_in veriler;
	unsigned short * port = (unsigned short *) arg;

	if((sckt = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		perror("socket");
		exit(0);
	}
	
	veriler.sin_family = AF_INET;
	veriler.sin_port = htons(*port);
	veriler.sin_addr.s_addr = inet_addr(ip);
	memset(&veriler.sin_zero, '\0', sizeof(veriler.sin_zero));
		
	if (connect(sckt, (struct  sockaddr *)&veriler, sizeof(struct sockaddr)) != SOCKET_ERROR)
	{
		char host[128];
        char service[128];
        getnameinfo((struct sockaddr *)&veriler, sizeof(struct sockaddr), host, sizeof(host), service, sizeof(service), 0);
        printf("TCP: Port : %d, Service : %s, Open\n", *port, service);
	}
/*
	closesocket(sckt);
	
	if((sckt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		perror("socket");
		exit(0);
	}
	
	if((rckt = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP)) == INVALID_SOCKET)
	{
		perror("socket");
		exit(0);
	}
	
	sendto(sckt, "1", 1, 0, (struct  sockaddr * )&veriler, sizeof(struct  sockaddr));
	int x= sizeof(struct  sockaddr);
	struct  sockaddr as;
	char c;
	
	if(recvfrom(rckt, &c, 1, 0, &as, &x) == 1)
	{
		struct servent * srvport = getservbyport(htons(*port), "udp");

		if (srvport != NULL)
			printf("tport %d: %sn", port, srvport->s_name);

		fflush(stdout); 
	}*/
	
	*port = 0;
	closesocket(sckt);
	
	return NULL;
}

