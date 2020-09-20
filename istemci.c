#include <stdio.h>
#include <pthread.h>

#include "lib/EA_Socket.h"
#include "set/ayar.h"

int main()
{
	pthread_t thread;
	char ver[255] = {'\0'};
	struct sockaddr_in veriler;

	EA_InitW();
	
	soket = EA_Socket(AF_INET, SOCK_STREAM, 0);
	
	do{
		InitC();
		
		EA_SockaddrIn(&veriler, AF_INET, htons(port), inet_addr(ip));
		
		if((check = EA_Connect(soket, (struct  sockaddr *)&veriler)) == -1)
			puts("HATALI veya NON-AKTIVE IP:Port!\n");
			
	}while(check == -1);

	puts("Connected!\n");
	
	pthread_create(&thread, NULL, VeriAl, NULL);
	
	while(1)
	{
		gotoxy(x11 = 1, y11 = yedek = y22 + 2);
		
		TextInput(ver);

		if(Kontrol(ver))
			continue;
		
		Protokol(ver);

		TextOutput(ver + strlen(ver) + 1, x22, &y22);
		TextOutput(ver, x22, &y22);

		if(Sender(soket, ver) <= 0)
			TextOutput("(Couldn't send the data'.)", x22, &y22);
		y22++;
	}
	
	EA_Close(soket);
	EA_CleanW();

	return 0;
}

#include "set/ayar.c"
#include "lib/EA_Socket.c"
