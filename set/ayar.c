void temizle()
{
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

void SetTerminal()
{
	#ifdef _WIN32
	
	CONSOLE_SCREEN_BUFFER_INFO alan;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &alan);
		
	yatay = alan.srWindow.Right - alan.srWindow.Left + 1;
	dikey = alan.srWindow.Bottom - alan.srWindow.Top + 1;
	
	#else
	
	struct winsize alan;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &alan);

	yatay = alan.ws_col;
	dikey = alan.ws_row;
		
	#endif

	ilk_blok_sonu = yatay / 2+ yatay % 2 - 1;
	ikinci_blok_basi = ilk_blok_sonu + 2;
	x22 = ikinci_blok_basi;
}

void TextInput(char mesaj[])
{
	int i = 0, a;
	short tut, tmp;
	
	memset(mesaj, '\0', 255);
	
	while(1)
	{
		yeni:
		
		tmp = getch();
		
		if(i == 0)
		{
			gotoxy(1, (tut = y11 = yedek) + dikey / 2 - 4);
			gotoxy(1, y11);
		}
		
		if(tmp != 8 && tmp != 127)
		{
			printf("%c", tmp);
			
			x11 += 1;

			mesaj[i] = (char)tmp;
			
			if(mesaj[i] == 10 || mesaj[i] == 13)
			{
				mesaj[i] = '\0';
				char reset[a = strlen(mesaj) + 1];
				
				for(i = 0; i < a; i++)
					reset[i] = ' ';
				reset[a] = '\0';
				
				TextOutput(reset, 1, &tut);
				
				break;
			}
			i++;
		}	
		else
		{
			if(i != 0)
			{
				if(i % ilk_blok_sonu == 0)
				{
					gotoxy(x11=ilk_blok_sonu + 1, y11-=1);
				}
				x11-=1;
				mesaj[--i]='\0';
				printf("\b \b");
			}
			goto yeni;
		}
	
		if(i % ilk_blok_sonu == 0 && i != 0)
		{
			printf("\n");
			y11+=(x11=1);
		}
		
		if(i == 238)
		{	
			while(a != 10 && a != 13 && a != 8 && a != 127)
				a = getch();

			char reset[239];
			for(i=0; i < 239; i++)
				reset[i]=' ';
			reset[238]='\0';
			
			TextOutput(reset, 1, &tut);
			
			break;
		}
	}
}

void *VeriAl(void *arg)
{
	int de;
	char al[255] = {'\0'};

	while(1)
	{
		if((de = Recver(soket, al)) <= 0)
		{
			TextOutput("(Hata! (kontrol) Veri okunamadý.)", x22, &y22);
			EA_Close(soket);
			
			break;
		}

		TextOutput(al + strlen(al) + 1, x22, &y22);
		TextOutput(al, x22, &y22);
		
		memset(al, '\0', 255);

		yedek = y22 = y22 + 1;
		gotoxy(x11, y11);
	}
	
	return (void *)1;
}

void Protokol(char yeni[])
{
	int i = 0;
	unsigned char a = strlen(yeni) + 1;
	
   	for(i = 0; isim[i] != '\0'; i++)
   		*(yeni + a + i) = *(isim + i);
   		
   	*(yeni + a + i) = '\0';
	yeni[254] = '\0';
}

void TextOutput(char mesaj[], short int xx, short int *yy)
{
	gotoxy(xx, *yy);

	int x = 0;
	unsigned char a = strlen(mesaj);
	char format[] = {'%', '.', ilk_blok_sonu / 10 + 48, ilk_blok_sonu % 10 + 48, 's', '\0'};
	
	while((unsigned int)x != strlen(mesaj))
	{
		if(a <= ilk_blok_sonu)
		{
			printf("%s", &mesaj[x]);
			x = x + a;
		}	
		else
		{
			printf(format, &mesaj[x]);
			x = x + ilk_blok_sonu;
			a = a - ilk_blok_sonu;
		}
		
		gotoxy(xx, *yy = *yy + 1);
	}
}

void InitC()
{
	if(check == 0)
	{
		#ifdef _WIN32
		
		system("color 0A");												/* Siyah arkaplan yeþil yazý */
		system("TITLE PMoH V3");
		
		#else

		printf("\033[1;32m");											/* Siyah arkaplan yeþil yazý */
		printf("%c]0;%s%c", '\033', "PMoH V3", '\007');
		
		#endif
		
		puts("Ad gir! (En fazla 13 karakter) : ");
		scanf("%13s", isim);
		
		fflush(stdin);
		
		isim[13]='\0';
		
		sprintf(isim + strlen(isim), " :%c", '\0');
		
		SetTerminal();
	}
	
	printf("Sunucu IP gir! (Örn : X.X.X.X) : ");
	scanf("%15s", ip);
	
	fflush(stdin);
	
	ip[15] = 0;

	printf("Sunucu portu gir! (Örn : 5000) : ");
	scanf("%d", &port);
	
	fflush(stdin);
	
	temizle();
}

int Kontrol(char *DATA)
{
	if(strcmp(DATA, "-sil") == 0)
	{
		temizle();
		y11 = y22 = yedek = 1;
		return 1;
	}
	return 0;
}
