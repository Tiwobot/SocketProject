#ifndef _INC_EA_SOCKET
#define _INC_EA_SOCKET

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TUR  (2)
#define ALM  (1)
#define ABD (-8)

#ifdef _WIN32

#include <windows.h>

#define EA_LEN int
#define EA_MASK SOCKET
#define EA_SOKET SOCKET
#define __LIMIT FD_SETSIZE
#define __ELEMAN FD_SETSIZE
	
#else

#include <netdb.h>
#include <unistd.h>
#include <termios.h>
	
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
	
#define EA_SOKET int
#define EA_MASK fd_mask
#define EA_LEN socklen_t
#define	__LIMIT (((FD_SETSIZE)+((NFDBITS)-1))/(NFDBITS))
#define __ELEMAN __LIMIT*8*sizeof(fd_mask)

#endif

typedef struct EA_FDWL	/* Not for users ( EA_FDWL ). Use EA_FDSET*/
{
	u_int fd_count;
	EA_MASK fd_array[__LIMIT];
} EA_FDWL;

typedef struct EA_FDSET
{
	EA_SOKET maks;
	char ip[__ELEMAN][16];
	u_int fd_count;
	EA_MASK fd_array[__ELEMAN];
} EA_FDSET;

void EA_InitW();																	/*Windows için baþlangýç*/
void EA_CleanW();																	/*Windows için bitiþ*/
void EA_Close(EA_SOKET);															/*Soket kapatýr*/
int EA_StoIP(EA_SOKET, EA_FDSET *, char *);										/*Kapanmýþ soketin verilerini almak için*/
void EA_Setsockopt(EA_SOKET, int, int, const void *, EA_LEN);						/*Setsockopt ile aynýdýr*/
void EA_SockaddrIn(struct sockaddr_in *, short, unsigned short, unsigned long);		/*Normalde Sockaddr_in yappýsýna eþitlik yaparak atanýlýrdý fonk. hali.*/
void EA_Bind(EA_SOKET, struct  sockaddr *);											/*Bind ile ayný*/
void EA_Listen(EA_SOKET, int);														/*Listen ile ayný*/
void EA_Fdzero(EA_FDSET *);															/*FD setleri ayarlýdýr select için iki tane oluþturmaya gerek yoktur.*/
void EA_Fdset(EA_SOKET, EA_FDSET *);												/*FD_ISSET yoktur*/
void EA_Fdclr(EA_SOKET, EA_FDSET *);

EA_SOKET EA_Socket(int, int, int);													/* selectin ilk argumaný alýnmaz. 1. arguman stable. 2. arguman unknown.*/ 
EA_SOKET EA_Select(EA_FDSET *, EA_FDSET *, EA_FDSET *, struct timeval *);			/*3. arguman EA_FDSET not set. Geri dönüþ deðeri tetiklenen sokettir.*/
EA_SOKET EA_Accept(EA_SOKET, struct  sockaddr *);

int EA_Recv(EA_SOKET, char *, int);													/*recv ve sendin aynýlarý. Son arguman yok.*/
int EA_Send(EA_SOKET, const char *, int);
int EA_Connect(EA_SOKET, struct  sockaddr *);

#endif
