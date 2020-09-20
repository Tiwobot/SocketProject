#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sys/stat.h>
#include "lib/EA_Socket.h"
#include "set/sr.c"

int port;
EA_SOKET soket, i;
EA_FDSET soketler;
struct sockaddr_in verilerim;
char al[255] = {'\0'}, yes = '1';

void InitS();
void KabulKontrol();
void HepsineGonder();
int ProtokolKontrol();
int GetArg(int ARGC, char *[]);
int DosyaKontrol(const char *);
void WarningClient(EA_SOKET, char *);

int main(/*int argc, char *argv[]*/) /*if(GetArg(argc, argv) == 0)*/
{
	InitS();
	EA_InitW();
	
	soket = EA_Socket(AF_INET, SOCK_STREAM, 0);
	EA_Setsockopt(soket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	EA_SockaddrIn(&verilerim, AF_INET, htons(port), INADDR_ANY);
	EA_Bind(soket, (struct  sockaddr *)&verilerim);
	EA_Listen(soket, __LIMIT);
	
	EA_Fdzero(&soketler);
	EA_Fdset(soket, &soketler);
	
	for(;;)
	{
		i = EA_Select(&soketler, NULL, NULL, NULL);

		if (i == soket)					/* Ýstek geldiyse */
			KabulKontrol();				/* Raw soket tabanýnda açýk var mý bak */
		else							/* Ýstemci baðlantýsý kopmasý durumunda yada istemciden veri gelmesi durumunda */
			if(Recver(i, al) <= 0)		/* Kopma durumu */
				WarningClient(i, "Baðlantý koptu.");
			else						/* Veri gelme durumu */
				if(ProtokolKontrol())	/* Protokol */
					HepsineGonder();
	}
	
	EA_Close(soket);
	EA_CleanW();
	
	return 0;
}

int GetArg(int ARGC, char *ARGV[])
{
	if(ARGC == 2)
		return sscanf(ARGV[1], "%d", &port);
	return 0;
}

int DosyaKontrol(const char* filename){
    struct stat buffer;
    return stat(filename, &buffer);
}

void InitS()
{	
	setlocale(LC_ALL, "Turkish");
	
	puts("-*- PMoh V3 -*-");
	printf("Bilgiler :\nFD_SETSIZE : %d\n\n", FD_SETSIZE);
	
	printf("Etkinleþecek sunucu portunu girin : \n");
	scanf("%d", &port);
}

void WarningClient(EA_SOKET _sil, char _A[])
{
	time_t _t;
	char _IP[16] = {'\0'};
	
	time(&_t);
	EA_StoIP(_sil, &soketler, _IP);
	
	printf("(%s %s Socket : %ld) %s\n", _IP, _A,(long int) _sil, ctime(&_t));
	
	EA_Close(_sil);
	EA_Fdclr(_sil, &soketler);
}

void KabulKontrol()
{
	time_t _t;
	int _i, _j = 0;
	EA_SOKET _gelen;
	struct sockaddr_in _verileri;
	
	if((_gelen = EA_Accept(soket, (struct  sockaddr *)&_verileri)))
	{
		time(&_t);
    
		if(soketler.fd_count >= (unsigned int)(__LIMIT - 313))	/* Sunucu güvenliði için */
			EA_Close(_gelen);
		else
		{
			for(_i = 1; (unsigned int)_i < soketler.fd_count; _i++)	
				if(strcmp(inet_ntoa(_verileri.sin_addr), soketler.ip[_i]) == 0)
					_j++;
					
			if(_j >= 2)
			{
				for(_i = 1; (unsigned int)_i < soketler.fd_count; _i++)
					if(strcmp(inet_ntoa(_verileri.sin_addr), soketler.ip[_i]) == 0)
					{
						EA_Close(soketler.fd_array[_i]);
						EA_Fdclr(soketler.fd_array[_i--], &soketler);
					}
				WarningClient(_gelen, "Dos Attack.");
			}
			else
			{
				EA_Fdset(_gelen, &soketler);
				printf("(%s istemcisi geldi. Socket : %ld) %s\n", soketler.ip[soketler.fd_count - 1], (long int)_gelen, ctime(&_t));
			}
		}		
	}
	else
		printf("(Bir istemci kabul edilemedi.) %s\n", ctime(&_t));
}

void HepsineGonder()
{
	int _j;
	/*char VERI[255] = {'\0'};*/
	EA_SOKET CURRENT = i;
	/*
	for(_j = 0; _j < sizeof(al); _j++)
		VERI[_j] = al[_j];*/
	
	for(_j = 0; (unsigned int)_j < soketler.fd_count; _j++)
	{
		if((soketler.fd_array[_j] != soket) && (soketler.fd_array[_j] != CURRENT))
		{
            if(Sender(soketler.fd_array[_j], al)<=0)
            {
            	time_t _t;
				
            	time(&_t);
    			
            	printf("(%s ye gönderilemedi. Socket : %ld) %s\n", soketler.ip[_j], (long int)CURRENT, ctime(&_t));
            	
				EA_Fdclr(CURRENT, &soketler);
            	EA_Close(soketler.fd_array[_j]);

				continue;
            }
		}
	}
	memset(al, '\0', 255);
}

int ProtokolKontrol()
{
	time_t _t;
	EA_SOKET _sil = i;
	struct sockaddr_in ADDR;
	int _sayi = sizeof(ADDR);
	
	if((strlen(al + strlen(al) + 1 ) > 2) && (strlen(al + strlen(al) + 1 ) < 16) && (strlen(al) < 239))
    	return 1;
	
	time(&_t);
	
	getpeername(_sil, (struct sockaddr *)&ADDR, &_sayi);
	
	printf("(%s den gelen protokol yanlýþ. Socket : %ld) %s\n", inet_ntoa(ADDR.sin_addr), (long int)_sil, ctime(&_t));
	
	EA_Close(_sil);
	EA_Fdclr(_sil, &soketler);
	
	return 0;
}

#include "lib/EA_Socket.c"
